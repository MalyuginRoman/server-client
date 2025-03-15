#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <cstring>
#include <vector>
#include "dop_function.h"
int main(void)
{
    dop_function df;
    bool isNeedAutorized = true;
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start AUTORIZED SERVER                   " << std::endl;
std::cout << "_________________________________________________" << std::endl;
std::cout << "001 _________________________________________________" << std::endl;
    const char IP_SERV[] = "10.124.40.14";
std::cout << "002 _________________________________________________" << std::endl;
    const int PORT_NUM = 650;
std::cout << "003 _________________________________________________" << std::endl;
    const short BUFF_SIZE = 1024;
std::cout << "004 _________________________________________________" << std::endl;
    int erStat;
std::cout << "005 _________________________________________________" << std::endl;
    in_addr ip_to_num;
std::cout << "00 _________________________________________________" << std::endl;
    erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);
std::cout << "01 _________________________________________________" << std::endl;
    int ServSock = socket(AF_INET, SOCK_STREAM, 0);
std::cout << "02 _________________________________________________" << std::endl;
    struct sockaddr_in servInfo;
std::cout << "03 _________________________________________________" << std::endl;
    memset(&servInfo, '0', sizeof(servInfo));
std::cout << "04 _________________________________________________" << std::endl;
    servInfo.sin_family = AF_INET;
std::cout << "05 _________________________________________________" << std::endl;
    servInfo.sin_addr = ip_to_num;
std::cout << "06 _________________________________________________" << std::endl;
    servInfo.sin_port = htons(PORT_NUM);
std::cout << "1 _________________________________________________" << std::endl;
    bind(ServSock, (struct sockaddr*)&servInfo, sizeof(servInfo)); 
std::cout << "2 _________________________________________________" << std::endl;
    while(isNeedAutorized)
    {
        listen(ServSock , SOMAXCONN);
std::cout << "3 _________________________________________________" << std::endl;
        struct sockaddr_in clientInfo;
        memset(&clientInfo, '0', sizeof(clientInfo));
        socklen_t clientInfo_size = sizeof(clientInfo);
        int ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
        std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
        short packet_size = 0;
        while (isNeedAutorized)
        {
std::cout << "4 _________________________________________________" << std::endl;
            std::string answer = "Your login: ";
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            std::cout << "Client login: ";
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SO_ERROR)
            {
                close(ServSock);
                close(ClientConn);
                return 1;
            }
            //packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            std::string clientLogin = "PetrPetrov";
            clientLogin = df.convert_char_to_string(servBuff, clientLogin);
            std::cout << servBuff.data();
            clientBuff = df.clearBuf(clientBuff);
            answer = "Your password: ";
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            std::cout << "Client password: ";
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SO_ERROR)
            {
                close(ServSock);
                close(ClientConn);
                return 1;
            }
            //packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            std::string clientPassword = "123";
            clientPassword = df.convert_char_to_string(servBuff, clientPassword);
            std::cout << servBuff.data();
            std::string username1 = "PetrPetrov";
            std::string username2 = "IvanIvanov";
            std::string username3 = "JohnSmith";
            std::string username4 = "Barcelona";
            std::string username5 = "RealMadrid";
            std::string username6 = "ETyumencev";
            std::string usersecrt = "123";
            if(((clientLogin == username1) || (clientLogin == username2) ||
                (clientLogin == username3) || (clientLogin == username4) ||
                (clientLogin == username5) || (clientLogin == username6))
                    && (clientPassword == usersecrt))
            {
                answer = "Your autorized!";
                isNeedAutorized = false;
                std::cout << "Client autorized!" << std::endl;
            }
            else
            {
                answer = "Your are not autorized!";
                std::cout << "Client is not autorized!" << std::endl;
            }
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            if (packet_size == SO_ERROR)
            {
                close(ServSock);
                close(ClientConn);
                return 1;
            }
            clientBuff = df.clearBuf(clientBuff);
            servBuff = df.clearBuf(servBuff);
        }
        close(ClientConn);
        isNeedAutorized = true;
    }
    close(ServSock);
    return 0;
}
