#include "Utility.h"

bool DirectoryExists(const std::string& filename)
{
    return std::filesystem::exists(filename);
}

bool Exist(const std::string& filename)
{
    return std::filesystem::exists(filename);
}

std::vector<std::string> GetFilesInDirectory(std::string folder)
{
    std::vector<std::string> returnV;
    for(auto& p: std::filesystem::directory_iterator(folder))
        returnV.push_back(p.path().string());
    return returnV;
}

std::string GetFileName(const std::string& Directory)
{
    std::string returnV;
    size_t pos=Directory.find_last_of("\\/");
    returnV= Directory.substr(pos +1);
    pos = Directory.find_last_of('.');
    return returnV.substr(0,pos -1);
}


std::error_code DeleteDirectory(const std::string &dir)
{
    std::error_code err;
    std::filesystem::remove(dir, err);
    return err;
}


std::string GetExtension(const std::string& path)
{
    size_t pos=path.find_last_of('.');
    return path.substr(pos+1);
}


std::string RemoveExtension(const std::string& in)
{
    std::string path=GetFileName(in);
    size_t pos=path.find_last_of('.');
    return path.substr(0,pos-1);
}

std::string GetLastErrorAsString(void)
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}
