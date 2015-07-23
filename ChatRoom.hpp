#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include "Poller.hpp"
#include "MessageHandler.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class CNetwork;
class CChatRoom: public CPoller, public CMessageHandler{
private:
    CNetwork *_network;
    std::vector<std::string> _users;
    std::string _name;
    int _totalUsernameSize; 
    int _capacity = 0;
    int _size = 0;
    int _threated = 0;
    bool _infoModified = false;

public:
    friend CNetwork;
    CChatRoom();
    CChatRoom(int, std::string, CNetwork*);
    CChatRoom(const CChatRoom&);
    int getSize();
    int getCapacity();
    void startReading();
    bool addUser(std::string);
    bool removeUser(std::string);
    bool readPoll(int);
    void processMessage(int, int, std::string);
    void processDeliverMessage(int, std::string);
    void processDisconnect(int,std::string);
    void processLeaveRoom(int, std::string);
    void writeToAll(std::string);
    void startChatRoom();
};

#endif