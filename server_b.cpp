#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <QString>
#include <QDir>
#include <QDebug>

#pragma comment(lib, "Ws2_32.lib")

#include "dop_function.h"

int main(void)
{
    dop_function df;
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start GAME Creator                       " << std::endl;
std::cout << "_________________________________________________" << std::endl;
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
    std::string answerYes = "Yes";
    std::string answerNo  = "No" ;
    //--------------------------------------------------------------------------// Write in file
    QString fileName;                                                           //
    int CurrentGameID;                                                          // ID текущей игры (для записи в файл и конектов к ней)
    //--------------------------------------------------------------------------//
    std::string readFileName = "D:/Models/OTUS/server-client/game_count.txt";   // чтение количества созданных игр
    std::fstream readFile(readFileName);                                        //
    std::string str;                                                            //
    if (readFile.is_open())                                                     //
    {                                                                           //
        readFile >> str;                                                        //
        CurrentGameID = std::stoi(str);                                         //
    }                                                                           //
    else                                                                        //
    {                                                                           //
        CurrentGameID = 0;                                                      // если файла нет, то создаем
        fileName = "D:/Models/OTUS/server-client/game_count.txt";               //
        QFile file0(fileName);                                                  //
        if (!file0.open( QIODevice::WriteOnly | QIODevice::Text )) {            //
            qDebug() << "Не удалось открыть файл»";                             //
            return 1;                                                           //
        }                                                                       //
        QTextStream stream0(&file0);                                            //
        stream0 << CurrentGameID;                                               //
        file0.close();                                                          //
    }                                                                           //
    //--------------------------------------------------------------------------//
    while (true)                                                                // Цикл № 1 - для создания игра / определения наличия и формата доступа клиентов к игре
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
            closesocket(ClientConn);
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
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------//
        int numPlayers;                                                             //
        std::vector<std::string> NamePlayers;                                       // для верификации игроков
//----------------------------------------------------------------------------------//
        if(isCreating == answerYes)
        {
std::cout << "_________________________________________________" << std::endl;
std::cout << "        Start creating GAME                      " << std::endl;
std::cout << "_________________________________________________" << std::endl;
            //--------------------------------------------------------------------------//
            std::string ToWriteInFile = std::to_string(CurrentGameID);                  //
            fileName = "D:/Models/OTUS/server-client/game_status." +                    //
                                QString::fromStdString(ToWriteInFile) + ".txt";         //
            QFile file(fileName);                                                       //
            if (!file.open( QIODevice::WriteOnly | QIODevice::Text )) {                 //
                qDebug() << "Не удалось открыть файл»";                                 //
                return 1;                                                               //
            }                                                                           //
            QTextStream stream(&file);                                                  //
            //--------------------------------------------------------------------------//
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
                    NamePlayers.push_back(NameConcretePlayer);
                }
            }
            answer = "Start creating game ...";
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            stream << "Game status = 0" << endl;
            stream << "Create game with ID : " << QString::fromStdString(ToWriteInFile) << endl;
            QString strX = QString::fromStdString(numberPlayers);
            stream << "Number players: " << strX << endl;
            for(int i = 0; i < numPlayers; i++)
            {
                if(i == 0) SerialNumberPlayer = "first";
                else if(i == 1) SerialNumberPlayer = "second";
                else if(i == 2) SerialNumberPlayer = "third";
                else if(i == 3) SerialNumberPlayer = "fourth";
                else if(i == 4) SerialNumberPlayer = "fifth";
                stream << QString::fromStdString(SerialNumberPlayer) << " : " << QString::fromStdString(NamePlayers.at(i)) << endl;
            }
            if (packet_size == SOCKET_ERROR)
            {
                std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
                closesocket(ClientConn);
            }
        //--------------------------------------------------------------------------//
            fileName = "D:/Models/OTUS/server-client/game_count.txt";               // обновляем файл с количеством игр
            QFile file0(fileName);                                                  //
            if (!file0.open( QIODevice::WriteOnly | QIODevice::Text )) {            //
                qDebug() << "Не удалось открыть файл»";                             //
                return 1;                                                           //
            }                                                                       //
            QTextStream stream0(&file0);                                            //
            stream0 << CurrentGameID;                                               //
            file0.close();                                                          //
        //--------------------------------------------------------------------------//
            file.close();
            answer = std::to_string(CurrentGameID);
            clientBuff = df.clearBuf(clientBuff);
            clientBuff = df.convert_string_to_char(clientBuff, answer);
            packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
            CurrentGameID ++;
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
                // ....
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
                int gameID = stoi(isGameID);
                bool isPlayerConection = false;
                //--------------------------------------------------------------------------//
                std::list<std::list<std::string>> list_game_config = df.readGameConfig();   // чтение файлов с конфигарацией игр
                while(!list_game_config.empty())                                            //
                {                                                                           //
                    int read_count = 0;                                                     //
                    while(!list_game_config.front().empty())                                //
                    {                                                                       //
                        if(read_count == 0)
                            std::cout << "File name: ";
                        else if(read_count == 1)
                            std::cout << "Game status = ";
                        else if(read_count == 2)
                            std::cout << "Create game with ID: ";
                        else if(read_count == 3)
                            std::cout << "Number players: ";
                        else if(read_count == 4)
                            std::cout << "first: ";
                        else if(read_count == 5)
                            std::cout << "second: ";
                        else if(read_count == 6)
                            std::cout << "third: ";
                        else if(read_count == 7)
                            std::cout << "fourth: ";
                        else if(read_count == 8)
                            std::cout << "fifth: ";
                        std::cout << list_game_config.front().front() << std::endl;         //
                        if(read_count == 2)
                        {
                            int usingGameID = stoi(list_game_config.front().front());
                            if(usingGameID != gameID)
                                break;
                        }
                        if(read_count > 3)
                        {
                            if(playerName == list_game_config.front().front())
                            {
                                isPlayerConection = true;
                                break;
                            }
                        }
                        list_game_config.front().pop_front();                               //
                        read_count++;                                                       //
                    }                                                                       //
                    if(isPlayerConection)
                        break;
                    list_game_config.pop_front();                                           //
                }                                                                           //
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
                    answer = std::to_string(CurrentGameID);
                    clientBuff = df.clearBuf(clientBuff);
                    clientBuff = df.convert_string_to_char(clientBuff, answer);
                    packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
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
                //...
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
    return 0;
}
