/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.properties;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Label properties: FONTANGLE, AUTOPOSITION, POSITION, AUTOROTATION
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends ContentLayout {
    private JTextField cCoordinates;
    private JTextField cX;
    private JTextField cY;

    /**
     * Components of the property: Auto Position
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void autoPosition(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAutoPosition = new JLabel();
        final JComboBox cAutoPosition = new JComboBox();
        addLabelComboBox(panel, lAutoPosition, MessagesGED.auto_position,
                         cAutoPosition, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cAutoPosition.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_AUTO_POSITION__,
                    cAutoPosition.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Auto Position
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_AUTO_POSITION__);
        cAutoPosition.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Auto Rotation
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void autoRotation(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAutoRotation = new JLabel();
        final JComboBox cAutoRotation = new JComboBox();
        addLabelComboBox(panel, lAutoRotation, MessagesGED.auto_rotation,
                         cAutoRotation, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cAutoRotation.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_AUTO_ROTATION__,
                    cAutoRotation.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Auto Rotation
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_AUTO_ROTATION__);
        cAutoRotation.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Font Anlge
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void fontAngle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFontAngle = new JLabel();
        final JTextField cFontAngle = new JTextField();
        addLabelTextField(panel, lFontAngle, MessagesGED.font_angle,
                          cFontAngle, true, LEFTMARGIN, COLUMN, ROW++);
        cFontAngle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setFontAngle(cFontAngle, UID);
            }
        });
        cFontAngle.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setFontAngle(cFontAngle, UID);
            }
        });

        //Get the current status of the property: Font Angle.
        cFontAngle.setText(Double.toString((Double) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_FONT_ANGLE__)));
    }

    /**
    * Set Font Angle.
    * @param cFontAngle JTextField.
    * @param UID id.
    */
    private void setFontAngle(JTextField cFontAngle, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_FONT_ANGLE__,
                Double.parseDouble(cFontAngle.getText()));
        } catch (NumberFormatException e) {
            cFontAngle.setText(Double.toString((Double) GraphicController.getController()
                                               .getProperty(UID, GraphicObjectProperties.__GO_FONT_ANGLE__)));
        }
    }

    /**
     * Components of the property: Position (coordinates)
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void position(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        JLabel lCoordinates = new JLabel();
        cCoordinates = new JTextField();
        final JPanel pCoordinates = new JPanel();
        final JToggleButton bCoordinates = new JToggleButton();
        JLabel lX = new JLabel();
        cX = new JTextField();
        JLabel lY = new JLabel();
        cY = new JTextField();

        addInnerPanel(panel, pCoordinates, bCoordinates, lCoordinates, cCoordinates, MessagesGED.coordinates, ROW);
        bCoordinates.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pCoordinates.setVisible(!bCoordinates.isSelected());
            }
        });
        //Position X
        addLabelTextField(pCoordinates, lX, MessagesGED.x_coordinate,
                          cX, true, LEFTMARGINIP, COLUMNIP, 0);
        cX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateCoordinates(UID);
            }
        });
        cX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateCoordinates(UID);
            }
        });
        //Position Y
        addLabelTextField(pCoordinates, lY, MessagesGED.y_coordinate,
                          cY, true, LEFTMARGINIP, COLUMNIP, 1);
        cY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateCoordinates(UID);
            }
        });
        cY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateCoordinates(UID);
            }
        });
        //Get the current status of the property: Position
        getCoordinates(UID);
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
    private void getCoordinates(Integer UID) {
        Double[] coordinates = (Double []) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_POSITION__);
        cX.setText(coordinates[0].toString());
        cY.setText(coordinates[1].toString());
        titleCoordinates();
    }

    /**
    * Updates the property: Position.
    */
    private void updateCoordinates(Integer UID) {
        try {
            Double[] value = new Double[3];
            value[0] = Double.parseDouble(cX.getText());
            value[1] = Double.parseDouble(cY.getText());
            value[2] = 0.0;
            GraphicController.getController()
            .setProperty(UID, GraphicObjectProperties.__GO_POSITION__, value);
            titleCoordinates();
        } catch (NumberFormatException e) {
            getCoordinates(UID);
        }
    }

    /**
     * Components of the property: Text Strings
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void text(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lText = new JLabel();
        final JTextField cText = new JTextField();
        addLabelTextField(panel, lText, MessagesGED.text,
                          cText, true, LEFTMARGIN, COLUMN, ROW++);
        cText.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                String[] text = new String[] {cText.getText()};
                GraphicController.getController().setProperty(UID, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
            }
        });
        cText.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                String[] text = new String[] {cText.getText()};
                GraphicController.getController().setProperty(UID, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
            }
        });
        // Get the current status of the property: Text
        String[] text = (String[]) GraphicController.getController()
                        .getProperty(UID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        cText.setText(text[0]);
    }
}