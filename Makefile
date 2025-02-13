all:
	g++ -std=c++17 server.cpp -o server
	g++ -std=c++17 client.cpp -lcppunit -o client
test:
	chmod +x server
	./client
clean:
	$(RM) server client
