#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <QString>
#include <QDir>
#include <QDebug>

#pragma comment(lib, "Ws2_32.lib")

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


int main(void)
{

    //Key constants
    const char IP_SERV[] = "10.124.40.14";	// Enter local Server IP address
    const int PORT_NUM = 8080;				// Enter Open working server port
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

    while (true)
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
        ZeroMemory(&clientInfo, sizeof(clientInfo));	// Initializing clientInfo structure

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

        std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);			// Creation of buffers for sending and receiving data
        short packet_size = 0;												// The size of sending / receiving packet in bytes

//----------------------------------------------------------------------------------
        clientBuff = clearBuf(clientBuff);
        servBuff = clearBuf(servBuff);
        std::string answer = "Create game? ";
        clientBuff = convert_string_to_char(clientBuff, answer);
        std::cout << clientBuff.data();

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
        std::string isCreating;
        isCreating = convert_char_to_string(servBuff, isCreating);
        std::cout << servBuff.data();
//----------------------------------------------------------------------------------
        clientBuff = clearBuf(clientBuff);
        answer = "Number players: ";
        clientBuff = convert_string_to_char(clientBuff, answer);
        std::cout << clientBuff.data();

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
        std::string numberPlayers;
        numberPlayers = convert_char_to_string(servBuff, numberPlayers);
        std::cout << servBuff.data();
//----------------------------------------------------------------------------------
        std::string answer1 = "Yes";
        std::string answer2_1 = "1";
        std::string answer2_2 = "2";
        std::string answer2_3 = "3";
        std::string answer2_4 = "4";
        std::string answer2_5 = "5";

        std::cout << "numberPlayers " << numberPlayers << std::endl;
        std::cout << "isCreating " << isCreating << std::endl;

        if(((numberPlayers == answer2_1) || (numberPlayers == answer2_2) || (numberPlayers == answer2_3) || (numberPlayers == answer2_4) || (numberPlayers == answer2_5))
                && (isCreating == answer1))
            answer = "Start creating game ...";
        else
        {
            answer = "Your answers is false!";
            shutdown(ClientConn, SD_BOTH);
            closesocket(ServSock);
            closesocket(ClientConn);
            WSACleanup();
            return 0;
        }
        clientBuff = clearBuf(clientBuff);
        clientBuff = convert_string_to_char(clientBuff, answer);

        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);

        QString fileName = "D:/Models/OTUS/server-client/game_status.txt";
        QFile file(fileName);
        //file.rename("D:/Models/OTUS/server-client/game_status.txt");
        QString A5 = QFileInfo(fileName).absoluteFilePath();
        if (!file.open( QIODevice::WriteOnly | QIODevice::Text )) {
            qDebug() << "Не удалось открыть файл»";
            return 1;
        }
        QTextStream stream(&file);
        stream << "Create game" << endl;
        QString strX = QString::fromStdString(numberPlayers);
        stream << "Number players: " << strX << endl;
        file.close();

        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
            closesocket(ServSock);
            closesocket(ClientConn);
            WSACleanup();
            return 1;
        }
        closesocket(ClientConn);
//----------------------------------------------------------------------------------
    }

    closesocket(ServSock);
    WSACleanup();

    return 0;

}
