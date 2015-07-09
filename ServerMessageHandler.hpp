#ifndef SERVERMESSAGEHANDLER_HPP
#define SERVERMESSAGEHANDLER_HPP

#include "MessageHandler.hpp"
#include <fstream>
#include <iostream>
class CServerMessageHandler: public CMessageHandler{
public:
    CServerMessageHandler();
    void A_ClientAuth(int, std::string&);
    void A_GetAvailableRooms(int, std::string&);
    void A_GetRoomStatus(int, std::string&);
    void A_CreateRoom(int, std::string&);
    void A_JoinRoom(int, std::string&);
    void A_LeaveRoom(int, std::string&);
    void A_DeliverMessagePacket(int, std::string&);
    void A_Disconnect(int);
};

#endif