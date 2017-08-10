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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.history_browser.CommandHistory;
import org.scilab.modules.history_browser.CommandHistoryMessages;

/**
 * Manage Evaluate Actions
 * @author Vincent COUVERT
 */
public final class EvaluateAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL = CommandHistoryMessages.EVALUATE_SELECTION;
    private static final char MNEMONIC = 'E';

    private static final String KEY = "ENTER";
    private static final String EVALUATE = "Evaluate";

    /**
     * Constructor
     */
    public EvaluateAction() {
        super("");
    }

    /**
     * Register the key for the action
     */
    public static void registerKeyAction() {
        CommandHistory.getTree().getActionMap().put(EVALUATE, new EvaluateAction());
        CommandHistory.getTree().getInputMap().put(KeyStroke.getKeyStroke(KEY), EVALUATE);
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
        return menuItem;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = EvaluateAction.class.getConstructor().newInstance();
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
        String commands = CommandHistory.getSelectedCommands();
        if (commands == null) {
            return;
        }
        ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(commands, true /* display */, true /* store in history */);
    }
}
