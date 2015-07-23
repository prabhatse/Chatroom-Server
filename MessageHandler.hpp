#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>

class CMessageHandler{
protected:
    int sockfd;
public:
    CMessageHandler();
    /*
      Add info to message
    */
    void appendInt(std::string&, uint32_t);
    void appendShort(std::string&, uint16_t);
    void appendString(std::string&, std::string);

    /* 
      Take out info from message
    */
    int removeInt(int);
    int removeShort(int);
    std::string removeBytes(int, int);

    /*
      Send message through sockfd
    */
    void writeMsg(int, std::string &);

    /*
      Various types of message handling
    */
    friend class Network;
};

#endif