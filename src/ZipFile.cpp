//
// Created by fighter on 18-7-2.
//

#include "ZipFile.h"
#include <zip.h>
#include <utils/utils.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


void ZipFile::init()
{

    const std::wstring pwd = utf8_to_wstring(boost::filesystem::initial_path<boost::filesystem::path>().string());
    // get xml
    std::wstring config_xml = pwd + L"/config.xml";
    if(!boost::filesystem::exists(config_xml)) {
        std::wcerr << "error! didn't find wind_jvm/config.xml. maybe you deleted it or didn't run the program under the wind_jvm/ folder and using the ./bin/wind_jvm command?" << std::endl;
    }
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(wstring_to_utf8(config_xml), pt);

    std::wstring rtjar_folder;
#if (defined (__APPLE__))
    rtjar_folder = utf8_to_wstring(pt.get<std::string>("path.mac"));
#elif (defined (__linux__))
    rtjar_folder = utf8_to_wstring(pt.get<std::string>("path.linux"));
#else
    std::cerr << "do not support for windows!" << std::endl;
	assert(false);
#endif
    const std::wstring  zipPath =
            rtjar_folder + L"rt.jar";
    
    
    int error = 0;
    z = zip_open(wstring_to_utf8(zipPath).c_str(),0,&error);
    if(z == nullptr){
        std::wcerr<<L"open file error :"+zipPath<<std::endl;
        exit(-1);
    }
}

std::string * ZipFile::getFile(std::wstring path)
{

    std::string name = wstring_to_utf8(path);

    struct zip_stat st;
    zip_stat_init(&st);
    int ret = zip_stat(z,name.c_str(),0,&st);
    if(ret!=0){
        return nullptr;
    }


    char * buf = new char[st.size];

    zip_file * f = zip_fopen(z,name.c_str(),0);
    zip_fread(f,buf,st.size);
    zip_fclose(f);

    return new std::string(buf,st.size);
}

ZipFile::~ZipFile()
{
    zip_close(z);
}
