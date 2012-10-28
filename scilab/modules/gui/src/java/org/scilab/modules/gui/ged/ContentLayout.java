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
import javax.swing.DefaultComboBoxModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

/**
* Preconfigured components in Grid Bag Constraints
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class ContentLayout extends JPanel{
    private GridBagLayout layout;  
    protected GridBagConstraints gbc;  

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
        separator.setMinimumSize(new Dimension(40, 10));
        separator.setPreferredSize(new Dimension(40, 10));

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
        fieldPanel.setMinimumSize(new Dimension(40, 20));
        fieldPanel.setPreferredSize(new Dimension(70, 20));
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

                    Class cls = Class.forName("org.scilab.modules.gui.ged." + packClass);
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

    public void addDataField(JPanel parentPanel, JPanel fieldPanel, JButton dataButton, JLabel dataLabel, int column, int row) {
        fieldPanel.setBackground(new Color(255, 255, 255));
        fieldPanel.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        fieldPanel.setMinimumSize(new Dimension(40, 20));
        fieldPanel.setPreferredSize(new Dimension(70, 20));
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

    public void addJComboBox(JPanel panel, JComboBox combobox, String[] options, int column, int row) {
        combobox.setPreferredSize(new Dimension(70, 20));
        combobox.setMinimumSize(new Dimension(40, 20));
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
        label.setPreferredSize(new Dimension(70, 20));
        label.setMinimumSize(new Dimension(40, 20));
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
        textField.setPreferredSize(new Dimension(70, 20));
        textField.setMinimumSize(new Dimension(40, 20));
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
}
