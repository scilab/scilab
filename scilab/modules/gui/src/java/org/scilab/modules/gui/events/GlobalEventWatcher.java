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
import java.awt.Toolkit;

/**
 * This implements a Global event watcher,
 * means a very high level one : quite the same level as the JVM
 * @author Bruno Jofret
 */
public final class GlobalEventWatcher {

    private static GlobalEventWatcher me;
    private static GlobalKeyEventWatcher keyWatcher;
    private static GlobalMouseEventWatcher mouseWatcher;
    private static boolean activated;
    private static Integer axesUID = null;
    private static boolean catchingCallback = false;

    /**
     * Private Constructor : SINGLETON.
     */
    private GlobalEventWatcher() {	}

    /**
     * Singleton enable.
     *
     * @return the global watcher instance
     */
    public static GlobalEventWatcher getInstance() {
        if (me == null) {
            me = new GlobalEventWatcher();
        }
        return me;
    }


    /**
     * Singleton enable keyWatcher.
     *
     * @param keyWatcher : the Key Listener.
     */
    public static void enable(GlobalKeyEventWatcher keyWatcher) {
        GlobalEventWatcher.keyWatcher = keyWatcher;
        Toolkit.getDefaultToolkit().addAWTEventListener(keyWatcher,
                AWTEvent.KEY_EVENT_MASK);
        activated = true;
    }

    /**
     * Singleton enable keyWatcher.
     *
     * @param mouseWatcher : the Mouse Listener.
     */
    public static void enable(GlobalMouseEventWatcher mouseWatcher) {
        GlobalEventWatcher.mouseWatcher = mouseWatcher;
        Toolkit.getDefaultToolkit().addAWTEventListener(mouseWatcher,
                mouseWatcher.getEventMask());
        activated = true;
    }


    /**
     * Disable the global watcher
     */
    public static void disable() {
        Toolkit.getDefaultToolkit().removeAWTEventListener(GlobalEventWatcher.keyWatcher);
        Toolkit.getDefaultToolkit().removeAWTEventListener(GlobalEventWatcher.mouseWatcher);
        activated = false;
        catchingCallback = false;
    }

    /**
     * Is the GlobalWatcher active or not.
     * @return activation status.
     */
    public static boolean isActivated() {
        return activated;
    }

    public static void setAxesUID(Integer axesUID) {
        GlobalEventWatcher.axesUID = axesUID;
    }

    public static Integer getAxesUID() {
        return axesUID ;
    }

    public static void enableCatchingCallback() {
        catchingCallback = true;
    }

    public static void disableCatchingCallback() {
        catchingCallback = false;
    }

    public static boolean isCatchingCallback() {
        return catchingCallback;
    }

}
