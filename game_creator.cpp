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
    const char IP_SERV[] = "10.124.40.14";	// Enter local Server IP address
    const int PORT_NUM = 8080;				// Enter Open working server port
    const short BUFF_SIZE = 1024;			// Maximum size of buffer for exchange info between server and client
    int erStat;								// Keeps socket errors status
    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);
    if (erStat <= 0) {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if ( erStat != 0 ) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 999;
    }
    else
        std::cout << "WinSock initialization is OK" << std::endl;
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSock == INVALID_SOCKET) {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 999;
    }
    else
        std::cout << "Server socket initialization is OK" << std::endl;
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
        return 999;
    }
    else
        std::cout << "Binding socket to Server info is OK" << std::endl;
    std::string answerYes = "Yes";
    std::string answerNo  = "No" ;
//--------------------------------------------------------------------------------//
    bool isPlayerConection = false;
    game current_game;
    current_game.gameID = 999;
    while (!isAllPlayer)                                                          // Цикл № 1 - для создания игра / определения наличия и формата доступа клиентов к игре
    {
        erStat = listen(ServSock, SOMAXCONN);

        if ( erStat != 0 ) {
            std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
            closesocket(ServSock);
            WSACleanup();
            return 999;
        }
        else {
            std::cout << "Listening..." << std::endl;
        }
        sockaddr_in clientInfo;
        ZeroMemory(&clientInfo, sizeof(clientInfo));                                // Initializing clientInfo structure
        int clientInfo_size = sizeof(clientInfo);
        SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
        if (ClientConn == INVALID_SOCKET) {
            std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientConn);
        }
        else {
            std::cout << "Connection to a client established successfully" << std::endl;
            char clientIP[22];
            inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format
            std::cout << "Client connected with IP address " << clientIP << std::endl;
        }
        std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);              // Creation of buffers for sending and receiving data
        short packet_size = 0;                                                      // The size of sending / receiving packet in bytes
//----------------------------------------------------------------------------------
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);        // получаем имя подключившегося игрока
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientConn);
        }
        std::string playerName;
        playerName = df.convert_char_to_string(servBuff, playerName);
        std::string answer = "What are you want? ";
        clientBuff = df.clearBuf(clientBuff);
        clientBuff = df.convert_string_to_char(clientBuff, answer);
        std::cout << clientBuff.data() << std::endl;
        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientConn);
        }
        clientBuff = df.clearBuf(clientBuff);
        servBuff = df.clearBuf(servBuff);
        answer = "Create game? ";
        clientBuff = df.convert_string_to_char(clientBuff, answer);
        std::cout << clientBuff.data();
        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR)
        {
            std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
            closesocket(ClientConn);
        }
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
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
            }
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
                shutdown(ClientConn, SD_BOTH);
                closesocket(ClientConn);
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
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
            }
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
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
            }
            packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
            if (packet_size == SOCKET_ERROR) {
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
            }
            else
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
                if (packet_size == SOCKET_ERROR)
                {
                    std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientConn);
                }
                packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientConn);
                }
                else
                    std::cout << servBuff.data();
                std::string isGameID;
                isGameID = df.convert_char_to_string(servBuff, isGameID);
                gameID = stoi(isGameID);
                if(gameID < game_count)
                {
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
                    if (packet_size == SOCKET_ERROR)
                    {
                        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientConn);
                    }
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
                    if (packet_size == SOCKET_ERROR)
                    {
                        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientConn);
                    }
                }
            }
            else if(isConection == answerNo)  // при подключении к игре пользователь не прошел верификацию
            {
                answer = "View game statuses? ";
                clientBuff = df.clearBuf(clientBuff);
                clientBuff = df.convert_string_to_char(clientBuff, answer);
                std::cout << clientBuff.data();
                packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                if (packet_size == SOCKET_ERROR)
                {
                    std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientConn);
                }
                packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
                if (packet_size == SOCKET_ERROR) {
                    std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                    closesocket(ClientConn);
                }
                else
                    std::cout << servBuff.data();
                std::string isViewing;
                isViewing = df.convert_char_to_string(servBuff, isViewing);
                if(isViewing == answerYes)
                {
                    //...
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
                    if (packet_size == SOCKET_ERROR)
                    {
                        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientConn);
                    }
                    answer = "Game are going: " + std::to_string(game1);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    if (packet_size == SOCKET_ERROR)
                    {
                        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientConn);
                    }
                    answer = "Game are ending: " + std::to_string(game2);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    std::cout << clientBuff.data() << std::endl;
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
                    if (packet_size == SOCKET_ERROR)
                    {
                        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                        closesocket(ClientConn);
                    }
                }
                isPlayerConection = false;
            }
        }
        else
        {
            std::cout << "Invalid answer " << std::endl;
            closesocket(ClientConn);
        }
        closesocket(ClientConn);
    }
    closesocket(ServSock);
    WSACleanup();
    return gameID;
}

void game_creator::default_games(std::vector<game> *games)
{
    game cur_game;
    for(int i = 0; i < 6; i++)
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
    }

}
