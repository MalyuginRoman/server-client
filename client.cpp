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

int main(void)
{
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start CLIENT 1                           " << std::endl;
std::cout << "_________________________________________________" << std::endl;
//--------------------------------------------------------------------------------
// autorised
//--------------------------------------------------------------------------------
    //Key constants
    std::string playerName;
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
    vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);           // Buffers for sending and receiving data
    short packet_size = 0;												// The size of sending / receiving packet in bytes
    bool isNeedAutorization = true;
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
    while(isNeedAutorization)
    {
        packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);  // <== "Your login: "
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data();
        clientBuff = clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);                     // <== User write login
        packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        playerName = convert_char_to_string(clientBuff, playerName);
        packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);  // <== "Your password: "
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data();
        clientBuff = clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);                     // <== User write password
        packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);
        std::string answer;
        answer = convert_char_to_string(servBuff, answer);
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_a);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data() << endl;
        std::string isGood = "Your autorized!";
        if(answer == isGood)
        {
            isNeedAutorization = false;
            closesocket(ClientSock_a);
        }
        clientBuff = clearBuf(clientBuff);
        servBuff = clearBuf(servBuff);
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
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
        return 1;
    }
    else
        cout << "WinSock initialization is OK" << endl;

    bool isWork = true;
    while(isWork)
    {
        SOCKET ClientSock_g = socket(AF_INET, SOCK_STREAM, 0);
        if (ClientSock_g == INVALID_SOCKET) {
            cout << "Error initialization socket # " << WSAGetLastError() << endl;
            closesocket(ClientSock_g);
            WSACleanup();
        }
        sockaddr_in servInfo;
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
        clientBuff = clearBuf(clientBuff);
        clientBuff = convert_string_to_char(clientBuff, playerName);        // отправляем имя подключившегося игрока
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
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_g);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data() << endl;
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
        std::string isCreating;
        isCreating = convert_char_to_string(clientBuff, isCreating);
        std::string answerYes = "Yes";
        std::string answerNo  = "No" ;
        if(isCreating == answerYes)
        {
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
            if(isConcrete == answerYes)
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
                    cout << servBuff.data() << endl;
            }
            isWork = false;                 // выходим из цикла Create Game
        }
        else if(isCreating == answerNo)
        {
            packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Connection to Game? "
            if (packet_size == SOCKET_ERROR) {
                cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            else
                cout << servBuff.data();
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if connection to Game
            if (packet_size == SOCKET_ERROR) {
                cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
                closesocket(ClientSock_g);
                WSACleanup();
                return 1;
            }
            std::string isConnection;
            isConnection = convert_char_to_string(clientBuff, isConnection);
            if(isConnection == answerYes)
            {
                //...
                packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Write game ID: "
                if (packet_size == SOCKET_ERROR) {
                    cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                else
                    cout << servBuff.data();
                fgets(clientBuff.data(), clientBuff.size(), stdin);
                packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // game ID
                if (packet_size == SOCKET_ERROR) {
                    cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                //...
                packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientSock_g);
                    WSACleanup();
                    return 1;
                }
                else
                    std::cout << servBuff.data() << endl;
                std::string isPlayerConection;
                isPlayerConection = convert_char_to_string(servBuff, isPlayerConection);
                if(isPlayerConection == "You are connection in game")
                {
                    //...
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
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
        return 1;
    }
    else
        cout << "WinSock initialization is OK" << endl;
    int max_round_count = 100;
    int current_round = 0;
    while(current_round < max_round_count)
    {
        SOCKET ClientSock_c = socket(AF_INET, SOCK_STREAM, 0);
        if (ClientSock_c == INVALID_SOCKET) {
            cout << "Error initialization socket # " << WSAGetLastError() << endl;
            closesocket(ClientSock_c);
            WSACleanup();
        }
        sockaddr_in servInfo;
        ZeroMemory(&servInfo, sizeof(servInfo));
        servInfo.sin_family = AF_INET;
        servInfo.sin_addr = ip_to_num;
        servInfo.sin_port = htons(SERVER_PORT_NUM_C);
        erStat = connect(ClientSock_c, (sockaddr*)&servInfo, sizeof(servInfo));
        if (erStat != 0) {
            cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        current_round++;
        packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "________"
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data() << endl;
        packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Round = ..."
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data() << endl;
        packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write your action: "
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        else
            cout << servBuff.data() << endl;
        clientBuff = clearBuf(clientBuff);
        fgets(clientBuff.data(), clientBuff.size(), stdin);
        packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock_c);
            WSACleanup();
            return 1;
        }
        closesocket(ClientSock_c);
    }
    WSACleanup();
//--------------------------------------------------------------------------------
    return 0;
}
