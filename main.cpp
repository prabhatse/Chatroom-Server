#include "Network.hpp"
#include <iostream>
#include <poll.h>
#include <fstream>
#include <mutex>
#include "Poller.hpp"

void readInput(CNetwork*);
std::mutex _mtx;
std::ofstream _log("log.txt", std::ofstream::out);
int main(){
    CNetwork &network = CNetwork::getNetwork(49999);
    //a thread that accept user input to check on network
    std::thread t1(readInput, &network);
    while(1){
        network.acceptConnection();
        network.start();
        _log.flush();     

    }
    return 0;
}

void readInput(CNetwork *network){
    std::string input;
    while(1){
    getline(std::cin, input);
    if(input.compare("users") == 0){
      network->printUsers();
    }
    }
}