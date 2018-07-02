//
// Created by fighter on 18-7-2.
//

#ifndef WIND_JVM_ZIPFILE_H
#define WIND_JVM_ZIPFILE_H


#include <string>
struct zip;
class ZipFile {

private :
    zip *z;

    void init();

public:
    ZipFile():z(nullptr){init();}

    /**
     * get file content
     * @param path
     * @return file content,need to free manuel.
     */
    std::string  * getFile(std::wstring path);

    ~ZipFile();

};


#endif //WIND_JVM_ZIPFILE_H
