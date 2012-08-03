/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

    /**
     * Constructor
     * @param command : the command to execute.
     */
    private ScilabCallBack(String command) {
        super(command, CallBack.UNTYPED);
    }

    /**
     * Callback Factory to easily create a callback
     * just like in scilab.
     * @param command : the command to execute.
     * @return a usable Scilab callback
     */
    public static ScilabCallBack create(String command) {
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
        });
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
