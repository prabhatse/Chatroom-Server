#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP


#define MESSAGE_SIZE 4
#define ACTION_SIZE 2

#include <iostream>
#include <string>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>

class MessageHandler{
private:
    int sockfd;
public:
    MessageHandler();
    void appendInt(std::string&, uint32_t);
    void appendShort(std::string&, uint16_t);
    void appendString(std::string&, std::string);
    uint32_t removeInt(int);
    uint16_t removeShort(int);
    std::string removeBytes(int, int);
    void writeMsg(int, std::string &);

    friend class Network;
};

#endif