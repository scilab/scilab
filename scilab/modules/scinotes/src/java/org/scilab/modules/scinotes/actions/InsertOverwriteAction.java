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

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * Handle the INSERT key
 * @author Calixte DENIZET
 *
 */
public final class InsertOverwriteAction extends AbstractAction {

    private static final long serialVersionUID = 8047960139101685363L;
    /**
     * The key associated with this action
     */
    private static String key = "INSERT";

    /**
     * Constructor
     */
    public InsertOverwriteAction() { }

    /**
     * actionPerformed
     * @param ev ActionEvent
     */
    public void actionPerformed(ActionEvent ev) {
        ScilabEditorPane sep = (ScilabEditorPane) ev.getSource();
        sep.setOverwriteMode(!sep.getOverwriteMode());
        if (sep.getOverwriteMode()) {
            sep.setInfoBarText("Overwrite Mode");
        } else {
            sep.setInfoBarText("");
        }
    }

    /**
     * putInInputMap
     * @param textPane JComponent
     */
    public static void putInInputMap(JComponent textPane) {
        textPane.getInputMap().put(KeyStroke.getKeyStroke(key), new InsertOverwriteAction());
    }
}
