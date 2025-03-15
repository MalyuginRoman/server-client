#include "game_server.h"

bool game_server::game_server_main(std::vector<game> *games, SOCKET ServSock1)
{
    dop_function df;
/*    std::cout << "_________________________________________________" << std::endl;
    std::cout << "        Start GAME Server                        " << std::endl;
    std::cout << "_________________________________________________" << std::endl;
    //Key constants
    const char IP_SERV[] = "10.124.40.14";	// Enter local Server IP address
    const int PORT_NUM = 8008;				// Enter Open working server port*/
    const short BUFF_SIZE = 1024;			// Maximum size of buffer for exchange info between server and client
    /*// Key variables for all program*/
    int erStat;								// Keeps socket errors status
    /*//IP in string format to numeric format for socket functions. Data is in "ip_to_num"
    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);
    if (erStat <= 0) {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }
    // WinSock initialization*/
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    /*if ( erStat != 0 ) {
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
        std::cout << "Binding socket to Server info is OK" << std::endl;*/
    SOCKET ServSock = ServSock1;
std::string answer;                                                         //

    while (true)                                                            // Цикл № 2 - отправка сообщений во время проведения игры
    {
        int max_round_count = 100;
        int current_round = 0;
        int game_status_int, game_ID_int, number_players_int;
        std::map<int, system_okr> p_map_c_a;
        std::map<int, system_okr> p_map_c_b;
        std::map<std::string, std::function<ICommand*()>> *m_map;
        std::map<std::string, std::string> *m_scope;
        std::vector<player> player_list;
        objectVector obj_vector;
        collisionObjects isEvent;
        isEvent.isActive = false;
        bool is_obj_create[5];
        bool isLogging = false;
        for(int i = 0; i < 5; i++)
            is_obj_create[i] = false;
        //scope *my_scope = new scope(&m_map, &m_scope);
        start_game Game1(m_map, m_scope, &player_list, &obj_vector);
        p_map_c_a = Game1.create_pole(1);
        p_map_c_b = Game1.create_pole(2);
        orderVector player_messages;
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
            std::string GameID;
            GameID = df.convert_char_to_string(servBuff, GameID);
//-----------------------------------------------------------------------------------// открываем файл и перезаписываем все содержимоу
//            QString fileName = "D:/Models/OTUS/server-client/game_status." +         //
//                            QString::fromStdString(GameID) + ".txt";                 // обновляем файл с количеством игр
//            std::string readFileName = fileName.toStdString();                       //
//            std::fstream readFile(readFileName);                                     //
//            QFile file(fileName);                                                    //
//            if (!file.open( QIODevice::Append /*ReadWrite*/ | QIODevice::Text )) {   //
//                qDebug() << "Не удалось открыть файл»";                              //
//                return 1;                                                            //
//            }                                                                        //
//            QTextStream stream(&file);                                               //
//            std::string str, game_status_str, game_ID_str, number_players_str;       //
//            if (readFile.is_open())                                                  //
//            {                                                                        //
//                while(readFile >> str)                                               //
//                {                                                                    //
//                    if(str == "Game" /*status = "*/)                                 //
//                    {                                                                //
//                        readFile >> str >> str >> str;                               //
//                        game_status_str = str;                                       //
//                        game_status_int = stoi(game_status_str);                     //
//                    //    stream << QString::fromStdString(str);                     //
//                    }                                                                //
//                    else if(str == "Create" /*game with ID :"*/)                     //
//                    {                                                                //
//                        readFile >> str >> str >> str >> str >> str;                 //
//                        game_ID_str = str;                                           //
//                        game_ID_int = stoi(game_ID_str);                             //
//                    }                                                                //
//                    else if(str == "Number" /* players:"*/)                          //
//                    {                                                                //
//                        readFile >> str >> str;                                      //
//                        number_players_str = str;                                    //
//                        number_players_int = stoi(number_players_str);               //
//                    }                                                                //
//                    else if(str == "first" /*:"*/)                                   //
//                    {                                                                //
//                        readFile >> str >> str;                                      //
//                        if(current_round == 0 && !is_obj_create[1])                  //
//                        {                                                            //
//                            std::cout << "first: " << str << std::endl;                  //
//                            Game1.create_player(str, &player_list, &obj_vector,      //
//                                            &p_map_c_a, &p_map_c_b);                 //
//                            is_obj_create[1] = true;                                 //
//                        }                                                            //
//                    }                                                                //
//                    else if(str == "second" /*:"*/)                                  //
//                    {                                                                //
//                        readFile >> str >> str;                                      //
//                        if(current_round == 0 && !is_obj_create[2])                  //
//                        {                                                            //
//                            std::cout << "second: " << str << std::endl;                 //
//                            Game1.create_player(str, &player_list, &obj_vector,      //
//                                            &p_map_c_a, &p_map_c_b);                 //
//                            is_obj_create[2] = true;                                 //
//                        }                                                            //
//                    }                                                                //
//                    else if(str == "third" /*:"*/)                                   //
//                    {                                                                //
//                        readFile >> str >> str;                                      //
//                        std::cout << "third: " << str << std::endl;                  //
//                    }                                                                //
//                    else if(str == "fourth" /*:"*/)                                  //
//                    {                                                                //
//                        readFile >> str >> str;                                      //
//                        std::cout << "fourth: " << str << std::endl;                 //
//                    }                                                                //
//                    else if(str == "fifth" /*:"*/)                                   //
//                    {                                                                //
//                        readFile >> str >> str;                                      //
//                        std::cout << "fifth: " << str << std::endl;                  //
//                    }                                                                //
//                }                                                                    //
//            }                                                                        //
//            int obj_vector_count = obj_vector.count();                               //
//for(int i = 0; i < obj_vector_count; i++) {                                          //
//std::cout << obj_vector.at(i)->playerID()                                            //
//          << ", "     << obj_vector.at(i)->objectID()                                //
//          << ": u = " << obj_vector.at(i)->state().velocity                          //
//          << ", v = " << obj_vector.at(i)->state().angularVelocity                   //
//          << ", f = " << obj_vector.at(i)->state().fuel                              //
//          << ", x = " << obj_vector.at(i)->place().placeX                            //
//          << ", y = " << obj_vector.at(i)->place().placeY                            //
//          << ", a = " << obj_vector.at(i)->place().angular << std::endl;   }         //
//-----------------------------------------------------------------------------------//
//            current_round++;
std::cout << "_________________________________________________" << std::endl;
if(!isLogging)
//    stream << "_________________________________________________" << endl;
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
if(!isLogging)
//    stream << "        Round = " << current_round << endl;
isLogging = true;
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
            std::string InitialVelocity = "";
            UserAction = df.convert_char_to_string(servBuff, UserAction);
            std::cout << UserName << "in game with ID:" << GameID << " is " << UserAction << std::endl;
//stream << QString::fromStdString(UserName) << " using " << QString::fromStdString(UserAction) << endl;
//-------------------------------------------------------
            if(UserAction == "StartMove")
            {
                answer = "Write initial velocity: ";
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
                InitialVelocity = df.convert_char_to_string(servBuff, InitialVelocity);
                int InitialVelocity_int = stoi(InitialVelocity);
                if(InitialVelocity_int > Umax)
                    InitialVelocity = std::to_string(Umax);
                std::cout << "Start velocity with " << InitialVelocity << std::endl;
            }
            else if(UserAction == "Rotate")
            {
                answer = "Write angle: ";
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
                InitialVelocity = df.convert_char_to_string(servBuff, InitialVelocity);
                std::cout << "Start rotate with " << InitialVelocity << std::endl;
            }
//-------------------------------------------------------

            int UserID, objectID;
            for(int i = 0; i < player_list.size(); i++)
            {
                if(UserName == player_list.at(i).playerName)
                {
                    UserID = player_list.at(i).playerId;
                    objectID = player_list.at(i).playerObject.at(0);
                }
            }

            player_messages.add(UserID, objectID, UserAction, InitialVelocity);
            closesocket(ClientConn);
            if(player_messages.count() == number_players_int)
            {
                current_round++;
                Game1.play_round(&player_messages, &player_list, &obj_vector,
                                 &p_map_c_a, &p_map_c_b);
                player_messages.reset();
                isLogging = false;
                Game1.delete_objects(&player_list, &obj_vector);
                isEvent = Game1.check_collision(&obj_vector, &p_map_c_a, &p_map_c_b);
            }
            if(isEvent.isActive)
            {
                std::vector<int> lose_players;
                for(int i = 0; i < number_players_int; i++)
                {
                    for(std::vector<int>::iterator it = player_list.at(i).playerObject.begin();
                                                   it!= player_list.at(i).playerObject.end();)
                    {
                        if((*it == isEvent.obj1) || (*it == isEvent.obj2))
                        it = player_list.at(i).playerObject.erase(it);
                        else ++it;
                    }
                    if(player_list.at(i).playerObject.empty())
                        lose_players.push_back(player_list.at(i).playerId);
                }
            }
//----------- обрабатывается выход объекта за пределы игрового поля
            for(std::vector<player>::iterator it = player_list.begin();
                                              it!= player_list.end();)
            {
                if(it->playerObject.empty())
                    it = player_list.erase(it);
                else ++it;
            }
//-----------
            if(player_list.empty())
            {
                std::cout << "Game is over!!! Draw in the Game!!!" << std::endl;
//stream << "Game is over!!! Draw in the Game!!!" << endl;
                break;
            }
            else if(player_list.size() == 1)
            {
                std::cout << "Game is over!!! " << player_list.at(0).playerName << " is winner!!!" << std::endl;
                UserName = player_list.at(0).playerName;
//stream << "Game is over!!! "<< QString::fromStdString(UserName) << " is winner!!!" << endl;
                break;
            }
//            file.close();
        }
    }
    closesocket(ServSock);
    WSACleanup();
    return 0;
}

SOCKET game_server::bind_server()
{
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
    /*SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
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
    }*/

    return ServSock;
}
