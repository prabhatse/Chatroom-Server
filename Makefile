Server: main.o Network.o MessageHandler.o Poller.o ChatRoom.o
	g++ -std=c++11 -o Server main.o Network.o Poller.o MessageHandler.o ChatRoom.o 
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
Network.o: Network.hpp Network.cpp Poller.hpp
	g++ -std=c++11 -c Network.cpp
MessageHandler.o: MessageHandler.hpp MessageHandler.cpp
	g++ -std=c++11 -c MessageHandler.cpp
Poller.o: Poller.hpp Poller.cpp
	g++ -std=c++11 -c Poller.cpp
ChatRoom.o: ChatRoom.hpp ChatRoom.cpp
	g++ -std=c++11 -c ChatRoom.cpp
clean:
	rm *.o Server