#ifndef __STRUTILS_H
#define __STRUTILS_H 1

#include <iostream>

namespace utils {
	std::string string_or(std::string str1, std::string str2)
	{
		return str1.empty() ? str2 : str1;
	}

    bool iequals(const std::string& a, const std::string& b) 
    {
        unsigned int sz = a.size();
        if (b.size() != sz)
            return false;
        for (unsigned int i = 0; i < sz; ++i)
            if (tolower(a[i]) != tolower(b[i]))
                return false;
        return true;
    }

    std::vector<std::string> split(const std::string& s, char delim) 
    {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (getline(ss, item, delim)) {
            result.push_back(item);
        }

        return result;
    }

    std::string join(const std::vector<std::string>& v, const std::string& c) 
    {
        std::string result = "";
        for (int i = 0; i < v.size(); i++) {
            result += v[i];
            if (i != v.size() - 1) {
                result += c;
            }
        }
        return result;
    }

    std::string replace_all(std::string str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }
}

#endif