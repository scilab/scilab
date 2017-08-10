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
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.JTextField;

import org.scilab.modules.commons.gui.FindIconHelper;
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
 * DuplicateVariableAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class DuplicateVariableAction extends CommonCallBack {

    private static final String KEY = "OSSCKEY D";
    private static final String CREATE = "Duplicate";
    private static final int GAP = 5;

    private final SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    private DuplicateVariableAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(CREATE, new DuplicateVariableAction(editor, CREATE));
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), CREATE);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        JTable table = editor.getCurrentTable();
        String varName = askForName();
        if (!varName.isEmpty()) {
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            model.execCommand(varName + "=" + model.getVarName() + ";editvar(\"" + varName + "\")");
        }
    }

    public String askForName() {
        final JDialog dialog = new JDialog();
        final String[] ret = new String[] { "" };
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
        panel.add(label, gbc);

        final JTextField textField = new JTextField(24 + 1);
        textField.setText(UI_data.getUnnamedVariable());
        textField.selectAll();
        gbc.gridx = 1;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        panel.add(textField, gbc);

        JButton cancelButton = new JButton(UiDataMessages.CANCEL);
        JButton okButton = new JButton(UiDataMessages.OK);
        JPanel panelButton = new JPanel();
        okButton.setPreferredSize(cancelButton.getPreferredSize());
        panelButton.setLayout(new GridLayout(1, 2, GAP, GAP));
        panelButton.setBorder(BorderFactory.createEmptyBorder(GAP, 0, 0, 0));
        panelButton.add(cancelButton);
        panelButton.add(okButton);

        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.gridwidth = 1;
        gbc.weightx = 0;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.EAST;
        gbc.insets = new Insets(GAP, 0, 0, GAP);
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
            }
        });

        textField.addKeyListener(new KeyAdapter() {
            @Override
            public void keyReleased(KeyEvent e) {
                int code = e.getKeyCode();
                if (code == KeyEvent.VK_ENTER) {
                    dialog.dispose();
                    ret[0] = textField.getText();
                } else if (code == KeyEvent.VK_ESCAPE) {
                    dialog.dispose();
                }
            }
        });

        dialog.setTitle(UiDataMessages.CREATEVAR);
        dialog.pack();
        dialog.setResizable(false);
        dialog.setLocationRelativeTo(editor);
        dialog.setVisible(true);

        return ret[0];
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabVariableEditor editor, String title) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new DuplicateVariableAction(editor, title));
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
