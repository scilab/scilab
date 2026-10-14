/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.AWTEvent;
import java.awt.Component;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;

import org.flexdock.docking.drag.DragGlasspane;
import org.flexdock.docking.drag.DragManager;
import org.flexdock.util.RootWindow;

/**
 * FlexDock follows the window under a drag through enter/exit events on its
 * glasspanes, which macOS does not reliably deliver during a drag: the drag
 * then stays on a window it left. An exit sent to every window not under the
 * cursor lets the bounds-based lookup enabled by "heavyweight.dockables",
 * which this relies on, pick the right one.
 */
final class DragTargetTracker implements AWTEventListener {

    private static boolean installed;

    private DragTargetTracker() {
    }

    static synchronized void install() {
        if (!installed) {
            installed = true;
            Toolkit.getDefaultToolkit().addAWTEventListener(new DragTargetTracker(), AWTEvent.MOUSE_MOTION_EVENT_MASK);
        }
    }

    /** Runs before the drag source's listeners, hence before FlexDock handles the event. */
    @Override
    public void eventDispatched(AWTEvent event) {
        if (event.getID() != MouseEvent.MOUSE_DRAGGED || DragManager.getCurrentDragOperation() == null) {
            return;
        }
        MouseEvent e = (MouseEvent) event;
        // Already handled by FlexDock: the native event notified after its lightweight retarget.
        if (e.isConsumed()) {
            return;
        }
        Point screen = e.getLocationOnScreen();
        for (RootWindow window : RootWindow.getVisibleWindows()) {
            Component glass = window.getGlassPane();
            if (glass instanceof DragGlasspane && !window.getBounds().contains(screen)) {
                glass.dispatchEvent(new MouseEvent(glass, MouseEvent.MOUSE_EXITED, e.getWhen(), 0, 0, 0, 0, false));
            }
        }
    }
}
