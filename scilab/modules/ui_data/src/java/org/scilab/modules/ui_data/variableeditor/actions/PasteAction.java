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

package org.scilab.modules.ui_data.variableeditor.actions;

import java.awt.Cursor;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.text.NumberFormat;
import java.text.ParsePosition;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JTable;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * PasteAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class PasteAction extends CommonCallBack {

    private static final String KEY = "OSSCKEY V";
    private static final String PASTE = "Paste";

    private final SwingScilabVariableEditor editor;

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
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), PASTE);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int col = table.getSelectedColumn();
        int row = table.getSelectedRow();

        if (col == -1) {
            col = 0;
        }

        if (row == -1) {
            row = 0;
        }

        table.setColumnSelectionInterval(col, col);
        table.setRowSelectionInterval(row, row);
        String str = "";
        editor.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        try {
            str = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this).getTransferData(DataFlavor.stringFlavor);
        } catch (UnsupportedFlavorException ex1) {
            System.err.println(ex1);
        } catch (IOException ex2) {
            System.err.println(ex2);
        }

        StringTokenizer rElems = new StringTokenizer(str, "\n");
        int countRows = rElems.countTokens();
        Vector vr = new Vector(countRows);
        NumberFormat format = NumberFormat.getInstance();
        ParsePosition position = new ParsePosition(0);
        format.setParseIntegerOnly(false);
        for (int i = 0; i < countRows; i++) {
            StringTokenizer cElems = new StringTokenizer(rElems.nextToken(), "\t");
            int countCols = cElems.countTokens();
            Vector vc = new Vector(countCols);
            for (int j = 0; j < countCols; j++) {
                String ss = cElems.nextToken();
                Number x = format.parse(ss, position);
                if (position.getIndex() == ss.length()) {
                    vc.addElement(x.toString());
                } else {
                    vc.addElement(ss);
                }
                position.setIndex(0);
            }
            vr.addElement(vc);
        }

        SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
        int oldRows = model.getScilabMatrixRowCount();
        int oldCols = model.getScilabMatrixColCount();

        if (vr.size() == 1 && ((Vector) vr.get(0)).size() == 1 && row < oldRows && col < oldCols) {
            model.setValueAtAndUpdate(true, true, ((Vector) vr.get(0)).get(0), row, col);
        } else {
            Vector oldVector = model.cloneDatas();
            model.setValues(vr, row, col);
            model.updateFullMatrix(oldVector, oldRows, oldCols);
        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(SwingScilabVariableEditor editor, String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new PasteAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("edit-paste"));
        button.setIcon(imageIcon);

        return button;
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabVariableEditor editor, String title) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new PasteAction(editor, title));
        menu.setText(title);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(ScilabKeyStroke.getKeyStroke(KEY));

        return menu;
    }

    /**
     * Create a menu item as a SwingScilabMenuItem
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static SwingScilabMenuItem createJMenuItem(SwingScilabVariableEditor editor, String title) {
        return (SwingScilabMenuItem) createMenuItem(editor, title).getAsSimpleMenuItem();
    }
}
