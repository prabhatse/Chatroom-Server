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

  //disable naggle's algorithm
  int optval = 1;
  status = setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
  assert(optval == 1);
  if(status == -1){
          perror("Error on disabling Naggle's algorithm.");
  }

  // bind to server and listen
  status = bind(_sockfd, (sockaddr *) &_serv_addr, sizeof(_serv_addr));
  if(status == -1){
    perror("Error on bind.");
    exit(EXIT_FAILURE);
  }

  // listen for new connection
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
  }

  return true;

}


bool CNetwork::processReadPoll(int numToRead){
  int bob = 5;
  return bob;
}

void CNetwork::start(){
  int numToRead = setReadPoll(500);
  processReadPoll(numToRead);
}
