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

import javax.swing.AbstractAction;

import org.scilab.modules.gui.events.GlobalEventFilter;
import org.scilab.modules.gui.events.GlobalEventWatcher;

/**
 * Abstract class to manage all callbacks.
 * Those that need Java code, and those that need Scilab execution.
 *
 * @author bruno
 *
 */
public abstract class CallBack extends AbstractAction {

    /**
     * Unmanaged command type constant
     */
    public static final int UNTYPED = -1;
    /**
     * Scilab instruction command type constant
     */
    public static final int SCILAB_INSTRUCTION = 0;
    /**
     * C or Fortran function type constant
     */
    public static final int C_FORTRAN = 1;
    /**
     * Scilab function type constant
     */
    public static final int SCILAB_FUNCTION = 2;
    /**
     * Scilab function type constant
     */
    public static final int SCILAB_OUT_OF_XCLICK_AND_XGETMOUSE = -2;
    /**
     * Java function type constant
     */
    public static final int JAVA = 3;

    /**
     * Scilab instruction without GCBO setting (old addmenu compatibility)
     */
    public static final int SCILAB_INSTRUCTION_WITHOUT_GCBO = 4;

    /**
     * The Command to Store and remember.
     */
    protected String command;

    /**
     * Constructor to be seen by specifics CallBack.
     * @param command the command associated to the ccallback
     */
    public CallBack(String command) {
	this.command = command;
    }

    /**
     * @return the command if it's a Scilab instruction.
     */
    public String getCommand() {
	return command;
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
    public static CallBack createCallback(String command, int callbackType) {
	switch (callbackType) {
	case JAVA :
	    return JavaCallBack.create(command);
	case SCILAB_INSTRUCTION_WITHOUT_GCBO :
	    return ScilabCallBack.create(command);
	case SCILAB_OUT_OF_XCLICK_AND_XGETMOUSE :
	    return ScilabCallBack.createOutOfXclickAndXgetmouse(command);
	default:
	    return ScilabCallBack.create(command);
	}
    }

    /**
     * Create a Callback from Scilab data
     * @param command the instruction
     * @param callbackType the type of the instruction
     * @param objectIndex the index of the object in the UIElementMapper
     * @return the Callback
     */
    public static CallBack createCallback(String command, int callbackType, int objectIndex) {
	switch (callbackType) {
	case JAVA :
	    return JavaCallBack.create(command);
	case SCILAB_INSTRUCTION_WITHOUT_GCBO :
	    return ScilabCallBack.create(command);
	case SCILAB_OUT_OF_XCLICK_AND_XGETMOUSE :
	    return ScilabCallBack.createOutOfXclickAndXgetmouse(command);
	default:
	    return ScilabCallBack.create("if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
		    + "gcbo = getcallbackobject(" + objectIndex + ");"
		    + command
		    + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;");
	}
    }

    /**
     * To match the standard Java Action management.
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     * @param e The event that launch the callback.
     */
    public void actionPerformed(ActionEvent e) {
	if (!GlobalEventWatcher.isActivated()) {
	    callBack();
	} else {
	    if (this.command != null) {
		GlobalEventFilter.filterCallback(this.command);
	    }
	}
    }
}
