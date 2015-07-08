#ifndef POLLER_HPP
#define POLLER_HPP

#include <poll.h>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>

class CPoller{
  protected:
    std::vector<pollfd> _userPollfd;
    std::vector<int> _userFD;
    virtual int setReadPoll(int);
    virtual bool readPoll(int) = 0;
    virtual bool removePollfd(int);
};

#endif