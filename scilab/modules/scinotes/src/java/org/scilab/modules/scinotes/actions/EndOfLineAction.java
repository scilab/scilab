/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class EndOfLineAction
 * @author Allan CORNET
 *
 */
public class EndOfLineAction extends DefaultCheckAction {

    private static final String EOL_LINUX = "\n";
    private static final String EOL_MACOS = "\r";
    private static final String EOL_WINDOWS = "\r\n";
    private static final String LINE_SEPARATOR = "line.separator";

    private static JRadioButtonMenuItem[] radioEolTypes;

    /**
     * generated serialVersionUID
     */
    private static final long serialVersionUID = 7147038540238271944L;

    /* default */
    private String eolMenuLabel = SciNotesMessages.EOL_AUT0;

    /**
     * Constructor
     * @param eolName String
     * @param editor SciNotes
     */
    public EndOfLineAction(String eolName, SciNotes editor) {
        super(eolName, editor);
        eolMenuLabel = eolName;
    }

    /**
     * createRadioButtonMenuItem
     * @param editor SciNotes
     * @return JRadioButtonMenuItem
     */
    public JRadioButtonMenuItem createRadioButtonMenuItem(SciNotes editor) {
        JRadioButtonMenuItem radio = new JRadioButtonMenuItem(eolMenuLabel);
        radio.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                doAction();
            }
        });
        return radio;
    }

    /**
     * Clean
     */
    public static void close() {
        radioEolTypes = null;
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabDocument styleDocument = ((ScilabDocument) getEditor().getTextPane().getDocument());

        // default
        String newOel = System.getProperty(LINE_SEPARATOR);

        if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_AUT0) == 0) {
            newOel = System.getProperty(LINE_SEPARATOR);
        } else if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_LINUX) == 0) {
            newOel = EOL_LINUX;
        } else if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_MACOS) == 0) {
            newOel = EOL_MACOS;
        } else if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_WINDOWS) == 0) {
            newOel = EOL_WINDOWS;
        }

        if (styleDocument.getEOL().compareTo(newOel) != 0) {
            styleDocument.setEOL(newOel);
            styleDocument.setContentModified(true);
            // changing such a property dicards undo
            styleDocument.getUndoManager().discardAllEdits();
            getEditor().updateTabTitle();
        }
    }

    /**
     * create End Of Line sub Menu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return a Menu
     */
    public static Menu createMenu(String label, SciNotes editor, KeyStroke key) {
        String eolLinux = ScilabDocument.EOLUNIX;
        String eolMacOs = ScilabDocument.EOLMAC;
        String eolWindows = ScilabDocument.EOLWIN;
        String defaultEolLabel = SciNotesMessages.EOL_LINUX;

        // selected by default O.S
        String systemEolValue = System.getProperty(LINE_SEPARATOR);

        if (systemEolValue.compareTo(eolLinux) == 0) {
            defaultEolLabel = SciNotesMessages.EOL_LINUX;
        }

        if (systemEolValue.compareTo(eolWindows) == 0) {
            defaultEolLabel = SciNotesMessages.EOL_WINDOWS;
        }

        if (systemEolValue.compareTo(eolMacOs) == 0) {
            defaultEolLabel = SciNotesMessages.EOL_MACOS;
        }

        Menu eolTypeMenu = ScilabMenu.createMenu();
        eolTypeMenu.setText(label);

        radioEolTypes = new JRadioButtonMenuItem[3];
        ButtonGroup groupEol = new ButtonGroup();

        radioEolTypes[0] =  (new EndOfLineAction(SciNotesMessages.EOL_LINUX, editor)).createRadioButtonMenuItem(editor);
        groupEol.add(radioEolTypes[0]);
        ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[0]);

        radioEolTypes[1] =  (new EndOfLineAction(SciNotesMessages.EOL_WINDOWS, editor)).createRadioButtonMenuItem(editor);
        groupEol.add(radioEolTypes[1]);
        ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[1]);

        radioEolTypes[2] =  (new EndOfLineAction(SciNotesMessages.EOL_MACOS, editor)).createRadioButtonMenuItem(editor);
        groupEol.add(radioEolTypes[2]);
        ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[2]);

        // selected menu
        for (int k = 0; k < radioEolTypes.length; k++) {
            if (radioEolTypes[k].getText().compareTo(defaultEolLabel) == 0) {
                radioEolTypes[k].setSelected(true);
            }
        }

        return eolTypeMenu;
    }


    /**
     * Update the selected item in the EOL pull down menu of the document.
     * @param scilabDocument the document for which the End Of Line menu should
     * be updated
     */
    public static void updateEolMenu(ScilabDocument scilabDocument) {
        String eolLinux = ScilabDocument.EOLUNIX;
        String eolMacOs = ScilabDocument.EOLMAC;
        String eolWindows = ScilabDocument.EOLWIN;

        String eolUsedLabel = SciNotesMessages.EOL_LINUX;
        String eolUsed = scilabDocument.getEOL();

        if (eolUsed.compareTo(eolLinux) == 0) {
            eolUsedLabel = SciNotesMessages.EOL_LINUX;
        }

        if (eolUsed.compareTo(eolMacOs) == 0) {
            eolUsedLabel = SciNotesMessages.EOL_MACOS;
        }

        if (eolUsed.compareTo(eolWindows) == 0) {
            eolUsedLabel = SciNotesMessages.EOL_WINDOWS;
        }

        for (int k = 0; k < radioEolTypes.length; k++) {
            if (radioEolTypes[k].getText().compareTo(eolUsedLabel) == 0) {
                radioEolTypes[k].setSelected(true);
            }
        }
    }
}
