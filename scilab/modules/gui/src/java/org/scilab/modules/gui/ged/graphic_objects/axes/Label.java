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
* Construction and startup of all components of the section: Label.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends Axes implements SimpleSection {
    private String currentAxes;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bLabel;
    private JLabel lLabel;
    private JSeparator sLabel;
    private static JPanel pLabel;
    private JTextField cTitle;
    private JLabel lTitle;
    private JTextField cXLabel;
    private JTextField cYLabel;
    private JTextField cZLabel;
    private JLabel lXLabel;
    private JLabel lYLabel;
    private JLabel lZLabel;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of figure.
    */
    public Label(String objectID) {
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
        bLabel = new JToggleButton();
        lLabel = new JLabel();
        sLabel = new JSeparator();
        pLabel = new JPanel();
        lTitle = new JLabel();
        cTitle = new JTextField();
        lXLabel = new JLabel();
        cXLabel = new JTextField();
        lYLabel = new JLabel();
        cYLabel = new JTextField();
        lZLabel = new JLabel();
        cZLabel = new JTextField();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.label;
        this.setName(SECTIONNAME);
        layout.addHeader(this, pLabel, bLabel, lLabel, sLabel, SECTIONNAME);
        bLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pLabel.setVisible(!bLabel.isSelected());
                HideAxes.checkAllButtons();
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

        //Components of the property: Title
        layout.addLabelTextField(pLabel, lTitle, MessagesGED.title,
                                 cTitle, true, LEFTMARGIN, COLUMN, ROW++);
        cTitle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTitle();
            }
        });
        cTitle.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTitle();
            }
        });

        //Components of the property: X Label
        layout.addLabelTextField(pLabel, lXLabel, MessagesGED.x_label,
                                 cXLabel, true, LEFTMARGIN, COLUMN, ROW++);
        cXLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateLabel(0);
            }
        });
        cXLabel.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateLabel(0);
            }
        });

        //Components of the property: Y Label
        layout.addLabelTextField(pLabel, lYLabel, MessagesGED.y_label,
                                 cYLabel, true, LEFTMARGIN, COLUMN, ROW++);
        cYLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateLabel(1);
            }
        });
        cYLabel.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateLabel(1);
            }
        });

        //Components of the property: Z Label
        layout.addLabelTextField(pLabel, lZLabel, MessagesGED.z_label,
                                 cZLabel, true, LEFTMARGIN, COLUMN, ROW++);
        cZLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateLabel(2);
            }
        });
        cZLabel.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateLabel(2);
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of axis.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentAxes = objectID;

            // Get the current status of the property: Title
            String titleID = (String) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_TITLE__);
            String[] title = (String[]) GraphicController.getController()
                    .getProperty(titleID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitle.setText(title[0]);

            // Get the current status of the property: X Label
            String labelID = (String) GraphicController.getController()
                    .getProperty(currentAxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
            String[] label = (String[]) GraphicController.getController()
                    .getProperty(labelID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cXLabel.setText(label[0]);

            // Get the current status of the property: Y Label
            labelID = (String) GraphicController.getController()
                    .getProperty(currentAxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
            label = (String[]) GraphicController.getController()
                    .getProperty(labelID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cYLabel.setText(label[0]);

            // Get the current status of the property: Z Label
            labelID = (String) GraphicController.getController()
                    .getProperty(currentAxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
            label = (String[]) GraphicController.getController()
                    .getProperty(labelID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cZLabel.setText(label[0]);
        }
    }


    /**
    * Updates the property: Title.
    */
    private void updateTitle() {
        String titleID = (String) GraphicController.getController()
                .getProperty(currentAxes, GraphicObjectProperties.__GO_TITLE__);
        String[] text = new String[] {cTitle.getText()};
        GraphicController.getController()
                .setProperty(titleID, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
    }

    /**
    * Updates the property: Label.
    * @param axis x=0  y=1  z=2
    */
    private void updateLabel(int axis) {
        String[] text = new String[1];
        String axisLabel = null;
        switch (axis){
            case 0:
                axisLabel = (String) GraphicController.getController()
                         .getProperty(currentAxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                text[0] = cXLabel.getText();
                break;
            case 1:
                axisLabel = (String) GraphicController.getController()
                         .getProperty(currentAxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                text[0] = cYLabel.getText();
                break;
            case 2:
                axisLabel = (String) GraphicController.getController()
                         .getProperty(currentAxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                text[0] = cZLabel.getText();
                break;
        }
        GraphicController.getController()
                .setProperty(axisLabel, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pLabel.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pLabel.setVisible(visible);
        bLabel.setSelected(!visible);
    }
}