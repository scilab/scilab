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

package org.scilab.modules.uiwidget.components;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.DisplayMode;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;

import javax.swing.FocusManager;

public class ScreenInfo {

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

    public static Dimension getScreenDimension(int id) {
        if (id >= 0) {
            DisplayMode mode = GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices()[id].getDisplayMode();

            return new Dimension(mode.getWidth(), mode.getHeight());
        }

        return new Dimension(0, 0);
    }

    public static Rectangle getScreenBounds(int id) {
        if (id >= 0) {
            return GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices()[id].getDefaultConfiguration().getBounds();
        }

        return new Rectangle(0, 0, 0, 0);
    }

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