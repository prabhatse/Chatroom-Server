#ifndef CLIENTMESSAGEHANDLER_HPP
#define CLIENTMESSAGEHANDLER_HPP

#include "MessageHandler.hpp"
#include <fstream>
#include <iostream>
class CServerMessageHandler: public CMessageHandler{
public:
    CServerMessageHandler();
    void A_ClientAuth();
    void A_GetAvailableRooms();
    void A_GetRoomStatus();
    void A_CreateRoom();
    void A_JoinRoom();
    void A_LeaveRoom();
    void A_DeliverMessagePacket();
    void A_Disconnect();

};

#endif