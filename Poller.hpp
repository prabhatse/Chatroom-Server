#ifndef POLLER_HPP
#define POLLER_HPP

#include "Macroz.hpp"

#include <poll.h>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>

class CPoller{
  protected:
    /*_userPollfd is a vector of struct pollfd
     * it is use by poll.h
     */
    std::vector<pollfd> _userPollfd;
    /* _userFD is just a list of integer representing the file descriptors
     * user will communicate with server throught these file descriptors
     */
    std::vector<int> _userFD;
  public:
    /* The will be called at the beginning to watch all the filedescriptors
     * for incoming messages
     */
    virtual int setReadPoll(int);
    /* It is up to the child class to define how to handle the 
     * polls
     */
    virtual bool readPoll(int) = 0;
    /* When a user is no longer needed in a class his information will be removed
     * this includes file descriptor and pollfd
     */
    virtual bool removePollfd(int);

    /* Add to _userPollfd */
    bool addPollfd(int);

    /* Close file descriptors */
    void closeSocketfd(int);
};

#endif