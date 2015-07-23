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
  //_mtx.lock();
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
  //_mtx.unlock();
}

bool CPoller::addPollfd(int sockfd){
  //_mtx.lock();
  struct pollfd newpfd;
  newpfd.fd = sockfd;
  _userPollfd.push_back(newpfd);
  logsv(added fd, sockfd);
  _userFD.push_back(sockfd);
  logsv(added socket, sockfd);
  //_mtx.unlock();
}

void CPoller::closeSocketfd(int sockfd){
 int closeResult;
    if(closeResult = close(sockfd) == 0){
      // Successfully closed pfd
      _log << "Successfully closed pfd " << sockfd <<"\n";
    }

    else{//Unsuccessful at closing pfd
      if(closeResult == EBADF){
        _log << "Error on close, EBADF\n";
      }

      else if(closeResult == EINTR){
        _log << "Error on close, EINTR\n";
      }

      else if(closeResult == EIO){
        _log << "Error on close, EIO\n";
      }
      _log << "Error cannot close pfd " << sockfd << closeResult<< "\n";
    }
}