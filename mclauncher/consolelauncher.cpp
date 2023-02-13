#include "consolelauncher.h"

#include <iostream>

#include "cliutils.h"
#include "strutils.h"
#include "fileutils.h"
#include "os.h"

#include "download.h"
#include "constants.h"

#include "exec.h"

void ConsoleLauncher::run(int argc, char** argv) 
{
	utils::InputParser inputs(argc, argv);
	handle_inputs(inputs);

	utils::logl("Starting launcher...");
	utils::logl("Version: " + m_Version);
	utils::logl("Username: " + m_Username);
	
	make_directories();

	download_file(VERSION_MANIFEST_URL, VERSION_MANIFEST);

	utils::logl("File structure checked");

	json manifest = utils::parse_json(VERSION_MANIFEST);

	json manifest_versions = manifest["versions"];

	json manifest_version_object;

	bool found = false;
	for (json v : manifest_versions)
	{
		if (v["id"] == m_Version)
		{
			manifest_version_object = v;
			found = true;
			break;
		}
	}

	if (!found)
	{
		utils::logl("Version not found in manifest, exiting...");
		exit(0);
	}

	string version_folder = VERSIONS_FOLDER + utils::separator() + m_Version;
	string version_json = version_folder + utils::separator() + m_Version + ".json";
	string version_jar = version_folder + utils::separator() + m_Version + ".jar";

	string version_url = manifest_version_object["url"];

	utils::mkdir(version_folder);

	download_file(version_url, version_json);

	json version = utils::parse_json(version_json);
	json asset_index = version["assetIndex"];

	string asset_index_path = ASSETS_INDEXES_FOLDER + utils::separator();
	asset_index_path.append(asset_index["id"]);
	asset_index_path.append(".json");


	if (!fs::exists(asset_index_path) && utils::sha1(asset_index_path) != asset_index["sha1"]) {
		string asset_index_url = asset_index["url"];
		download_file(asset_index_url, asset_index_path);
	}

	utils::logl("Checking assets");
	download_assets(asset_index_path);

	utils::logl("Checking libraries");
	download_libraries(version_json);

	utils::logl("Unpacking natives");
	unpack_natives();

	utils::logl("Checking game jar");
	json download = version["downloads"]["client"];
	if (!fs::exists(version_jar) && utils::sha1(version_jar) != download["sha1"]) {
		string download_url = download["url"];
		download_file(download_url, version_jar);
	}

	utils::logl("Launching game");

	string main_class = version["mainClass"];
	generate_vm_args(version_jar);
	generate_program_args(version);

	string cmd = "cd " + GAME_FOLDER + " && javaw " + utils::join(vm_args, " ") + " " + main_class + " " +
		utils::join(program_args, " ");

	exec(cmd.c_str());

	utils::logl("Goodbye :)");
	utils::logl("Press a key to quit");

	std::cin.get();
}

void ConsoleLauncher::handle_inputs(utils::InputParser& inputs) 
{
	if (inputs.cmdOptionExists("-h") || inputs.cmdOptionExists("--help"))
	{
		utils::help();
	}

	if (!inputs.cmdOptionExists("-u") && !inputs.cmdOptionExists("--username")) 
	{
		utils::logl("Username not specified, exiting...");
		exit(0);
	}

	if (!inputs.cmdOptionExists("-v") && !inputs.cmdOptionExists("--version"))
	{
		utils::logl("Version not specified, exiting...");
		exit(0);
	}

	m_Username = utils::string_or(inputs.getCmdOption("-u"), inputs.getCmdOption("--username"));
	m_Version = utils::string_or(inputs.getCmdOption("-v"), inputs.getCmdOption("--version"));
}

void ConsoleLauncher::make_directories()
{
	utils::mkdir(GAME_FOLDER);
	utils::mkdir(VERSIONS_FOLDER);
	utils::mkdir(SERVER_RESOURCE_PACKS_FOLDER);
	utils::mkdir(RESOURCE_PACKS_FOLDER);
	utils::mkdir(ASSETS_FOLDER);
	utils::mkdir(ASSETS_INDEXES_FOLDER);
	utils::mkdir(ASSETS_OBJECTS_FOLDER);
	utils::mkdir(LIBRARIES_FOLDER);
	utils::mkdir(NATIVES_FOLDER);

	utils::clean(NATIVES_FOLDER);
}

void ConsoleLauncher::download_assets(const std::string& asset_index) 
{
	json index = utils::parse_json(asset_index);
	json objects = index["objects"];

	for (json::iterator it = objects.begin(); it != objects.end(); it++) 
	{
		std::string hash = it.value()["hash"];
		std::string path = ASSETS_OBJECTS_FOLDER + utils::separator() + hash.substr(0, 2);
		utils::mkdir(path);
		path += utils::separator() + hash;
		if (!fs::exists(path) || utils::sha1(path) != hash)
		{
			std::string url = "https://resources.download.minecraft.net/" + hash.substr(0, 2) + "/" + hash;
			download_file(url, path);
		}
	}
}

void ConsoleLauncher::download_libraries(const std::string& version_json) 
{
	json client = utils::parse_json(version_json);
	json libraries = client["libraries"];
	utils::mkdir(LIBRARIES_FOLDER);

	vector<string> libs;
	vector<string> natives_list;

	for (json lib : libraries) {
		string name = lib["name"];
		vector<string> maven_name = utils::split(name, ':');
		vector<string> maven_name_artifact = utils::split(maven_name[0], '.');
		std::remove(maven_name.begin(), maven_name.end(), maven_name[0]);

		string full_path = utils::join(maven_name_artifact, utils::separator()) + utils::separator() + utils::join(maven_name, utils::separator());

		string dir = LIBRARIES_FOLDER + utils::separator() + full_path;
		string jar = dir + utils::separator() + maven_name[0] + "-" + maven_name[1] + ".jar";

		utils::mkdir(dir);

		vector<string> disallowed = { "windows", "osx", "linux" };
		json rules = lib["rules"];

		json downloads = lib["downloads"];
		json artifact = downloads["artifact"];
		json classifiers = downloads["classifiers"];

		if (!artifact.empty()) 
		{
			libs.push_back(jar);
			if (!fs::exists(jar) || artifact["size"] != fs::file_size(jar))
			{
				download_file(artifact["url"], jar);
			}
		}

		if (!rules.empty())
		{
			for (json rule : rules)
			{
				if (utils::iequals(rule["action"], "allow")) {
					if (!rule["os"].empty()) 
					{
						std::remove(disallowed.begin(), disallowed.end(), rule["os"]["name"]);
					}
					else 
					{
						disallowed.clear();
					}
				}
				else if (utils::iequals(rule["action"], "disallow")) 
				{
					if (!rule["os"].empty()) 
					{
						disallowed.push_back(rule["os"]["name"]);
					}
					else 
					{
						disallowed.emplace_back("osx");
						disallowed.emplace_back("windows");
						disallowed.emplace_back("linux");
					}
				}
			}
		}

		// this is windows project so dont implement linux and mac

		json natives = lib["natives"];
		if (!classifiers.empty() && !natives.empty() && natives.contains("windows") && (rules.empty() || std::find(disallowed.begin(), disallowed.end(), "windows") == disallowed.end())) {
			std::string replaced_name = utils::replace_all(lib["natives"]["windows"], "${arch}", utils::is_64_bit() ? "64" : "32");
			json artifact_native = classifiers[replaced_name];
			std::string native_jar = dir + utils::separator() + maven_name[0] + "-" + maven_name[1] + "-" + replaced_name + ".jar";
			if (!fs::exists(native_jar)) 
			{
				download_file(artifact_native["url"], native_jar);
			}
			else if (artifact_native["size"] != fs::file_size(native_jar)) 
			{
				fs::remove(native_jar);
				download_file(artifact_native["url"], native_jar);
			}
			natives_list.push_back(native_jar);
		}

	}

	m_FoundLibs = libs;
	m_FoundNatives = natives_list;
}
void ConsoleLauncher::unpack_natives() 
{
	for (string native : m_FoundNatives) 
	{
		utils::unzip(native, NATIVES_FOLDER, "META-INF/");
	}
}

void ConsoleLauncher::generate_vm_args(const std::string& jar) 
{
	m_FoundLibs.push_back(jar);

	std::string classpath = "";

	for (std::string lib : m_FoundLibs) {
		classpath += lib + utils::path_separator();
	}

	classpath = classpath.substr(0, classpath.length() - utils::path_separator().length());

	vm_args = {
			"-Djava.library.path=" + NATIVES_FOLDER,
			"-Dfml.ignoreInvalidMinecraftCertificates=true",
			"-Dfml.ignorePatchDiscrepancies=true",
			"-cp",
			"\"" + classpath + "\""
	};
}

void ConsoleLauncher::generate_program_args(const json& version_json) 
{
	program_args = {
				"--username",
				m_Username,
				"--version",
				version_json["id"],
				"--gameDir",
				GAME_FOLDER,
				"--assetsDir",
				ASSETS_FOLDER,
				"--assetIndex",
				version_json["assetIndex"]["id"],
				"--uuid",
				"0",
				"--accessToken",
				"0",
				"--userType",
				"legacy",
				"--userProperties",
				"{}",
	};
}