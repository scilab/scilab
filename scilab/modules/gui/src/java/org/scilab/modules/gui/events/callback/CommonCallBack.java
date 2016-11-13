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

import javax.swing.AbstractAction;

import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.events.GlobalEventFilter;
import org.scilab.modules.gui.events.GlobalEventWatcher;

/**
 * Abstract class to manage all callbacks.
 * Those that need Java code, and those that need Scilab execution.
 *
 * @author bruno
 *
 */
public abstract class CommonCallBack extends AbstractAction {

    private static final long serialVersionUID = -919820581757579168L;

    private CallBack callback;

    /**
     * @param command the callback command
     * @param callbackType the callback command type
     */
    public CommonCallBack(String command) {
        callback = new CallBack(command, CallBack.UNTYPED);
    }
    /**
     * @param command the callback command
     * @param callbackType the callback command type
     */
    public CommonCallBack(String command, int callbackType) {
        callback = new CallBack(command, callbackType);
    }

    /**
     * Callback function that will be call on some event.
     */
    public abstract void callBack();

    /**
     * Create a Callback from Scilab data
     * @param command the instruction
     * @param callbackType the type of the instruction
     * @return the Callback
     */
    public static CommonCallBack createCallback(String command, int callbackType) {
        CommonCallBack callback = null;
        switch (callbackType) {
            case CallBack.JAVA :
                callback = JavaCallBack.create(command);
                break;
            case CallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE :
                callback = JavaCallBack.createOutOfXclickAndXgetmouse(command);
                break;
            case CallBack.SCILAB_INSTRUCTION_WITHOUT_GCBO :
                callback = ScilabCallBack.create(command);
                break;
            case CallBack.SCILAB_OUT_OF_XCLICK_AND_XGETMOUSE :
                callback = ScilabCallBack.createOutOfXclickAndXgetmouse(command);
                break;
            case CallBack.SCILAB_NOT_INTERRUPTIBLE_FUNCTION:
            case CallBack.SCILAB_NOT_INTERRUPTIBLE_INSTRUCTION:
                callback = ScilabCallBack.create(command, false);
                break;
            default:
                callback = ScilabCallBack.create(command);
                break;
        }
        return callback;
    }

    /**
     * Create a Callback from Scilab data
     * @param command the instruction
     * @param callbackType the type of the instruction
     * @param objectUID the object UID
     * @return the Callback
     */
    public static CommonCallBack createCallback(String command, int callbackType, int objectUID) {
        CommonCallBack callback = null;
        switch (callbackType) {
            case CallBack.JAVA :
                callback = JavaCallBack.create(command);
                break;
            case CallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE :
                callback = JavaCallBack.createOutOfXclickAndXgetmouse(command);
                break;
            case CallBack.SCILAB_INSTRUCTION_WITHOUT_GCBO :
                callback = ScilabCallBack.create(command, false);
                break;
            case CallBack.SCILAB_OUT_OF_XCLICK_AND_XGETMOUSE :
                callback = ScilabCallBack.createOutOfXclickAndXgetmouse(command);
                break;
            case CallBack.SCILAB_NOT_INTERRUPTIBLE_FUNCTION:
            case CallBack.SCILAB_NOT_INTERRUPTIBLE_INSTRUCTION:
                callback = ScilabCallBack.create("if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                                                 + "gcbo = getcallbackobject(" + objectUID + ");"
                                                 + command
                                                 + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;", false);
                break;
            default:
                callback = ScilabCallBack.create("if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                                                 + "gcbo = getcallbackobject(" + objectUID + ");"
                                                 + command
                                                 + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;");
                break;
        }
        return callback;
    }

    /**
     * To match the standard Java Action management.
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     * @param e The event that launch the callback.
     */
    public void actionPerformed(ActionEvent e) {
        if (!GlobalEventWatcher.isCatchingCallback()) {
            callBack();
        } else {
            if (this.callback.getCommand() != null) {
                GlobalEventFilter.filterCallback(this.callback.getCommand());
            }
        }
    }

    /**
     * @return the command of the callback
     */
    public String getCommand() {
        return callback.getCommand();
    }
}
