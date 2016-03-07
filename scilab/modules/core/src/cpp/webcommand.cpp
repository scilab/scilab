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

#include "webcommand.hxx"

extern "C"
{
#include "storeCommand.h"
}

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib,"sioclientd.lib")
#else
#pragma comment(lib,"sioclient.lib")
#endif
#endif

WebCommand* WebCommand::me = nullptr;

void WebCommand::create(const std::string& addr)
{
    if (me == nullptr)
    {
        me = new WebCommand(addr);
    }
}

WebCommand::WebCommand(const std::string& addr)
{
    //set and start sioclient
    //bind main event on socket
    client.set_open_listener(std::bind(&command_listener::on_connected, &l));
    client.set_close_listener(std::bind(&command_listener::on_close, &l, std::placeholders::_1));
    client.set_fail_listener(std::bind(&command_listener::on_fail, &l));

    //connect it
    client.connect(addr);

    //wait connection OK
    l.lock.lock();
    if (!l.connect_finish)
    {
        l.cond.wait(l.lock);
    }

    l.lock.unlock();

    //get connected socket
    s = client.socket();

    s->on("command", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp)
    {
        //lock socket
        l.lock.lock();

        //get command from message
        std::string cmd = data->get_map()["data"]->get_string();

        //put command on storecommand with wait flag
        StoreConsoleCommand((char*)(cmd + ";").data(), 1);

        l.lock.unlock();

        s->emit("command_end");
    }));
}
