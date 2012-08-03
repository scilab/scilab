/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
