#include "Utility.h"

bool DirectoryExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with the path

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;    //directory

  return false;    //not a directory!
}

bool Exist(const std::string& filename)
{
    std::ifstream File; //simple file exist check
    File.open(filename, std::ifstream::in);
    return File.is_open();
}

std::vector<std::string> GetFilesInDirectory(std::string folder)
{
    std::vector<std::string> names;
    std::string search_path = folder + "/*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                names.push_back(folder + std::string("\\")+fd.cFileName);
            }
        } while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}


std::string GetTempPath(void)
{
    char Path[MAX_PATH];
    GetTempPath(MAX_PATH, Path);
    return std::string(Path);
}


std::string GetFileName(const std::string& Directory)
{
    std::string returnV;
    size_t pos=Directory.find_last_of("\\/");
    returnV= Directory.substr(pos +1);
    pos = Directory.find_last_of('.');
    return returnV.substr(0,pos -1);
}


int DeleteDirectory(const std::string &refcstrRootDirectory,bool bDeleteSubdirectories)
{
  bool bSubdirectory = false;
  HANDLE          hFile;
  std::string     strFilePath;
  std::string     strPattern;
  WIN32_FIND_DATA FileInformation;


  strPattern = refcstrRootDirectory + "\\*.*";
  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bDeleteSubdirectories)
          {
            // Delete subdirectory
            int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
            if(iRC)
              return iRC;
          }
          else
            bSubdirectory = true;
        }
        else
        {
          // Set file attributes
          if(::SetFileAttributes(strFilePath.c_str(),
                                 FILE_ATTRIBUTE_NORMAL) == FALSE)
            return ::GetLastError();

          // Delete file
          if(::DeleteFile(strFilePath.c_str()) == FALSE)
            return ::GetLastError();
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
    else
    {
      if(!bSubdirectory)
      {
        // Set directory attributes
        if(::SetFileAttributes(refcstrRootDirectory.c_str(),
                               FILE_ATTRIBUTE_NORMAL) == FALSE)
          return ::GetLastError();

        // Delete directory
        if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
          return ::GetLastError();
      }
    }
  }
  return 0;
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
