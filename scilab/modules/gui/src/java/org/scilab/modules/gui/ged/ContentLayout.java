/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.lang.reflect.Method;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.BorderFactory;
import javax.swing.Box.Filler;
import javax.swing.DefaultComboBoxModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTable;
import javax.swing.JToggleButton;
import javax.swing.ListSelectionModel;
import javax.swing.table.DefaultTableModel;
import org.scilab.modules.graphic_objects.PolylineData;

/**
* Preconfigured components in Grid Bag Constraints
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class ContentLayout extends JPanel {
    private GridBagLayout layout;
    protected GridBagConstraints gbc;
    private static String imagepath = System.getenv("SCI") + "/modules/gui/images/icons/";

    public ContentLayout() {
        layout = new GridBagLayout();
        setLayout(layout);
        gbc = new GridBagConstraints();
        gbc.weightx = 1;
        gbc.weighty = 1;
        gbc.insets = new Insets(0, 0, 0, 0);
    }

    public void addSHbutton(JPanel panel, JToggleButton showHideButton, int column, int row) {
        showHideButton.setBorder(null);
        showHideButton.setBorderPainted(false);
        showHideButton.setContentAreaFilled(false);
        showHideButton.setMaximumSize(new Dimension(16, 16));
        showHideButton.setMinimumSize(new Dimension(16, 16));
        showHideButton.setPreferredSize(new Dimension(16, 16));
        showHideButton.setRolloverEnabled(false);
        showHideButton.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        showHideButton.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));

        gbc = new GridBagConstraints();
        gbc.gridx = column;
        gbc.gridy = row;
        panel.add(showHideButton, gbc);
    }

    public void addSectionTitle(JPanel panel, JLabel label, String text, int row) {
        label.setText(text);

        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = 1;
        gbc.gridy = row;
        gbc.insets = new Insets(0, 3, 0, 0);
        panel.add(label, gbc);
    }

    public void addSeparator(JPanel panel, JSeparator separator, int row) {
        separator.setPreferredSize(new Dimension(5, 10));

        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = 1;
        gbc.gridy = row;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 0, 1, 0);
        panel.add(separator, gbc);
    }

    public void addColorField(JPanel parentPanel, JPanel fieldPanel, JButton colorButton, JLabel fieldColor, int column, int row) {
        fieldPanel.setBackground(new Color(255, 255, 255));
        fieldPanel.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        fieldPanel.setPreferredSize(new Dimension(5, 20));
        fieldPanel.setLayout(new GridBagLayout());

        colorButton.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        colorButton.setContentAreaFilled(false);
        colorButton.setMaximumSize(new Dimension(16, 16));
        colorButton.setMinimumSize(new Dimension(16, 16));
        colorButton.setPreferredSize(new Dimension(16, 16));
        colorButton.setIcon(new ImageIcon(SwingInspector.icon_color_fill));
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridheight = 2;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.insets = new Insets(2, 37, 2, 0);
        fieldPanel.add(colorButton, gbc);

        fieldColor.setText(" ");
        fieldColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        fieldColor.setOpaque(true);
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.ipadx = 22;
        gbc.ipady = -8;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.insets = new Insets(4, 11, 0, 68);
        fieldPanel.add(fieldColor, gbc);

        gbc = new GridBagConstraints();
        gbc.gridx = column;
        gbc.gridy = row;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.ipadx = 70;
        gbc.insets = new Insets(0, 4, 5, 0);
        parentPanel.add(fieldPanel, gbc);
    }

    public void addColorDialog(final JDialog colorDialog,
                               final JColorChooser colorChooser,
                               JButton ok,
                               final JLabel colorField,
                               final String parentFigure,
                               final String packClass,
                               final String method,
                               final Object methobj) {

        colorDialog.setTitle(MessagesGED.choose_color);
        colorDialog.setMinimumSize(new Dimension(567, 340));
        colorDialog.setModal(true);
        colorDialog.setResizable(true);
        colorDialog.getContentPane().setLayout(new GridBagLayout());
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        colorDialog.getContentPane().add(colorChooser, gbc);

        ok.setText(" OK ");
        ok.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                try {
                    okActionPerformed();
                } catch (ClassNotFoundException ex) {
                    Logger.getLogger(ContentLayout.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            /**
            * Implement the action on the OK button to save the color chosen by the user.
            */
            private void okActionPerformed() throws ClassNotFoundException {
                Color choice = colorChooser.getColor();
                double red = choice.getRed();
                double green = choice.getGreen();
                double blue = choice.getBlue();
                Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);

                try {
                    Class partypes[] = new Class[1];
                    partypes[0] = Integer.TYPE;

                    Class cls = Class.forName("org.scilab.modules.gui.ged.graphic_objects." + packClass);
                    Method meth = cls.getMethod(method, partypes);

                    Object arglist[] = new Object[1];
                    arglist[0] = new Integer(scilabColor);

                    meth.invoke(methobj, arglist);
                } catch (Throwable e) {
                    System.err.println(e);
                }
                colorField.setBackground(choice);
                colorDialog.dispose();
            }
        });
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.insets = new Insets(0, 522, 2, 0);
        colorDialog.getContentPane().add(ok, gbc);
    }

    public void addDataField(JPanel parentPanel,
                             JPanel fieldPanel,
                             JButton dataButton,
                             JLabel dataLabel,
                             int column,
                             int row,
                             String objectID) {
        fieldPanel.setBackground(new Color(255, 255, 255));
        fieldPanel.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        fieldPanel.setPreferredSize(new Dimension(5, 20));
        fieldPanel.setLayout(new GridBagLayout());

        dataButton.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        dataButton.setContentAreaFilled(false);
        dataButton.setMinimumSize(new Dimension(16, 16));
        dataButton.setPreferredSize(new Dimension(16, 16));
        dataButton.setIcon(new ImageIcon(SwingInspector.icon_table));

        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridheight = 2;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.weighty = 0.01;
        gbc.insets = new Insets(2, 10, 2, 0);
        fieldPanel.add(dataButton, gbc);

        dataLabel.setBackground(new Color(239, 77, 31));
        dataLabel.setBorder(null);
        dataLabel.setMaximumSize(new Dimension(100, 20));
        dataLabel.setMinimumSize(new Dimension(50, 20));
        dataLabel.setPreferredSize(new Dimension(50, 20));
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.ipadx = 22;
        gbc.anchor = GridBagConstraints.PAGE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 11, 0, 0);
        fieldPanel.add(dataLabel, gbc);

        gbc = new GridBagConstraints();
        gbc.gridx = column;
        gbc.gridy = row;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.ipadx = 70;
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 4, 5, 0);
        parentPanel.add(fieldPanel, gbc);
    }

    public void addDataDialog(final JDialog dataDialog,
                              final JScrollPane scroll,
                              final JTable table,
                                    JButton append,
                                    JButton delete,
                                    JButton refresh,
                                    JButton ok,
                              final String objectID) {
        dataDialog.setTitle(MessagesGED.data_editor);
        dataDialog.setMinimumSize(new Dimension(150, 340));
        dataDialog.setModal(true);
        dataDialog.setResizable(true);
        dataDialog.getContentPane().setLayout(new GridBagLayout());

        table.setModel(new DefaultTableModel(
                           new Object[][] {null, null}, new String [] {"X", "Y"}
        ) {
            Class[] types = new Class [] {
                Double.class, Double.class, Double.class
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }
        });
        final DefaultTableModel tableModel = (DefaultTableModel) table.getModel();
        table.getColumnModel().getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        scroll.setViewportView(table);

        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 4;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.weightx = 1.0;
        gbc.weighty = 1.0;
        dataDialog.getContentPane().add(scroll, gbc);

        append.setIcon(new ImageIcon(imagepath + "16x16/actions/list-add.png"));
        append.setToolTipText(MessagesGED.append);
        append.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                if (PolylineData.insertPoint(objectID, tableModel.getRowCount() - 1, 0, 0, 0) != 0)
                    tableModel.addRow(new Object[] {0.0, 0.0});
            }
        });
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.insets = new Insets(3, 10, 0, 0);
        dataDialog.getContentPane().add(append, gbc);

        delete.setIcon(new ImageIcon(imagepath + "16x16/actions/list-remove.png"));
        delete.setToolTipText(MessagesGED.delete);
        delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                if (table.getSelectedRow() != -1)
                    if (PolylineData.removePoint(objectID, table.getSelectedRow()) != 0)
                        tableModel.removeRow(table.getSelectedRow());
            }
        });
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.insets = new java.awt.Insets(3, 3, 0, 0);
        dataDialog.getContentPane().add(delete, gbc);

        refresh.setIcon(new ImageIcon(imagepath + "16x16/actions/view-refresh.png"));
        refresh.setToolTipText(MessagesGED.refresh);
        gbc = new GridBagConstraints();
        gbc.gridx = 2;
        gbc.gridy = 1;
        gbc.anchor = java.awt.GridBagConstraints.NORTHWEST;
        gbc.insets = new java.awt.Insets(3, 3, 0, 0);
        dataDialog.getContentPane().add(refresh, gbc);

        ok.setIcon(new ImageIcon(imagepath + "16x16/actions/dynamic-blue-up.png"));
        ok.setToolTipText(MessagesGED.submit_close);
        gbc = new GridBagConstraints();
        gbc.gridx = 3;
        gbc.gridy = 1;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.insets = new Insets(3, 3, 0, 0);
        dataDialog.getContentPane().add(ok, gbc);
    }

    public void addShiftDialog (final JDialog shiftDialog,
                                final JScrollPane scroll,
                                final JTable table,
                                      JButton refresh,
                                      JButton ok) {

        shiftDialog.setTitle(MessagesGED.data_editor);
        shiftDialog.setMinimumSize(new Dimension(140, 340));
        shiftDialog.setModal(true);
        shiftDialog.setResizable(true);
        shiftDialog.getContentPane().setLayout(new GridBagLayout());

        table.setModel(new DefaultTableModel(
        new Object [][] {
            {null, null}
        },
        new String [] {
            "Null", "Null"
        }
        ) {
            Class[] types = new Class [] {
                Double.class, Double.class
            };
            boolean[] canEdit = new boolean [] {
                false, false
            };
            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }
            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });

        scroll.setViewportView(table);

        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.weightx = 1.0;
        gbc.weighty = 1.0;
        shiftDialog.getContentPane().add(scroll, gbc);

        refresh.setIcon(new ImageIcon(imagepath + "16x16/actions/view-refresh.png"));
        refresh.setToolTipText(MessagesGED.refresh);
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.anchor = java.awt.GridBagConstraints.NORTHWEST;
        gbc.insets = new java.awt.Insets(3, 10, 0, 0);
        shiftDialog.getContentPane().add(refresh, gbc);

        ok.setIcon(new ImageIcon(imagepath + "16x16/actions/dynamic-blue-up.png"));
        ok.setToolTipText(MessagesGED.submit_close);
        gbc = new java.awt.GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.anchor = java.awt.GridBagConstraints.NORTHWEST;
        gbc.insets = new java.awt.Insets(3, 3, 0, 0);
        shiftDialog.getContentPane().add(ok, gbc);
    }

    public void addJComboBox(JPanel panel, JComboBox combobox, String[] options, int column, int row) {
        combobox.setPreferredSize(new Dimension(5, 20));
        if(options != null)
            combobox.setModel(new DefaultComboBoxModel(options));

        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = column;
        gbc.gridy = row;
        gbc.ipadx = 70;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 4, 5, 0);
        panel.add(combobox, gbc);
    }

    public void addJLabel(JPanel panel, JLabel label, String text, int column, int row, int insetLeft) {
        label.setBackground(new Color(255, 255, 255));
        label.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        label.setOpaque(true);
        label.setPreferredSize(new Dimension(5, 20));
        label.setText(" " + text);

        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = column;
        gbc.gridy = row;
        gbc.ipadx = 70;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, insetLeft, 5, 0);
        panel.add(label, gbc);
    }

    public void addJTextField(JPanel panel, JTextField textField, boolean editable, int column, int row, int insetLeft) {
        textField.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        textField.setPreferredSize(new Dimension(5, 20));
        textField.setEditable(editable);

        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = column;
        gbc.gridy = row;
        gbc.ipadx = 70;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, insetLeft, 5, 0);
        panel.add(textField, gbc);
    }

    /**
    * Add Section (Button, Title, Separator and Main JPanel).
    * @param mpanel Main JPanel
    * @param spanel Main JPanel of Group
    * @param tbutton Show/Hide button
    * @param label JLabel for title of section
    * @param separator Separator
    * @param message Title of section
    * @param section Position of section - [1,n] 1=top n=bottom
    */
    public void addHeader(JPanel mpanel, JPanel spanel, JToggleButton tbutton, JLabel label,
                           JSeparator separator, String message, int section) {
        int row = 3 * (section-1);
        addSHbutton(mpanel, tbutton, 0, row);
        addSectionTitle(mpanel, label, message, row);
        addSeparator(mpanel, separator, row+1);
        spanel.setLayout(new GridBagLayout());

        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = row+2;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.FIRST_LINE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 0, 12, 0);
        mpanel.add(spanel, gbc);
    }

    /**
    * Add Vertical Filler.
    * @param mpanel Main JPanel
    * @param filler Filler
    * @param row Row in GBC
    */
    public void addFiller(JPanel mpanel, Filler filler, int row) {
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = row;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.VERTICAL;
        gbc.weighty = 0.1;
        mpanel.add(filler, gbc);
    }

    /**
    * Add JLabel + JTextField + Inner JPanel.
    * @param gpanel Main JPanel of Group
    * @param ipanel Inner JPanel
    * @param tbutton Show/Hide button
    * @param label JLabel for title of property
    * @param field JTextField
    * @param message Title of property
    * @param row Row number
    */
    public void addInnerPanel(JPanel gpanel, JPanel ipanel, JToggleButton tbutton, JLabel label,
                        JTextField field, String message, int row) {
        tbutton.setSelected(true);
        ipanel.setVisible(false);
        addSHbutton(gpanel, tbutton, 0, row);
        addJLabel(gpanel, label, message, 1, row, 0);
        addJTextField(gpanel, field, false, 2, row, 4);
        ipanel.setLayout(new GridBagLayout());

        //Positioning Inner JPanel.
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = row+1;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 0.1;
        gpanel.add(ipanel, gbc);
    }
}