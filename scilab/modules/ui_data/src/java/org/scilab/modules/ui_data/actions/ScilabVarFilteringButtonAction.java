/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.ui_data.actions;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;

@SuppressWarnings(value = { "serial" })
public class ScilabVarFilteringButtonAction extends CommonCallBack {
    /** Name of the action */
    public static final String NAME = UiDataMessages.SCILABVAR;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public ScilabVarFilteringButtonAction(String name) {
        super(name);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        ScilabVariableBrowser.getVariableBrowser().updateRowFiltering();
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new ScilabVarFilteringButtonAction(title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("edit-clear"));
        button.setIcon(imageIcon);

        return button;
    }
}

