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
package org.scilab.modules.gui.ged.graphic_objects.polyline;

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
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Base Properties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class BaseProperties extends Polyline implements SimpleSection {
    private static BaseProperties instance;
    private String currentpolyline;
    private ContentLayout layout = new ContentLayout();
    private int LEFTMARGIN = 16;
    private int LEFTCOLUMN = 0;
    private int RIGHTCOLUMN = 1;

    private static JToggleButton bBaseProperties;
    private static JPanel pBaseProperties;
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

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public BaseProperties(String objectID) {
        instance = this;
        currentpolyline = objectID;
        insertBase();
        components();
        values(objectID);
    }

    /**
    * Insert show/hide button, title and main JPanel of group.
    */
    @Override
    public final void insertBase() {
        String SECTIONNAME = MessagesGED.base_properties;
        this.setName(SECTIONNAME);
        bBaseProperties = new JToggleButton();
        lBaseProperties = new JLabel();
        sBaseProperties = new JSeparator();
        pBaseProperties = new JPanel();

        //Positioning JPanel Base Properties.
        layout.addHeader(this, pBaseProperties, bBaseProperties, lBaseProperties, sBaseProperties, SECTIONNAME);
        bBaseProperties.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pBaseProperties.setVisible(!bBaseProperties.isSelected());
                HidePolyline.checkAllButtons();
            }
        });
    }

    /**
    * It has all the components of the section Base Properties.
    */
    @Override
    public final void components() {
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
        int ROW = 0;

        //Components of the property: Closed.
        layout.addJLabel(pBaseProperties, lClosed, MessagesGED.closed, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJComboBox(pBaseProperties, cClosed, messageOffOn, RIGHTCOLUMN, ROW);
        cClosed.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentpolyline, GraphicObjectProperties.__GO_CLOSED__,
                        cClosed.getSelectedIndex() == 0 ? false : true);
            }
        });
        ROW++;

        //Components of the property: Fill Mode.
        layout.addJLabel(pBaseProperties, lFillMode, MessagesGED.fill_mode, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJComboBox(pBaseProperties, cFillMode, messageOffOn, RIGHTCOLUMN, ROW);
        cFillMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentpolyline, GraphicObjectProperties.__GO_FILL_MODE__,
                    cFillMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        ROW++;

        //Components of the property: Line Mode.
        layout.addJLabel(pBaseProperties, lLineMode, MessagesGED.line_mode, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJComboBox(pBaseProperties, cLineMode, messageOffOn, RIGHTCOLUMN, ROW);
        cLineMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentpolyline, GraphicObjectProperties.__GO_LINE_MODE__,
                    cLineMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        ROW++;

        //Components of the property: Mark Mode.
        layout.addJLabel(pBaseProperties, lMarkMode, MessagesGED.mark_mode, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJComboBox(pBaseProperties, cMarkMode, messageOffOn, RIGHTCOLUMN, ROW);
        cMarkMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__,
                    getMarkMode());
            }
        });
        ROW++;

        //Components of the property: Visible.
        layout.addJLabel(pBaseProperties, lVisible, MessagesGED.visible, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJComboBox(pBaseProperties, cVisible, messageOffOn, RIGHTCOLUMN, ROW);
        cVisible.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentpolyline, GraphicObjectProperties.__GO_VISIBLE__,
                        cVisible.getSelectedIndex() == 0 ? false : true);
            }
        });
    }

    /**
    * Loads the current properties of group: Base Properties.
    * @param objectID Enters the identification of polyline.
    */
    @Override
    public final void values(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;
            boolean enable;

            // Get the current status of the property: Closed
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_CLOSED__);
            cClosed.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Fill Mode
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_FILL_MODE__);
            cFillMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Line Mode
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_MODE__);
            cLineMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Mark Mode
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__);
            cMarkMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Visible
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_VISIBLE__);
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
    * Get Mark Mode.
    * @return boolean
    */
    public boolean getMarkMode() {
        return cMarkMode.getSelectedIndex() == 0 ? false : true;
    }

    /**
    * Set Mark Mode.
    * @param boolean
    */
    public void setMarkMode(boolean enable) {
        cMarkMode.setSelectedIndex(enable?1:0);
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__,
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