/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.action_binding.utils;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;

/**
 * Perform the communication between java code and scilab code trough an UID.
 */
public final class Signal {

    private static final int DELAY_EXEC = 100; // milliseconds

    private static Map<String, Object> waiters = Collections.synchronizedMap(new HashMap<String, Object>());

    /** This class is a static singleton, thus it must not be instantiated */
    private Signal() {
    }

    /**
     * Add a waiter with the corresponding uid.
     *
     * @param index
     *            The uid we are waiting for.
     */
    public static void wait(String index) {
        Object data = new Object();
        waiters.put(index, data);

        synchronized (data) {
            try {
                data.wait();
            } catch (InterruptedException e) {
                Logger.getLogger(Signal.class.getName()).severe(e.toString());
            }
        }
    }

    /**
     * Notify a waiter.
     *
     * The associated scilab call is xcosNotify.
     *
     * @param index
     *            The uid to be notified. No one is waiting for the uid at time
     *            N, this method wait and retry each 100 milliseconds.
     */
    public static void notify(String index) {
        Object data = waiters.get(index);
        while (data == null) {
            try {
                Thread.sleep(DELAY_EXEC);
                data = waiters.get(index);
            } catch (InterruptedException e) {
                Logger.getLogger(Signal.class.getName()).severe(e.toString());
            }
        }

        synchronized (data) {
            data.notify();
        }
        waiters.remove(index);
    }
}
