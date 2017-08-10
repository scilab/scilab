/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
