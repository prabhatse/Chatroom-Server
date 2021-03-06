#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Poller.hpp"
#include "MessageHandler.hpp"
#include "ChatRoom.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <poll.h>
#include <fstream>
#include <thread>

class CNetwork: public CPoller, public CMessageHandler{
private:
    static CNetwork* network;
    int _sockfd, _portno;
    sockaddr_in _serv_addr;
    CNetwork(int portno);
    /* a vector of CChatRooms but only create upon 
     * clients request
    */
    CChatRoom _rooms[MAX_ROOMS];
    int _numRooms = 0;
    /* users are added when clients authenticate with their username
     * users will be removed from network but added to chatroom when
     * they join a room 
     */
    std::vector<std::string> _users;
    bool _infoModified = false;

public: 
    static CNetwork& getNetwork(int portno=49999);
    bool acceptConnection();
    /* set up and wait for any messages */
    bool readPoll(int);
    /* respond to any messages */
    void start();
    void addRoom(int, std::string);
    bool addUser(std::string);
    void removeUser(std::string);
    /**********************
    *** Write packets****
    **********************/
    bool processMessage(int, int, std::string&);
    /* Write to client, are all responses */
    void Write_ClientAuth(int, std::string&);
    void Write_GetAvailableRooms(int, std::string&);
    void Write_GetRoomStatus(int, std::string&);
    void Write_CreateRoom(int, std::string&);
    void Write_JoinRoom(int, std::string&);
    void Write_LeaveRoom(int, std::string&);
    void Write_DeliverMessagePacket(int, std::string&);
    void Write_Disconnect(int, std::string&);

    void threadChatRoom();
    void printUsers();
};

#endif