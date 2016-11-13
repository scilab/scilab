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

import java.awt.Dialog.ModalityType;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.SpinnerNumberModel;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.ui_data.UI_data;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class CreateNewVariableAction extends CommonCallBack {

    private static final String KEY = "OSSCKEY N";
    private static final String CREATENEW = "Create new";
    private static final int GAP = 5;

    private final SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    private CreateNewVariableAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(CREATENEW, new CreateNewVariableAction(editor, CREATENEW));
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), CREATENEW);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        JTable table = editor.getCurrentTable();
        Object[] values = askForNewMatrix();
        if (!((String) values[0]).isEmpty()) {
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            String defaultValue = SwingEditvarTableModel.getDataAsScilabString(((String) values[3]));
            if (defaultValue.isEmpty()) {
                defaultValue = "0";
            }

            StringBuilder command = new StringBuilder();
            command.append("L$8625083632641564277=warning(\"query\");warning(\"off\");");
            command.append("if execstr(\"");
            command.append((String) values[0]);
            command.append("=");
            command.append("repmat(");
            command.append(defaultValue);
            command.append(",");
            command.append(((Integer) values[1]).toString());
            command.append(",");
            command.append(((Integer) values[2]).toString());
            command.append(");editvar(\"\"");
            command.append((String) values[0]);
            command.append("\"\")\",\"errcatch\") <> 0 then messagebox(\"Could not create variable: \" + lasterror() + \"\"");
            command.append(",\"Variable editor\",\"error\",\"modal\");");
            command.append("end;");
            command.append("warning(L$8625083632641564277);clear(\"L$8625083632641564277\");");
            command.append("updatebrowsevar()");

            model.execCommand(command.toString());
        }
    }

    public Object[] askForNewMatrix() {
        final JDialog dialog = new JDialog();
        final Object[] ret = new Object[] { "", new Integer(0), new Integer(0), "" };
        dialog.setModalityType(ModalityType.APPLICATION_MODAL);
        dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        dialog.setIconImage(new ImageIcon(FindIconHelper.findIcon("rrze_table", "256x256")).getImage());

        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        JLabel label = new JLabel(UiDataMessages.VARNAME);
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 1;
        gbc.gridheight = 1;
        gbc.insets = new Insets(GAP, GAP, 0, GAP);
        gbc.anchor = GridBagConstraints.WEST;
        panel.add(label, gbc);

        final JTextField textField = new JTextField(24 + 1);
        textField.setText(UI_data.getUnnamedVariable());
        textField.selectAll();
        gbc.gridx = 1;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(GAP, 0, 0, GAP);
        panel.add(textField, gbc);

        JLabel labelRow = new JLabel(UiDataMessages.NUMROWS);
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.gridwidth = 1;
        gbc.gridheight = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.insets = new Insets(GAP, GAP, 0, GAP);
        panel.add(labelRow, gbc);

        final JSpinner spinRow = new JSpinner();
        ((SpinnerNumberModel) spinRow.getModel()).setMinimum(new Comparable<Integer>() {
            @Override
            public int compareTo(Integer o) {
                return -o.intValue();
            }
        });
        spinRow.setValue(0);
        ((JSpinner.DefaultEditor) spinRow.getEditor()).getTextField().setColumns(6);
        gbc.gridx = 1;
        gbc.insets = new Insets(GAP, 0, 0, GAP);
        panel.add(spinRow, gbc);

        JLabel labelCol = new JLabel(UiDataMessages.NUMCOLS);
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.insets = new Insets(GAP, GAP, 0, GAP);
        panel.add(labelCol, gbc);

        final JSpinner spinCol = new JSpinner();
        ((SpinnerNumberModel) spinCol.getModel()).setMinimum(new Comparable<Integer>() {
            @Override
            public int compareTo(Integer o) {
                return -o.intValue();
            }
        });
        spinCol.setValue(0);
        ((JSpinner.DefaultEditor) spinCol.getEditor()).getTextField().setColumns(6);
        gbc.gridx = 1;
        gbc.insets = new Insets(GAP, 0, 0, GAP);
        panel.add(spinCol, gbc);

        JLabel labelFill = new JLabel(UiDataMessages.FILLWITH);
        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.insets = new Insets(GAP, GAP, 0, GAP);
        panel.add(labelFill, gbc);

        final JTextField fillField = new JTextField(15);
        gbc.gridx = 1;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(GAP, 0, 0, GAP);
        panel.add(fillField, gbc);

        JButton cancelButton = new JButton(UiDataMessages.CANCEL);
        JButton okButton = new JButton(UiDataMessages.OK);
        JPanel panelButton = new JPanel();
        okButton.setPreferredSize(cancelButton.getPreferredSize());
        panelButton.setLayout(new GridLayout(1, 2, GAP, GAP));
        panelButton.setBorder(BorderFactory.createEmptyBorder(GAP, 0, 0, 0));
        panelButton.add(cancelButton);
        panelButton.add(okButton);

        gbc.gridx = 1;
        gbc.gridy = 4;
        gbc.gridwidth = 1;
        gbc.weightx = 0;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.EAST;
        panel.add(panelButton, gbc);

        dialog.setContentPane(panel);

        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dialog.dispose();
            }
        });

        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dialog.dispose();
                ret[0] = textField.getText();
                ret[1] = spinRow.getValue();
                ret[2] = spinCol.getValue();
                ret[3] = fillField.getText();
            }
        });

        dialog.setTitle(UiDataMessages.CREATENEWVAR);
        dialog.pack();
        dialog.setResizable(false);
        dialog.setLocationRelativeTo(editor);
        dialog.setVisible(true);

        return ret;
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
        button.addActionListener(new CreateNewVariableAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("variable-new"));
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
        menu.setCallback(new CreateNewVariableAction(editor, title));
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
