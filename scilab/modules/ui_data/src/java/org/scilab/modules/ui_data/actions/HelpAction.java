/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
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

package org.scilab.modules.ui_data.actions;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;

/**
 * Launch Scilab help class
 * @author Vincent COUVERT
 */
@SuppressWarnings(value = { "serial" })
public final class HelpAction extends CommonCallBack {

    private static ImageIcon icon = new ImageIcon(FindIconHelper.findIcon("help-browser"));

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
    public static JButton createButton(String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new HelpAction(title));
        button.setToolTipText(title);
        button.setIcon(icon);

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
