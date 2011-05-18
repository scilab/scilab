/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 * Copyright (C) 2011 - Calixte DENIZET
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
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorKit;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * LineBeautifierAction Class
 * @author Bernard HUGUENEY
 * @author Calixte DENIZET
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
        ScilabEditorPane sep = (ScilabEditorPane) ev.getSource();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        int pos = sep.getCaretPosition();
        Element root = doc.getDefaultRootElement();
        int end = root.getElement(root.getElementIndex(pos)).getEndOffset() - 1;
        String str = "";
        try {
            str = doc.getText(pos, end - pos);
        } catch (BadLocationException e) {
            System.err.println(e);
        }
        if (str.matches("[ \\t]+(then|do)")) {
            sep.setCaretPosition(end);
        }

        super.actionPerformed(ev);
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
