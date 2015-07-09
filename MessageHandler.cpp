#include "MessageHandler.hpp"
extern std::ofstream _log;

CMessageHandler::CMessageHandler(){

}

void CMessageHandler::appendInt(std::string& msg, uint32_t size){
  _log << "---------------------- appendInt -------------------------\n";
  int start = msg.size();
  msg.resize(start + sizeof(size));
  _log << "appendInt " << size << "\n";
  uint32_t networkSize = htonl(size);
  *(uint32_t*)(msg.data() + start) = networkSize;
  _log.flush();
}

void CMessageHandler::appendShort(std::string& msg, uint16_t action){
  _log << "---------------------- appendShort -------------------------\n";
  int start = msg.size();
  msg.resize(start + sizeof(action));
  _log << "appendShort " << action << "\n";
  uint16_t networkAction = htons(action);
  *(uint16_t*)(msg.data() + start) = networkAction;
  _log.flush();
}

void CMessageHandler::appendString(std::string& msg, std::string str){
  _log << "---------------------- appendString -------------------------\n";
  int start = msg.size();
  str += ',';
  _log << "appendString string and size " << str << " "
    << str.size() << "\n";
  msg.resize(start + str.size());

  memcpy((void *)(msg.data() + start), str.data(), str.size());
  _log.flush();
}

int CMessageHandler::removeInt(int sockfd){
  _log << "---------------------- removeInt -------------------------\n";
  uint32_t uInteger;
  int result = read(sockfd, &uInteger, sizeof(uInteger));
  if(result == 0){
    std::cout << "Error only EOF is read" << std::endl;
    _log << "Error only EOF is read, will close socket " << sockfd
      << "\n";
    _log.flush();
    return -1;

  }
  else if(result < 0){
    std::cout << "Error reading socket." << std::endl;
    _log << "Error reading socket, will close socket\n";
    _log.flush();
    return -1;
  }
  if(result != MESSAGE_SIZE){
    _log << "Error not reading 4 bytes for removeInt\n"; 
    _log.flush();
    return -1;
  }
  uInteger = ntohl(uInteger);
  _log << "Read " << result << " bytes from sockfd " 
    << sockfd << "\n";
  _log << "Read value " << uInteger << "\n";
  _log.flush();
  return uInteger;
}

int CMessageHandler::removeShort(int sockfd){
  _log << "---------------------- removeShort -------------------------\n";
  uint16_t uShort;
  int result = read(sockfd, &uShort, sizeof(uShort));
  if(result == 0){
    std::cout << "Error only EOF is read" << std::endl;
    _log << "Error only EOF is read, close socket\n";
    return -1;
  }
  else if(result < 0){
    std::cout << "Error reading socket" << std::endl;
    _log << "Error reading socket, close socket\n";
    return -1;
  }
  if(result != ACTION_SIZE){
    _log << "Error not reading 2 bytes for removeShort, close socket\n";
    return -1;
  }
  uShort = ntohs(uShort);
  _log << "Read " << result << " bytes from sockfd "
    << sockfd << "\n";
  _log << "Read value " << uShort <<"\n";
  _log.flush();
  return uShort;
}

std::string CMessageHandler::removeBytes(int sockfd, int numBytes){
  _log << "---------------------- removeBytes -------------------------\n";
  std::string msg;
  msg.resize(numBytes);
  int result = read(sockfd, (void *)msg.data(), (size_t)numBytes);
  if(result == 0){
    std::cout << "Error only EOF is read\n" << std::endl;
    _log << "Error only EOF is read, close socket\n";
  }
  else if(result < 0){
    std::cout << "Error reading socket\n" << std::endl;
    _log << "Error reading socket, close socket \n";
    std::string emptyMsg;
    return emptyMsg;
  }
  if(result != numBytes){
    _log << "Error not reading " << numBytes << " from removeBytes\n";
  }

  _log << "Read " << result << " bytes from sockfd " 
    << sockfd << "\n";
  _log << "Read value: " << msg << "\n";
  _log.flush();
  return msg;
}

void CMessageHandler::writeMsg(int sockfd, std::string &msg){
  _log << "---------------------- writeMsg -------------------------\n";

  int result = write(sockfd, (void *)msg.data(), msg.size());
  _log << "Sent message of size " << msg.size() << 
  " to sockfd" << sockfd << "\n";
  if(result != msg.size()){
    std::cout << "Error cannot write entire message" << std::endl;
  }
  _log.flush();
}

bool CMessageHandler::processMessage(int sockfd, int action, std::string &msg){
  switch(action){
    case CLIENT_AUTH:
      A_ClientAuth(sockfd, msg);
      break;
    case GET_AVAILABLE_ROOMS:
      A_GetAvailableRooms(sockfd);
      break;
    case GET_ROOM_STATUS:
      A_GetRoomStatus(sockfd, msg);
      break;
    case CREATE_ROOM:
      A_CreateRoom(sockfd, msg);
      break;
    case JOIN_ROOM:
      A_JoinRoom(sockfd, msg);
      break;
    case LEAVE_ROOM:
      A_LeaveRoom(sockfd, msg);
      break;
    case DELIVER_MESSAGE_PACKET:
      A_DeliverMessagePacket(sockfd, msg);
      break;
    case DISCONNECT:
      A_Disconnect(sockfd);
      break;
  }
  return true;
}

void CMessageHandler::A_ClientAuth(int, std::string&){}
void CMessageHandler::A_GetAvailableRooms(int){}
void CMessageHandler::A_GetRoomStatus(int, std::string&){}
void CMessageHandler::A_CreateRoom(int, std::string&){}
void CMessageHandler::A_JoinRoom(int, std::string&){}
void CMessageHandler::A_LeaveRoom(int, std::string&){}
void CMessageHandler::A_DeliverMessagePacket(int, std::string&){}
void CMessageHandler::A_Disconnect(int){}
