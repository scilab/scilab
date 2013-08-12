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
package org.scilab.modules.gui.ged.graphic_objects.legend;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JComboBox;
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
* Construction and startup of all components of the section: Position.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Position extends Legend implements SimpleSection {
    private String currentLegend;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bPosition;
    private JLabel lPosition;
    private JSeparator sPosition;
    private static JPanel pPosition;
    private JLabel lLocation;
    private JComboBox cLocation;
    private JPanel pCoordinates;
    private JToggleButton bCoordinates;
    private JLabel lCoordinates;
    private JTextField cCoordinates;
    private JLabel lX;
    private JTextField cX;
    private JLabel lY;
    private JTextField cY;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of legend.
    */
    public Position(String objectID) {
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
        bPosition = new JToggleButton();
        lPosition = new JLabel();
        sPosition = new JSeparator();
        pPosition = new JPanel();

        lLocation = new JLabel();
        cLocation = new JComboBox();
        bCoordinates = new JToggleButton();
        pCoordinates = new JPanel();
        lCoordinates = new JLabel();
        cCoordinates = new JTextField();
        lX = new JLabel();
        cX = new JTextField();
        lY = new JLabel();
        cY = new JTextField();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.position;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pPosition, bPosition, lPosition, sPosition, SECTIONNAME);
        bPosition.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pPosition.setVisible(!bPosition.isSelected());
                HideLegend.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        int ROW = 0;
        int LEFTMARGIN = 0; //to components
        int COLUMN = 1; //first column
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        //Components of the property: Legend Location.
        String[] options = new String[] {"in_upper_right", "in_upper_left",
                                         "in_lower_right", "in_lower_left",
                                         "out_upper_right", "out_upper_left",
                                         "out_lower_right", "out_lower_left",
                                         "upper_caption", "lower_caption",
                                         "by_coordinates"};
        layout.addLabelComboBox(pPosition, lLocation, MessagesGED.location,
                                cLocation, options, LEFTMARGIN, COLUMN, ROW++);
        cLocation.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentLegend, GraphicObjectProperties.__GO_LEGEND_LOCATION__,
                        cLocation.getSelectedIndex());
            }
        });

        //Components of the property: Position (coordinates).
        ROW = layout.addInnerPanel(pPosition, pCoordinates, bCoordinates, lCoordinates, cCoordinates, MessagesGED.coordinates, ROW);
        bCoordinates.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pCoordinates.setVisible(!bCoordinates.isSelected());
                HideLegend.checkAllButtons();
            }
        });
        //Position X
        layout.addLabelTextField(pCoordinates, lX, MessagesGED.x_coordinate,
                                 cX, true, LEFTMARGINIP, COLUMNIP, 0);
        cX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateCoordinates();
            }
        });
        cX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateCoordinates();
            }
        });
        //Position Y
        layout.addLabelTextField(pCoordinates, lY, MessagesGED.y_coordinate,
                                 cY, true, LEFTMARGINIP, COLUMNIP, 1);
        cY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateCoordinates();
            }
        });
        cY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateCoordinates();
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of legend.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentLegend = objectID;

            //Get the current status of the property: Legend Location
            cLocation.setSelectedIndex((Integer) GraphicController.getController()
                                .getProperty(currentLegend, GraphicObjectProperties.__GO_LEGEND_LOCATION__));

            //Get the current status of the property: Position
            getCoordinates();
        }
    }

    /**
    * Inserts the current state of property: Position.
    */
    public final void titleCoordinates() {
        cCoordinates.setText(" [" + cX.getText() + " , " + cY.getText() + "]");
    }

    /**
    * Get status of the property: Position (coordinates).
    */
    private void getCoordinates() {
        Double[] coordinates = (Double []) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_POSITION__);
        cX.setText(coordinates[0].toString());
        cY.setText(coordinates[1].toString());
        titleCoordinates();
    }

    /**
    * Updates the property: Position.
    */
    private void updateCoordinates() {
        try {
            if(cLocation.getSelectedIndex()!=10)
                cLocation.setSelectedIndex(10);
            Double[] value = new Double[2];
            value[0] = Double.parseDouble(cX.getText());
            value[1] = Double.parseDouble(cY.getText());
            GraphicController.getController()
                    .setProperty(currentLegend, GraphicObjectProperties.__GO_POSITION__, value);
            titleCoordinates();
        } catch (NumberFormatException e) {
            getCoordinates();
        }
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pPosition.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pPosition.setVisible(visible);
        bPosition.setSelected(!visible);
    }
}