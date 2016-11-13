/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.variableeditor.actions;

import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;

import javax.swing.JTable;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * CopySelectionAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
class CopySelectionAction extends CommonCallBack {

    protected SwingScilabVariableEditor editor;
    private String begin;
    private String end;
    private String colSep;
    private String rowSep;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    protected CopySelectionAction(SwingScilabVariableEditor editor, String name, String begin, String end, String colSep, String rowSep) {
        super(name);
        this.editor = editor;
        this.begin = begin;
        this.end = end;
        this.colSep = colSep;
        this.rowSep = rowSep;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            StringBuffer buf = new StringBuffer(begin);
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            for (int i = 0; i < rows.length; i++) {
                for (int j = 0; j < cols.length; j++) {
                    String val = model.getScilabValueAt(rows[i], cols[j], false);
                    if (val != null) {
                        buf.append(val);
                    }
                    if (j < cols.length - 1) {
                        buf.append(colSep);
                    }
                }
                if (i < rows.length - 1) {
                    buf.append(rowSep);
                }
            }
            buf.append(end);

            StringSelection sel  = new StringSelection(buf.toString());
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
        }
    }
}
