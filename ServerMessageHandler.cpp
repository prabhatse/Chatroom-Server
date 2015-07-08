#include "ServerMessageHandler.hpp"
extern std::ofstream _log;

CServerMessageHandler::CServerMessageHandler(){
    
}

void CServerMessageHandler::A_ClientAuth(){
  _log << "-------handleClientAuth--------\n";
}

void CServerMessageHandler::A_GetAvailableRooms(){
  _log << "-------handleGetAvailableRooms--------\n";
}

void CServerMessageHandler::A_GetRoomStatus(){
 _log << "-------handleGetRoomStatus--------\n";
}

void CServerMessageHandler::A_CreateRoom(){
  _log << "-------handleCreateRoom--------\n";
}

void CServerMessageHandler::A_JoinRoom(){
  _log << "-------handleJoinRoom--------\n";
}

void CServerMessageHandler::A_LeaveRoom(){
  _log << "-------handleLeaveRoom--------\n";
}

void CServerMessageHandler::A_DeliverMessagePacket(){
  _log << "-------handleDeliverMessagePacket--------\n";
}

void CServerMessageHandler::A_Disconnect(){
  _log << "-------handleDisconnect--------\n";
}