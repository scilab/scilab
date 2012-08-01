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
package org.scilab.modules.gui.ged.axes;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Label.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends AxisRulers {
    //Header - Label
    protected static JToggleButton bLabel;
    protected JLabel lLabel;
    protected static JPanel pLabel;
    protected JSeparator sLabel;
    //Components of the property: Title Page
    protected JLabel lTitlePage;
    protected JTextField cTitlePage;
    //Components of the property: Axis Title
    protected static JToggleButton bAxisTitle;
    protected JLabel lAxisTitle;
    protected JLabel cAxisTitle;
    protected static JPanel pAxisTitle;
    protected JLabel lTitleX;
    protected JTextField cTitleX;
    protected JLabel lTitleY;
    protected JTextField cTitleY;
    protected JLabel lTitleZ;
    protected JTextField cTitleZ;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Axes.
    */
    public Label(String objectID) {
        super(objectID);
        position();
        setIconsLabel();
        initPropertiesLabel(objectID);
    }

    /**
    * It has all the components of the section Label.
    */
    @Override
    public void labelComponents() {
        //Header
        bLabel = new JToggleButton();
        lLabel = new JLabel();
        sLabel = new JSeparator();
        pLabel = new JPanel();
        //Title Page
        lTitlePage = new JLabel();
        cTitlePage = new JTextField();
        //Axis Title
        bAxisTitle = new JToggleButton();
        lAxisTitle = new JLabel();
        cAxisTitle = new JLabel();
        pAxisTitle = new JPanel();
        lTitleX = new JLabel();
        cTitleX = new JTextField();
        lTitleY = new JLabel();
        cTitleY = new JTextField();
        lTitleZ = new JLabel();
        cTitleZ = new JTextField();

        //Components of the header: Axis Rulers
        bLabel.setBorder(null);
        bLabel.setBorderPainted(false);
        bLabel.setContentAreaFilled(false);
        bLabel.setMaximumSize(new Dimension(16, 16));
        bLabel.setMinimumSize(new Dimension(16, 16));
        bLabel.setPreferredSize(new Dimension(16, 16));
        bLabel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bLabelActionPerformed(evt);
            }
        });

        lLabel.setText(MessagesGED.label);

        sLabel.setPreferredSize(new Dimension(50, 2));

        //Components of the property: Title Page.
        lTitlePage.setBackground(new Color(255, 255, 255));
        lTitlePage.setText(" " + MessagesGED.title_page);
        lTitlePage.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lTitlePage.setOpaque(true);
        lTitlePage.setPreferredSize(new Dimension(70, 20));

        cTitlePage.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cTitlePage.setPreferredSize(new Dimension(70, 20));
        cTitlePage.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cTitlePageActionPerformed(evt);
            }
        });

        //Components of the property: Axis Title.
        bAxisTitle.setSelected(true);
        bAxisTitle.setBorder(null);
        bAxisTitle.setBorderPainted(false);
        bAxisTitle.setContentAreaFilled(false);
        bAxisTitle.setMaximumSize(new Dimension(16, 16));
        bAxisTitle.setMinimumSize(new Dimension(16, 16));
        bAxisTitle.setPreferredSize(new Dimension(16, 16));
        bAxisTitle.setRolloverEnabled(false);
        bAxisTitle.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bAxisTitleActionPerformed(evt);
            }
        });

        lAxisTitle.setBackground(new Color(255, 255, 255));
        lAxisTitle.setText(" " + MessagesGED.axis_title);
        lAxisTitle.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lAxisTitle.setOpaque(true);
        lAxisTitle.setPreferredSize(new Dimension(70, 20));

        cAxisTitle.setBackground(new Color(255, 255, 255));
        cAxisTitle.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cAxisTitle.setOpaque(true);
        cAxisTitle.setPreferredSize(new Dimension(70, 20));

        pAxisTitle.setVisible(false);

        lTitleX.setBackground(new Color(255, 255, 255));
        lTitleX.setText(" X");
        lTitleX.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lTitleX.setOpaque(true);
        lTitleX.setPreferredSize(new Dimension(70, 20));

        cTitleX.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cTitleX.setPreferredSize(new Dimension(70, 20));
        cTitleX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cTitleXActionPerformed(evt);
            }
        });

        lTitleY.setBackground(new Color(255, 255, 255));
        lTitleY.setText(" Y");
        lTitleY.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lTitleY.setOpaque(true);
        lTitleY.setPreferredSize(new Dimension(70, 20));

        cTitleY.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cTitleY.setPreferredSize(new Dimension(70, 20));
        cTitleY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cTitleYActionPerformed(evt);
            }
        });

        lTitleZ.setBackground(new Color(255, 255, 255));
        lTitleZ.setText(" Z");
        lTitleZ.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lTitleZ.setOpaque(true);
        lTitleZ.setPreferredSize(new Dimension(70, 20));

        cTitleZ.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cTitleZ.setPreferredSize(new Dimension(70, 20));
        cTitleZ.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cTitleZActionPerformed(evt);
            }
        });

        //Positioning the components of the property: Axis Title.
        GroupLayout pAxisTitleLayout = new GroupLayout(pAxisTitle);
        pAxisTitle.setLayout(pAxisTitleLayout);
        pAxisTitleLayout.setHorizontalGroup(
            pAxisTitleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addComponent(lTitleX, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cTitleX, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addComponent(lTitleY, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cTitleY, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addComponent(lTitleZ, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cTitleZ, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        pAxisTitleLayout.setVerticalGroup(
            pAxisTitleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addGroup(pAxisTitleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lTitleX, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitleX, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxisTitleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lTitleY, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitleY, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxisTitleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lTitleZ, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitleZ, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Positioning all the components of the Label.
    */
    private void position() {
        //Positioning all components.
        GroupLayout pLabelLayout = new GroupLayout(pLabel);
        pLabel.setLayout(pLabelLayout);
        pLabelLayout.setHorizontalGroup(
            pLabelLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pLabelLayout.createSequentialGroup()
                .addComponent(bAxisTitle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lAxisTitle, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cAxisTitle, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pLabelLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pLabelLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pLabelLayout.createSequentialGroup()
                        .addComponent(lTitlePage, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(cTitlePage, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addComponent(pAxisTitle, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)))
        );
        pLabelLayout.setVerticalGroup(
            pLabelLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pLabelLayout.createSequentialGroup()
                .addGroup(pLabelLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lTitlePage, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitlePage, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pLabelLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pLabelLayout.createSequentialGroup()
                        .addGap(3, 3, 3)
                        .addComponent(bAxisTitle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                    .addComponent(lAxisTitle, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAxisTitle, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addComponent(pAxisTitle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
        );
    }

    /**
    * Loads the current properties of the section Label.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesLabel(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Title Page */
            String titlePage = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_TITLE__);

            String[] textTitlePage = (String[])GraphicController.getController()
                    .getProperty(titlePage, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitlePage.setText(textTitlePage[0]);

            /** Get the current status of the property: Axis Title */
            String axisTitleX = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);

            String[] textTitleX = (String[])GraphicController.getController()
                    .getProperty(axisTitleX, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitleX.setText(textTitleX[0]);

            String axisTitleY = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);

            String[] textTitleY = (String[])GraphicController.getController()
                    .getProperty(axisTitleY, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitleY.setText(textTitleY[0]);

            String axisTitleZ = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);

            String[] textTitleZ = (String[])GraphicController.getController()
                    .getProperty(axisTitleZ, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitleZ.setText(textTitleZ[0]);

            titleAxis();
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private final void setIconsLabel() {
        bLabel.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bLabel.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bAxisTitle.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bAxisTitle.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bLabelActionPerformed(ActionEvent evt) {
        if (bLabel.isSelected()) {
            pLabel.setVisible(false);
            HideAxis.checkAllButtons();
        } else {
            pLabel.setVisible(true);
            HideAxis.checkAllButtons();
        }
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bAxisTitleActionPerformed(ActionEvent evt) {
        if (bAxisTitle.isSelected()) {
            pAxisTitle.setVisible(false);
        } else {
            pAxisTitle.setVisible(true);
        }
    }

    /**
    * Inserts the current state of functionality: AXIS TITLE in main label.
    */
    public void titleAxis() {
        String titleAxisX = cTitleX.getText();
        String titleAxisY = cTitleY.getText();
        String titleAxisZ = cTitleZ.getText();
        String titleAxis = titleAxisX + " , " + titleAxisY + " , " + titleAxisZ;
        cAxisTitle.setText(" [" + titleAxis + "]");
    }

    /**
    * Updates the property: x Title Axis.
    *
    * @param evt ActionEvent.
    */
    private void cTitleXActionPerformed(ActionEvent evt) {
        String axisTitleX = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
        String[] text = new String[1];
        text[0] = cTitleX.getText();
        GraphicController.getController()
                .setProperty(axisTitleX, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);

        titleAxis();
    }

    /**
    * Updates the property: y Title Axis.
    *
    * @param evt ActionEvent.
    */
    private void cTitleYActionPerformed(ActionEvent evt) {
        String axisTitleY = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
        String[] text = new String[1];
        text[0] = cTitleY.getText();
        GraphicController.getController()
                .setProperty(axisTitleY, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
        titleAxis();
    }

    /**
    * Updates the property: z Title Axis.
    *
    * @param evt ActionEvent.
    */
    private void cTitleZActionPerformed(ActionEvent evt) {
        String axisTitleZ = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
        String[] text = new String[1];
        text[0] = cTitleZ.getText();
        GraphicController.getController()
                .setProperty(axisTitleZ, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
        titleAxis();
    }

    /**
    * Updates the property: Title Page.
    *
    * @param evt ActionEvent.
    */
    private void cTitlePageActionPerformed(ActionEvent evt) {
        String titlePage = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_TITLE__);
        String[] text = new String[1];
        text[0] = cTitlePage.getText();
        GraphicController.getController()
                .setProperty(titlePage, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
    }
}