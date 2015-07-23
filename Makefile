Server: main.o Network.o MessageHandler.o Poller.o ChatRoom.o
	g++ -std=c++11 -pthread -o Server main.o Network.o Poller.o MessageHandler.o ChatRoom.o 
main.o: main.cpp
	g++ -std=c++11 -pthread -c main.cpp
Network.o: Network.hpp Network.cpp
	g++ -std=c++11 -pthread -c Network.cpp Macroz.hpp
MessageHandler.o: MessageHandler.hpp MessageHandler.cpp
	g++ -std=c++11 -pthread -c MessageHandler.cpp
Poller.o: Poller.hpp Poller.cpp
	g++ -std=c++11 -pthread -c Poller.cpp
ChatRoom.o: ChatRoom.hpp ChatRoom.cpp
	g++ -std=c++11 -pthread -c ChatRoom.cpp
clean:
	rm *.o Server