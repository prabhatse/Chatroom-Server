#include "ServerMessageHandler.hpp"
extern std::ofstream _log;

CServerMessageHandler::CServerMessageHandler(){
    
}

void CServerMessageHandler::A_ClientAuth(int sockfd, std::string &msg){
  _log << "-------handleClientAuth--------\n";
  std::string result;
  unsigned int packetSize = ACTION_SIZE + 1 + 1;
  std::string authenticated("1");
  appendInt(result, packetSize);
  _log << "result size is " << result.size() << "\n";
  appendShort(result, CLIENT_AUTH);
  _log << "result size is " << result.size() << "\n";
  appendString(result, authenticated);
  _log << "send auth message of size " << result.size() << "\n";
  writeMsg(sockfd, result);
}

void CServerMessageHandler::A_GetAvailableRooms(int sockfd, std::string &msg){
  _log << "-------handleGetAvailableRooms--------\n";
}

void CServerMessageHandler::A_GetRoomStatus(int sockfd, std::string &msg){
 _log << "-------handleGetRoomStatus--------\n";
}

void CServerMessageHandler::A_CreateRoom(int sockfd, std::string &msg){
  _log << "-------handleCreateRoom--------\n";
}

void CServerMessageHandler::A_JoinRoom(int sockfd, std::string &msg){
  _log << "-------handleJoinRoom--------\n";
}

void CServerMessageHandler::A_LeaveRoom(int sockfd, std::string &msg){
  _log << "-------handleLeaveRoom--------\n";
}

void CServerMessageHandler::A_DeliverMessagePacket(int sockfd, std::string &msg){
  _log << "-------handleDeliverMessagePacket--------\n";
}

void CServerMessageHandler::A_Disconnect(int sockfd){
  _log << "-------handleDisconnect--------\n";
}

