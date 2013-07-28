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
package org.scilab.modules.gui.ged.graphic_objects.figure;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: DataProperties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends Figure implements SimpleSection {
    private String currentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bDataProperties;
    private JLabel lDataProperties;
    private JSeparator sDataProperties;
    private static JPanel pDataProperties;
    private JLabel lFigureID;
    private JTextField cFigureID;
    private JLabel lFigureName;
    private JTextField cFigureName;
    private JLabel lInfoMessage;
    private JTextField cInfoMessage;
    private JLabel lTag;
    private JTextField cTag;
    private JLabel lUserData;
    private JTextField cUserData;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of figure.
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

        lFigureID = new JLabel();
        cFigureID = new JTextField();
        lFigureName = new JLabel();
        cFigureName = new JTextField();
        lInfoMessage = new JLabel();
        cInfoMessage = new JTextField();
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
                HideFigure.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        int ROW = 0;
        int LEFTMARGIN = 16; //to inner components
        int COLUMN = 0; //first column

        //Components of the property: Figure ID.
        layout.addLabelTextField(pDataProperties, lFigureID, MessagesGED.figure_id,
                                 cFigureID, true, LEFTMARGIN, COLUMN, ROW++);
        cFigureID.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigureID();
            }
        });
        cFigureID.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigureID();
            }
        });

        //Components of the property: Figure Name.
        layout.addLabelTextField(pDataProperties, lFigureName, MessagesGED.figure_name,
                                 cFigureName, true, LEFTMARGIN, COLUMN, ROW++);
        cFigureName.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigureName();
            }
        });
        cFigureName.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigureName();
            }
        });

        //Components of the property: Info Message.
        layout.addLabelTextField(pDataProperties, lInfoMessage, MessagesGED.info_message,
                                 cInfoMessage, true, LEFTMARGIN, COLUMN, ROW++);
        cInfoMessage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateInfoMessage();
            }
        });
        cInfoMessage.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateInfoMessage();
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
    * @param objectID Enters the identification of Figure.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentFigure = objectID;

            // Get the current status of the property: Figure ID
            cFigureID.setText(((Integer) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_ID__)).toString());

            // Get the current status of the property: Figure Name
            cFigureName.setText((String) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_NAME__));

            // Get the current status of the property: Info Message
            cInfoMessage.setText((String) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_INFO_MESSAGE__));

            // Get the current status of the property: Tag
            cTag.setText((String) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_TAG__));

            // Get the current status of the property: User Data
            cUserData.setText((String) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_USER_DATA__));
        }
    }

    /**
    * Updates the property: Figure ID.
    */
    private void updateFigureID() {
        int setfigureID = Integer.parseInt(cFigureID.getText());
        GraphicController.getController()
                .setProperty(currentFigure, GraphicObjectProperties.__GO_ID__, setfigureID);
    }

    /**
    * Updates the property: Figure Name.
    */
    private void updateFigureName() {
        GraphicController.getController()
                .setProperty(currentFigure, GraphicObjectProperties.__GO_NAME__, cFigureName.getText());
    }

    /**
    * Updates the property: Info Message.
    */
    private void updateInfoMessage() {
        GraphicController.getController().setProperty(
                currentFigure, GraphicObjectProperties.__GO_INFO_MESSAGE__, cInfoMessage.getText());
    }

    /**
    * Updates the property: Tag.
    */
    private void updateTag() {
        GraphicController.getController().setProperty(
                currentFigure, GraphicObjectProperties.__GO_TAG__, cTag.getText());
    }

    /**
    * Updates the property: User Data.
    */
    private void updateUserData() {
        GraphicController.getController().setProperty(
                currentFigure, GraphicObjectProperties.__GO_USER_DATA__, cUserData.getText());
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