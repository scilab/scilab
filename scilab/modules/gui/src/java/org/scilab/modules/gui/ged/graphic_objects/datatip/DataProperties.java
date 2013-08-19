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
package org.scilab.modules.gui.ged.graphic_objects.datatip;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends Datatip implements SimpleSection {
    private String UID;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bDataProperties;
    private JLabel lDataProperties;
    private JSeparator sDataProperties;
    private static JPanel pDataProperties;
    private JToggleButton bTipData;
    private JLabel lTipData;
    private JTextField cTipData;
    private JPanel pTipData;
    private JLabel lTipDataX;
    private JTextField cTipDataX;
    private JLabel lTipDataY;
    private JTextField cTipDataY;
    private JLabel lTag;
    private JTextField cTag;
    private JLabel lUserData;
    private JTextField cUserData;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of datatip.
    */
    public DataProperties(String objectID) {
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
        bDataProperties = new JToggleButton();
        lDataProperties = new JLabel();
        sDataProperties = new JSeparator();
        pDataProperties = new JPanel();

        bTipData = new JToggleButton();
        lTipData = new JLabel();
        cTipData = new JTextField();
        pTipData = new JPanel();
        lTipDataX = new JLabel();
        cTipDataX = new JTextField();
        lTipDataY = new JLabel();
        cTipDataY = new JTextField();
        lTag = new JLabel();
        cTag = new JTextField();
        lUserData = new JLabel();
        cUserData = new JTextField();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.data_properties;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pDataProperties, bDataProperties, lDataProperties, sDataProperties, SECTIONNAME);
        bDataProperties.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pDataProperties.setVisible(!bDataProperties.isSelected());
                HideDatatip.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        int ROW = 0;
        int LEFTMARGIN = 0; //to inner components
        int COLUMN = 1; //first column
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        //Components of the property: Tip Data.
        ROW = layout.addInnerPanel(pDataProperties, pTipData, bTipData, lTipData, cTipData, MessagesGED.data, ROW);
        bTipData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pTipData.setVisible(!bTipData.isSelected());
                HideDatatip.checkAllButtons();
            }
        });
        //Tip Data X
        layout.addLabelTextField(pTipData, lTipDataX, "X",
                                 cTipDataX, true, LEFTMARGINIP, COLUMNIP, 0);
        cTipDataX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTipData();
            }
        });
        cTipDataX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTipData();
            }
        });
        //Tip Data Y
        layout.addLabelTextField(pTipData, lTipDataY, "Y",
                                 cTipDataY, true, LEFTMARGINIP, COLUMNIP, 1);
        cTipDataY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTipData();
            }
        });
        cTipDataY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTipData();
            }
        });

        //Components of the property: Tag.
        layout.addLabelTextField(pDataProperties, lTag, MessagesGED.tag,
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
        layout.addLabelTextField(pDataProperties, lUserData, MessagesGED.user_data,
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
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of datatip.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            UID = objectID;

            //Get the current status of the property: Tip Data
            getTipData();

            // Get the current status of the property: Tag
            String tag = (String) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_TAG__);
            cTag.setText(tag);

            // Get the current status of the property: User Data
            cUserData.setText((String) GraphicController.getController()
                                .getProperty(UID, GraphicObjectProperties.__GO_USER_DATA__));
        }
    }

    /**
    * Inserts the current situation of the tip_data.
    */
    public final void titleTipData() {
        cTipData.setText(" [" + cTipDataX.getText() + " , " + cTipDataY.getText() + "]");
    }

    /**
    * Get status of the property: tip_data.
    */
    private void getTipData() {
        Double[] currentTipData = (Double []) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_DATA__);
        cTipDataX.setText(currentTipData[0].toString());
        cTipDataY.setText(currentTipData[1].toString());
        titleTipData();
    }

    /**
    * Updates the property: tip_data.
    */
    private void updateTipData() {
        try {
            Double[] value = new Double[3];
            value[0] = Double.parseDouble(cTipDataX.getText());
            value[1] = Double.parseDouble(cTipDataY.getText());
            value[2] = 0.0;
            GraphicController.getController()
                       .setProperty(UID, GraphicObjectProperties.__GO_DATATIP_DATA__, value);
            titleTipData();
        } catch (NumberFormatException e) {
            getTipData();
        }
    }

    /**
    * Updates the property: Tag.
    */
    private void updateTag() {
        GraphicController.getController().setProperty(
                UID, GraphicObjectProperties.__GO_TAG__, cTag.getText());
    }

    /**
    * Updates the property: User Data.
    */
    private void updateUserData() {
        GraphicController.getController().setProperty(
                UID, GraphicObjectProperties.__GO_USER_DATA__, cUserData.getText());
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pDataProperties.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pDataProperties.setVisible(visible);
        bDataProperties.setSelected(!visible);
    }
}