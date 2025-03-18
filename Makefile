all:
	g++ -std=c++17 server_autorization.cpp -o server_a
	g++ -std=c++17 server_autorization_test.cpp -lcppunit -o server_a_test
	g++ -std=c++17 client_1.cpp -o client_1
test:
	chmod +x server_a
	./server_a_test
	./client_1
clean:
	$(RM) server_a server_a_test client_1
