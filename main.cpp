#include "Network.hpp"
#include <iostream>
#include <poll.h>
#include <fstream>
#include "Poller.hpp"

std::ofstream _log("log.txt", std::ofstream::out);
int main(){
    CNetwork &network = CNetwork::getNetwork(49999);
    while(1){
        network.acceptConnection();
        network.start();
        _log.flush();     

    }
    return 0;
}