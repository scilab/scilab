/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
*
* Copyright (C) 2012 - 2016 - Scilab Enterprises
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
*
*/

#ifndef __WEBCOMMAND_HXX__
#define __WEBCOMMAND_HXX__

#include <iostream>
#include <string>
#include <condition_variable>

#include "sio_client.h"

class command_listener
{
public:

    command_listener() : connect_finish(false) {}

    void on_connected()
    {
        std::cout << "command.on_connected" << std::endl;
        lock.lock();
        cond.notify_all();
        connect_finish = true;
        lock.unlock();
    }

    void on_close(sio::client::close_reason const& reason)
    {
        std::cout << "command.on_close" << std::endl;
    }

    void on_fail()
    {
        std::cout << "command.on_fail" << std::endl;
    }

    std::condition_variable_any cond;
    bool connect_finish;
    std::mutex lock;
};


class WebCommand
{
private :
    WebCommand(const std::string& addr);
    ~WebCommand() {}

    static WebCommand* me;

public :
    static void create(const std::string& addr);
    static void destroy() {}

private:
    std::string addr;
    sio::socket::ptr s;
    sio::client client;
    command_listener l;

};

#endif /* !__WEBCOMMAND_HXX__ */