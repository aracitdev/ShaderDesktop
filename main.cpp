#define _WIN32_WINNT 0x500
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <tinyxml2.h>

#include "Utility.h"
#include "Timer.h"

int EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    if(FindWindowEx(hwnd, nullptr,  "SHELLDLL_DefView", nullptr) != nullptr)
        *((HWND*)(lParam)) =FindWindowEx(nullptr,hwnd, "WorkerW", nullptr);
    return true;
}

HWND GetWorkerW(void)
{   //Retrieves the desktop window handle
    HWND ProgMan=FindWindow("ProgMan", nullptr);
    if(!ProgMan)
    {
        std::cout <<"ProgMan could not be found\n";
        return nullptr;
    }
    SendMessageTimeout(ProgMan, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    HWND WorkerW = nullptr;
    LPARAM Param = (LPARAM)&WorkerW;
    EnumWindows(EnumWindowsProc, Param);
    return WorkerW;
}

bool LoadConfig(const std::string& filename, uint32_t& framerate, std::string& shaderFolderPath, bool& enableMouse, uint32_t& timeBetween)
{
    framerate=30;
    timeBetween=60000;
    shaderFolderPath="Shaders";
    enableMouse=false;
    tinyxml2::XMLDocument Doc;
    if(Doc.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        return false;

    tinyxml2::XMLElement* BaseElement=Doc.FirstChildElement();
    if(!BaseElement)
        return false;

    tinyxml2::XMLElement* FPSElement=BaseElement->FirstChildElement("FPS");
    if(FPSElement)
    {
        int64_t tempFPS=FPSElement->Int64Text(30);
        framerate=(tempFPS < 0) ? 30: tempFPS;  //ensure its positive or zero
    }
    tinyxml2::XMLElement* shaderElement=BaseElement->FirstChildElement("Path");
    if(shaderElement)
    {
        shaderFolderPath=shaderElement->GetText();
    }

    tinyxml2::XMLElement* mouseElement=BaseElement->FirstChildElement("UseMouse");
    if(mouseElement)
    {
        enableMouse=mouseElement->BoolText(false);
    }

    tinyxml2::XMLElement* timeBetweenElement=BaseElement->FirstChildElement("TimeBetweenSwitch");
    if(timeBetweenElement)
    {
        int64_t tempBetween= timeBetweenElement->Int64Text(60000);
        timeBetween= (tempBetween>0) ? tempBetween : 60000;
    }
    return true;
}


std::string ReplaceOccurrences(std::string& s, const std::string& toReplace, const std::string& replaceWith)
{
    size_t pos = s.find(toReplace);

    while(pos != std::string::npos)
    {
        s=s.replace(pos, toReplace.length(), replaceWith);
        pos= s.find(toReplace);
    }
    return s;
}

std::string GetFileBuffer(std::string file)
{
    std::ifstream t(file.c_str());
    if(!t.is_open())
        return " ";
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    char* buffer=new char[size];
    t.seekg(0);
    t.read(buffer, size);
    std::string returnV=std::string(buffer,size);
    delete[] buffer;
    return returnV;
}

int main(int argc, char* argv[])
{

    //sf::Vector2f ScreenSize= sf::Vector2f(GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN));
    HWND WorkerW=GetWorkerW();  //desktop window
    if(WorkerW == nullptr)
    {
        std::cout <<"Failed to get the desktop background window.\n";
        return 0;
    }


    uint32_t framerate=30;
    uint32_t timeBetweenSwitch=60000;   //60 second default
    std::string folder="Shaders";
    bool useMouse=false;
    if(!LoadConfig("config.xml", framerate, folder, useMouse, timeBetweenSwitch))
    {
        std::cout <<"Failed to load config.xml. Going with defaults.\n";
    }

    std::cout <<"Settings.\nFPS " <<framerate <<" Folder " <<folder <<" Time " <<timeBetweenSwitch<<"\n";
    std::cout <<"Mouse Enabled: " << (useMouse ? "True" : "False")<<"\n";

    if(!DirectoryExists(folder.c_str()))
        CreateDirectory(folder.c_str(), nullptr);


    std::vector<std::string> ShaderList=GetFilesInDirectory(folder.c_str());
    uint32_t currentShader=0;

    sf::RenderWindow Window(WorkerW);   //create an sfml window off workerw
    sf::Vector2f ScreenSize=((sf::Vector2f)Window.getSize());


    std::cout <<"Window Created.\n";
    Window.setFramerateLimit(framerate);

    sf::RectangleShape Shape(ScreenSize);
    Shape.setPosition(0,0);
    Shape.setFillColor(sf::Color::Red);


    std::vector<sf::Shader*> Shaders(ShaderList.size());
    for(uint32_t counter=0; counter < Shaders.size(); counter++)
    {
        Shaders[counter]=new sf::Shader();
        if(!Shaders[counter]->loadFromFile(ShaderList[counter], sf::Shader::Fragment))
        {
            std::cout <<"Failed to load the shader " <<ShaderList[counter]<<"\n";
            delete Shaders[counter];
            Shaders[counter]=nullptr;
        }
        Shaders[counter]->setUniform("iResolution", ScreenSize); //this never changes, so we can set it now
    }


    Timer Clock;


    while(Window.isOpen())
    {
        if(Clock.GetTime() > timeBetweenSwitch)
        {
            currentShader=(currentShader+1)%Shaders.size(); //next shaders
            while(!Shaders[currentShader])  //skip any empty failed ones
                currentShader=(currentShader+1)%Shaders.size();
            Clock.Restart();
        }
        sf::Vector2f MousePos = sf::Vector2f(0,0);
        if(useMouse)
            MousePos=sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        float Time = ((float)Clock.GetTime())/1000.0f;
        Shaders[currentShader]->setUniform("iTime", Time);
        Shaders[currentShader]->setUniform("iMouse", MousePos);
        Window.clear();
        Window.draw(Shape, Shaders[currentShader]);
        Window.display();
    }
    for(uint32_t counter=0; counter < Shaders.size(); counter++)
        delete Shaders[counter];
    Shaders.clear();
    return 0;
}
