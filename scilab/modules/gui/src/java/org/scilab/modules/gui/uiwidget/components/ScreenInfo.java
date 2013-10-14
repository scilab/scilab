/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.DisplayMode;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;

import javax.swing.FocusManager;

/**
 * Screen tools
 */
public class ScreenInfo {

    /**
     * Get the screen id containing the top-left corner of the given component
     * @param c the component
     * @return the screen id
     */
    public static int getScreenID(Component c) {
        GraphicsDevice[] gds = GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices();
        for (int i = 0; i < gds.length; i++) {
            Rectangle r = gds[i].getDefaultConfiguration().getBounds();
            if (r.contains(c.getLocationOnScreen())) {
                return i;
            }
        }

        return 0;
    }

    /**
     * Get the screen dimensions
     * @param id the screen id
     * @return the dimensions
     */
    public static Dimension getScreenDimension(int id) {
        if (id >= 0) {
            DisplayMode mode = GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices()[id].getDisplayMode();

            return new Dimension(mode.getWidth(), mode.getHeight());
        }

        return new Dimension(0, 0);
    }

    /**
     * Get the screen bounds
     * @param id the screen id
     * @return the bounds
     */
    public static Rectangle getScreenBounds(int id) {
        if (id >= 0) {
            return GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices()[id].getDefaultConfiguration().getBounds();
        }

        return new Rectangle(0, 0, 0, 0);
    }

    /**
     * Center a window on the window containing parent (if not null). If parent is null, the window
     * is centered on the screen of the last Java component which had the focus or on the first screen
     * @param win the window to center
     * @param parent the parent Component (can be null)
     */
    public static void center(Window win, Component parent) {
        if (win != null) {
            win.setLocationRelativeTo(null);
            Dimension wsize = win.getSize();
            Point p = new Point(0, 0);
            Dimension size;

            if (parent != null) {
                p = parent.getLocationOnScreen();
                size = parent.getSize();
            } else {
                Component c = FocusManager.getCurrentManager().getFocusOwner();
                if (c == null) {
                    size = ScreenInfo.getScreenDimension(0);
                } else {
                    Rectangle r = ScreenInfo.getScreenBounds(ScreenInfo.getScreenID(c));
                    size = new Dimension(r.width, r.height);
                    p.x = r.x;
                    p.y = r.y;
                }
            }

            win.setLocation(p.x + (size.width - wsize.width) / 2, p.y + (size.height - wsize.height) / 2);
        }
    }
}
