/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.commons.gui;

import java.awt.event.WindowEvent;
import java.awt.event.WindowFocusListener;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

/**
 * GUI utilities
 * @author Calixte DENIZET
 */
public class ScilabGUIUtilities {

    /**
     * Bring a window to the front
     * @param window the window to bring to the front
     */
    public static void toFront(final JFrame window) {
        WindowFocusListener listener = new WindowFocusListener() {

            public void windowGainedFocus(WindowEvent e) {
                window.setAlwaysOnTop(true);
            }

            public void windowLostFocus(WindowEvent e) {
                window.setAlwaysOnTop(false);
                window.removeWindowFocusListener(this);
            }
        };
        window.addWindowFocusListener(listener);
        window.toFront();
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                window.requestFocus();
            }
        });
    }
}
