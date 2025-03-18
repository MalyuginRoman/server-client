all:
	g++ -std=c++17 server_autorization.cpp -o server_a
	g++ -std=c++17 client_1.cpp -o client
	g++ -std=c++17 server_autorization_test.cpp -lcppunit -o server_a_test
test:
	chmod +x server_a
	./server_a_test
clean:
	$(RM) server_a client server_a_test
