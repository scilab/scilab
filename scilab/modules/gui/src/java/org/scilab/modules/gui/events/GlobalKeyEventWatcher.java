/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno Jofret
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
