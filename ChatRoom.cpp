#include "ChatRoom.hpp"
#include "Macroz.hpp"
CChatRoom::CChatRoom(int capacity, std::string roomName, CNetwork *network){
  _capacity = capacity;
  _name = roomName;
  _totalUsernameSize = 0;
  _network = network;
}

CChatRoom::CChatRoom(){}

CChatRoom::CChatRoom(const CChatRoom &c){
  _capacity = c._capacity;
  _name = c._name;
  _totalUsernameSize = c._totalUsernameSize;
  _network = c._network;
  _users = c._users;
}

int CChatRoom::getSize(){
  return _size;
}

int CChatRoom::getCapacity(){
  return _capacity;
}

void CChatRoom::startReading(){
  int numToRead = setReadPoll(400);
  if(numToRead > 0){
      psv(One request found in room ,_name);
      logsv(One request found in room ,_name);
      for(struct pollfd& pfd: _userPollfd){
        int tempFD = pfd.fd;

        /*There is a message to be processed*/
        if(pfd.revents & POLLIN){
          /* 
            In the case that something goes wrong
            remove it from pollfd and close socket connection 
          */
          /* Read for messages */
          if(readPoll(pfd.fd) == false){//something went wrong
            //do nothing because it is already handled
            }

            else{
              _log << "Error fail to remove pfd.fd " << tempFD << "\n";
            }
          }

          else{
            _log << "readPoll successful\n";
          }

        if(pfd.revents & POLLHUP){
          _log << "Poll detected that " << pfd.fd << " hanged up POLLHUP\n";
          closeSocketfd(pfd.fd);
          removePollfd(pfd.fd);
        }
      }
  }
  if(_infoModified){
    _infoModified = false;
    return;
  }
}

bool CChatRoom::addUser(std::string user){
  //_mtx.lock();
  _users.push_back(user);
  _size++;
  _totalUsernameSize += user.size();
  logsvsv(added user, user, to room , _name);
  psvsv(added user, user, to room , _name);
  //_mtx.unlock();
}

bool CChatRoom::removeUser(std::string user){
  //_mtx.lock();
_log << "------------removeUser---------------\n";
  int index = 0;
  for(std::string person: _users){
      if(person.compare(user) == 0){
          _users.erase(_users.begin() + index);
          _totalUsernameSize -= user.size();
          _size -= 1;
          _log << "Remove user " << person << " from " 
              << _name << "\n";
          psvsv(removed user, person, from, _name);
          return true;
      }
      index++;
  }
  _log << "User " << user << " not in " << _name << "\n";
  return false;
  //_mtx.unlock();
}

bool CChatRoom::readPoll(int sockfd){
  logsvs(= Unpack packet from, sockfd, =);
  int packetLength = removeInt(sockfd);
  /* Close socket if its EOF which is msgLength = -1*/
  if(packetLength == -1){
    closeSocketfd(sockfd);
    for(int i = 0; i < _userFD.size(); ++i){
      if(sockfd == _userFD[i]){
        psvs(user , _users[i], has diconnected and removed);
        removePollfd(sockfd);
        _users.erase(_users.begin() + i);
      }
    }
    return false;
  }

  int action = removeShort(sockfd);

  /* Subtract action's two bytes */
  packetLength -= 2;
  if(packetLength < 0){
    logs(Error message is 0 or smaller in size);
    return false;
  }

  std::string msg = removeBytes(sockfd, packetLength);
  logs(= Finish unpack message =);
  processMessage(sockfd, action, msg);
  return true;
}

void CChatRoom::processMessage(int sockfd, int action, std::string msg){
  switch(action){
    case DELIVER_MESSAGE_PACKET:
    processDeliverMessage(sockfd, msg);
    break;

    case LEAVE_ROOM:
    processLeaveRoom(sockfd, msg);
    break;

    case DISCONNECT:
    processDisconnect(sockfd, msg);
    break;
    
    default:
    break;
  }
}

void CChatRoom::processDeliverMessage(int sockfd, std::string msg){
  //msg is Username,Message
  logs(Delivering message-----);
  ps(Delivering message-----);
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  std::string username = words[0];
  std::string message = words[1];

  //make packet
  int packetSize = ACTION_SIZE + username.size() + 1 + message.size() + 1;
  std::string packet;
  appendInt(packet, packetSize);
  appendShort(packet, DELIVER_MESSAGE_PACKET);
  appendString(packet, username);
  appendString(packet, message);
  writeToAll(packet);




}

void CChatRoom::processLeaveRoom(int sockfd, std::string msg){
  //msg is Username, Roomname
  //_mtx.lock();
  logsv(processLeaveRoom for sockfd, sockfd);
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  std::string username = words[0];
  std::string room = words[1];
  int index = 0;
  for(std::string &usr : _users){
    if(usr.compare(username) == 0){
      break;
    }
    index++;
  }
  _network->addUser(username);
  _network->addPollfd(_userFD[index]);
  psvsv(Erase , _users[index], from room , _name);
  _users.erase(_users.begin() + index);
  psvsv(Erase , _userFD[index], from room , _name);
  _userFD.erase(_userFD.begin() + index);
  _userPollfd.erase(_userPollfd.begin() + index);
  _size--;
  _infoModified = true;
  //_mtx.unlock();
}

void CChatRoom::processDisconnect(int sockfd, std::string msg){
  logsv(processDisconnect for sockfd, sockfd);
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  std::string username = words[0];
  int index; 

  for(std::string &usr : _users){
    if(usr.compare(username) == 0){
      break;
    }
    index++;
  }
  _users.erase(_users.begin() + index);
  _userFD.erase(_userFD.begin() + index);
  _userPollfd.erase(_userPollfd.begin() + index);
}

void CChatRoom::writeToAll(std::string msg){
  logs(writeToAll -------------------------);
    for(int fd: _userFD){ 
    int result = write(fd, (void *)msg.data(), msg.size());
    logsvsv(Sent message of size, msg.size(), to sockfd, fd);
    if(result != msg.size()){
      psv(Error cannot write entire message to socket, fd);
      logsvsv(Error write only, result, when expected to write, msg.size());
    }
  }
  _log.flush();
}

void CChatRoom::startChatRoom(){
  //indicate that it is threated
  logsv(Successfully threated , _name);
  _threated = 1;
  while(1){
    startReading();
    _log.flush();     
  }
}
