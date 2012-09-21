/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.actions;

import javax.swing.ImageIcon;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;

/**
 * Launch Scilab help class
 * @author Vincent COUVERT
 */
@SuppressWarnings(value = { "serial" })
public final class HelpAction extends CommonCallBack {

    private static ImageIcon icon = new ImageIcon(ScilabSwingUtilities.findIcon("help-browser"));

    /**
     * Constructor
     * @param name the name of the action
     */
    public HelpAction(String name) {
        super(name);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(null, "help");
        } catch (InterpreterException e) {
            System.err.println(e);
        }
    }

    /**
     * Create a button for a tool bar
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new HelpAction(title));
        button.setToolTipText(title);
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(icon);

        return button;
    }

    /**
     * Create the menu for the menubar
     * @param label the menu label
     * @return the menu
     */
    public static MenuItem createMenuItem(String label) {
        MenuItem menuItem = ScilabMenuItem.createMenuItem();
        menuItem.setText(label);
        SwingScilabMenuItem swingItem = (SwingScilabMenuItem) menuItem.getAsSimpleMenuItem();
        swingItem.setCallback(new HelpAction(label));
        swingItem.setIcon(icon);
        return menuItem;
    }
}
