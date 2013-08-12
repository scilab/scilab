/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.surface;

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
public class BaseProperties extends Surface implements SimpleSection {
    private String currentSurface;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bBaseProperties;
    private static JPanel pBaseProperties;
    private JLabel lBaseProperties;
    private JSeparator sBaseProperties;
    private JLabel lSurfaceMode;
    private JComboBox cSurfaceMode;
    private JLabel lVisible;
    private JComboBox cVisible;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of surface.
    */
    public BaseProperties(String objectID) {
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

        lSurfaceMode = new JLabel();
        cSurfaceMode = new JComboBox();
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
                HideSurface.checkAllButtons();
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

        //Components of the property: Surface Mode.
        layout.addLabelComboBox(pBaseProperties, lSurfaceMode, MessagesGED.surface_mode,
                                cSurfaceMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cSurfaceMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentSurface, GraphicObjectProperties.__GO_SURFACE_MODE__,
                    cSurfaceMode.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Visible.
        layout.addLabelComboBox(pBaseProperties, lVisible, MessagesGED.visible,
                                cVisible, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cVisible.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentSurface, GraphicObjectProperties.__GO_VISIBLE__,
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
            currentSurface = objectID;
            boolean enable;

            // Get the current status of the property: Surface Mode
            enable = (Boolean) GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_SURFACE_MODE__);
            cSurfaceMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Visible
            enable = (Boolean) GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_VISIBLE__);
            cVisible.setSelectedIndex(enable?1:0);
        }
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