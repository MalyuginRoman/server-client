#include <iostream>
#include <stdio.h>
#include <vector>
#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 

using namespace std;


int main(void)
{
    //Key constants
    const char SERVER_IP[] = "10.124.40.14";
    const short SERVER_PORT_NUM = 650;
    const short BUFF_SIZE = 1024;

    int erStat;

    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, SERVER_IP, &ip_to_num);
    if (erStat <= 0)
    {
        cout << "Error in IP translation to special numeric format" << endl;
        return 1;
    }
    
    int ServSock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servInfo;
    memset(&servInfo, '0', sizeof(servInfo));
        
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(SERVER_PORT_NUM);
        
    bind(ServSock, (struct sockaddr*)&servInfo, sizeof(servInfo)); 
    
    while (true)
    {
        listen(ServSock , SOMAXCONN);
        
        socklen_t clientInfo_size = sizeof(clientInfo);
        int ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

        vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
        short packet_size = 0;

        packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

        fgets(clientBuff.data(), clientBuff.size(), stdin);
        packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

        packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

        fgets(clientBuff.data(), clientBuff.size(), stdin);
        packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

        packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

        closesocket(ClientSock);
    }
    return 0;
}
