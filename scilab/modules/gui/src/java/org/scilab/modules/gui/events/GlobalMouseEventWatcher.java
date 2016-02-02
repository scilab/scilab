/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno Jofret

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

package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.utils.SciTranslator;

/**
 * This class create a global mouse event watcher
 * means at a very high level in the JVM.
 * @author Bruno Jofret
 */
public abstract class GlobalMouseEventWatcher implements AWTEventListener {

    private boolean isControlDown;
    private boolean inCanvas = true;
    private boolean freedom = true;
    private long eventMask;
    private SciTranslator clickTranslator;
    private MouseEvent lastMouse;

    // Match through Canonical name to have both GLCanvas and GLJPanel wrapper.
    private final String ScilabOpenGLComponentCanonicalName = "org.scilab.modules.gui.bridge.canvas.SwingScilabCanvasImpl";

    /**
     * Constructor.
     *
     * @param eventMask : a Mask for mouse event watching.
     * xclick is AWTEvent.MOUSE_EVENT_MASK
     * xgetmouse is AWTEvent.MOUSE_EVENT_MASK + AWTEvent.MOUSE_MOTION_EVENT_MASK
     */
    public GlobalMouseEventWatcher(long eventMask) {
        this.eventMask = eventMask;
        //this.canvas = null;
        clickTranslator = new SciTranslator();
    }

    /**
     *
     * When there is a mouse click on a Canvas, just go through the GlobalFilter.
     * @param mouseEvent : The mouse event caught.
     *
     * @see java.awt.event.AWTEventListener#eventDispatched(java.awt.AWTEvent)
     */
    public void eventDispatched(AWTEvent mouseEvent) {
        // DEBUG
        //Debug.DEBUG(this.getClass().getSimpleName(),((MouseEvent) mouseEvent).toString());
        //if (this.axes != null) {
        //Debug.DEBUG("axes number " + this.axes.getFigureId());
        //}
        /*
         * Managing Canvas
         * PRESSED
         * CLICKED
         * DCLICKED
         * MOVED
         */

        this.lastMouse = (MouseEvent) mouseEvent;
        /*
         * Use match on package name to match GLJPanel and GLCanvas
         * GLJPanel are still used under MacOSX
         */
        if (mouseEvent.getSource().getClass().getName().startsWith(ScilabOpenGLComponentCanonicalName)) {
            this.isControlDown = lastMouse.isControlDown();
            switch (mouseEvent.getID()) {
                    /* CLICKED */
                case MouseEvent.MOUSE_CLICKED :
                    this.inCanvas = true;
                    //this.axes = (SwingScilabAxes) mouseEvent.getSource();
                    if (lastMouse.getClickCount() == 1) {
                        clickTranslator.setClickAction(SciTranslator.CLICKED);
                    } else {
                        /*
                         * Means mouseEvent.getClickCount() >= 2
                         */
                        clickTranslator.setClickAction(SciTranslator.DCLICKED);
                        synchronized (clickTranslator) {
                            // To unlock javaClick2Scilab done in launchfilter
                            clickTranslator.notify();
                        }
                    }
                    break;
                    /* PRESSED */
                case MouseEvent.MOUSE_PRESSED :
                    this.inCanvas = true;
                    //this.axes = (SwingScilabAxes) mouseEvent.getSource();
                    clickTranslator.setClickAction(SciTranslator.PRESSED);
                    if (this.freedom) {
                        Thread timer = new Thread() {
                            public void run() {
                                launchFilter();
                            }
                        };
                        timer.start();
                    }
                    break;
                    /* ENTERED */
                case MouseEvent.MOUSE_ENTERED :
                    this.inCanvas = true;
                    //this.axes = (SwingScilabAxes) mouseEvent.getSource();
                    mouseEventFilter(lastMouse, GlobalEventWatcher.getAxesUID(), SciTranslator.MOVED, this.isControlDown);
                    break;
                    /* MOVED */
                case MouseEvent.MOUSE_MOVED :
                    this.inCanvas = true;
                    mouseEventFilter(lastMouse, GlobalEventWatcher.getAxesUID(), SciTranslator.MOVED, this.isControlDown);
                    break;
                case MouseEvent.MOUSE_DRAGGED :
                    if (this.inCanvas) {
                        if (lastMouse.getID() == MouseEvent.MOUSE_PRESSED) {
                            clickTranslator.setClickAction(SciTranslator.PRESSED);
                            synchronized (clickTranslator) {
                                // To unlock javaClick2Scilab done in launchfilter
                                clickTranslator.notify();
                            }
                        } else {
                            mouseEventFilter(lastMouse, GlobalEventWatcher.getAxesUID(), SciTranslator.MOVED, this.isControlDown);
                        }
                    }
                    break;
                    /* EXITED */
                case MouseEvent.MOUSE_EXITED :
                    this.inCanvas = false;
                    break;
                default:
                    break;
            }

        }

        /*
         * Use match on package name to match GLJPanel and GLCanvas
         * GLJPanel are still used under MacOSX
         */
        if (mouseEvent.getSource().getClass().getName().startsWith(ScilabOpenGLComponentCanonicalName)) {
            switch (mouseEvent.getID()) {
                case MouseEvent.MOUSE_ENTERED :
                    this.inCanvas = true;
                    //mouseEventFilter(lastMouse, canvas, SciTranslator.MOVED, this.isControlDown);
                    break;
                case MouseEvent.MOUSE_EXITED :
                    this.inCanvas = false;
                    break;
                default:
                    break;
            }
        }

        /*
         * Manage RELEASED on a Canvas
         * Means we are still in a Canvas (ENTERED && !EXITED)
         * and the event is not comming from a Canvas itself.
         * and got a RELEASED
         */
        if (mouseEvent.getID() == MouseEvent.MOUSE_RELEASED && inCanvas
                && mouseEvent.getSource().getClass().getName().startsWith(ScilabOpenGLComponentCanonicalName)
                && (clickTranslator.getClickAction() == SciTranslator.UNMANAGED
                    || clickTranslator.getClickAction() == SciTranslator.MOVED)) {
            clickTranslator.setClickAction(SciTranslator.RELEASED);
            mouseEventFilter(lastMouse, GlobalEventWatcher.getAxesUID(), clickTranslator.getClickAction(), this.isControlDown);
        }
    }

    /**
     * Thread to manage old style scilab
     * click management.
     */
    private void launchFilter() {
        this.freedom = false;
        mouseEventFilter(lastMouse, GlobalEventWatcher.getAxesUID(), clickTranslator.javaClick2Scilab(), this.isControlDown);
        this.freedom = true;
    }

    /**
     * Method to filter the event received.
     * Depends off what kind of function is called.
     *
     * @param mouseEvent : the mouse event caught (as seen in Scilab)
     * @param axes : the axes where action occurs.
     * @param scilabMouseAction : the integer scilab code for mouse action.
     * @param isControlDown true if the CTRL key has been pressed
     */
    public abstract void mouseEventFilter(MouseEvent mouseEvent, Integer axesUID, int scilabMouseAction, boolean isControlDown);

    /**
     * Event Mask getter.
     *
     * @return eventMask
     */
    public long getEventMask() {
        return eventMask;
    }
}
