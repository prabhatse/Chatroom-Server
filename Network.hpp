#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "MessageHandler.hpp"
#include "ServerMessageHandler.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <poll.h>
#include <fstream>

#include "Poller.hpp"

class CNetwork: public CPoller{
private:
    static CNetwork* network;
    int _sockfd, _portno;
    CServerMessageHandler msgHandler;
    sockaddr_in _serv_addr;
    CNetwork(int portno);
    std::string rooms[10];

public: 
    static CNetwork& getNetwork(int portno=49999);
    void closeSocketfd(int);
    bool acceptConnection();
    bool readPoll(int);
    void start();

};

#endif