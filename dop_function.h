#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <list>
#include <vector>

namespace fs = std::filesystem;

class dop_function
{
public:
    std::vector <char> convert_string_to_char(std::vector <char> result, std::string s)
    {
        size_t l = s.length();
        for(int i = l - 1; i > -1; i--)
        {
            result.emplace(result.begin(), s.at(i));
            result.erase(result.end() - 1);
        }
        return result;
    }
    std::string convert_char_to_string(std::vector <char> c, std::string result)
    {
        result = "";
        int i = 0;
        while(!c.empty())
        {
            if(c.at(i) != '\0' && c.at(i) != '\n')
                result += c.at(i);
            c.erase(c.begin());
        }
        return result;
    }
    std::vector <char> clearBuf(std::vector <char> result)
    {
        result.erase(result.begin(), result.end());
        const short BUFF_SIZE = 1024;
        for(int i = BUFF_SIZE - 1; i > -1; i--)
            result.emplace(result.begin(), '\0');
        return result;
    }
    std::string split(std::string str, char del)
    {
        std::string result = "";
        std::string temp = "";
        for(int i=0; i<(int)str.size(); i++)
        {
            if(str[i] != del)
                temp += str[i];
            else
            {
                result += temp + "/";
                temp = "";
            }
        }
        std::cout << std::endl;
        return result;
    }
};

