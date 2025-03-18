all:
	g++ -std=c++17 server_autorization.cpp -o server_a
	g++ -std=c++17 middleware.cpp eventloop.cpp exceptionhandler.cpp game_creator.cpp game_server.cpp icommand.cpp istate.cpp minor.cpp movecommand.cpp object.cpp objposition.cpp order.cpp scope.cpp start_game.cpp -o middleware
	g++ -std=c++17 client_1.cpp -o client1
	g++ -std=c++17 client_2.cpp -o client2
	g++ -std=c++17 server_autorization_test.cpp -lcppunit -o server_a_test
test:
	chmod +x server_a
	./server_a_test
clean:
	$(RM) server_a middleware client1 client2 server_a_test
