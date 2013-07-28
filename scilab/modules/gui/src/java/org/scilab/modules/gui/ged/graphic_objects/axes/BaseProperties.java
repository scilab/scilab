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
package org.scilab.modules.gui.ged.graphic_objects.axes;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
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
public class BaseProperties extends Axes implements SimpleSection {
    private ContentLayout layout = new ContentLayout();
    private String currentAxes;

    private static JToggleButton bBaseProperties;
    private static JPanel pBaseProperties;
    private JSeparator sBaseProperties;
    private JLabel lBaseProperties;
    private JComboBox cAutoClear;
    private JPanel cAxesVisible;
    private JCheckBox cAxesVisibleX;
    private JCheckBox cAxesVisibleY;
    private JCheckBox cAxesVisibleZ;
    private JPanel cLogFlags;
    private JCheckBox cLogFlagsX;
    private JCheckBox cLogFlagsY;
    private JCheckBox cLogFlagsZ;
    private JTextField cTag;
    private JTextField cUserData;
    private JComboBox cVisible;
    private JLabel lAutoClear;
    private JLabel lAxesVisible;
    private JLabel lLogFlags;
    private JLabel lTag;
    private JLabel lUserData;
    private JLabel lVisible;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
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
        lAxesVisible = new JLabel();
        cAxesVisible = new JPanel();
        cAxesVisibleX = new JCheckBox();
        cAxesVisibleY = new JCheckBox();
        cAxesVisibleZ = new JCheckBox();
        lAutoClear = new JLabel();
        cAutoClear = new JComboBox();
        lTag = new JLabel();
        cTag = new JTextField();
        lUserData = new JLabel();
        cUserData = new JTextField();
        lVisible = new JLabel();
        cVisible = new JComboBox();
        lLogFlags = new JLabel();
        cLogFlags = new JPanel();
        cLogFlagsX = new JCheckBox();
        cLogFlagsY = new JCheckBox();
        cLogFlagsZ = new JCheckBox();
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
                HideAxes.checkAllButtons();
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

        //Components of the property: Auto Clear.
        layout.addLabelComboBox(pBaseProperties, lAutoClear, MessagesGED.auto_clear,
                                cAutoClear, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cAutoClear.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_AUTO_CLEAR__,
                        cAutoClear.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Axes Visible.
        layout.addLabel3CheckBox(pBaseProperties, lAxesVisible, MessagesGED.axes_visible,
                cAxesVisible, cAxesVisibleX, cAxesVisibleY, cAxesVisibleZ, LEFTMARGIN, COLUMN, ROW++);
        cAxesVisibleX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__,
                        cAxesVisibleX.isSelected());
            }
        });
        cAxesVisibleY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__,
                        cAxesVisibleY.isSelected());
            }
        });
        cAxesVisibleZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__,
                        cAxesVisibleZ.isSelected());
            }
        });

        //Components of the property: Log Flags.
        layout.addLabel3CheckBox(pBaseProperties, lLogFlags, MessagesGED.log_flags,
                cLogFlags, cLogFlagsX, cLogFlagsY, cLogFlagsZ, LEFTMARGIN, COLUMN, ROW++);
        cLogFlagsX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__,
                        cLogFlagsX.isSelected());
            }
        });
        cLogFlagsY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__,
                        cLogFlagsY.isSelected());
            }
        });
        cLogFlagsZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__,
                        cLogFlagsZ.isSelected());
            }
        });

        //Components of the property: Tag.
        layout.addLabelTextField(pBaseProperties, lTag, MessagesGED.tag,
                                 cTag, true, LEFTMARGIN, COLUMN, ROW++);
        cTag.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTag();
            }
        });
        cTag.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTag();
            }
        });

        //Components of the property: User Data.
        layout.addLabelTextField(pBaseProperties, lUserData, MessagesGED.user_data,
                                 cUserData, true, LEFTMARGIN, COLUMN, ROW++);
        cUserData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateUserData();
            }
        });
        cUserData.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateUserData();
            }
        });

        //Components of the property: Visible.
        layout.addLabelComboBox(pBaseProperties, lVisible, MessagesGED.visible,
                                cVisible, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cVisible.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_VISIBLE__,
                        cVisible.getSelectedIndex() == 0 ? false : true);
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of axes.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentAxes = objectID;
            boolean enable;

            // Get the current status of the property: Auto Clear
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_AUTO_CLEAR__);
            cAutoClear.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Axes Visible
            cAxesVisibleX.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__));
            cAxesVisibleY.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__));
            cAxesVisibleZ.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__));

            //Get the current status of the property: Log Flags
            cLogFlagsX.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__));
            cLogFlagsY.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__));
            cLogFlagsZ.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__));

            // Get the current status of the property: Tag
            cTag.setText((String) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_TAG__));

            // Get the current status of the property: User Data
            cUserData.setText((String) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_USER_DATA__));

            // Get the current status of the property: Visible
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_VISIBLE__);
            cVisible.setSelectedIndex(enable?1:0);
        }
    }

    /**
    * Updates the property: Tag.
    */
    private void updateTag() {
        GraphicController.getController().setProperty(
                currentAxes, GraphicObjectProperties.__GO_TAG__, cTag.getText());
    }

    /**
    * Updates the property: User Data.
    */
    private void updateUserData() {
        GraphicController.getController().setProperty(
                currentAxes, GraphicObjectProperties.__GO_USER_DATA__, cUserData.getText());
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