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
import java.awt.datatransfer.StringSelection;

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
 * RefreshAction class
 * @author Calixte DENIZET
 */
public final class CutAction extends CallBack {

    private static final String KEY = "ctrl X";
    private static final String CUT = "Cut";

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public CutAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(CUT, new CutAction(editor, CUT));
        table.getInputMap().put(KeyStroke.getKeyStroke(KEY), CUT);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
            table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);
            StringBuffer buf = new StringBuffer();
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            for (int i = rows[0]; i <= rows[rows.length - 1]; i++) {
                for (int j = cols[0]; j <= cols[cols.length - 1]; j++) {
                    String exp = model.getCellEditor().getExpression(i, j);
                    if (exp != null) {
                        buf.append("=" + exp);
                    } else {
                        String val = model.getScilabValueAt(i, j, false);
                        if (val != null) {
                            buf.append(val);
                        }
                    }
                    model.emptyValueAt(i, j);
                    if (j < cols[cols.length - 1]) {
                        buf.append("\t");
                    }
                }
                buf.append("\n");
            }
            for (int i = rows[rows.length - 1]; i >= rows[0]; i--) {
                model.removeRow(i, cols[0], cols[cols.length - 1]);
            }
            for (int j = cols[cols.length - 1]; j >= cols[0]; j--) {
                model.removeCol(table, j, rows[0], rows[rows.length - 1]);
            }
            model.updateMatrix();
            StringSelection sel  = new StringSelection(buf.toString());
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(SwingScilabVariableEditor editor, String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new CutAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/edit-cut.png");
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }
}
