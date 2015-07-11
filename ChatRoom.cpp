#include "ChatRoom.hpp"

extern std::ofstream _log;
CChatRoom::CChatRoom(int capacity, std::string roomName):_capacity(capacity),_roomName(roomName){

}

int CChatRoom::getSize(){
    return _size;
}

int CChatRoom::getCapacity(){
    return _capacity;
}

void CChatRoom::start(){

}

bool CChatRoom::addUser(std::string *user){
    _users.push_back(user);
}

bool CChatRoom::removeUser(std::string *user){
_log << "------------removeUser---------------\n";
    int index = 0;
    for(std::string *person: _users){
        if(person == user){
            _users.erase(_users.begin() + index);
            _log << "Remove user " << *person << " from " 
                << _roomName << "\n";
            return true;
        }
        index++;
    }
    _log << "User " << *user << " not in " << _roomName << "\n";
    return false;
}