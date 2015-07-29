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
    /* 
    * Add four bytes as in unsigned integer to packet
    */

    void appendInt(std::string&, uint32_t);

    /*
    *  Add two bytes as in unsigned short to packet
    */
    void appendShort(std::string&, uint16_t);

    /*
    *  Add string to packet, string can be any size and size 
    *  should be equal to length
    */
    void appendString(std::string&, std::string);

    /* 
    * Take out info from message basically the reverse of append
    * this is for reading replies from server
    */
    int removeInt(int);
    int removeShort(int);
    std::string removeBytes(int, int);

    /*
      Send message through sockfd
    */
    void writeMsg(int, std::string &);

};

#endif