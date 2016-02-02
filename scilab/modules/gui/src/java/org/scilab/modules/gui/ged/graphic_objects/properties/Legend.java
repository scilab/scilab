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
import javax.swing.JToggleButton;
import javax.swing.JTextField;

import org.scilab.modules.gui.editor.EditorManager;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Legend properties: LEGENDLOCATION, POSITION , SIZE
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Legend extends ContentLayout {
    private JComboBox cLocation;
    private JTextField cCoordinates;
    private JTextField cX;
    private JTextField cY;

    /**
     * Components of the property: Legend Location
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void legendLocation(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lLocation = new JLabel();
        cLocation = new JComboBox();
        String[] options = new String[] {"in_upper_right", "in_upper_left",
                                         "in_lower_right", "in_lower_left",
                                         "out_upper_right", "out_upper_left",
                                         "out_lower_right", "out_lower_left",
                                         "upper_caption", "lower_caption",
                                         "by_coordinates"
                                        };
        addLabelComboBox(panel, lLocation, MessagesGED.location,
                         cLocation, options, LEFTMARGIN, COLUMN, ROW++);
        cLocation.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_LEGEND_LOCATION__,
                    cLocation.getSelectedIndex());
            }
        });
        //Get the current status of the property: Legend Location
        cLocation.setSelectedIndex((Integer) GraphicController.getController()
                                   .getProperty(UID, GraphicObjectProperties.__GO_LEGEND_LOCATION__));
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
            if (cLocation.getSelectedIndex() != 10) {
                cLocation.setSelectedIndex(10);
            }
            Double[] value = new Double[2];
            value[0] = Double.parseDouble(cX.getText());
            value[1] = Double.parseDouble(cY.getText());
            GraphicController.getController()
            .setProperty(UID, GraphicObjectProperties.__GO_POSITION__, value);
            titleCoordinates();
        } catch (NumberFormatException e) {
            getCoordinates(UID);
        }
    }

    /**
     * Components of the property: Foreground Color (Line Color).
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void foregroundColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN,
                                final Integer UID, final Integer parentFigure) {
        JLabel lForeground = new JLabel();
        JPanel pForeground = new JPanel();
        JButton bForeground = new JButton();
        JLabel cForeground = new JLabel();
        JDialog dialogForeground = new JDialog();
        JColorChooser chooserForeground = new JColorChooser();
        JButton okForeground = new JButton();

        addColorDialog(dialogForeground, chooserForeground, okForeground, cForeground, UID,
                       parentFigure, "properties.ContouredObject", "setForegroundColor", this);

        addLabelColorField(panel, lForeground, MessagesGED.foreground_color,
                           dialogForeground, cForeground, pForeground, bForeground,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Foreground Color
        Integer scilabForegroundColor;
        if ((Boolean)GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_MARK_MODE__)) {
            scilabForegroundColor = (Integer) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
        } else {
            scilabForegroundColor = (Integer) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_LINE_COLOR__);
        }
        Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
        cForeground.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                            rgbForegroundColor[1].intValue(),
                                            rgbForegroundColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor, Integer UID) {
        if ((Boolean)GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_MARK_MODE__)) {
            GraphicController.getController().setProperty(UID, GraphicObjectProperties.__GO_MARK_FOREGROUND__, scilabColor);
        } else {
            GraphicController.getController().setProperty(UID, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
        }
    }
}
