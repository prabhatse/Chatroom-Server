#include "Network.hpp"
#include "Macroz.hpp"

CNetwork* CNetwork::network;
CNetwork::CNetwork(int portno):
        _sockfd(0),
        _portno(portno)
{
  int status;
  _serv_addr.sin_family = AF_INET;
  _serv_addr.sin_addr.s_addr = INADDR_ANY;
  _serv_addr.sin_port = htons(_portno);

  //Create tcp socket
  _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(_sockfd == -1){
    perror("Error on binding socket.");
    exit(EXIT_FAILURE);
  }

  //Make socket non-blocking:don't wait for packets
  int flags = fcntl(_sockfd, F_GETFL, 0) | O_NONBLOCK;
  status = fcntl(_sockfd, F_SETFL, flags);
  if(status == -1){
    perror("Error on bind.");
    exit(EXIT_FAILURE);
  }

  //Disable nagle's algorithm
  int optval = 1;
  status = setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
  assert(optval == 1);
  if(status == -1){
          perror("Error on disabling Naggle's algorithm.");
  }

  //Bind to server and listen
  status = bind(_sockfd, (sockaddr *) &_serv_addr, sizeof(_serv_addr));
  if(status == -1){
    perror("Error on bind.");
    exit(EXIT_FAILURE);
  }

  // Listen for new connection
  status = listen(_sockfd, MAX_WAITING_CONNECTIONS);
  if(status == -1){
    perror("Error on bind");
    exit(EXIT_FAILURE);
  }

  if(status < 0){
    close(_sockfd);
    fprintf(stderr, "Failure initializing Network. "
      "Program will now exit. \n");
    exit(EXIT_FAILURE);
  }

}// Network


CNetwork& CNetwork::getNetwork(int portno){

  if(network == nullptr){
    network = new CNetwork(portno);
  }

  return *network;
}//getNetwork

bool CNetwork::acceptConnection(){
  // add new connection if available
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);
  int newsockfd = accept(_sockfd, (struct sockaddr* ) &cli_addr, &clilen);
  // Problem with connection
  if(newsockfd == -1){
    if(errno == EAGAIN || errno == EWOULDBLOCK){
      return false;
    }
    else{
      fprintf(stderr, "ERROR main: some error on"
              " accepting connection.");
      return false;
    }
  }
  // add connection to the list
  else{
    struct pollfd fileDes;
    fileDes.fd = newsockfd;
    _userFD.push_back(newsockfd);
    _userPollfd.push_back(fileDes);
    _log << "Add " << fileDes.fd << " to vector of file descriptors\n";
    std::cout << "added new connection " << newsockfd << std::endl;
  }

  return true;

}


bool CNetwork::readPoll(int sockfd){
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
        std::cout.flush();
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

void CNetwork::start(){
  int numToRead = setReadPoll(400);

  if(numToRead > 0){
    std::cout << "One request found in server." << std::endl;
    psv(_userFD size is , _userFD.size());
    psv(_userPollfd size is , _userPollfd.size());
    psv(_user size is , _users.size());
    _log << "One request from poll\n";
    for(struct pollfd& pfd: _userPollfd){

      /*There is a message to be processed*/
      if(pfd.revents & POLLIN){
        /* 
          In the case that something goes wrong
          remove it from pollfd and close socket connection 
        */
        /* Read for messages */
        if(readPoll(pfd.fd) == false){//something went wrong
          int tempFD = pfd.fd;// save pfd.fd

          if(removePollfd(pfd.fd)){ //remove pollfd struct
            closeSocketfd(tempFD);
          }

          else{
            _log << "Error fail to remove pfd.fd " << tempFD << "\n";
          }
        }

        else{
          _log << "after readPoll\n";
          ps(readPoll successful);
          psv(_userFD size is , _userFD.size());
          psv(_userPollfd size is , _userPollfd.size());
          psv(_user size is , _users.size());
        }
      }

      else if(pfd.revents & POLLHUP){
        _log << "Poll detected that " << pfd.fd << " hanged up POLLHUP\n";
        closeSocketfd(pfd.fd);
        _infoModified = true;
        removePollfd(pfd.fd);
      }
    }

    if(_infoModified == true){
      _infoModified = false;
      return;
    }
  }
}

void CNetwork::addRoom(int capacity, std::string name){
  _log << "Add room with capacity " << capacity <<
    " and name " << name << "\n";
    _rooms[_numRooms] = CChatRoom(capacity, name, this);
    _numRooms++;
}

bool CNetwork::addUser(std::string username){
  //add only if user did not exist
  //_mtx.lock();addRaddR
  for(std::string name: _users){
    if(name.compare(username) == 0){
      logsvs(user, username, already exist);
      psvs(cannot add user, username, he already exist);
      return false;
    }
  }

  _users.push_back(username);
  logsv(added user, username);
  psv(added user, username);
  return true;
  //_mtx.unlock();
}

/*****************************
*******Manages packets********
*****************************/
bool CNetwork::processMessage(int sockfd, int action, std::string& msg){
  switch(action){
    case CLIENT_AUTH:
      Write_ClientAuth(sockfd, msg);
      break;
    case GET_AVAILABLE_ROOMS:
      Write_GetAvailableRooms(sockfd, msg);
      break;
    case GET_ROOM_STATUS:
      Write_GetRoomStatus(sockfd, msg);
      break;
    case CREATE_ROOM:
      Write_CreateRoom(sockfd, msg);
      break;
    case JOIN_ROOM:
      Write_JoinRoom(sockfd, msg);
      break;
    case LEAVE_ROOM:
      Write_LeaveRoom(sockfd, msg);
      break;
    case DELIVER_MESSAGE_PACKET:
      Write_DeliverMessagePacket(sockfd, msg);
      break;
    case DISCONNECT:
      Write_Disconnect(sockfd, msg);
      break;
  }
  return true;
}
void CNetwork::Write_ClientAuth(int sockfd, std::string &msg){
  _log << "ClientAuth--------\n";
  //add user into _user
  std::string authenticated("1");
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);

  //if you cannot add user
  if(addUser(words[0]) == false){
    authenticated = "0";
  }
  //send auth success
  std::string result;
  unsigned int packetSize = ACTION_SIZE + 1 + 1;
  appendInt(result, packetSize);
  appendShort(result, CLIENT_AUTH);
  appendString(result, authenticated);
  writeMsg(sockfd, result);

  //remove connection that has same username
  if(authenticated.compare("0") == 0){
    removePollfd(sockfd);
  }
}

void CNetwork::Write_GetAvailableRooms(int sockfd, std::string &msg){
  _log << "GetAvailableRooms--------\n";
  /* Size(4) | Action (2) | NumberRooms(?) |,(1)|rooms|... */
  psvs(connection ,sockfd, requested a list of available rooms);
  std::string result;
  uint32_t packetSize = 0;
  uint16_t action = GET_AVAILABLE_ROOMS;
  std::string numRooms;
  // Plus 2
  packetSize = ACTION_SIZE;

  // Should add mutex here */
  int numberOfRooms = _numRooms;
  /* Turn number of rooms into string */
  numRooms = std::to_string(numberOfRooms);
  /* If no rooms then don't add anything */
  if(numberOfRooms == 0){
    // + 1 stands for ','
    packetSize += numRooms.size() + 1;
    appendInt(result, packetSize);
    appendShort(result, action);
    appendString(result, numRooms);
    _log << "No room found \n";
  }
  /* Add each room's name to the packet */
  else{
    // + 1 stands for ','
    packetSize += numRooms.size() + 1;
    for(int i = 0; i < _numRooms; i++){
      packetSize += (_rooms[i]._name).size() + 1;
    }

    appendInt(result, packetSize);
    appendShort(result, action);
    appendString(result, numRooms);
    for(int i = 0; i < _numRooms; i++){
      appendString(result, _rooms[i]._name);
    }
    /* Append each room */
  }
  /* Send out message */
  writeMsg(sockfd, result);
  psvs(sent connection ,sockfd, a list of available rooms);
}

void CNetwork::Write_GetRoomStatus(int sockfd, std::string &msg){
 _log << "GetRoomStatus--------\n";
  //  Size(4)|Action(2)|Roomname(?)|,(1)|Capacity(?)|,(1)|Size(?)|,(1)|Username(?)|,(1)
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  std::string result;
  std::string roomName = words[1];
  std::string capacity = "0";
  std::string size = "0";

  psvsv(connection ,sockfd, requests room status of ,roomName);
  // If room not found packet will only have Action|Roomname|Capacity
  uint32_t packetSize = ACTION_SIZE + roomName.size() + 1 + capacity.size() + 1;
  for(int i = 0; i < _numRooms; i++){
    //make a reference
    CChatRoom &r = _rooms[i];
    //found the room
    if(r._name.compare(roomName) == 0){
      capacity = std::to_string(r._capacity);
      size = std::to_string(r._size);
      int totalUsernameSize = r._totalUsernameSize;
      packetSize = ACTION_SIZE + roomName.size() + 1 + capacity.size() + 1
        + size.size() + 1 + totalUsernameSize + r._size;
      // Make packet 
      appendInt(result, packetSize);
      appendShort(result, GET_ROOM_STATUS);
      appendString(result, roomName);
      appendString(result, capacity);
      appendString(result, size);
      for(std::string usr: r._users){
        appendString(result, usr);
      }
      writeMsg(sockfd, result);
      return;
    }
  }

  // Reaches here means chatroom not found
  appendInt(result, packetSize);
  appendShort(result, GET_ROOM_STATUS);
  appendString(result, roomName);
  appendString(result, capacity);
  writeMsg(sockfd, result);

  psvsv(send connection , sockfd, room status for , roomName);
}

void CNetwork::Write_CreateRoom(int sockfd, std::string &msg){
  _log << "CreateRoom--------\n";
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  int capacity;
  std::string user = words[1];
  std::string roomName = words[2];
  psvsv(user, user, wanted to create room , roomName);
  /* Take two bytes from string msg */
  try {
    capacity = boost::lexical_cast<int>( words[0] );
  } catch( boost::bad_lexical_cast const& ) {
    std::cout << "Error: input string was not valid" << std::endl;
  }
  std::string result = "1";
  //Check for duplicated rooms
  for(int i = 0; i < _numRooms; i++){
    if(_rooms[i]._name.compare(roomName) == 0){
      result = "0";
    }
  }
  if(result.compare("1") == 0){
    addRoom(capacity, roomName);
  }
  //Make packet
  std::string packet;
  int packetSize = ACTION_SIZE + result.size() + 1 + roomName.size() + 1;
  appendInt(packet, packetSize);
  appendShort(packet, CREATE_ROOM);
  appendString(packet, result);
  appendString(packet, roomName);
  writeMsg(sockfd, packet);
  psvsv(send user , user, results on creating room , roomName);
}

void CNetwork::Write_JoinRoom(int sockfd, std::string &msg){
  _log << "Making JoinRoom Result=====\n";
  /*
    Size(4)|Action(2)|Username(?)|,(1)|Room name(?)|,(1)
  */

  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  std::string username = words[0];
  std::string room = words[1];
  /* Make sure username and room exist */
  std::string joinRoomResult = "0";
  CChatRoom *savedRoom;
  for(int i = 0; i < _numRooms; ++i){
    //room exist
    CChatRoom &r = _rooms[i];
    if(r._name.compare(room) == 0){
      logs(Requested room found);
      //only process it when room is not full
      if(r._size < r._capacity){
        for(std::string& usr: _users){
          //user exist
          if(usr.compare(username) == 0){
            logsvs(Player , username, found);
            _mtx.lock();
            r.addUser(usr);
            // add socket information to 
            r.addPollfd(sockfd);
            // remove socket information from network
            removePollfd(sockfd);
            removeUser(username);
            _infoModified = true;
            _mtx.unlock();
            joinRoomResult ="1";
            savedRoom = &r;
          }
        }
      }
    }
  }
  /* Make packet */
  std::string result;
  uint32_t packetSize = ACTION_SIZE + joinRoomResult.size() + 1 + room.size() + 1;
  uint16_t actionNumber = JOIN_ROOM;
  appendInt(result, packetSize);
  appendShort(result, actionNumber);
  appendString(result, joinRoomResult);
  appendString(result, room);
  writeMsg(sockfd, result);
  //Launch a thread only if success
  // Only thread if not yet threaded
  if((joinRoomResult.compare("1") == 0) & savedRoom->_threated == 0){
    std::thread t1([savedRoom] {savedRoom->startChatRoom();});
    t1.detach();
  }

}

void CNetwork::Write_LeaveRoom(int sockfd, std::string &msg){
  // Size(4)|Action(2)|Result(2)|Roomname(?)

  _log << "Making LeaveRoom result--------\n";
  std::string result;
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  std::string username = words[0];
  std::string roomname = words[1];
  std::string leaveRoomResult = "0";
  int packetSize = ACTION_SIZE + leaveRoomResult.size() + 1 + roomname.size() + 1;
  for(int i = 0; i < _numRooms; i++){
    CChatRoom &r = _rooms[i];
    if(r._name.compare(roomname) == 0){
      if(r.removeUser(username) == false){
        return;
      }
      leaveRoomResult = "1";
      //Make packet
      appendInt(result, packetSize);
      appendShort(result, LEAVE_ROOM);
      appendString(result, leaveRoomResult);
      appendString(result, roomname);
      //Send packet
      writeMsg(sockfd, result);
    }
  }
}

void CNetwork::Write_DeliverMessagePacket(int sockfd, std::string &msg){
  _log << "DeliverMessagePacket--------\n";
  //make packet
  int packetSize = ACTION_SIZE + msg.size();
  std::string packet;
  appendInt(packet, packetSize);
  appendShort(packet, DELIVER_MESSAGE_PACKET);
  //custom append string
  int start = packet.size();
  msg.resize(start + msg.size());

  memcpy((void *)(packet.data() + start), msg.data(), msg.size());

  for(int fd: _userFD){
    writeMsg(fd, packet);
  }
}

void CNetwork::Write_Disconnect(int sockfd, std::string &msg){
  //Size(4)|Action(2)|Username(?)|,(1);
  _log << "Disconnect--------\n";
  std::vector<std::string> words;
  boost::split(words, msg, boost::is_any_of(","), boost::token_compress_on);
  removeUser(words[0]);
  removePollfd(sockfd);

}

void CNetwork::removeUser(std::string toBeRemoved){
  //_mtx.lock();
  logsv(removeUser , toBeRemoved);
  int index = 0;
  for(std::string usr: _users){
    if(usr.compare(toBeRemoved) == 0){
      _users.erase(_users.begin()+index);
      _infoModified = true;
      return;
    }
    index++;
  }
  //_mtx.unlock();
}

void CNetwork::printUsers(){
  ps(Users: );
  for(std::string usr:_users){
    std::cout << usr << " ";
  }
  ps();
}