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
package org.scilab.modules.gui.ged.figure;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: DataProperties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends Control {
    protected static JToggleButton bData;
    protected static JPanel pData;
    protected JLabel lData;
    protected JSeparator sData;
    protected JTextField cFigureID;
    protected JLabel lFigureID;
    protected JTextField cFigureName;
    protected JLabel lFigureName;
    protected JComboBox cUnits;
    protected JLabel lUnits;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Figure.
    */
    public DataProperties(String objectID) {
        super(objectID);
        position();
        setIconsData();
        initPropertiesData(objectID);
    }

    /**
    * It has all the components of the section Control.
    */
    @Override
    public void dataComponents() {
        pData = new JPanel();
        bData = new JToggleButton();
        lData = new JLabel();
        sData = new JSeparator();
        lFigureID = new JLabel();
        cFigureID = new JTextField();
        lFigureName = new JLabel();
        cFigureName = new JTextField();
        lUnits = new JLabel();
        cUnits = new JComboBox();

        //Components of the header: DataProperties.
        pData.setAlignmentX(0.0F);
        pData.setAlignmentY(0.0F);

        bData.setBorder(null);
        bData.setBorderPainted(false);
        bData.setContentAreaFilled(false);
        bData.setMaximumSize(new Dimension(16, 16));
        bData.setMinimumSize(new Dimension(16, 16));
        bData.setPreferredSize(new Dimension(16, 16));
        bData.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bDataActionPerformed(evt);
            }
        });

        lData.setText(MessagesGED.data);

        sData.setPreferredSize(new Dimension(50, 2));

        //Components of the property: Figure ID.
        lFigureID.setBackground(new Color(255, 255, 255));
        lFigureID.setText(" " + MessagesGED.figure_id);
        lFigureID.setAlignmentX(0.5F);
        lFigureID.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lFigureID.setOpaque(true);
        lFigureID.setPreferredSize(new Dimension(70, 20));

        cFigureID.setBackground(new Color(238, 238, 238));
        cFigureID.setEditable(true);
        cFigureID.setToolTipText(MessagesGED.figure_id_tooltip);
        cFigureID.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cFigureID.setPreferredSize(new Dimension(70, 20));
        cFigureID.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cFigureIDActionPerformed(evt);
            }
        });
        cFigureID.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cFigureIDFocusLost(evt);
            }
        });

        //Components of the property: Figure Name.
        lFigureName.setBackground(new Color(255, 255, 255));
        lFigureName.setText(" " + MessagesGED.figure_name);
        lFigureName.setAlignmentX(0.5F);
        lFigureName.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lFigureName.setOpaque(true);
        lFigureName.setPreferredSize(new Dimension(70, 20));

        cFigureName.setToolTipText(MessagesGED.figure_name_tooltip);
        cFigureName.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cFigureName.setPreferredSize(new Dimension(93	, 20));
        cFigureName.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cFigureNameActionPerformed(evt);
            }
        });
        cFigureName.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cFigureNameFocusLost(evt);
            }
        });

        //Components of the property: Units.
        lUnits.setBackground(new Color(255, 255, 255));
        lUnits.setText(" " + MessagesGED.units);
        lUnits.setAlignmentX(0.5F);
        lUnits.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lUnits.setOpaque(true);
        lUnits.setPreferredSize(new Dimension(127, 20));

        cUnits.setModel(new DefaultComboBoxModel(new String[] {
                            "Inches", "Centimeters", "Normalized",
                            "Points", "Pixels", "Characters"
                        }));
        cUnits.setSelectedIndex(4);
        cUnits.setBorder(null);
        cUnits.setEditor(null);
        cUnits.setPreferredSize(new Dimension(70, 20));
    }

    /**
    * Positioning all the components of the DataProperties.
    */
    private void position() {
        GroupLayout pDataLayout = new GroupLayout(pData);
        pData.setLayout(pDataLayout);
        pDataLayout.setHorizontalGroup(
            pDataLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pDataLayout.createSequentialGroup()
                      .addGap(18, 18, 18)
                      .addGroup(pDataLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addGroup(pDataLayout.createSequentialGroup()
                                          .addComponent(lFigureID, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                          .addGap(4, 4, 4)
                                          .addComponent(cFigureID, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                                .addGroup(pDataLayout.createSequentialGroup()
                                          .addComponent(lFigureName, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                          .addGap(4, 4, 4)
                                          .addComponent(cFigureName, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                                .addGroup(pDataLayout.createSequentialGroup()
                                          .addComponent(lUnits, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                          .addGap(4, 4, 4)
                                          .addComponent(cUnits, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
        );
        pDataLayout.setVerticalGroup(
            pDataLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pDataLayout.createSequentialGroup()
                      .addGroup(pDataLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lFigureID, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                                .addGroup(pDataLayout.createSequentialGroup()
                                          .addGap(1, 1, 1)
                                          .addComponent(cFigureID, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
                      .addGap(6, 6, 6)
                      .addGroup(pDataLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lFigureName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                                .addGroup(pDataLayout.createSequentialGroup()
                                          .addGap(1, 1, 1)
                                          .addComponent(cFigureName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
                      .addGap(6, 6, 6)
                      .addGroup(pDataLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addGroup(pDataLayout.createSequentialGroup()
                                          .addGap(1, 1, 1)
                                          .addComponent(lUnits, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                                .addComponent(cUnits, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Loads the current properties of the section DataProperties.
    *
    * @param objectID Enters the identification of Figure.
    */
    public void initPropertiesData(String objectID) {
        if (objectID != null) {
            currentfigure = objectID;
            /** Get the current status of the property: Figure ID */
            Integer currentFigureID = (Integer) GraphicController.getController()
                                      .getProperty(currentfigure, GraphicObjectProperties.__GO_ID__);
            String figureID;
            figureID = currentFigureID.toString();
            cFigureID.setText(figureID);
            /** Get the current status of the property: Figure Name */
            String figureName = (String) GraphicController.getController()
                                .getProperty(currentfigure, GraphicObjectProperties.__GO_NAME__);
            cFigureName.setText(figureName);
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsData() {
        bData.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bData.setIcon(new ImageIcon(SwingInspector.icon_collapse));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bDataActionPerformed(ActionEvent evt) {
        if (bData.isSelected()) {
            pData.setVisible(false);
            HideFigure.checkAllButtons();
        } else {
            pData.setVisible(true);
            HideFigure.checkAllButtons();
        }
    }

    /**
    * Updates the property: Figure ID.
    */
    private void updateFigureID() {
        int setfigureID = Integer.parseInt(cFigureID.getText());
        GraphicController.getController()
        .setProperty(currentfigure, GraphicObjectProperties.__GO_ID__, setfigureID);
    }

    /**
    * Updates the property: Figure ID.
    *
    * @param evt ActionEvent.
    */
    private void cFigureIDActionPerformed(ActionEvent evt) {
        updateFigureID();
    }

    /**
    * Updates the property: Figure ID.
    *
    * @param evt FocusEvent.
    */
    private void cFigureIDFocusLost(FocusEvent evt) {
        updateFigureID();
    }

    /**
    * Updates the property: Figure Name.
    */
    private void updateFigureName() {
        GraphicController.getController()
        .setProperty(currentfigure, GraphicObjectProperties.__GO_NAME__, cFigureName.getText());
    }

    /**
    * Updates the property: Figure Name.
    *
    * @param evt ActionEvent.
    */
    private void cFigureNameActionPerformed(ActionEvent evt) {
        updateFigureName();
    }

    /**
    * Updates the property: Figure Name.
    *
    * @param evt FocusEvent.
    */
    private void cFigureNameFocusLost(FocusEvent evt) {
        updateFigureName();
    }
}