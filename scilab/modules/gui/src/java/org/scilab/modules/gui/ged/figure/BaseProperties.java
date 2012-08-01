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

import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Base Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class BaseProperties extends Roof {
    protected static JToggleButton bBaseProperties;
    protected static JPanel pBaseProperties;
    protected JLabel lBaseProperties;
    protected JSeparator sBaseProperties;
    protected JComboBox cVisible;
    private JLabel lVisible;

    protected String currentfigure = null;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Figure.
    */
    public BaseProperties(String objectID) {
        position();
        setIconsBaseProperties();
        initPropertiesBase(objectID);
    }

    /**
    * It has all the components of the section Base Properties.
    */
    @Override
    public void basePropertiesComponents() {
        pBaseProperties = new JPanel();
        bBaseProperties = new JToggleButton();
        lBaseProperties = new JLabel();
        sBaseProperties = new JSeparator();
        lVisible = new JLabel();
        cVisible = new JComboBox();

        //Components of the header: Base Properties.
        pBaseProperties.setAlignmentX(0.0F);
        pBaseProperties.setAlignmentY(0.0F);

        bBaseProperties.setBorder(null);
        bBaseProperties.setBorderPainted(false);
        bBaseProperties.setContentAreaFilled(false);
        bBaseProperties.setMaximumSize(new Dimension(16, 16));
        bBaseProperties.setMinimumSize(new Dimension(16, 16));
        bBaseProperties.setPreferredSize(new Dimension(16, 16));
        bBaseProperties.setRolloverEnabled(false);
        bBaseProperties.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bBasePropertiesActionPerformed(evt);
            }
        });

        lBaseProperties.setText(MessagesGED.base_properties);

        sBaseProperties.setPreferredSize(new Dimension(50, 2));

        //Components of the property: Visible.
        lVisible.setBackground(new Color(255, 255, 255));
        lVisible.setText(" " + MessagesGED.visible);
        lVisible.setAlignmentX(0.5F);
        lVisible.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lVisible.setOpaque(true);
        lVisible.setPreferredSize(new Dimension(70, 20));

        cVisible.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cVisible.setBorder(null);
        cVisible.setEditor(null);
        cVisible.setPreferredSize(new Dimension(70, 20));
        cVisible.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cVisibleActionPerformed(evt);
            }
        });
    }

    /**
    * Positioning all the components of the Basic Properties.
    */
    private void position() {
        GroupLayout pBasePropertiesLayout = new GroupLayout(pBaseProperties);
        pBaseProperties.setLayout(pBasePropertiesLayout);
        pBasePropertiesLayout.setHorizontalGroup(
            pBasePropertiesLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pBasePropertiesLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addComponent(lVisible, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cVisible, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        pBasePropertiesLayout.setVerticalGroup(
            pBasePropertiesLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pBasePropertiesLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(lVisible, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
            .addComponent(cVisible, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        );
    }

    /**
    * Loads the current properties of the section Base Properties.
    *
    * @param objectID Enters the identification of Figure.
    */
    private void initPropertiesBase(String objectID) {
        if (objectID != null) {
            currentfigure = objectID;
            // Get the current status of the property: Visible
            boolean isVisible = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private final void setIconsBaseProperties() {
        bBaseProperties.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bBaseProperties.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bBasePropertiesActionPerformed(ActionEvent evt) {
        if (bBaseProperties.isSelected()) {
            pBaseProperties.setVisible(false);
            HideFigure.checkAllButtons();
        } else {
            pBaseProperties.setVisible(true);
            HideFigure.checkAllButtons();
        }
    }

    /**
    * Updates the property: Visible.
    *
    * @param evt ActionEvent.
    */
    private void cVisibleActionPerformed(ActionEvent evt) {
        boolean setVisible = true;
        if (cVisible.getSelectedIndex() == 0) {
            setVisible = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_VISIBLE__, setVisible);
    }
}