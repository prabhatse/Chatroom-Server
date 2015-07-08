#include "Network.hpp"
#define MAX_WAITING_CONNECTIONS 10

extern std::ofstream _log;
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
    std::cout << "New connection " << newsockfd << std::endl;
  }

  return true;

}


bool CNetwork::readPoll(int sockfd){
  _log << "Attempt to read from sockfd " << sockfd << "\n";
  int msgLength = msgHandler.removeInt(sockfd);
  /* Close socket if its EOF which is msgLength = -1*/
  if(msgLength == -1){
    closeSocketfd(sockfd);
    return false;
  }

  int action = msgHandler.removeShort(sockfd);

  if(action == -1){
    closeSocketfd(sockfd);
  }
  if(action < 0){
    _log << "Error action number is negative\n";
    return false;
  }

  /* Subtract action's two bytes */
  msgLength -= 2;
  if(msgLength <= 0){
    _log << "Error message is 0 or smaller in size\n";
    return false;
  }

  std::string msg = msgHandler.removeBytes(sockfd, msgLength);
  if(msg.empty()){
    _log << "Error message string is empty\n";
    closeSocketfd(sockfd);
    return false;
  }

  msgHandler.processMessage(action, msg);

  return true;

}

void CNetwork::start(){
  int numToRead = setReadPoll(400);

  if(numToRead > 0){
    std::cout << "One connection found." << std::endl;
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
            //closeSocketfd(tempFD);
          }

          else{
            _log << "Error fail to remove pfd.fd " << tempFD << "\n";
          }
        }

        else{
          _log << "readPoll successful\n";
        }
      }

      else if(pfd.revents & POLLHUP){
        _log << "Poll detected that " << pfd.fd << " hanged up POLLHUP\n";
        closeSocketfd(pfd.fd);
        removePollfd(pfd.fd);
      }
    }
  }
}

void CNetwork::closeSocketfd(int sockfd){
 int closeResult;
    if(closeResult = close(sockfd) == 0){
      // Successfully closed pfd
      _log << "Successfully closed pfd " << sockfd <<"\n";
    }

    else{//Unsuccessful at closing pfd
      if(closeResult == EBADF){
        _log << "Error on close, EBADF\n";
      }

      else if(closeResult == EINTR){
        _log << "Error on close, EINTR\n";
      }

      else if(closeResult == EIO){
        _log << "Error on close, EIO\n";
      }
      _log << "Error cannot close pfd " << sockfd << closeResult<< "\n";
    }
}

