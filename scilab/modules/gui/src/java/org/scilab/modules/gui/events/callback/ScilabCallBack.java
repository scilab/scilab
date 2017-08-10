/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;

/**
 * ScilabCallback abstract class to easily manage callbacks
 * that throws commands to Scilab.
 *
 * @author Bruno JOFRET
 *
 */
public abstract class ScilabCallBack extends CommonCallBack {

    private static final long serialVersionUID = -4923246233703990342L;

    private boolean isInterruptible = true;

    /**
     * Constructor
     * @param command : the command to execute.
     */
    private ScilabCallBack(String command) {
        super(command, CallBack.UNTYPED);
    }

    private ScilabCallBack(String command, boolean isInterruptible) {
        super(command, CallBack.UNTYPED);
        this.isInterruptible = isInterruptible;
    }
    /**
     * Callback Factory to easily create a callback
     * just like in scilab.
     * @param command : the command to execute.
     * @return a usable Scilab callback
     */
    public static ScilabCallBack create(String command) {
        return create(command, true);
    }

    /**
     * Callback Factory to easily create a callback
     * just like in scilab.
     * @param command : the command to execute.
     * @return a usable Scilab callback
     */
    public static ScilabCallBack create(String command, boolean isInterruptible) {
        return (new ScilabCallBack(command, isInterruptible) {

            private static final long serialVersionUID = -7286803341046313407L;

            public void callBack() {
                Thread launchMe = new Thread() {
                    public void run() {
                        if (isInterruptible()) {
                            InterpreterManagement.putCommandInScilabQueue(getCommand());
                        } else {
                            InterpreterManagement.requestScilabExec(getCommand());
                        }
                    }
                };
                launchMe.start();
            }
        });
    }

    boolean isInterruptible() {
        return isInterruptible;
    }

    /**
     * Callback Factory to easily create a callback
     * just like in scilab.
     * WARNING : this callback will be ignored by xclick & xgetmouse
     * @param command : the command to execute.
     * @return a usable Scilab callback
     */
    public static ScilabCallBack createOutOfXclickAndXgetmouse(String command) {
        return (new ScilabCallBack(command) {

            private static final long serialVersionUID = -7286803341046313407L;

            public void callBack() {
                Thread launchMe = new Thread() {
                    public void run() {
                        InterpreterManagement.putCommandInScilabQueue(getCommand());
                    }
                };
                launchMe.start();
            }

            /**
             * To match the standard Java Action management.
             * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
             * @param e The event that launch the callback.
             */
            public void actionPerformed(ActionEvent e) {
                callBack();
            }
        });
    }

}
