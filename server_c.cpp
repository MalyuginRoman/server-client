//#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <QString>
#include <QDir>
#include <QDebug>

#pragma comment(lib, "Ws2_32.lib")

#include "dop_function.h"
#include "start_game.h"

int main(int ac, char **av)
{
    dop_function df;
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
std::string answer;                                                         //

    while (true)                                                            // Цикл № 2 - отправка сообщений во время проведения игры
    {
        int max_round_count = 100;
        int current_round = 0;
        std::map<std::string, std::function<ICommand*()>> *m_map;
        std::map<std::string, std::string> *m_scope;
        start_game Game1(m_map, m_scope);
        Game1.create_game();
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

//-----------------------------------------------------------------------------------
            servBuff = df.clearBuf(servBuff);
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
                WSACleanup();
                return 1;
            }
            //else
            //    std::cout << servBuff.data() << std::endl;
            std::string UserName;
            UserName = df.convert_char_to_string(servBuff, UserName);

            servBuff = df.clearBuf(servBuff);
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
                WSACleanup();
                return 1;
            }
            //else
            //    std::cout << servBuff.data() << std::endl;
            std::string GameID;
            GameID = df.convert_char_to_string(servBuff, GameID);
//-----------------------------------------------------------------------------------// открываем файл и перезаписываем все содержимоу
            QString fileName = "D:/Models/OTUS/server-client/game_status." +         //
                            QString::fromStdString(GameID) + ".txt";                 // обновляем файл с количеством игр
            std::string readFileName = fileName.toStdString();                       //
            std::fstream readFile(readFileName);                                     //
            QFile file(fileName);                                                    //
            if (!file.open( QIODevice::Append /*ReadWrite*/ | QIODevice::Text )) {   //
                qDebug() << "Не удалось открыть файл»";                              //
                return 1;                                                            //
            }                                                                        //
            QTextStream stream(&file);                                               //
            std::string str;                                                         //
            if (readFile.is_open())                                                  //
            {                                                                        //
                while(readFile >> str)                                               //
                {                                                                    //
                    //if(str == "Game" /*status = "*/)
                    //{
                    //    readFile >> str >> str >> str;
                    //    str = "1";
                    //    stream << QString::fromStdString(str);
                    //}
                    //else
                    readFile >> str;                                            //
                }                                                                    //
            }                                                                        //
//-----------------------------------------------------------------------------------//
            current_round++;
std::cout << "_________________________________________________" << std::endl;
stream << "_________________________________________________" << endl;
answer = "_________________________________________________";
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
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
stream << "        Round = " << current_round << endl;
answer = "        Round = " + std::to_string(current_round);
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
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
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
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
            UserAction = df.convert_char_to_string(servBuff, UserAction);
            std::cout << UserName << "in game with ID:" << GameID << " is " << UserAction << std::endl;
stream << QString::fromStdString(UserName) << " using " << QString::fromStdString(UserAction) << endl;
            closesocket(ClientConn);
            file.close();
        }
    }
    closesocket(ServSock);
    WSACleanup();
    return 0;
}
