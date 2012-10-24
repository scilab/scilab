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
package org.scilab.modules.gui.ged.polyline;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Insets;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Base Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class BaseProperties extends Roof {
    protected static JToggleButton bBaseProperties;
    protected static JPanel pBaseProperties;
    private JSeparator sBaseProperties;
    private JLabel lBaseProperties;
    private JComboBox cVisible;
    private JLabel lVisible;
    private JLabel lClosed;
    private JComboBox cClosed;
    private JLabel lFillMode;
    private JComboBox cFillMode;
    private JLabel lLineMode;
    private JComboBox cLineMode;
    private JLabel lMarkMode;
    private JComboBox cMarkMode;
    protected String currentpolyline = null;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public BaseProperties(String objectID){
        initPropertiesBase(objectID);
    }

    /**
    * It has all the components of the section Base Properties.
    */
    @Override
    public void basePropertiesComponents() {
        ContentLayout layout = new ContentLayout();

        bBaseProperties = new JToggleButton();
        lBaseProperties = new JLabel();
        sBaseProperties = new JSeparator();
        pBaseProperties = new JPanel();
        lClosed = new JLabel();
        cClosed = new JComboBox();
        lFillMode = new JLabel();
        cFillMode = new JComboBox();
        lLineMode = new JLabel();
        cLineMode = new JComboBox();
        lMarkMode = new JLabel();
        cMarkMode = new JComboBox();
        lVisible = new JLabel();
        cVisible = new JComboBox();

        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};

        //Components of the header: Base Properties.
        bBaseProperties.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bBasePropertiesActionPerformed(evt);
            }
        });
        layout.addSHbutton(this, bBaseProperties, 0, 0);

        layout.addSectionTitle(this, lBaseProperties, MessagesGED.base_properties, 0);

        layout.addSeparator(this, sBaseProperties, 1);

        pBaseProperties.setLayout(new GridBagLayout());

        //Components of the property: Closed.
        layout.addJLabel(pBaseProperties, lClosed, MessagesGED.closed, 0, 0, 16);

        cClosed.setModel(new DefaultComboBoxModel());
        cClosed.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cClosedActionPerformed(evt);
            }
        });
        layout.addJComboBox(pBaseProperties, cClosed, messageOffOn, 1, 0);

        //Components of the property: Fill Mode.
        layout.addJLabel(pBaseProperties, lFillMode, MessagesGED.fill_mode, 0, 1, 16);

        cFillMode.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cFillModeActionPerformed(evt);
            }
        });
        layout.addJComboBox(pBaseProperties, cFillMode, messageOffOn, 1, 1);

        //Components of the property: Line Mode.
        layout.addJLabel(pBaseProperties, lLineMode, MessagesGED.line_mode, 0, 2, 16);

        cLineMode.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cLineModeActionPerformed(evt);
            }
        });
        layout.addJComboBox(pBaseProperties, cLineMode, messageOffOn, 1, 2);

        //Components of the property: Mark Mode.
        layout.addJLabel(pBaseProperties, lMarkMode, MessagesGED.mark_mode, 0, 3, 16);

        cMarkMode.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarkModeActionPerformed(evt);
            }
        });
        layout.addJComboBox(pBaseProperties, cMarkMode, messageOffOn, 1, 3);

        //Components of the property: Visible.
        layout.addJLabel(pBaseProperties, lVisible, MessagesGED.visible, 0, 4, 16);

        cVisible.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cVisibleActionPerformed(evt);
            }
        });
        layout.addJComboBox(pBaseProperties, cVisible, messageOffOn, 1, 4);

        //Positioning JPanel Base Properties.
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 2;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.FIRST_LINE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 0, 12, 0);
        add(pBaseProperties, gbc);
    }

    /**
    * Loads the current properties of the section Base Properties.
    *
    * @param objectID Enters the identification of polyline.
    */
    public void initPropertiesBase(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;

            // Get the current status of the property: Closed
            boolean isClosed = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_CLOSED__);
            if (isClosed) {
                cClosed.setSelectedIndex(1);
            } else {
                cClosed.setSelectedIndex(0);
            }

            // Get the current status of the property: Fill Mode
            boolean fillmode = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_FILL_MODE__);
            if (fillmode) {
                cFillMode.setSelectedIndex(1);
            } else {
                cFillMode.setSelectedIndex(0);
            }

            // Get the current status of the property: Line Mode
            boolean linemode = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_MODE__);
            if (linemode) {
                cLineMode.setSelectedIndex(1);
            } else {
                cLineMode.setSelectedIndex(0);
            }

            // Get the current status of the property: Mark Mode
            boolean markmode = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__);
            if (markmode) {
                cMarkMode.setSelectedIndex(1);
            } else {
                cMarkMode.setSelectedIndex(0);
            }

            // Get the current status of the property: Visible
            boolean isVisible = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
        }
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bBasePropertiesActionPerformed(ActionEvent evt) {
        if (bBaseProperties.isSelected()) {
            pBaseProperties.setVisible(false);
            HidePolyline.checkAllButtons();
        } else {
            pBaseProperties.setVisible(true);
            HidePolyline.checkAllButtons();
        }
    }

    /**
    * Updates the property: Closed.
    * @param evt ActionEvent.
    */
    private void cClosedActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty
                (currentpolyline, GraphicObjectProperties.__GO_CLOSED__,
                cClosed.getSelectedIndex() == 0 ? false : true
                );
    }

    /**
    * Updates the property: Fill Mode.
    * @param evt ActionEvent.
    */
    private void cFillModeActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_FILL_MODE__,
                cFillMode.getSelectedIndex() == 0 ? false : true
                );
    }

    /**
    * Updates the property: Line Mode.
    * @param evt ActionEvent.
    */
    private void cLineModeActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_LINE_MODE__,
                cLineMode.getSelectedIndex() == 0 ? false : true
                );
    }

    /**
    * Updates the property: Mark Mode.
    * @param evt ActionEvent.
    */
    private void cMarkModeActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__,
                cMarkMode.getSelectedIndex() == 0 ? false : true
                );
    }

    /**
    * Updates the property: Visible.
    * @param evt ActionEvent.
    */
    private void cVisibleActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_VISIBLE__,
                cVisible.getSelectedIndex() == 0 ? false : true
                );
    }
}
