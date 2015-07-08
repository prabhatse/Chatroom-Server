#include "Poller.hpp"

extern std::ofstream _log;

int CPoller::setReadPoll(int timeout){
  
  for(struct pollfd& pfd: _userPollfd){
   pfd.events = POLLIN;// | POLLPRI;
   pfd.revents = 0;
  }

  int ret = poll(_userPollfd.data(), _userPollfd.size(), timeout);
  if(ret < 0){
    std::cout << "Problem with return from poll" << std::endl;
    return 0;
  }

  return ret;
}

bool CPoller::removePollfd(int sockfd){
    int index = 0;
    for(struct pollfd& pfd: _userPollfd){
        if(pfd.fd == sockfd){
            _userPollfd.erase(_userPollfd.begin() + index);
            _userFD.erase(_userFD.begin() + index);
            _log << "removed sockfd " << sockfd << " from _userPollfd\n";
            return true;
        }
        index++;
    }

    return false;
}