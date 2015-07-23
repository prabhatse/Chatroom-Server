#include "MessageHandler.hpp"
#include "Macroz.hpp"

CMessageHandler::CMessageHandler(){

}

void CMessageHandler::appendInt(std::string& msg, uint32_t size){
  int start = msg.size();
  msg.resize(start + sizeof(size));
  logsv(appendInt 4 bytes value, size);
  uint32_t networkSize = htonl(size);
  *(uint32_t*)(msg.data() + start) = networkSize;
  _log.flush();
}

void CMessageHandler::appendShort(std::string& msg, uint16_t action){
  int start = msg.size();
  msg.resize(start + sizeof(action));
  logsv(appendShort 2 bytes value, action);
  uint16_t networkAction = htons(action);
  *(uint16_t*)(msg.data() + start) = networkAction;
  _log.flush();
}

void CMessageHandler::appendString(std::string& msg, std::string str){
  int start = msg.size();
  str += ',';
  logsvsv(appendString , str, size , msg.size());
  msg.resize(start + str.size());

  memcpy((void *)(msg.data() + start), str.data(), str.size());
  _log.flush();
}

int CMessageHandler::removeInt(int sockfd){
  uint32_t uInteger;
  int result = read(sockfd, &uInteger, sizeof(uInteger));
  if(result == 0){
    ps(Error only EOF is read);
    logsv(Error connection closed, sockfd);
    _log.flush();
    return -1;
  }
  else if(result < 0){
    psv(Error reading socket, sockfd)
    logsv(Error reading socket, sockfd);
    _log.flush();
    return -1;
  }
  if(result != PACKET_SIZE){
    logsv(Error read only, result); 
    logsv(When there is bytes, PACKET_SIZE);
    _log.flush();
    return -1;
  }
  uInteger = ntohl(uInteger);
  logsv(removeInt read 4 bytes result , uInteger); 
  _log.flush();
  return uInteger;
}

int CMessageHandler::removeShort(int sockfd){
  uint16_t uShort;
  int result = read(sockfd, &uShort, sizeof(uShort));
  if(result == 0){
    psv(Error connection closed for sockfd, sockfd);
    logsv(Error connection closed for sockfd, sockfd);
    return -1;
  }
  else if(result < 0){
    psv(Error reading socket, sockfd);
    logsv(Error reading socket, sockfd);
    return -1;
  }
  if(result != ACTION_SIZE){
    logs(Error not reading 2 bytes for removeShort);
    return -1;
  }
  uShort = ntohs(uShort);
  logsv(removeShort: read 2 bytes value , uShort);
  _log.flush();
  return uShort;
}

std::string CMessageHandler::removeBytes(int sockfd, int numBytes){
  if(numBytes == 0){
    return "";
  }
  std::string msg;
  msg.resize(numBytes);
  int result = read(sockfd, (void *)msg.data(), (size_t)numBytes);
  if(result == 0){
    psv(Error connection closed for sockfd, sockfd);
    logsv(Error connection closed for sockfd, sockfd);
  }
  else if(result < 0){
    psvs(Error reading socket, sockfd, result -1);
    logsvs(Error reading socket, sockfd, result -1);
    std::string emptyMsg;
    return emptyMsg;
  }
  if(result != numBytes){
    logsv(Error not reading expect number bytes, numBytes);
  }

  logsvsv(removeBytes: read , result, bytes value:, msg); 
  _log.flush();
  return msg;
}

void CMessageHandler::writeMsg(int sockfd, std::string &msg){
  logs(writeMsg -------------------------);

  int result = write(sockfd, (void *)msg.data(), msg.size());
  logsvsv(Sent message of size, msg.size(), to sockfd, sockfd);
  if(result != msg.size()){
    ps(Error cannot write entire message);
    logsvsv(Error write only, result, when expected to write, msg.size());
  }
  _log.flush();
}

