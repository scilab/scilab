/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variablebrowser.actions;

import javax.swing.ImageIcon;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
public final class RefreshAction extends CallBack {

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public RefreshAction(String name) {
        super(name);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(null, "browsevar");
        } catch (InterpreterException e) {
            System.err.println(e);
        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new RefreshAction(title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/actions/view-refresh.png");
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }
}
