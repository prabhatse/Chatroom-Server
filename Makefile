CC=g++
CFLAGS=-std=c++11 -pthread -c
CFLAGSO=-std=c++11 -pthread -o
Server: main.o Network.o MessageHandler.o Poller.o ChatRoom.o
	$(CC) $(CFLAGSO) Server main.o Network.o Poller.o MessageHandler.o ChatRoom.o 
main.o: main.cpp Network.hpp 
	$(CC) $(CFLAGS) main.cpp
Network.o: Network.hpp Network.cpp
	$(CC) $(CFLAGS) Network.cpp
MessageHandler.o: MessageHandler.hpp MessageHandler.cpp
	$(CC) $(CFLAGS) MessageHandler.cpp
Poller.o: Poller.hpp Poller.cpp
	$(CC) $(CFLAGS) Poller.cpp
ChatRoom.o: ChatRoom.hpp ChatRoom.cpp
	$(CC) $(CFLAGS) ChatRoom.cpp
clean:
	rm -f *.o *.gch *.txt Server 