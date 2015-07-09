Server: main.o Network.o MessageHandler.o ServerMessageHandler.o Poller.o
	g++ -std=c++11 -o Server main.o Network.o Poller.o MessageHandler.o ServerMessageHandler.o 
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
Network.o: Network.hpp Network.cpp Poller.hpp ServerMessageHandler.hpp
	g++ -std=c++11 -c Network.cpp
ServerMessageHandler.o: ServerMessageHandler.hpp ServerMessageHandler.cpp MessageHandler.hpp
	g++ -std=c++11 -c ServerMessageHandler.cpp
MessageHandler.o: MessageHandler.hpp MessageHandler.cpp
	g++ -std=c++11 -c MessageHandler.cpp
Poller.o: Poller.hpp Poller.cpp
	g++ -std=c++11 -c Poller.cpp
clean:
	rm *.o Server