#ifndef __CONSOLELAUNCHER_H
#define __CONSOLELAUNCHER_H 1

#include "cliutils.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ConsoleLauncher {
public:
	void run(int argc, char** argv);
private:
	void handle_inputs(utils::InputParser& input);
	void make_directories();
	void download_assets(const std::string& asset_index);
	void download_libraries(const std::string& version_json);
	void unpack_natives();
	void generate_vm_args(const std::string& jar);
	void generate_program_args(const json& version_json);
	std::string m_Version;
	std::string m_Username;
	std::vector<std::string> m_FoundLibs;
	std::vector<std::string> m_FoundNatives;
	std::vector<std::string> vm_args;
	std::vector<std::string> program_args;

};

#endif