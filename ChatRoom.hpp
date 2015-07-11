#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class CServerMessageHandler;
class CNetwork;
class CChatRoom{
private:
    std::vector<std::string *> _users;
    std::string _roomName;
    CServerMessageHandler *_msgHandler;  
    int _capacity = 0;
    int _size = 0;

public:
    friend CNetwork;
    CChatRoom(int, std::string);
    int getSize();
    int getCapacity();
    void start();
    bool addUser(std::string *);
    bool removeUser(std::string *);
};

#endif