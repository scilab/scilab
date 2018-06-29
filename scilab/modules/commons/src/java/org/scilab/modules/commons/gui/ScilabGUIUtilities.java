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
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;

import org.flexdock.view.View;

/**
 * GUI utilities
 * @author Calixte DENIZET, Stéphane MOTTELET
 */
public class ScilabGUIUtilities {

    /**
     * Bring a window to the front
     * @param window the window to bring to the front
     */
    public static void toFront(final JFrame window) {
        /*
        WindowFocusListener listener = new WindowFocusListener() {

            public void windowGainedFocus(WindowEvent e) {
                window.setAlwaysOnTop(true);
            }

            public void windowLostFocus(WindowEvent e) {
                window.setAlwaysOnTop(false);
                window.removeWindowFocusListener(this);
            }
        };
        */

        int state = window.getExtendedState();
        if ((state & JFrame.ICONIFIED) == JFrame.ICONIFIED) {
            window.setExtendedState(state - JFrame.ICONIFIED);
        }

        /* not sure to understand the goal of that Oo*/
        //window.addWindowFocusListener(listener);
        window.toFront();
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                window.requestFocus();
            }
        });
    }

    public static void toFront(final View panel, final String panelName) {

        JTabbedPane pane = (JTabbedPane) SwingUtilities.getAncestorOfClass(JTabbedPane.class, panel);
        if (pane != null) { /* Help browser is docked in a tab */
            int tabCount = pane.getTabCount();
            for (int i = 0; i < tabCount; i++) {
                String tabTitle = pane.getTitleAt(i);
                if (tabTitle.equals(panelName)) {
                    pane.setSelectedIndex(i);
                    break;
                }
            }
        }

        /* move the parent window to front */
        JFrame window = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, panel);
        toFront(window);
        /* give focus to the panel */
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                panel.requestFocus();
            }
        });
    }

}
