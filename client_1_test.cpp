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
int isConnect(in_addr ip_to_num, short SERVER_PORT_NUM, int erStat)
{
    int CreateSocket = 0;
    struct sockaddr_in ipOfServer;
    if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("Socket not created \n");
        return 1;
    }
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr = ip_to_num;
    ipOfServer.sin_port = htons(SERVER_PORT_NUM);
    if(connect(CreateSocket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer))<0)
    {
        printf("Connection failed due to port and ip problems\n");
        return 1;
    }
    return CreateSocket;
}
int main(void)
{
    dop_function df;
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start CLIENT 1                           " << std::endl;
std::cout << "_________________________________________________" << std::endl;
//--------------------------------------------------------------------------------
// autorised
//--------------------------------------------------------------------------------
    std::string playerName;
    int usingGameID;
    const char SERVER_IP_A[] = "10.124.40.14";
    const short SERVER_PORT_NUM_A = 650;
    const short BUFF_SIZE = 1024;
    int erStat;
    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, SERVER_IP_A, &ip_to_num);
    std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
    short packet_size = 0;
    bool isNeedAutorization = true;
    int ClientSock_a = 220; // isConnect(ip_to_num, SERVER_PORT_NUM_A, erStat);
    while(isNeedAutorization)
    {
        //packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);  // <== "Your login: "
        clientBuff = df.clearBuf(clientBuff);
        //fgets(clientBuff.data(), clientBuff.size(), stdin);                     // <== User write login
            answer = "PetrPetrov";
            clientBuff = df.convert_string_to_char(clientBuff, answer);
        packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
        playerName = df.convert_char_to_string(clientBuff, playerName);
        //packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);  // <== "Your password: "
        clientBuff = df.clearBuf(clientBuff);
        //fgets(clientBuff.data(), clientBuff.size(), stdin);                     // <== User write password
            answer = "123";
            clientBuff = df.convert_string_to_char(clientBuff, answer);
        packet_size = send(ClientSock_a, clientBuff.data(), clientBuff.size(), 0);
        //packet_size = recv(ClientSock_a, servBuff.data(), servBuff.size(), 0);
        std::string answer;
        answer = df.convert_char_to_string(servBuff, answer);
        std::string isGood = "Your autorized!";
        if(answer == isGood)
        {
            isNeedAutorization = false;
            close(ClientSock_a);
        }
        clientBuff = df.clearBuf(clientBuff);
        servBuff = df.clearBuf(servBuff);
    }
std::cout << "End autorization" << std::endl;
//--------------------------------------------------------------------------------
// Create game
//--------------------------------------------------------------------------------
    bool isNonStop = true;
    while(isNonStop)
    {
        const char SERVER_IP_G[] = "10.124.40.14";
        const short SERVER_PORT_NUM_G = 8080;
        erStat = inet_pton(AF_INET, SERVER_IP_G, &ip_to_num);
        bool isWork = true;
        while(isWork)
        {
            int ClientSock_g = 228; // isConnect(ip_to_num, SERVER_PORT_NUM_G, erStat);
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, playerName);        // отправляем имя подключившегося игрока
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
            //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "What are you want?"
            //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Create game?"
            clientBuff = df.clearBuf(clientBuff);
            fgets(clientBuff.data(), clientBuff.size(), stdin);
            packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
            std::string isCreating;
            isCreating = df.convert_char_to_string(clientBuff, isCreating);
            std::string answerYes = "Yes";
            std::string answerNo  = "No" ;
            if(isCreating == answerYes)
            {
                //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Number players:"
                clientBuff = df.clearBuf(clientBuff);
                fgets(clientBuff.data(), clientBuff.size(), stdin);
                std::string numberPlayers;
                numberPlayers = df.convert_char_to_string(clientBuff, numberPlayers);
                packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);
                //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Start creating game ..."
                //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Need concrete players?"
                clientBuff = df.clearBuf(clientBuff);
                //fgets(clientBuff.data(), clientBuff.size(), stdin);
                    answer = "No";
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if concrete players
                std::string isConcrete;
                isConcrete = df.convert_char_to_string(clientBuff, isConcrete);
                if(isConcrete == answerYes)
                {
                    int numPlayers = std::stoi(numberPlayers);
                    for(int i = 0; i < numPlayers; i++)
                    {
                        //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Name concrete player"
                        clientBuff = df.clearBuf(clientBuff);
                        //fgets(clientBuff.data(), clientBuff.size(), stdin);
                            answer = "4";
                            clientBuff = df.convert_string_to_char(clientBuff, answer);
                        packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if concrete players
                    }
                    //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Start creating game ..."
                }
                //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== get using GameID
                std::string usingGameIDstring = df.convert_char_to_string(servBuff, usingGameIDstring);
                usingGameID = stoi(usingGameIDstring);
                isWork = false;                 // выходим из цикла Create Game
            }
            else if(isCreating == answerNo)
            {
                //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Connection to Game? "
                //fgets(clientBuff.data(), clientBuff.size(), stdin);
                    answer = "Yes";
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // Yes if connection to Game
                std::string isConnection;
                isConnection = df.convert_char_to_string(clientBuff, isConnection);
                if(isConnection == answerYes)
                {
                    //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Write game ID: "
                    //fgets(clientBuff.data(), clientBuff.size(), stdin);
                        answer = "5";
                        clientBuff = df.convert_string_to_char(clientBuff, answer);
                    packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // game ID
                    std::string usingGameIDstring;
                    usingGameIDstring = df.convert_char_to_string(clientBuff, usingGameIDstring);
                    //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);
                    std::string isPlayerConection;
                    isPlayerConection = "Yes"; //df.convert_char_to_string(servBuff, isPlayerConection);
                    if(isPlayerConection == "You are connection in game")
                    {
                        usingGameID = stoi(usingGameIDstring);
                        isWork = false;                 // выходим из цикла Create Game
                    }
                }
                else if(isConnection == answerNo)
                {
                    //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "View game statuses?"
                    //fgets(clientBuff.data(), clientBuff.size(), stdin);
                        answer = "Yes";
                        clientBuff = df.convert_string_to_char(clientBuff, answer);
                    packet_size = send(ClientSock_g, clientBuff.data(), clientBuff.size(), 0);  // "Yes" or "No"
                    std::string isViewing;
                    isViewing = df.convert_char_to_string(clientBuff, isViewing);
                    if(isViewing == answerYes)
                    {
                        std::cout << "Game ready to start "; //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Game ready to start"
                        std::cout << servBuff.data() << std::endl;
                        std::cout << "Game are going "; //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Game are going"
                        std::cout << servBuff.data() << std::endl;
                        std::cout << "Game are ending "; //packet_size = recv(ClientSock_g, servBuff.data(), servBuff.size(), 0);  // <== "Game are ending"
                        std::cout << servBuff.data() << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid answer " << std::endl;
                    close(ClientSock_g);
                    return 1;
                }
            }
            else
            {
                std::cout << "Invalid answer " << std::endl;
                close(ClientSock_g);
            }
            close(ClientSock_g);
        }
std::cout << "End game create" << std::endl;
//-------------------------------------------------------------------------------- Start game - UserAction raund by raund
// Start game
//--------------------------------------------------------------------------------
        const char SERVER_IP_C[] = "10.124.40.14";
        const short SERVER_PORT_NUM_C = 8008;
        erStat = inet_pton(AF_INET, SERVER_IP_C, &ip_to_num);
        int max_round_count = 100;
        int current_round = 0;
        while(current_round < max_round_count)
        {
            int ClientSock_c = 228; // isConnect(ip_to_num, SERVER_PORT_NUM_C, erStat);
            current_round++;
            //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== Game status - resume or over
            std::string Game_status;
            Game_status = df.convert_char_to_string(servBuff, Game_status);
            if(Game_status == "Game Over!")
            {
                close(ClientSock_c);
                break;
            }
            clientBuff = df.clearBuf(clientBuff);
            std::string answer = playerName;                                        // получение UserName
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            clientBuff = df.clearBuf(clientBuff);
            answer = std::to_string(usingGameID);                                   // получение GameID
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "________"
            //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Round = ..."
            //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write your action: "
            clientBuff = df.clearBuf(clientBuff);
            //fgets(clientBuff.data(), clientBuff.size(), stdin);
                        answer = "Go";
                        clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            std::string UserAction;
            UserAction = df.convert_char_to_string(clientBuff, UserAction);
            std::string answerStartMove = "StartMove" ;
            std::string answerRotate = "Rotate" ;
            if(UserAction == answerStartMove)
            {
                //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write initial velocity: "
                clientBuff = df.clearBuf(clientBuff);
                //fgets(clientBuff.data(), clientBuff.size(), stdin);
                        answer = "5";
                        clientBuff = df.convert_string_to_char(clientBuff, answer);
                packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            }
            else if(UserAction == answerRotate)
            {
                //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Write angle: "
                clientBuff = df.clearBuf(clientBuff);
                //fgets(clientBuff.data(), clientBuff.size(), stdin);
                        answer = "90";
                        clientBuff = df.convert_string_to_char(clientBuff, answer);
                packet_size = send(ClientSock_c, clientBuff.data(), clientBuff.size(), 0);
            }
            //packet_size = recv(ClientSock_c, servBuff.data(), servBuff.size(), 0);  // <== "Resume Game!!!" or "Game over!!!"
            std::string isGameStatus = "Game over!!!";
            isGameStatus = df.convert_char_to_string(servBuff, isGameStatus);
            if(isGameStatus == "Game over!!!")
            {
                close(ClientSock_c);
                break;
            }
            close(ClientSock_c);
        }
std::cout << "End game" << std::endl;
//--------------------------------------------------------------------------------
    }
    return 0;
}
