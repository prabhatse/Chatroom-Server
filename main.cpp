#include "Network.hpp"
#include <iostream>
#include <poll.h>
#include <fstream>
#include "Poller.hpp"
int main(){
    std::ofstream _log("log.txt", std::ofstream::out);
    CNetwork &network = CNetwork::getNetwork(49999);
    while(1){
        network.acceptConnection();
        network.start();
        

    }
    network.start();
    return 0;
}