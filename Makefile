all:
	g++ -std=c++17 server_autorization.cpp -o server_a
	g++ -std=c++17 client_1.cpp -o client1
	g++ -std=c++17 client_2.cpp -o client2
	g++ -std=c++17 server_autorization_test.cpp -lcppunit -o server_a_test
test:
	chmod +x server_a
	./server_a_test
clean:
	$(RM) server_a client1 client2 server_a_test
