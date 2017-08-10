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
import javax.swing.JCheckBox;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JToggleButton;
import javax.swing.JTextField;
import javax.swing.JOptionPane;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Fec properties:
* ZBOUNDS, OUTSIDECOLOR, COLORRANGE
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Fec extends ContentLayout {
    private JTextField czBounds;
    private JTextField czBoundsMIN;
    private JTextField czBoundsMAX;

    private JTextField cColorRange;
    private JTextField cColorRangeMIN;
    private JTextField cColorRangeMAX;

    private JTextField cOutsideColor;
    private JTextField cOutsideColorMIN;
    private JTextField cOutsideColorMAX;

    /**
     * Components of the property: Color Range
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void colorRange(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lColorRange = new JLabel();
        final JPanel pColorRange = new JPanel();
        final JToggleButton bColorRange = new JToggleButton();
        JLabel lColorRangeMIN = new JLabel();
        JLabel lColorRangeMAX = new JLabel();
        cColorRange = new JTextField();
        cColorRangeMIN = new JTextField();
        cColorRangeMAX = new JTextField();

        addInnerPanel(panel, pColorRange, bColorRange, lColorRange,
                      cColorRange, MessagesGED.color_range, ROW);
        bColorRange.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pColorRange.setVisible(!bColorRange.isSelected());
            }
        });
        int rowZBounds = 0;
        //Color Range - MIN
        addLabelTextField(pColorRange, lColorRangeMIN, MessagesGED.min,
                          cColorRangeMIN, true, LEFTMARGINIP, COLUMNIP, rowZBounds++);
        cColorRangeMIN.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateColorRange(UID);
            }
        });
        cColorRangeMIN.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateColorRange(UID);
            }
        });
        //Color Range - MAX
        addLabelTextField(pColorRange, lColorRangeMAX, MessagesGED.max,
                          cColorRangeMAX, true, LEFTMARGINIP, COLUMNIP, rowZBounds++);
        cColorRangeMAX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateColorRange(UID);
            }
        });
        cColorRangeMAX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateColorRange(UID);
            }
        });

        //Get the current status of the property: Color Range
        getColorRange(UID);
    }

    /**
    * Get the current status of the property: Color Range.
    * @param UID objectID
    */
    public void getColorRange(Integer UID) {
        Integer[] value = (Integer[]) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_COLOR_RANGE__);
        cColorRangeMIN.setText(value[0].toString());
        cColorRangeMAX.setText(value[1].toString());
        titleColorRange();
    }

    /**
    * Inserts the current state of functionality: Color Range in main label.
    */
    public void titleColorRange() {
        cColorRange.setText(" [" + cColorRangeMIN.getText() + " , "
                            + cColorRangeMAX.getText() + "]");
    }

    /**
    * Updates the property: Color Range.
    */
    private void updateColorRange(Integer UID) {
        try {
            Integer[] value = {Integer.parseInt(cOutsideColorMIN.getText()), Integer.parseInt(cOutsideColorMAX.getText())};
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_COLOR_RANGE__,
                value);
            titleColorRange();
        } catch (NumberFormatException e) {
            getColorRange(UID);
        }
    }

    /**
     * Components of the property: Outside Color
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void outsideColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lOutsideColor = new JLabel();
        final JPanel pOutsideColor = new JPanel();
        final JToggleButton bOutsideColor = new JToggleButton();
        JLabel lOutsideColorMIN = new JLabel();
        JLabel lOutsideColorMAX = new JLabel();
        cOutsideColor = new JTextField();
        cOutsideColorMIN = new JTextField();
        cOutsideColorMAX = new JTextField();

        addInnerPanel(panel, pOutsideColor, bOutsideColor, lOutsideColor,
                      cOutsideColor, MessagesGED.outside_color, ROW);
        bOutsideColor.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pOutsideColor.setVisible(!bOutsideColor.isSelected());
            }
        });
        int rowZBounds = 0;
        //Outside Color - MIN
        addLabelTextField(pOutsideColor, lOutsideColorMIN, MessagesGED.min,
                          cOutsideColorMIN, true, LEFTMARGINIP, COLUMNIP, rowZBounds++);
        cOutsideColorMIN.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateOutsideColor(UID);
            }
        });
        cOutsideColorMIN.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateOutsideColor(UID);
            }
        });
        //Outside Color - MAX
        addLabelTextField(pOutsideColor, lOutsideColorMAX, MessagesGED.max,
                          cOutsideColorMAX, true, LEFTMARGINIP, COLUMNIP, rowZBounds++);
        cOutsideColorMAX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateOutsideColor(UID);
            }
        });
        cOutsideColorMAX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateOutsideColor(UID);
            }
        });

        //Get the current status of the property: Outside Color
        getOutsideColor(UID);
    }

    /**
    * Get the current status of the property: Outside Color.
    * @param UID objectID
    */
    public void getOutsideColor(Integer UID) {
        Integer[] value = (Integer[]) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_OUTSIDE_COLOR__);
        cOutsideColorMIN.setText(value[0].toString());
        cOutsideColorMAX.setText(value[1].toString());
        titleOutsideColor();
    }

    /**
    * Inserts the current state of functionality: Outside Color in main label.
    */
    public void titleOutsideColor() {
        cOutsideColor.setText(" [" + cOutsideColorMIN.getText() + " , "
                              + cOutsideColorMAX.getText() + "]");
    }

    /**
    * Updates the property: Outside Color.
    */
    private void updateOutsideColor(Integer UID) {
        try {
            Integer[] value = {Integer.parseInt(cOutsideColorMIN.getText()), Integer.parseInt(cOutsideColorMAX.getText())};
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_OUTSIDE_COLOR__,
                value);
            titleOutsideColor();
        } catch (NumberFormatException e) {
            getOutsideColor(UID);
        }
    }

    /**
     * Components of the property: Z Bounds
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void zBounds(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lzBounds = new JLabel();
        final JPanel pzBounds = new JPanel();
        final JToggleButton bzBounds = new JToggleButton();
        JLabel lzBoundsMIN = new JLabel();
        JLabel lzBoundsMAX = new JLabel();
        czBounds = new JTextField();
        czBoundsMIN = new JTextField();
        czBoundsMAX = new JTextField();

        addInnerPanel(panel, pzBounds, bzBounds, lzBounds,
                      czBounds, MessagesGED.z_bounds, ROW);
        bzBounds.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pzBounds.setVisible(!bzBounds.isSelected());
            }
        });
        int rowZBounds = 0;
        //Z Bounds - MIN
        addLabelTextField(pzBounds, lzBoundsMIN, MessagesGED.min,
                          czBoundsMIN, true, LEFTMARGINIP, COLUMNIP, rowZBounds++);
        czBoundsMIN.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateZBounds(UID);
            }
        });
        czBoundsMIN.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateZBounds(UID);
            }
        });
        //Z Bounds - MAX
        addLabelTextField(pzBounds, lzBoundsMAX, MessagesGED.max,
                          czBoundsMAX, true, LEFTMARGINIP, COLUMNIP, rowZBounds++);
        czBoundsMAX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateZBounds(UID);
            }
        });
        czBoundsMAX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateZBounds(UID);
            }
        });

        //Get the current status of the property: Z Bounds
        getZBounds(UID);
    }

    /**
    * Get the current status of the property: Z Bounds.
    * @param UID objectID
    */
    public void getZBounds(Integer UID) {
        Double[] value = (Double[]) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_Z_BOUNDS__);
        czBoundsMIN.setText(value[0].toString());
        czBoundsMAX.setText(value[1].toString());
        titleZBounds();
    }

    /**
    * Inserts the current state of functionality: ZBounds in main label.
    */
    public void titleZBounds() {
        czBounds.setText(" [" + czBoundsMIN.getText() + " , "
                         + czBoundsMAX.getText() + "]");
    }

    /**
    * Updates the property: Z Bounds.
    */
    private void updateZBounds(Integer UID) {
        try {
            Double[] value = {Double.parseDouble(czBoundsMIN.getText()), Double.parseDouble(czBoundsMAX.getText())};
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_Z_BOUNDS__,
                value);
            titleZBounds();
        } catch (NumberFormatException e) {
            getZBounds(UID);
        }
    }
}