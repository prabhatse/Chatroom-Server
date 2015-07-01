#include "MessageHandler.hpp"
extern std::ofstream _log;

MessageHandler::MessageHandler(){

}

void MessageHandler::appendInt(std::string& msg, uint32_t size){
    int start = msg.size();
    msg.resize(start + sizeof(size));
    _log << "appendInt " << size << "\n";
    uint32_t networkSize = htonl(size);
    *(uint32_t*)(msg.data() + start) = networkSize;
}

void MessageHandler::appendShort(std::string& msg, uint16_t action){
    int start = msg.size();
    msg.resize(start + sizeof(action));
    _log << "appendShort " << action << "\n";
    uint16_t networkAction = htons(action);
    *(uint16_t*)(msg.data() + start) = networkAction;
}

void MessageHandler::appendString(std::string& msg, std::string str){
    int start = msg.size();
    str += ',';
    _log << "appendString string and size " << str << " "
        << str.size() << "\n";
    msg.resize(start + str.size());

    memcpy((void *)(msg.data() + start), str.data(), str.size());
}

uint32_t MessageHandler::removeInt(int sockfd){
    uint32_t uInteger;
    int result = read(sockfd, &uInteger, sizeof(uInteger));
    if(result == 0){
        std::cout << "Error only EOF is read" << std::endl;
        _log << "Error only EOF is read\n";
    }
    else if(result < 0){
        std::cout << "Error reading socket." << std::endl;
        _log << "Error reading socket\n";
    }
    if(result != MESSAGE_SIZE){
        _log << "Error not reading 4 bytes for removeInt\n"; 
    }
    uInteger = ntohl(uInteger);
    _log << "Read " << result << " bytes from sockfd\n";
    _log << "Read value " << uInteger << "\n";
    return value;
}

uint16_t MessageHandler::removeShort(int sockfd){
    uint16_t uShort;
    int result = read(sockfd, &uShort, sizeof(uShort));
    if(result == 0){
        std::cout << "Error only EOF is read" << std::endl;
        _log << "Error only EOF is read\n";
    }
    else if(result < 0){
        std::cout << "Error reading socket" << std::endl;
        _log << "Error reading socket\n";
    }
    if(result != ACTION_SIZE){
        _log << "Error not reading 2 bytes for removeShort\n";
    }
    uShort = ntohs(uShort);
    _log << "Read action/short " << uShort <<"\n";
    return uShort;
}

std::string MessageHandler::removeBytes(int sockfd, int numBytes){
    std::string msg;
    msg.resize(numBytes);
    int result = read(sockfd, (void *)msg.data(), (size_t)numBytes);
    if(result == 0){
        std::cout << "Error only EOF is read\n" << std::endl;
    }
    else if(result < 0){
        std::cout << "Error reading socket\n" << std::endl;
    }
    if(result != numBytes){
        _log << "Error not reading " << numBytes << " from removeBytes\n";
    }

    _log << result << "bytes are read\n";
    _log << "Read message: " << msg << "\n";
    return msg;
}

void MessageHandler::writeMsg(int sockfd, std::string &msg){

    int result = write(sockfd, (void *)msg.data(), msg.size());
    _log << "Sent message of size " << msg.size() << "\n";
    if(result != msg.size()){
        std::cout << "Error cannot write entire message" << std::endl;
    }
}