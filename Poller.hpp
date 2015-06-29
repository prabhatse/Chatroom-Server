#ifndef POLLER_HPP
#define POLLER_HPP

#include <poll.h>
#include <vector>
#include <iostream>

class CPoller{
  protected:
    std::vector<pollfd> _userPollfd;
    virtual int setReadPoll(int);
    virtual bool processReadPoll(int) = 0;
};

#endif