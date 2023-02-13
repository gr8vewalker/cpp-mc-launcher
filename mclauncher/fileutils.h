#ifndef __FILEUTILS_H
#define __FILEUTILS_H 1

#include <iostream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <windows.h>
#include "unzip.h"

#include "sha1.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace utils {
    inline std::string separator() {
#ifdef _WIN32
        return "\\";
#else
        return "/";
#endif
    }

    inline std::string path_separator() {
#ifdef _WIN32
        return ";";
#else
        return ":";
#endif
    }

    void mkdir(std::string dir) {
        if (!fs::exists(dir)) {
            fs::create_directories(dir);
        }
    }

    void clean(std::string dir) {
        if (fs::exists(dir)) {
            fs::remove_all(dir);
            mkdir(dir);
        }
    }

    json parse_json(std::string path) {
        std::ifstream f(path);

        if (!f.is_open()) {
            std::cerr << "Failed to open " << path << std::endl;
            return json();
        }

        return json::parse(f);
    }

    std::string sha1(std::string path) {
        return SHA1::from_file(path);
    }

    void unzip(const std::string& zippath, const std::string& target, const std::string exclude = "") 
    {
        HZIP hz = OpenZip(zippath.c_str(), 0);
        ZIPENTRY ze; GetZipItem(hz, -1, &ze); int numitems = ze.index;
        SetUnzipBaseDir(hz, target.c_str());
        for (int zi = 0; zi < numitems; zi++)
        {
            ZIPENTRY ze; GetZipItem(hz, zi, &ze);
            // tchar to string
            TCHAR* chars = ze.name;
            std::string str;
#ifndef UNICODE
            str = chars;
#else
            std::wstring wStr = chars;
            str = std::string(wStr.begin(), wStr.end());
#endif
            if (!exclude.empty() && str.find(exclude) != std::string::npos) continue;
            UnzipItem(hz, zi, ze.name);
        }
        CloseZip(hz);
    }
}

#endif