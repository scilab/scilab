/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Clement DAVID
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

package org.scilab.tests.modules.action_binding.utils;

import org.scilab.modules.action_binding.utils.Signal;
import org.junit.*;

/**
 * Check of {@link Signal} class
 */
public class SignalTest {

    final String signal = "plop";

    final Runnable waiter = new Runnable() {
        @Override
        public void run() {
            Signal.wait(signal);
        }
    };

    final Runnable notifier = new Runnable() {
        @Override
        public void run() {
            Signal.notify(signal);
        }
    };

    @Test(timeout = 10000)
    public void simpleNotify() throws InterruptedException {
        Thread tWaiter = new Thread(waiter);
        Thread tNotifier = new Thread(notifier);

        // wait
        tWaiter.start();

        // then notify
        tNotifier.start();

        tWaiter.join();
        tNotifier.join();
    }

    @Test(timeout = 10000)
    public void notifyBeforeWait() throws InterruptedException {
        Thread tWaiter = new Thread(waiter);
        Thread tNotifier = new Thread(notifier);

        // notify
        tNotifier.start();

        // then wait
        tWaiter.start();

        tWaiter.join();
        tNotifier.join();
    }
}
