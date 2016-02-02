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
import java.awt.event.KeyEvent;

import org.scilab.modules.gui.utils.Debug;

/**
 * Global key event watcher.
 * @author Bruno Jofret
 */
public abstract class GlobalKeyEventWatcher implements AWTEventListener {

    /**
     *  Constructor.
     */
    public GlobalKeyEventWatcher() { }

    /**
     * Global key event watcher.
     *
     * @param event : The Key event caught.
     * @see java.awt.event.AWTEventListener#eventDispatched(java.awt.AWTEvent)
     */
    public void eventDispatched(AWTEvent event) {
        // DEBUG
        Debug.DEBUG(this.getClass().getSimpleName(), event.toString());
        keyEventFilter((KeyEvent) event);
    }

    /**
     * Method to filter the event received.
     * Depends off what kind of function is called.
     *
     * @param keyEvent : the key event caught
     */
    public abstract void keyEventFilter(KeyEvent keyEvent);
}
