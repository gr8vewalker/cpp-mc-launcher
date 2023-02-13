#ifndef __CLIUTILS_H
#define __CLIUTILS_H 1

#include <iostream>
#include <vector>

static const std::string PREFIX = "[Launcher] ";

namespace utils {

    inline void logl(const std::string& msg) 
    {
        std::cout << PREFIX << msg << std::endl;
    }

    inline void help()
    {
        logl("Usage: launcher -u <username> -v <version> [options]");
        logl("Options:");
        logl("-h, --help\t\tShows this output and exits");
        logl("-u, --username\t\tPlayer name for minecraft");
        logl("-v, --version\t\tMinecraft version to be launched");
        exit(0);
    }

    class InputParser {
    public:
        InputParser(int& argc, char** argv) {
            for (int i = 1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }

        const std::string& getCmdOption(const std::string& option) const {
            std::vector<std::string>::const_iterator itr;
            itr = std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }

        bool cmdOptionExists(const std::string& option) const {
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                != this->tokens.end();
        }

    private:
        std::vector<std::string> tokens;
    };
}
#endif