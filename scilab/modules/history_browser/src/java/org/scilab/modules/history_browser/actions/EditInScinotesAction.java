/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.history_browser.actions;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.history_browser.CommandHistory;
import org.scilab.modules.history_browser.CommandHistoryMessages;

/**
 * Manage Clear Actions
 * @author Vincent COUVERT
 */
public final class EditInScinotesAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL = CommandHistoryMessages.EDIT_IN_SCINOTES;
    private static final char MNEMONIC = 'D';
    private static final String SCINOTES_PACKAGE = "org.scilab.modules.scinotes.SciNotes";

    /**
     * Constructor
     */
    public EditInScinotesAction() {
        super("");
    }

    /**
     * Create the associated menu
     * @return the menu
     */
    public static MenuItem createMenuItem() {
        MenuItem menuItem = ScilabMenuItem.createMenuItem();
        menuItem.setText(LABEL);
        menuItem.setMnemonic(MNEMONIC);
        menuItem.setCallback(getCallBack());

        try {
            Class.forName(SCINOTES_PACKAGE);
        } catch (ClassNotFoundException e) {
            /* SciNotes not available */
            menuItem.setEnabled(false);
        }
        return menuItem;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = EditInScinotesAction.class.getConstructor().newInstance();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        return callback;
    }

    /**
     * Action!
     * @see org.scilab.modules.gui.events.callback.CallBack#callBack()
     */
    public void callBack() {
        try {
            /* Dynamic load of the SciNotes class.
             * This is done to avoid a cyclic dependency on gui <=> SciNotes
             */
            Class scinotesClass = Class.forName(SCINOTES_PACKAGE);
            Class[] arguments = new Class[] {String.class};
            Method method = scinotesClass.getMethod("scinotesWithText", arguments);
            method.invoke(scinotesClass, new Object[] {CommandHistory.getSelectedCommands()});

        } catch (ClassNotFoundException e) {
            System.err.println("Could not find SciNotes class");
            e.printStackTrace();
        } catch (SecurityException e) {
            System.err.println("Security error: Could not access to SciNotes class");
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            System.err.println("Could not access to scinoteshWithText method from object SciNotes");
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            System.err.println("Wrong argument used with scinotesWithText method from object SciNotes");
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            System.err.println("Illegal access with scinotesWithText method from object SciNotes");
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            System.err.println("Error of invocation with scinotesWithText method from object SciNotes");
            e.printStackTrace();
        }
    }
}
