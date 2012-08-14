/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.io.StringReader;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesCaret;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorKit;

/**
 * Class for paste action
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class PasteAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor to associate
     */
    public PasteAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Action !
     */
    public void doAction() {
        ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
        try {
            String str = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this).getTransferData(DataFlavor.stringFlavor);
            if (str != null && !str.isEmpty()) {
                str = ((ScilabEditorKit) getEditor().getEditorKit()).read(new StringReader(str)).content;
                doc.mergeEditsBegin();
                getEditor().getTextPane().replaceSelection(str);
                doc.mergeEditsEnd();
                if (!((SciNotesCaret) getEditor().getTextPane().getCaret()).isEmptySelection()) {
                    ((SciNotesCaret) getEditor().getTextPane().getCaret()).removeHighlights();
                }
            }
        } catch (UnsupportedFlavorException e) {
        } catch (IOException e) {
            System.err.println(e);
        }
    }

    /**
     * Create the MenuItem for print action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new PasteAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new PasteAction(tooltip, editor));
    }
}
