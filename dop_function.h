#ifndef DOP_FUNCTION_H
#define DOP_FUNCTION_H

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <list>
#include <vector>

namespace fs = std::experimental::filesystem;

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
    std::list<std::list<std::string>> readGameConfig()
    {
        fs::path current_path = fs::current_path();
        fs::path file_path(current_path);
        std::list<std::list<std::string>> list1;
        // Рекурсивный обход директории
        std::cout << file_path.parent_path() << std::endl;

        char del = '/';
        std::string directory = split(file_path.parent_path().generic_string(), del);
        std::cout << directory << std::endl;

        for (auto &p : fs::recursive_directory_iterator(directory))
        {
            std::string readFileName = directory + p.path().filename().generic_string();
            std::fstream readFile(readFileName);
            if (readFile.is_open())
            {
                std::cout << "File " << readFileName << " be opened" << std::endl;
                std::list<std::string> list2;
                list2.push_back(readFileName);
                std::string str;
                while(readFile >> str)
                {
                    if(str == "Game" /*status = "*/)
                    {
                        readFile >> str >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "Create" /*game with ID : "*/)
                    {
                        readFile >> str >> str >> str >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "Number" /*players: "*/)
                    {
                        readFile >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "first" /*: "*/)
                    {
                        readFile >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "second" /*: "*/)
                    {
                        readFile >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "third" /*: "*/)
                    {
                        readFile >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "fourth" /*: "*/)
                    {
                        readFile >> str >> str;
                        list2.push_back(str);
                    }
                    else if(str == "fifth" /*: "*/)
                    {
                        readFile >> str >> str;
                        list2.push_back(str);
                    }
                }
                list1.push_back(list2);
            }
            readFile.close();
        }
        return list1;
    }
};

#endif // DOP_FUNCTION_H
