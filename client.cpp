#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>
#include <QString>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

std::vector <char> clearBuf(std::vector <char> result)
{
    result.erase(result.begin(), result.end());
    const short BUFF_SIZE = 1024;
    for(int i = BUFF_SIZE - 1; i > -1; i--)
        result.emplace(result.begin(), '\0');
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

int main(void)
{
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start CLIENT 1                           " << std::endl;
std::cout << "_________________________________________________" << std::endl;
//--------------------------------------------------------------------------------
// autorised
//--------------------------------------------------------------------------------
    //Key constants
    const char SERVER_IP_A[] = "10.124.40.14";		// Enter IPv4 address of Server
    const short SERVER_PORT_NUM_A = 650;			// Enter Listening port on Server side
    const short BUFF_SIZE = 1024;					// Maximum size of buffer for exchange info between server and client
    int erStat;										// For checking errors in sockets functions
    in_addr ip_to_num;
    inet_pton(AF_INET, SERVER_IP_A, &ip_to_num);
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if (erStat != 0) {
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
        return 1;
    }
    else
        cout << "WinSock initialization is OK" << endl;
    //while (true)
    //{
    SOCKET ClientSock_a = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSock_a == INVALID_SOCKET) {
        cout << "Error initialization socket # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
    }
    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(SERVER_PORT_NUM_A);
    erStat = connect(ClientSock_a, (sockaddr*)&servInfo, sizeof(servInfo));
    if (erStat != 0) {
        cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
        return 1;
    }
    vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);           // Buffers for sending and receiving data
    short packet_size = 0;												// The size of sending / receiving packet in bytes
    packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data();
    clientBuff = clearBuf(clientBuff);
    fgets(clientBuff.data(), clientBuff.size(), stdin);
    packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
        return 1;
    }
    packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data();
    clientBuff = clearBuf(clientBuff);
    fgets(clientBuff.data(), clientBuff.size(), stdin);
    packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
        return 1;
    }
    packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_a);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data() << endl;;
    closesocket(ClientSock_a);
    //}
    WSACleanup();
//--------------------------------------------------------------------------------
// create game
//--------------------------------------------------------------------------------
    //Key constants
    const char SERVER_IP_G[] = "10.124.40.14";		// Enter IPv4 address of Server
    const short SERVER_PORT_NUM_G = 8080;			// Enter Listening port on Server side
    //const short BUFF_SIZE = 1024;					// Maximum size of buffer for exchange info between server and client
    //int erStat;									// For checking errors in sockets functions
    //in_addr ip_to_num;
    inet_pton(AF_INET, SERVER_IP_G, &ip_to_num);
    //WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if (erStat != 0) {
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
        return 1;
    }
    else
        cout << "WinSock initialization is OK" << endl;
    //while (true)
    //{
    SOCKET ClientSock_g = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSock_g == INVALID_SOCKET) {
        cout << "Error initialization socket # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
    }
    //sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(SERVER_PORT_NUM_G);
    erStat = connect(ClientSock_g, (sockaddr*)&servInfo, sizeof(servInfo));
    if (erStat != 0) {
        cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    //vector <char> servBuff(BUFF_SIZE_G), clientBuff(BUFF_SIZE_G);		// Buffers for sending and receiving data
    //short packet_size = 0;											// The size of sending / receiving packet in bytes
    packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Create game?"
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data();
    clientBuff = clearBuf(clientBuff);
    fgets(clientBuff.data(), clientBuff.size(), stdin);
    packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Number players:"
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data();
    clientBuff = clearBuf(clientBuff);
    fgets(clientBuff.data(), clientBuff.size(), stdin);
    std::string numberPlayers;
    numberPlayers = convert_char_to_string(clientBuff, numberPlayers);
    packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Start creating game ..."
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data() << endl;
    packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Need concrete players?"
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    else
        cout << servBuff.data();
    clientBuff = clearBuf(clientBuff);
    fgets(clientBuff.data(), clientBuff.size(), stdin);
    packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if concrete players
    if (packet_size == SOCKET_ERROR) {
        cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock_g);
        WSACleanup();
        return 1;
    }
    std::string isConcrete;
    isConcrete = convert_char_to_string(clientBuff, isConcrete);
    std::string answer1 = "Yes";
    if(isConcrete == answer1)
    {
        int numPlayers = std::stoi(numberPlayers);
        for(int i = 0; i < numPlayers; i++)
        {
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Name concrete player"
            if (packet_size == SOCKET_ERROR) {
                cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            else
                cout << servBuff.data();
            //packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
            clientBuff = clearBuf(clientBuff);
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if concrete players
            if (packet_size == SOCKET_ERROR) {
                cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
        }
        packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Name concrete player"   // <== "Start creating game ..."
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_g);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data();
    }
    closesocket(ClientSock_g);
    //}
    WSACleanup();
//--------------------------------------------------------------------------------

    return 0;
}
