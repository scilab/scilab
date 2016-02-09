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

#ifndef __SCILAB_WEB_VIEW_HXX__
#define __SCILAB_WEB_VIEW_HXX__

#include <iostream>
#include <string>
#include <condition_variable>

#include "sio_client.h"

#include "dynlib_graphic_objects.h"

//disable warnings about exports STL items
#pragma warning (disable : 4251)

extern "C"
{
    void ScilabNativeWebView__createObject(int id);
    void ScilabNativeWebView__deleteObject(int id);
    void ScilabNativeWebView__updateObject(int id, int prop);
}

class graphic_listener
{
public:

    graphic_listener() : connect_finish(false) {}

    void on_connected()
    {
        std::cout << "graphic.on_connected" << std::endl;
        lock.lock();
        cond.notify_all();
        connect_finish = true;
        lock.unlock();
    }

    void on_close(sio::client::close_reason const& reason)
    {
        std::cout << "graphic.on_close" << std::endl;
    }

    void on_fail()
    {
        std::cout << "graphic.on_fail" << std::endl;
    }

    std::condition_variable_any cond;
    bool connect_finish;
    std::mutex lock;
};

class GRAPHIC_OBJECTS_IMPEXP ScilabWebView
{
private :
    ScilabWebView() {}
    ~ScilabWebView() {}

    static ScilabWebView* me;
    // Define type for easy manipulation.
public :

    static ScilabWebView* getInstance() 
    {
        if (me == nullptr)
        {
            me = new ScilabWebView();
        }

        return me;
    }

    void createObject(int uid);
    void deleteObject(int uid);
    void updateObject(int uid, int prop);

    void registerToController(void);
    void unregisterToController(void);

private:
    std::string addr;
    sio::socket::ptr s;
    sio::client client;
    graphic_listener l;
};

#endif /* !__SCILAB_WEB_VIEW_HXX__ */

