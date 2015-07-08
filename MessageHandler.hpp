#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP


#define MESSAGE_SIZE 4
#define ACTION_SIZE 2

#define CLIENT_AUTH 0
#define GET_AVAILABLE_ROOMS 1
#define GET_ROOM_STATUS 2
#define CREATE_ROOM 3
#define JOIN_ROOM 4
#define LEAVE_ROOM 5
#define DELIVER_MESSAGE_PACKET 6
#define DISCONNECT 7
#include <iostream>
#include <string>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>

class CMessageHandler{
protected:
    int sockfd;
public:
    CMessageHandler();
    /*
      Add info to message
    */
    void appendInt(std::string&, uint32_t);
    void appendShort(std::string&, uint16_t);
    void appendString(std::string&, std::string);

    /* 
      Take out info from message
    */
    int removeInt(int);
    int removeShort(int);
    std::string removeBytes(int, int);

    /*
      Send message through sockfd
    */
    void writeMsg(int, std::string &);

    /*
      Various types of message handling
    */
    virtual void A_ClientAuth();
    virtual void A_GetAvailableRooms();
    virtual void A_GetRoomStatus();
    virtual void A_CreateRoom();
    virtual void A_JoinRoom();
    virtual void A_LeaveRoom();
    virtual void A_DeliverMessagePacket();
    virtual void A_Disconnect();

    /*
      Handles message base on action number
    */
    virtual bool processMessage(int, std::string&);

    friend class Network;
};

#endif