#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <QString>
#include <QDir>
#include <QDebug>

#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>

//using namespace std;
namespace fs = std::experimental::filesystem;

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
} /**/

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

std::list<std::string> readGameConfig()
{
    fs::path current_path = fs::current_path();
    fs::path file_path(current_path);
    std::list<std::string> list1;
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
            std::string str;
            while(readFile >> str)
            {
                if(str == "Create" /*game with ID : "*/)
                {
                    readFile >> str >> str >> str >> str >> str;
                    list1.push_back(str);
                }
                else if(str == "Number" /*players: "*/)
                {
                    readFile >> str >> str;
                    list1.push_back(str);
                }
                else if(str == "first" /*: "*/)
                {
                    readFile >> str >> str;
                    list1.push_back(str);
                }
                else if(str == "second" /*: "*/)
                {
                    readFile >> str >> str;
                    list1.push_back(str);
                }
                else if(str == "third" /*: "*/)
                {
                    readFile >> str >> str;
                    list1.push_back(str);
                }
                else if(str == "fourth" /*: "*/)
                {
                    readFile >> str >> str;
                    list1.push_back(str);
                }
                else if(str == "fifth" /*: "*/)
                {
                    readFile >> str >> str;
                    list1.push_back(str);
                }
            }
        }
        readFile.close();
    }
    return list1;
}

int main(void)
{
    std::list<std::string> list_game_config = readGameConfig();

    std::cout << list_game_config.size() << std::endl;
    while(!list_game_config.empty())
    {
        std::cout << list_game_config.front() << std::endl;
        list_game_config.pop_front();
    }
    std::cout << "_________________________________________________" << std::endl;
    std::cout << "        Start GAME Server                        " << std::endl;
    std::cout << "_________________________________________________" << std::endl;

    //Key constants
    const char IP_SERV[] = "10.124.40.14";	// Enter local Server IP address
    const int PORT_NUM = 8008;				// Enter Open working server port
    const short BUFF_SIZE = 1024;			// Maximum size of buffer for exchange info between server and client

    // Key variables for all program
    int erStat;								// Keeps socket errors status

    //IP in string format to numeric format for socket functions. Data is in "ip_to_num"
    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);

    if (erStat <= 0) {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }

    // WinSock initialization
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if ( erStat != 0 ) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
        std::cout << "WinSock initialization is OK" << std::endl;

    // Server socket initialization
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSock == INVALID_SOCKET) {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Server socket initialization is OK" << std::endl;

    // Server socket binding
    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));	// Initializing servInfo structure

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(PORT_NUM);

    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if ( erStat != 0 ) {
        std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Binding socket to Server info is OK" << std::endl;
//--------------------------------------------------------------------------// Write in file
//QString fileName;                                                           //
//int CurrentGameID = 0;                                                      // ID текущей игры (для записи в файл и конектов к ней)
//std::string ToWriteInFile = std::to_string(CurrentGameID);                  //
//fileName = "D:/Models/OTUS/server-client/game_status" +                     //
//                    QString::fromStdString(ToWriteInFile) + "_" +           //
//                    QString::fromStdString(ToWriteInFile) + ".txt";         //
//QFile file(fileName);                                                       //
//if (!file.open( QIODevice::WriteOnly | QIODevice::Text )) {                 //
//    qDebug() << "Не удалось открыть файл»";                                 //
//    return 1;                                                               //
//}                                                                           //
//QTextStream stream(&file);                                                  //
std::string answer;                                                         //
//--------------------------------------------------------------------------//
    while (true)                                                            // Цикл № 2 - отправка сообщений во время проведения игры
    {
        int max_round_count = 100;
        int current_round = 0;
        while(current_round < max_round_count)
        {
            //Starting to listen to any Clients
            erStat = listen(ServSock, SOMAXCONN);

            if ( erStat != 0 ) {
                std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
                closesocket(ServSock);
                WSACleanup();
                return 1;
            }
            else {
                std::cout << "Listening..." << std::endl;
            }
            //Client socket creation and acception in case of connection
            sockaddr_in clientInfo;
            ZeroMemory(&clientInfo, sizeof(clientInfo));                                // Initializing clientInfo structure
            int clientInfo_size = sizeof(clientInfo);
            SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
            if (ClientConn == INVALID_SOCKET) {
                std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ServSock);
                closesocket(ClientConn);
                WSACleanup();
                return 1;
            }
            else {
                std::cout << "Connection to a client established successfully" << std::endl;
                char clientIP[22];
                inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format
                std::cout << "Client connected with IP address " << clientIP << std::endl;
            }
            //Exchange text data between Server and Client. Disconnection if a client send "xxx"
            std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);              // Creation of buffers for sending and receiving data
            short packet_size = 0;                                                      // The size of sending / receiving packet in bytes

            current_round++;
std::cout << "_________________________________________________" << std::endl;
//stream << "_________________________________________________" << endl;
answer = "_________________________________________________";
            clientBuff = clearBuf(clientBuff);
            clientBuff = convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ServSock);
                closesocket(ClientConn);
                WSACleanup();
                return 1;
            }
std::cout << "        Round = " << current_round << std::endl;
//stream << "        Round = " << current_round << endl;
answer = "        Round = " + std::to_string(current_round);
            clientBuff = clearBuf(clientBuff);
            clientBuff = convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ServSock);
                closesocket(ClientConn);
                WSACleanup();
                return 1;
            }
            answer = "Write your action: ";
            clientBuff = clearBuf(clientBuff);
            clientBuff = convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ServSock);
                closesocket(ClientConn);
                WSACleanup();
                return 1;
            }
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            std::string UserAction;
            UserAction = convert_char_to_string(servBuff, UserAction);
            std::cout << UserAction << std::endl;
//            stream << QString::fromStdString(NamePlayers.at(0)) << " using " << QString::fromStdString(UserAction) << endl;
            closesocket(ClientConn);
        }
    }

//    file.close();
    closesocket(ServSock);
    WSACleanup();

    return 0;
}
