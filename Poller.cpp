#include "Poller.hpp"

extern std::ofstream _log;

int CPoller::setReadPoll(int timeout){
  
  for(struct pollfd& pfd: _userPollfd){
   pfd.events = POLLIN;
  }

  int ret = poll(_userPollfd.data(), _userPollfd.size(), timeout);
  if(ret < 0){
    std::cout << "Problem with return from poll" << std::endl;
  }

  return ret;
}