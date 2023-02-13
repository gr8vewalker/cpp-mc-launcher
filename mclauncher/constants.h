#ifndef __CONSTANTS_H
#define __CONSTANTS_H 1

#include "fileutils.h"
#include <iostream>

using namespace std;

const string APPDATA = getenv("APPDATA");
const string TEMP = getenv("TEMP");

const string GAME_FOLDER = APPDATA + utils::separator() + ".cpplauncher";

const string ASSETS_FOLDER = GAME_FOLDER + utils::separator() + "assets";
const string ASSETS_INDEXES_FOLDER = ASSETS_FOLDER + utils::separator() + "indexes";
const string ASSETS_OBJECTS_FOLDER = ASSETS_FOLDER + utils::separator() + "objects";

const string LIBRARIES_FOLDER = GAME_FOLDER + utils::separator() + "libraries";

const string VERSIONS_FOLDER = GAME_FOLDER + utils::separator() + "versions";

const string RESOURCE_PACKS_FOLDER = GAME_FOLDER + utils::separator() + "resourcepacks";
const string SERVER_RESOURCE_PACKS_FOLDER = GAME_FOLDER + utils::separator() + "server-resource-packs";

const string NATIVES_FOLDER = GAME_FOLDER + utils::separator() + "natives"; // refresh every launch

const string JRE = GAME_FOLDER + utils::separator() + "runtime";
const string TEMP_JRE = TEMP + utils::separator() + "runtime.lzma";

const string VERSION_MANIFEST = VERSIONS_FOLDER + utils::separator() + "version_manifest_v2.json";

const string VERSION_MANIFEST_URL = "https://launchermeta.mojang.com/mc/game/version_manifest_v2.json";

const string JRE_64 = "https://www.dropbox.com/s/cvbg1k3n3pxzjjx/jrewin64.lzma?dl=1";
const string JRE_32 = "https://www.dropbox.com/s/oqmt98usfgzxnaz/jrewin32.lzma?dl=1";

#endif