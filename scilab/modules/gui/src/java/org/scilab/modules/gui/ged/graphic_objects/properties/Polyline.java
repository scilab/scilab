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

import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.datatip.DatatipManagerMode;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.customComponents.JComboBox.MarkStyleRenderer;
import org.scilab.modules.gui.ged.graphic_objects.contouredObject.MarkStyle;

/**
* ContouredObject properties:
* CLOSED, ARROWSIZEFACTOR, POLYLINESTYLE, INTERPCOLORVECTOR, INTERPCOLORVECTORSET,
* INTERPCOLORMODE, BARWIDTH
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Polyline extends ContentLayout {

    /**
     * Components of the property: Arrow Size Factor
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void arrowSizeFactor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lArrowSize = new JLabel();
        final JTextField cArrowSize = new JTextField();
        addLabelTextField(panel, lArrowSize, MessagesGED.arrow_size_factor,
                          cArrowSize, true, LEFTMARGIN, COLUMN, ROW++);
        cArrowSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setArrowSize(cArrowSize, UID);
            }
        });
        cArrowSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setArrowSize(cArrowSize, UID);
            }
        });
        // Get the current status of the property: Arrow Size Factor
        cArrowSize.setText(Double.toString((Double) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__)));
    }

    /**
    * Set Arrow Size.
    * @param cArrowSize JTextField
    * @param UID objectid
    */
    private void setArrowSize(JTextField cArrowSize, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__,
                Double.parseDouble(cArrowSize.getText()));
        } catch (NumberFormatException e) {
            cArrowSize.setText(Double.toString((Double) GraphicController.getController()
                                               .getProperty(UID, GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__)));
        }
    }

    /**
     * Components of the property: Bar Width
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void barWidth(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lBarWidth = new JLabel();
        final JTextField cBarWidth = new JTextField();
        addLabelTextField(panel, lBarWidth, MessagesGED.bar_width,
                          cBarWidth, true, LEFTMARGIN, COLUMN, ROW++);
        cBarWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setBarWidth(cBarWidth, UID);
            }
        });
        cBarWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setBarWidth(cBarWidth, UID);
            }
        });
        // Get the current status of the property: Bar Width
        cBarWidth.setText(Double.toString((Double) GraphicController.getController()
                                          .getProperty(UID, GraphicObjectProperties.__GO_BAR_WIDTH__)));
    }

    /**
    * Set Bar Width.
    * @param width Bar Width.
    */
    private void setBarWidth(JTextField cBarWidth, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_BAR_WIDTH__,
                Double.parseDouble(cBarWidth.getText()));
        } catch (NumberFormatException e) {
            cBarWidth.setText(Double.toString((Double) GraphicController.getController()
                                              .getProperty(UID, GraphicObjectProperties.__GO_BAR_WIDTH__)));
        }
    }

    /**
     * Components of the property: Closed
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void closed(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lClosed = new JLabel();
        final JComboBox cClosed = new JComboBox();
        addLabelComboBox(panel, lClosed, MessagesGED.closed,
                         cClosed, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cClosed.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_CLOSED__,
                    cClosed.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Closed
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_CLOSED__);
        cClosed.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Polyline Style
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void polylineStyle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lPolyline = new JLabel();
        final JComboBox cPolyline = new JComboBox();
        String[] options = new String[] {MessagesGED.interpolated, MessagesGED.staircase, MessagesGED.barplot,
                                         MessagesGED.bar, MessagesGED.arrowed, MessagesGED.filled
                                        };
        addLabelComboBox(panel, lPolyline, MessagesGED.polyline_style,
                         cPolyline, options,
                         LEFTMARGIN, COLUMN, ROW++);
        cPolyline.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_POLYLINE_STYLE__,
                    cPolyline.getSelectedIndex());
            }
        });
    }
}