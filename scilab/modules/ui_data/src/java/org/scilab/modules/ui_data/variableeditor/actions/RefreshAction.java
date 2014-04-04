/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.actions;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class RefreshAction extends CommonCallBack {

    private final SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public RefreshAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        ((SwingEditvarTableModel) editor.getCurrentTable().getModel()).refreshMatrix();
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(SwingScilabVariableEditor editor, String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new RefreshAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("view-refresh"));
        button.setIcon(imageIcon);

        return button;
    }
}
