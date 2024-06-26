#include <iostream>
#include <sstream>
#include <regex>
#include <ctime>
#include "TestRunner.h"
#include "RdfData.h"

#include <typeinfo>
#include <utility>

/**
 * Constructor
 */
TestRunner::TestRunner(
        std::string ipath,
        std::vector<std::vector<std::string>> exe)
    :   path{std::move(ipath)},
        executables{std::move(exe)}
{
    // check that the path finishes with a path separator if
    // it doesn't then add one
    std::smatch result;
    std::regex expression ( ".*/$" );
    if ( ! regex_match( path, expression ) )
    {
        path = path + '/';
    }

    // initialise the executable iterator
    executable_iterator = 0;
    // get the command from the executables array
    auto command = executables.at(executable_iterator);
    // create the full path to the executable
    std::string fullpath ( path + command.at(0) );
    // get the name of the manifest
    manifest = command.at(1);
    // prime the command runner
    cr.set_command( fullpath );

    has_next_output = false;
}

/**
 *  The only thing that start needs to do is set the
 *  has next output to true.
 */
void TestRunner::start()
{
    has_next_output = true;
}

/**
 *  @return true if there is another test executable to run
 */
bool TestRunner::has_next() const
{
    return has_next_output;
}

/**
 * This method is used to format and return a TestResult object
 *
 * @return TestResult
 */
TestResult TestRunner::next_result()
{
    // Initialise TestResult
    TestResult tr;

    if ( ! has_next_output )
    {
        std::cerr << "No next output!" << std::endl;
        return tr;
    }

    // advance the stringstream to the next test result
    std::string next_output = find_next_result();
    // check for empty string and return empty TestResult
    if ( next_output.empty() ) return tr;

    // get pass/fail
    std::smatch result;
    std::regex_search( next_output, result, std::regex( "([A-Z]+)") );
    // get the test name
    std::smatch id;
    std::regex_search( next_output, id, std::regex( "[A-Za-z0-9]*(?= \\()") );
    // create the TestResult object
    tr.manifest = manifest;
    tr.test = id[0];
    tr.result = result[0];
    tr.time = time(nullptr);
    return tr;
}



/**
 * This function gets the next executable from the array and then runs the
 * command.  The output from the command (with all the debugging info and stuff
 * we don't need) is then passed to the string stream for processing.
 */
bool TestRunner::next_executable()
{
    // initialise this to false
    has_next_output = false;

    if ( executable_iterator < executables.size() )
    {
        // clear any error states on the stringstream
        ss.clear();
        // now clear the contents of the stringstream to reuse it
        ss.str(std::string());

        // get the next command
        auto command = executables.at(executable_iterator);
        // format the full path to the command
        std::string fullpath ( path + command.at(0) );
        // set the manifest name
        manifest = command.at(1);
        // prime the command runner
        cr.set_command( fullpath );
        // run the command runner
        std::string command_output = cr.run();
        // direct the output to the istream
        ss << command_output;
        // flag that there is a next output
        has_next_output = true;
        // increment the iterator
        executable_iterator++;
    }

    return has_next_output;
}


/**
 * Advances the stringstream to the next result and then returns the result
 * string
 *
 * @return string result
 */
std::string TestRunner::find_next_result()
{
    // initialise a variable to store the next line from the
    // command output stored in the stringstream
    std::string line;

    // at some point, we will hit the end of the individual test function
    // results and then we will start parsing the test suite summary. We
    // want to skip the summary, so if we see the start of it, then set
    // this flag to skip all following lines.
    bool doneParsingResults = false;

    // loop through all the lines in the command output
    while ( !doneParsingResults && std::getline( ss, line, '\n' ) )
    {
        // find the next result output
        if(line.find("[----------] Global test environment tear-down") != std::string::npos) {
            doneParsingResults = true;
        }
        else if ( ! line.find("[  ",0) )
        {
            // stop further execution of the function
            return line;
        }
    }

    // if we make it this far, get the next executable and
    // continue looking for the next result
    if ( next_executable() ) return find_next_result();

    // if no results are found then return a blank string
    return std::string();
}

