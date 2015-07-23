#ifndef MACROZ_HPP
#define MACROZ_HPP
#include <iostream>
#include <mutex>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
extern std::ofstream _log;
extern std::mutex _mtx;
#define MAX_WAITING_CONNECTIONS 10
#define MAX_ROOMS 10
// Delivery Scheme
#define PACKET_SIZE 4
#define ACTION_SIZE 2
#define CLIENT_AUTH 0
#define GET_AVAILABLE_ROOMS 1
#define GET_ROOM_STATUS 2
#define CREATE_ROOM 3
#define JOIN_ROOM 4
#define LEAVE_ROOM 5
#define DELIVER_MESSAGE_PACKET 6
#define DISCONNECT 7

#define logs(x) _log << #x << "\n";
#define logsv(x,y) _log << #x << " " << y << "\n";
#define logsvs(x,y,z) _log << #x << " " << y << " " << #z << "\n";
#define logsvsv(w,x,y,z) _log << #w << " " << x << " " << #y << " "<< z << "\n";

#define ps(x) std::cout << #x << std::endl;
#define psv(x,y) std::cout << #x << " " << y << std::endl;
#define psvs(x,y,z) std::cout << #x << " " << y << " " << #z << std::endl;
#define psvsv(w,x,y,z) std::cout << #w << " " << x << " " << #y << " "<< z << std::endl;



#endif

