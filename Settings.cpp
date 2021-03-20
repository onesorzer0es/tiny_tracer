#include "Settings.h"
#include "Util.h"

#include <vector>
#include <fstream>
#include <sstream>

#define KEY_FOLLOW_SHELLCODES           "FOLLOW_SHELLCODES"
#define KEY_LOG_RTDSC                   "TRACE_RDTSC"
#define KEY_LOG_SECTIONS_TRANSITIONS    "LOG_SECTIONS_TRANSITIONS"
#define KEY_LOG_SHELLCODES_TRANSITIONS  "LOG_SHELLCODES_TRANSITIONS"
#define KEY_SHORT_LOGGING               "ENABLE_SHORT_LOGGING"

t_shellc_options ConvertShcOption(int value)
{
    if (value >= SHELLC_OPTIONS_COUNT) {
        // choose the last option:
        return t_shellc_options(SHELLC_OPTIONS_COUNT - 1);
    }
    return (t_shellc_options)value;
}

//----

bool fillSettings(Settings &s, std::string line)
{
    std::vector<std::string> args;
    util::splitList(line, '=', args);

    if (args.size() < 2) {
        return false;
    }
    bool isFilled = false;
    std::string valName = args[0];
    std::string valStr = args[1];
    util::trim(valName);
    util::trim(valStr);

    const int val = util::loadInt(valStr);

    if (util::iequals(valName, KEY_FOLLOW_SHELLCODES)) {
        s.followShellcode = ConvertShcOption(val);
        isFilled = true;
    }
    if (util::iequals(valName, KEY_LOG_RTDSC)) {
        s.traceRDTSC = val;
        isFilled = true;
    }
    if (util::iequals(valName, KEY_LOG_SECTIONS_TRANSITIONS)) {
        s.logSectTrans = val;
        isFilled = true;
    }
    if (util::iequals(valName, KEY_LOG_SHELLCODES_TRANSITIONS)) {
        s.logShelcTrans = val;
        isFilled = true;
    }
    if (util::iequals(valName, KEY_SHORT_LOGGING)) {
        s.shortLogging = val;
        isFilled = true;
    }
    return isFilled;
}

void stripComments(std::string &str)
{
    size_t found = str.find_first_of(";#");
    if (found != std::string::npos) {
        str = str.substr(0, found - 1);
    }
}

bool Settings::loadINI(const std::string filename)
{
    std::ifstream myfile(filename.c_str());
    if (!myfile.is_open()) {
        return false;
    }
    const size_t MAX_LINE = 300;
    char line[MAX_LINE] = { 0 };
    bool filledAny = false;

    while (!myfile.eof()) {
        myfile.getline(line, MAX_LINE);
        std::string lineStr = line;
        stripComments(lineStr);
        
        if (fillSettings(*this, lineStr)) {
            filledAny = true;
        }
    }
    return filledAny;
}
