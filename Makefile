all:
	g++ -std=c++17 server_autorization.cpp -o server_a
	g++ -std=c++17 server_autorization.cpp -lcppunit -o server_a_test
test:
	chmod +x server_a
	./server_a_test
clean:
	$(RM) server_a server_a_test
