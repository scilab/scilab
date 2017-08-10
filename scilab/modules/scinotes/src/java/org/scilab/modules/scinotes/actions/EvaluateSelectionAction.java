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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.StringTokenizer;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;

import org.scilab.modules.history_manager.HistoryManagement;

/**
 * EvaluateSelectionAction class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class EvaluateSelectionAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 320938663765236236L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public EvaluateSelectionAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        /* Will do the job as if it was copy / paste in scilab Console */
        ScilabEditorPane sep = getEditor().getTextPane();
        String selection = sep.getCodeToExecute();
        if (selection.compareTo("") != 0) {
            StringTokenizer tokens = new StringTokenizer(selection, "\n");
            String[] lines = new String[tokens.countTokens()];
            int i = 0;
            while (tokens.hasMoreTokens()) {
                lines[i++] = tokens.nextToken();
            }
            HistoryManagement.appendLinesToScilabHistory(lines, lines.length);
            ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true, false);
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor scinotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer token = new StringTokenizer(label, ";\uff1b");
        final String label1 = token.nextToken();
        final String label2 = token.nextToken();
        final MenuItem menuitem = createMenu(label1, null, new EvaluateSelectionAction(label1, editor), key);
        if (!ScilabConsole.isExistingConsole()) { // Only available in STD mode
            ((JMenuItem) menuitem.getAsSimpleMenuItem()).setEnabled(false);
        }
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    String select = editor.getTextPane().getSelectedText();
                    if (select == null) {
                        menuitem.setText(label2);
                    } else {
                        menuitem.setText(label1);
                    }
                }
            }
        });

        return menuitem;
    }
}
