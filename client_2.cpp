#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>
#include <QString>

#pragma comment(lib, "ws2_32.lib")

#include "dop_function.h"

SOCKET isConnect(in_addr ip_to_num, short SERVER_PORT_NUM, int erStat)
{
    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);
        if (ClientSock == INVALID_SOCKET) {
            std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock);
            WSACleanup();
        }
    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(SERVER_PORT_NUM);
    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));
    if (erStat != 0) {
        std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
        closesocket(ClientSock);
        WSACleanup();
        return false;
    }
    return ClientSock;
}

int main(void)
{
    dop_function df;
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start CLIENT 2                           " << std::endl;
std::cout << "_________________________________________________" << std::endl;
//--------------------------------------------------------------------------------
// autorised
//--------------------------------------------------------------------------------
    //Key constants
    std::string playerName;
    int usingGameID;
    const char SERVER_IP_A[] = "10.124.40.14";		// Enter IPv4 address of Server
    const short SERVER_PORT_NUM_A = 650;			// Enter Listening port on Server side
    const short BUFF_SIZE = 1024;					// Maximum size of buffer for exchange info between server and client
    int erStat;										// For checking errors in sockets functions
    in_addr ip_to_num;
    inet_pton(AF_INET, SERVER_IP_A, &ip_to_num);
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if (erStat != 0) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
        std::cout << "WinSock initialization is OK" << std::endl;
    std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);           // Buffers for sending and receiving data
    short packet_size = 0;												// The size of sending / receiving packet in bytes
    bool isNeedAutorization = true;
    SOCKET ClientSock_a = isConnect(ip_to_num, SERVER_PORT_NUM_A, erStat);
    while(isNeedAutorization)
    {
        packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);  // <== "Your login: "
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data();
        clientBuff = df.clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);                     // <== User write login
        packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        playerName = df.convert_char_to_string(clientBuff, playerName);
        packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);  // <== "Your password: "
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data();
        clientBuff = df.clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);                     // <== User write password
        packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);
        std::string answer;
        answer = df.convert_char_to_string(servBuff, answer);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data() << std::endl;
        std::string isGood = "Your autorized!";
        if(answer == isGood)
        {
            isNeedAutorization = false;
            closesocket(ClientSock_a);
        }
        clientBuff = df.clearBuf(clientBuff);
        servBuff = df.clearBuf(servBuff);
    }
//--------------------------------------------------------------------------------
// Create game
//--------------------------------------------------------------------------------
    //Key constants
    const char SERVER_IP_G[] = "10.124.40.14";		// Enter IPv4 address of Server
    const short SERVER_PORT_NUM_G = 8080;			// Enter Listening port on Server side
    inet_pton(AF_INET, SERVER_IP_G, &ip_to_num);
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if (erStat != 0) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
        std::cout << "WinSock initialization is OK" << std::endl;

    bool isWork = true;
    while(isWork)
    {
        SOCKET ClientSock_g = isConnect(ip_to_num, SERVER_PORT_NUM_G, erStat);
        clientBuff = df.clearBuf(clientBuff);
        clientBuff = df.convert_string_to_char(clientBuff, playerName);        // отправляем имя подключившегося игрока
        //std::cout << clientBuff.data() << std::endl;
        packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_g);
            WSACleanup();
            return 1;
        }
        packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "What are you want?"
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_g);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data() << std::endl;
        //vector <char> servBuff(BUFF_SIZE_G), clientBuff(BUFF_SIZE_G);		// Buffers for sending and receiving data
        //short packet_size = 0;											// The size of sending / receiving packet in bytes
        packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Create game?"
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_g);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data();
        clientBuff = df.clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);
        packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_g);
            WSACleanup();
            return 1;
        }
        std::string isCreating;
        isCreating = df.convert_char_to_string(clientBuff, isCreating);
        std::string answerYes = "Yes";
        std::string answerNo  = "No" ;
        if(isCreating == answerYes)
        {
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Number players:"
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            else
                std::cout << servBuff.data();
            clientBuff = df.clearBuf(clientBuff);
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            std::string numberPlayers;
            numberPlayers = df.convert_char_to_string(clientBuff, numberPlayers);
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Start creating game ..."
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            else
                std::cout << servBuff.data() << std::endl;
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Need concrete players?"
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            else
                std::cout << servBuff.data();
            clientBuff = df.clearBuf(clientBuff);
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if concrete players
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            std::string isConcrete;
            isConcrete = df.convert_char_to_string(clientBuff, isConcrete);
            if(isConcrete == answerYes)
            {
                int numPlayers = std::stoi(numberPlayers);
                for(int i = 0; i < numPlayers; i++)
                {
                    packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Name concrete player"
                    if (packet_size == SOCKET_ERROR) {
                        std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientSock_g);
                        WSACleanup();
                        return 1;
                    }
                    else
                        std::cout << servBuff.data();
                    clientBuff = df.clearBuf(clientBuff);
                    fgets(clientBuff.data(), clientBuff.size(), stdin);
                    packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if concrete players
                    if (packet_size == SOCKET_ERROR) {
                        std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientSock_g);
                        WSACleanup();
                        return 1;
                    }
                }
                packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Name concrete player"   // <== "Start creating game ..."
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                else
                    std::cout << servBuff.data() << std::endl;
            }
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== get using GameID
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            std::string usingGameIDstring = df.convert_char_to_string(servBuff, usingGameIDstring);
            usingGameID = stoi(usingGameIDstring);
            isWork = false;                 // выходим из цикла Create Game
        }
        else if(isCreating == answerNo)
        {
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Connection to Game? "
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            else
                std::cout << servBuff.data();
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if connection to Game
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            std::string isConnection;
            isConnection = df.convert_char_to_string(clientBuff, isConnection);
            if(isConnection == answerYes)
            {
                //...
                packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Write game ID: "
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                else
                    std::cout << servBuff.data();
                fgets(clientBuff.data(), clientBuff.size(), stdin);
                packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // game ID
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                std::string usingGameIDstring;
                usingGameIDstring = df.convert_char_to_string(clientBuff, usingGameIDstring);
                //...
                packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                else
                    std::cout << servBuff.data() << std::endl;
                std::string isPlayerConection;
                isPlayerConection = df.convert_char_to_string(servBuff, isPlayerConection);
                if(isPlayerConection == "You are connection in game")
                {
                    //...
                    packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== get using GameID
                    if (packet_size == SOCKET_ERROR) {
                        std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientSock_g);
                        WSACleanup();
                        return 1;
                    }
                    usingGameID = stoi(usingGameIDstring);
                    isWork = false;                 // выходим из цикла Create Game
                }
                else if(isPlayerConection == "You are not connection in game")
                {
                    //...                           // повторяем цикл
                }
            }
            else if(isConnection == answerNo)
            {
                //...
            }
            else
            {
                std::cout << "Invalid answer " << std::endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
        }
        else
        {
            std::cout << "Invalid answer " << std::endl;
            closesocket(ClientSock_g);
        }
        closesocket(ClientSock_g);
    }
//-------------------------------------------------------------------------------- Start game - UserAction raund by raund
// Start game
//--------------------------------------------------------------------------------
    //Key constants
    const char SERVER_IP_C[] = "10.124.40.14";		// Enter IPv4 address of Server
    const short SERVER_PORT_NUM_C = 8008;			// Enter Listening port on Server side
    inet_pton(AF_INET, SERVER_IP_C, &ip_to_num);
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if (erStat != 0) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
        std::cout << "WinSock initialization is OK" << std::endl;
    int max_round_count = 100;
    int current_round = 0;
    while(current_round < max_round_count)
    {
        SOCKET ClientSock_c = isConnect(ip_to_num, SERVER_PORT_NUM_C, erStat);
        current_round++;
        clientBuff = df.clearBuf(clientBuff);
        std::string answer = playerName;                                        // получение UserName
        clientBuff = df.convert_string_to_char(clientBuff, answer);
        packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        clientBuff = df.clearBuf(clientBuff);
        answer = std::to_string(usingGameID);                                   // получение GameID
        clientBuff = df.convert_string_to_char(clientBuff, answer);
        packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "________"
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data() << std::endl;
        packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Round = ..."
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data() << std::endl;
        packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write your action: "
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        else
            std::cout << servBuff.data() << std::endl;
        clientBuff = df.clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);
        packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        std::string UserAction;
        UserAction = df.convert_char_to_string(clientBuff, UserAction);
        std::string answerStartMove = "StartMove" ;
        std::string answerRotate = "Rotate" ;
        if(UserAction == answerStartMove)
        {
            packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write initial velocity: "
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_c);
                WSACleanup();
                return 1;
            }
            else
                std::cout << servBuff.data();
            clientBuff = df.clearBuf(clientBuff);
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_c);
                WSACleanup();
                return 1;
            }
        }
        else if(UserAction == answerRotate)
        {
            packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write angle: "
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_c);
                WSACleanup();
                return 1;
            }
            else
                std::cout << servBuff.data();
            clientBuff = df.clearBuf(clientBuff);
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientSock_c);
                WSACleanup();
                return 1;
            }
        }

        closesocket(ClientSock_c);
    }
    WSACleanup();
//--------------------------------------------------------------------------------
    return 0;
}
