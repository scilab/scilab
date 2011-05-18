/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.actions;

import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.util.StringTokenizer;

import javax.swing.KeyStroke;
import javax.swing.ImageIcon;
import javax.swing.JTable;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * PasteAction class
 * @author Calixte DENIZET
 */
public final class PasteAction extends CallBack {

    private static final String KEY = "ctrl V";
    private static final String PASTE = "Paste";

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public PasteAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(PASTE, new PasteAction(editor, PASTE));
        table.getInputMap().put(KeyStroke.getKeyStroke(KEY), PASTE);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int col = table.getSelectedColumn();
        int row = table.getSelectedRow();
        table.setColumnSelectionInterval(col, col);
        table.setRowSelectionInterval(row, row);
        String str = "";
        try {
            str = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this).getTransferData(DataFlavor.stringFlavor);
        } catch (UnsupportedFlavorException ex1) {
            System.err.println(ex1);
        } catch (IOException ex2) {
            System.err.println(ex2);
        }
        StringTokenizer rElems = new StringTokenizer(str, "\n");
        SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
        for (int i = 0; rElems.hasMoreTokens(); i++) {
            StringTokenizer cElems = new StringTokenizer(rElems.nextToken(), "\t");
            for (int j = 0; cElems.hasMoreTokens(); j++) {
                String value = (String) cElems.nextToken();
                model.setValueAtAndUpdate(false, value, row + i, col + j);
            }
        }
        model.updateMatrix();
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(SwingScilabVariableEditor editor, String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new PasteAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/edit-paste.png");
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }
}
