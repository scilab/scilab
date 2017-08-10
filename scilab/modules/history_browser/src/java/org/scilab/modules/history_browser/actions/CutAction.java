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

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.history_browser.CommandHistory;
import org.scilab.modules.history_browser.CommandHistoryMessages;

/**
 * Manage Cut Actions
 * @author Vincent COUVERT
 */
public final class CutAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL = CommandHistoryMessages.CUT;
    private static final String ICON = FindIconHelper.findIcon("edit-cut");
    private static final char MNEMONIC = 'U';

    private static final String KEY = "OSSCKEY X";
    private static final String CUT = "Cut";

    /**
     * Constructor
     */
    public CutAction() {
        super("");
    }

    /**
     * Register the key for the action
     */
    public static void registerKeyAction() {
        CommandHistory.getTree().getActionMap().put(CUT, new CutAction());
        CommandHistory.getTree().getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), CUT);
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
        ((SwingScilabMenuItem) menuItem.getAsSimpleMenuItem()).setIcon(new ImageIcon(ICON));
        return menuItem;
    }

    /**
     * Create the associated button
     * @return the button
     */
    public static JButton createPushButton() {
        JButton pushButton = new JButton();
        ScilabLAF.setDefaultProperties(pushButton);
        pushButton.setIcon(new ImageIcon(ICON));
        pushButton.setToolTipText(LABEL);
        pushButton.addActionListener(getCallBack());

        return pushButton;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = CutAction.class.getConstructor().newInstance();
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
    @Override
    public void callBack() {
        CommonCallBack copyAction = new CopyAction();
        copyAction.callBack();
        CommonCallBack deleteAction = new DeleteAction();
        deleteAction.callBack();
    }
}
