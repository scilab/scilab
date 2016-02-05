/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.io.StringReader;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
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
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new PasteAction(tooltip, editor));
    }
}
