#ifndef __DOWNLOAD_H
#define __DOWNLOAD_H 1

#include <iostream>
#include <string>
#include <urlmon.h>

void download_file(std::string url, std::string path) 
{
	HRESULT hr = URLDownloadToFile(NULL, url.c_str(), path.c_str(), 0, NULL);

	if (hr != S_OK)
	{
		std::cerr << hr << " Error downloading file: " << url << std::endl;
	}
}

#endif