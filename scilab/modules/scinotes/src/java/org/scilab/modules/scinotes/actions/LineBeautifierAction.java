/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionEvent;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.scinotes.ScilabEditorKit;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * LineBeautifierAction Class
 * @author Bernard HUGUENEY
 *
 */
public final class LineBeautifierAction extends ScilabEditorKit.InsertBreakAction {

    /**
     * The key associated with tthis action
     */
    private static String key = "ENTER";

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -8313095922543576108L;

    /**
     * Constructor
     */
    public LineBeautifierAction() { }

    /**
     * actionPerformed
     * @param ev ActionEvent
     */
    public void actionPerformed(ActionEvent ev) {
        super.actionPerformed(ev);
        ScilabEditorPane sep = (ScilabEditorPane) ev.getSource();
        sep.getIndentManager().indentDoc(sep.getCaretPosition() - 1);
    }

    /**
     * putInInputMap
     * @param textPane JComponent
     */
    public static void putInInputMap(JComponent textPane) {
	textPane.getInputMap().put(KeyStroke.getKeyStroke(key), new LineBeautifierAction());
    }
}
