/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.tests.modules.graph.utils;

import java.util.Timer;
import java.util.TimerTask;

import org.scilab.modules.action_binding.utils.Signal;
import org.junit.*;

/**
 * Test for the {@link Signal} class.
 */
public class SignalTest {
    /**
     * Check that the following simple sequence is valid :
     * <pre>
     * wait
     * notify
     * </pre>
     */
    @Test
    public void simpleNotify() {
        final String index = "1";

        new Timer().schedule((new TimerTask() {
            @Override
            public void run() {
                Signal.notify(index);
            }
        }), 1000);

        Signal.wait(index);
    }

    /**
     * Check that the inverted sequence is valid :
     * <pre>
     * notify
     * wait
     * </pre>
     */
    @Test
    public void invertedNotify() {
        final String index = "1";

        new Timer().schedule((new TimerTask() {
            @Override
            public void run() {
                Signal.wait(index);
            }
        }), 1000);

        Signal.notify(index);
    }
}
