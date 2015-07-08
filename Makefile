Server: main.o Network.o MessageHandler.o ServerMessageHandler.o Poller.o
	g++ -std=c++11 -o Server Network.o MessageHandler.o ServerMessageHandler.o Poller.o
main.o: main.cpp Network.hpp Poller.hpp ServerMessageHandler.hpp MessageHandler.hpp 
	g++ -std=c++11 main.cpp
Network.o: Network.hpp Network.cpp Poller.hpp ServerMessageHandler.hpp
	g++ -std=c++11 Network.cpp
MessageHandler.o: MessageHandler.hpp MessageHandler.cpp
	g++ -std=c++11 MessageHandler.cpp
ServerMessageHandler.o: ServerMessageHandler.hpp ServerMessageHandler.cpp MessageHandler.hpp
	g++ -std=c++11 ServerMessageHandler.cpp
Poller.o: Poller.hpp
	g++ -std=c++11 Poller.cpp