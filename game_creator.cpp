#include <stdio.h> // standard input and output library
#include <stdlib.h> // this includes functions regarding memory allocation
#include <string.h> // contains string functions
#include <errno.h> //It defines macros for reporting and retrieving error conditions through error codes
#include <time.h> //contains various functions for manipulating date and time
#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include "game_creator.h"

int game_creator::game_create(std::vector<game> *games)
{
    dop_function df;
    size_t game_count = games->size();
    int gameID;
    bool isAllPlayer = false;
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start GAME Creator                       " << std::endl;
std::cout << "_________________________________________________" << std::endl;
    const char IP_SERV[] = "10.124.40.14";
    const int PORT_NUM = 8080;
    const short BUFF_SIZE = 1024;
    in_addr ip_to_num;
    int servSock = socket(AF_INET, SOCK_STREAM, 0); 
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr = ip_to_num;
    servAddr.sin_port = htons(PORT_NUM);
    bind(servSock, (struct sockaddr*)&servAddr, 
         sizeof(servAddr));
    std::string answerYes = "Yes";
    std::string answerNo  = "No" ;
//--------------------------------------------------------------------------------//
    bool isPlayerConection = false;
    game current_game;
    current_game.gameID = 999;
    while (!isAllPlayer)                                                          // Цикл № 1 - для создания игра / определения наличия и формата доступа клиентов к игре
    {
        listen(servSock, SOMAXCONN); 
        struct sockaddr_in clientInfo;
        memset(&clientInfo, '0', sizeof(clientInfo));
        socklen_t clientInfo_size = sizeof(clientInfo);
        int ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
        std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
        short packet_size = 0;
//----------------------------------------------------------------------------------
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);        // получаем имя подключившегося игрока
        std::string playerName;
        playerName = df.convert_char_to_string(servBuff, playerName);
        std::string answer = "What are you want? ";
        clientBuff = df.clearBuf(clientBuff);
        clientBuff = df.convert_string_to_char(clientBuff, answer);
        std::cout << clientBuff.data() << std::endl;
        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
        clientBuff = df.clearBuf(clientBuff);
        servBuff = df.clearBuf(servBuff);
        answer = "Create game? ";
        clientBuff = df.convert_string_to_char(clientBuff, answer);
        std::cout << clientBuff.data();
        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
        std::string isCreating;
        isCreating = df.convert_char_to_string(servBuff, isCreating);
        std::cout << servBuff.data();
        int numPlayers;                                                             //
        if(isCreating == answerYes)
        {
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start creating GAME                      " << std::endl;
std::cout << "_________________________________________________" << std::endl;
            clientBuff = df.clearBuf(clientBuff);
            answer = "Number players: ";
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            std::cout << clientBuff.data();
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            std::string numberPlayers;
            numberPlayers = df.convert_char_to_string(servBuff, numberPlayers);
            std::cout << servBuff.data();
//--------------------------------------------------------------------------//
            std::string answer2_1 = "1";
            std::string answer2_2 = "2";
            std::string answer2_3 = "3";
            std::string answer2_4 = "4";
            std::string answer2_5 = "5";
            if(((numberPlayers == answer2_1) || (numberPlayers == answer2_2) || (numberPlayers == answer2_3) || (numberPlayers == answer2_4) || (numberPlayers == answer2_5))
                    && (isCreating == answerYes))
                answer = "Start creating game ...";
            else
            {
                answer = "Your answers is false!";
                close(ClientConn);
            }
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            std::cout << answer << std::endl;
//--------------------------------------------------------------------------//
            answer = "Need concrete players? ";
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            std::cout << answer << std::endl;
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            std::string isConcrete;
            isConcrete = df.convert_char_to_string(servBuff, isConcrete);
            numPlayers = std::stoi(numberPlayers);
            std::string SerialNumberPlayer;
            if(isConcrete == answerYes)
            {
                for(int i = 0; i < numPlayers; i++)
                {
                    if(i == 0) SerialNumberPlayer = "first";
                    else if(i == 1) SerialNumberPlayer = "second";
                    else if(i == 2) SerialNumberPlayer = "third";
                    else if(i == 3) SerialNumberPlayer = "fourth";
                    else if(i == 4) SerialNumberPlayer = "fifth";
                    answer = "Write " + SerialNumberPlayer + " players: ";
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
                    std::string NameConcretePlayer;
                    NameConcretePlayer = df.convert_char_to_string(servBuff, NameConcretePlayer);
                    std::cout << SerialNumberPlayer << " : " << servBuff.data() << " ";
                    current_game.player_name.push_back(NameConcretePlayer);
                }
            }
            answer = "Start creating game ...";
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            answer = std::to_string(games->size()/*CurrentGameID*/);
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            current_game.game_status = 0;
            games->push_back(current_game);
        }
        else if(isCreating == answerNo)              // connecting to game
        {
            answer = "Connection to Game? ";
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            std::cout << clientBuff.data();
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            std::cout << servBuff.data();
            std::string isConection;
            isConection = df.convert_char_to_string(servBuff, isConection);
            if(isConection == answerYes)  // при подключении к игре пользователь прошел верификацию
            {
                answer = "Write game ID: ";
                clientBuff = df.clearBuf(clientBuff);
                clientBuff = df.convert_string_to_char(clientBuff, answer);
                std::cout << clientBuff.data();
                packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
                std::cout << servBuff.data();
                std::string isGameID;
                isGameID = df.convert_char_to_string(servBuff, isGameID);
                gameID = stoi(isGameID);
                if(gameID < game_count)
                {
                    isPlayerConection = false;
                    if(current_game.gameID != gameID)
                        current_game = games->at(gameID);
                    for(int c = 0; c < current_game.player_name.size(); c++)
                    {
                        if(playerName == current_game.player_name.at(c))
                            isPlayerConection = true;
                    }
                    if(current_game.game_status == 1)
                    {
                        std::cout << "Game is going now!!!" << std::endl;
                        isPlayerConection = false;
                    }
                    else if(current_game.game_status == 2)
                    {
                        std::cout << "Game is ending!!!" << std::endl;
                        isPlayerConection = false;
                    }
                }
                else if(gameID > game_count)
                {
                    std::cout << "gameID > game_count" << std::endl;
                    isPlayerConection = false;
                }
//--------------------------------------------------------------------------//
                if(isPlayerConection)                                                       // игрок подключен к игре (находится в списке)
                {
                    answer = "You are connection in game";
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    answer = std::to_string(gameID/*CurrentGameID*/);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    current_game.connection_player ++;
                    isAllPlayer = (current_game.connection_player == current_game.player_name.size());
                }
                else if(!isPlayerConection)                                                 // игрок отсутствует в списке (повторяем цикл)
                {
                    answer = "You are not connection in game";
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                }
            }
            else if(isConection == answerNo)  // при подключении к игре пользователь не прошел верификацию
            {
                answer = "View game statuses? ";
                clientBuff = df.clearBuf(clientBuff);
                clientBuff = df.convert_string_to_char(clientBuff, answer);
                std::cout << clientBuff.data();
                packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
                std::cout << servBuff.data();
                std::string isViewing;
                isViewing = df.convert_char_to_string(servBuff, isViewing);
                if(isViewing == answerYes)
                {
                    int Game_Count = games->size();
                    int game0 = 0, game1 = 0, game2 = 0;
                    for(int c = 0; c < Game_Count; c++)
                    {
                        if(games->at(c).game_status == 0)
                            game0 ++;
                        else if(games->at(c).game_status == 1)
                            game1 ++;
                        else if(games->at(c).game_status == 2)
                            game2 ++;
                    }
                    answer = "Game ready to start: " + std::to_string(game0);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    answer = "Game are going: " + std::to_string(game1);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    answer = "Game are ending: " + std::to_string(game2);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                }
                isPlayerConection = false;
            }
        }
        else
        {
            std::cout << "Invalid answer " << std::endl;
            close(ClientConn);
        }
        close(ClientConn);
    }
    close(ServSock);
    return gameID;
}

void game_creator::default_games(std::vector<game> *games)
{
    game cur_game;
    for(int i = 0; i < 7; i++)
    {
        cur_game.player_name.clear();
        if(i == 0)
        {
            cur_game.gameID = 0;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("IvanIvanov");
            cur_game.player_name.push_back("PetrPetrov");
            cur_game.player_name.push_back("Barcelona");
            cur_game.player_name.push_back("RealMadrid");
            cur_game.player_name.push_back("ETyumencev");
            games->push_back(cur_game);
        }
        else if (i == 1)
        {
            cur_game.gameID = 1;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("PetrPetrov");
            cur_game.player_name.push_back("IvanIvanov");
            games->push_back(cur_game);
        }
        else if (i == 2)
        {
            cur_game.gameID = 2;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("PetrPetrov");
            cur_game.player_name.push_back("RealMadrid");
            games->push_back(cur_game);
        }
        else if (i == 3)
        {
            cur_game.gameID = 3;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("PetrPetrov");
            cur_game.player_name.push_back("Barcelona");
            games->push_back(cur_game);
        }
        else if (i == 4)
        {
            cur_game.gameID = 4;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("PetrPetrov");
            cur_game.player_name.push_back("IvanIvanov");
            games->push_back(cur_game);
        }
        else if (i == 5)
        {
            cur_game.gameID = 5;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("PetrPetrov");
            games->push_back(cur_game);
        }
        else if (i == 6)
        {
            cur_game.gameID = 5;
            cur_game.game_status = 0;
            cur_game.connection_player = 0;
            cur_game.player_name.push_back("IvanIvanov");
            games->push_back(cur_game);
        }
    }

}
