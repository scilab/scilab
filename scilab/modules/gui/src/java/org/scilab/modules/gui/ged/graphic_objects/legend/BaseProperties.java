/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.legend;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Base Properties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class BaseProperties extends Legend implements SimpleSection {
    private static BaseProperties instance;
    private String currentLegend;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bBaseProperties;
    private static JPanel pBaseProperties;
    private JLabel lBaseProperties;
    private JSeparator sBaseProperties;
    private JLabel lFillMode;
    private JComboBox cFillMode;
    private JLabel lLineMode;
    private JComboBox cLineMode;
    private JLabel lVisible;
    private JComboBox cVisible;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of Legend.
    */
    public BaseProperties(String objectID) {
        instance = this;
        constructComponents();
        initMainPanel();
        initComponents();
        loadProperties(objectID);
    }

    /**
    * Construct the Components.
    */
    @Override
    public final void constructComponents() {
        bBaseProperties = new JToggleButton();
        lBaseProperties = new JLabel();
        sBaseProperties = new JSeparator();
        pBaseProperties = new JPanel();

        lFillMode = new JLabel();
        cFillMode = new JComboBox();
        lLineMode = new JLabel();
        cLineMode = new JComboBox();
        lVisible = new JLabel();
        cVisible = new JComboBox();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.base_properties;
        this.setName(SECTIONNAME);
        layout.addHeader(this, pBaseProperties, bBaseProperties, lBaseProperties, sBaseProperties, SECTIONNAME);
        bBaseProperties.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pBaseProperties.setVisible(!bBaseProperties.isSelected());
                HideLegend.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        int ROW = 0;
        int LEFTMARGIN = 16; //to inner components
        int COLUMN = 0; //first column

        //Components of the property: Fill Mode.
        layout.addLabelComboBox(pBaseProperties, lFillMode, MessagesGED.fill_mode,
                                cFillMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cFillMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentLegend, GraphicObjectProperties.__GO_FILL_MODE__,
                    cFillMode.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Line Mode.
        layout.addLabelComboBox(pBaseProperties, lLineMode, MessagesGED.line_mode,
                                cLineMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cLineMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentLegend, GraphicObjectProperties.__GO_LINE_MODE__,
                    cLineMode.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Visible.
        layout.addLabelComboBox(pBaseProperties, lVisible, MessagesGED.visible,
                                cVisible, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cVisible.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentLegend, GraphicObjectProperties.__GO_VISIBLE__,
                    cVisible.getSelectedIndex() == 0 ? false : true);
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of Legend.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentLegend = objectID;
            boolean enable;

            // Get the current status of the property: Line Mode
            enable = (Boolean) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_LINE_MODE__);
            cLineMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Fill Mode
            enable = (Boolean) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_FILL_MODE__);
            cFillMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Visible
            enable = (Boolean) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_VISIBLE__);
            cVisible.setSelectedIndex(enable?1:0);
        }
    }

    /**
    * Get Instance.
    * @return instance
    */
    public static BaseProperties getInstance() {
        return instance;
    }

    /**
    * Get Fill Mode.
    * @return boolean
    */
    public boolean getFillMode() {
        return cFillMode.getSelectedIndex() == 0 ? false : true;
    }

    /**
    * Set Fill Mode.
    * @param boolean
    */
    public void setFillMode(boolean enable) {
        cFillMode.setSelectedIndex(enable?1:0);
        GraphicController.getController().setProperty(
                currentLegend, GraphicObjectProperties.__GO_FILL_MODE__,
                enable);
    }

    /**
    * Get Line Mode.
    * @return boolean
    */
    public boolean getLineMode() {
        return cLineMode.getSelectedIndex() == 0 ? false : true;
    }

    /**
    * Set Line Mode.
    * @param boolean
    */
    public void setLineMode(boolean enable) {
        cLineMode.setSelectedIndex(enable?1:0);
        GraphicController.getController().setProperty(
                currentLegend, GraphicObjectProperties.__GO_LINE_MODE__,
                enable);
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pBaseProperties.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pBaseProperties.setVisible(visible);
        bBaseProperties.setSelected(!visible);
    }
}