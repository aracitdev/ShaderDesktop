#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

/**************************************************************************************************
* \file Utility.h
* \author Sean C
* Defines some helpful functions for dealing with windows.
***************************************************************************************************/

#include <string>
#include <vector>
#include <windows.h>
#include <filesystem>
/**************************************************************************************************
* \fn bool DiretoryExists(const std::string& dirName)
* \param dirName The directory to check for.
* \return Returns true if dirName exists, false otherwise.
* Checks whether or not the directory dirName exists.
***************************************************************************************************/
bool DirectoryExists(const std::string& dirName);

/**************************************************************************************************
* \fn bool Exists(const std::string& filename)
* \param filename The file to check for.
* \return Returns true if filename exists, false otherwise.
* Returns whether or not the file exists.
***************************************************************************************************/
bool Exist(const std::string& filename);

/***************************************************************************************************
* \fn std::vector<std::string> GetFilesInDirectory(const std::string& folder)
* \param folder The folder to look for files in.
* \return Returns a list of files inside the folder.
* Searches for and creates a list of files inside of the folder.
***************************************************************************************************/
std::vector<std::string> GetFilesInDirectory(std::string folder);

/***************************************************************************************************
* \fn std::string GetFileName(const std::string& Directory)
* \param Directory the directory to clip.
* \return The filename.
* Gets the filename from the full directory, clipping off the path.
***************************************************************************************************/
std::string GetFileName(const std::string& directory);


/***************************************************************************************************
* \fn std::string RemoveExtension(const std::string& in)
* \param in The string to clip.
* \return The clipped string.
* Gets the filename without the filetype extension.
***************************************************************************************************/
std::string RemoveExtension(const std::string& in);

/***************************************************************************************************
* \fn std::string GetExtension(const std::string& path)
* \param path The directory to clip.
* \return The file extension.
* Gets the file extension from the path.
***************************************************************************************************/
std::string GetExtension(const std::string& path);

/***************************************************************************************************
* \fn std::error_code DeleteDirectory(const std::string& directory, bool deleteSubdirectories=true)
* \param directory The directory to delete.
* \param deleteSubdirectories Whether or not subdirectories should be deleted.
* \return Returns the error code.
* Deletes a specified directory and optionally any subdirectories, including the files contained.
***************************************************************************************************/
std::error_code DeleteDirectory(const std::string &directory);

std::string GetLastErrorAsString(void);

#endif // UTILITY_H_INCLUDED
