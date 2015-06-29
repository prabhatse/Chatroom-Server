#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <poll.h>

#include "Poller.hpp"

class CNetwork: public CPoller{
private:
    static CNetwork* network;
    int _sockfd, _portno;
    std::vector<int> _userFD;
   // std::vector<pollfd> _userPollfd;
    sockaddr_in _serv_addr;
    CNetwork(int portno);

public: 
    static CNetwork& getNetwork(int portno=49999);
    bool acceptConnection();
    bool processReadPoll(int);
    void start();

};

#endif