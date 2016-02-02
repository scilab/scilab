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
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Camera properties:
* ArcDrawingMethod
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Arc extends ContentLayout {
    private JTextField cULP;
    private JTextField cULPx;
    private JTextField cULPy;
    private JTextField cULPz;

    /**
     * Components of the property: Arc Drawing Method
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID
     */
    public void arcDrawingMethod(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lArcDrawingMethod = new JLabel();
        final JComboBox cArcDrawingMethod = new JComboBox();
        addLabelComboBox(panel, lArcDrawingMethod, MessagesGED.arc_drawing_method,
                         cArcDrawingMethod, new String[] {MessagesGED.nurbs , MessagesGED.lines},
                         LEFTMARGIN, COLUMN, ROW++);
        cArcDrawingMethod.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_ARC_DRAWING_METHOD__,
                    cArcDrawingMethod.getSelectedIndex());
            }
        });
        // Get the current status of the property: Arc Drawing Method
        cArcDrawingMethod.setSelectedIndex((Integer) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_ARC_DRAWING_METHOD__));
    }

    /**
     * Components of the property: Upper Left Point
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void upperLeftPoint(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        final JPanel pULP = new JPanel();
        final JToggleButton bULP = new JToggleButton();
        JLabel lULP = new JLabel();
        JLabel lULPx = new JLabel();
        JLabel lULPy = new JLabel();
        JLabel lULPz = new JLabel();
        cULP = new JTextField();
        cULPx = new JTextField();
        cULPy = new JTextField();
        cULPz = new JTextField();

        addInnerPanel(panel, pULP, bULP, lULP, cULP, MessagesGED.upper_left_point, ROW);
        bULP.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pULP.setVisible(!bULP.isSelected());
            }
        });
        int rowULP = 0;
        //ULP - X
        addLabelTextField(pULP, lULPx, "X",
                          cULPx, true, LEFTMARGINIP, COLUMNIP, rowULP++);
        cULPx.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateULP(UID);
            }
        });
        cULPx.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateULP(UID);
            }
        });
        //ULP - Y
        addLabelTextField(pULP, lULPy, "Y",
                          cULPy, true, LEFTMARGINIP, COLUMNIP, rowULP++);
        cULPy.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateULP(UID);
            }
        });
        cULPy.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateULP(UID);
            }
        });
        //ULP - Z
        addLabelTextField(pULP, lULPz, "Z",
                          cULPz, true, LEFTMARGINIP, COLUMNIP, rowULP++);
        cULPz.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateULP(UID);
            }
        });
        cULPz.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateULP(UID);
            }
        });
        getULP(UID);
    }

    /**
    * Inserts the current situation of the upper left point.
    */
    private void titleULP() {
        cULP.setText(" [" + cULPx.getText() + " , " + cULPy.getText()  + " , " + cULPz.getText() + "]");
    }

    /**
    * Get status of the property: upper left point.
    */
    private void getULP(Integer UID) {
        Double[] currentULP = (Double []) GraphicController.getController()
                              .getProperty(UID, GraphicObjectProperties.__GO_UPPER_LEFT_POINT__);
        cULPx.setText(currentULP[0].toString());
        cULPy.setText(currentULP[1].toString());
        cULPz.setText(currentULP[2].toString());
        titleULP();
    }

    /**
    * Updates the property: upper left point.
    */
    private void updateULP(Integer UID) {
        try {
            Double[] value = new Double[3];
            value[0] = Double.parseDouble(cULPx.getText());
            value[1] = Double.parseDouble(cULPy.getText());
            value[2] = Double.parseDouble(cULPz.getText());
            GraphicController.getController()
            .setProperty(UID, GraphicObjectProperties.__GO_UPPER_LEFT_POINT__, value);
            titleULP();
        } catch (NumberFormatException e) {
            getULP(UID);
        }
    }

    /**
     * Components of the property: Width
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void width(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lWidth = new JLabel();
        final JTextField cWidth = new JTextField();

        addLabelTextField(panel, lWidth, MessagesGED.width,
                          cWidth, true, LEFTMARGIN, COLUMN, ROW++);
        cWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setWidth(cWidth, UID);
            }
        });
        cWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setWidth(cWidth, UID);
            }
        });
        // Get the current status of the property: Width
        cWidth.setText(Double.toString((Double) GraphicController.getController()
                                       .getProperty(UID, GraphicObjectProperties.__GO_WIDTH__)));

    }

    /**
    * Set Width
    * @param cWidth JTextField
    * @param UID id
    */
    private void setWidth(JTextField cWidth, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_WIDTH__,
                Double.parseDouble(cWidth.getText()));
        } catch (NumberFormatException e) {
            cWidth.setText(Double.toString((Double) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_WIDTH__)));
        }
    }

    /**
     * Components of the property: Height
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void height(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lHeight = new JLabel();
        final JTextField cHeight = new JTextField();

        addLabelTextField(panel, lHeight, MessagesGED.height,
                          cHeight, true, LEFTMARGIN, COLUMN, ROW++);
        cHeight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setHeight(cHeight, UID);
            }
        });
        cHeight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setHeight(cHeight, UID);
            }
        });
        // Get the current status of the property: Height
        cHeight.setText(Double.toString((Double) GraphicController.getController()
                                        .getProperty(UID, GraphicObjectProperties.__GO_HEIGHT__)));

    }

    /**
    * Set Width
    * @param cHeight JTextField
    * @param UID id
    */
    private void setHeight(JTextField cHeight, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_HEIGHT__,
                Double.parseDouble(cHeight.getText()));
        } catch (NumberFormatException e) {
            cHeight.setText(Double.toString((Double) GraphicController.getController()
                                            .getProperty(UID, GraphicObjectProperties.__GO_HEIGHT__)));
        }
    }

    /**
     * Components of the property: Start Angle
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void startAngle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lStartAngle = new JLabel();
        final JTextField cStartAngle = new JTextField();

        addLabelTextField(panel, lStartAngle, MessagesGED.start_angle,
                          cStartAngle, true, LEFTMARGIN, COLUMN, ROW++);
        cStartAngle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setStartAngle(cStartAngle, UID);
            }
        });
        cStartAngle.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setStartAngle(cStartAngle, UID);
            }
        });
        // Get the current status of the property: Start Angle
        cStartAngle.setText(Double.toString((Double) GraphicController.getController()
                                            .getProperty(UID, GraphicObjectProperties.__GO_START_ANGLE__)));

    }

    /**
    * Set Start Angle
    * @param cStartAngle JTextField
    * @param UID id
    */
    private void setStartAngle(JTextField cStartAngle, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_START_ANGLE__,
                Double.parseDouble(cStartAngle.getText()));
        } catch (NumberFormatException e) {
            cStartAngle.setText(Double.toString((Double) GraphicController.getController()
                                                .getProperty(UID, GraphicObjectProperties.__GO_START_ANGLE__)));
        }
    }

    /**
     * Components of the property: End Angle
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void endAngle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lEndAngle = new JLabel();
        final JTextField cEndAngle = new JTextField();

        addLabelTextField(panel, lEndAngle, MessagesGED.end_angle,
                          cEndAngle, true, LEFTMARGIN, COLUMN, ROW++);
        cEndAngle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setEndAngle(cEndAngle, UID);
            }
        });
        cEndAngle.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setEndAngle(cEndAngle, UID);
            }
        });
        // Get the current status of the property: End Angle
        cEndAngle.setText(Double.toString((Double) GraphicController.getController()
                                          .getProperty(UID, GraphicObjectProperties.__GO_END_ANGLE__)));

    }

    /**
    * Set End Angle
    * @param cEndAngle JTextField
    * @param UID id
    */
    private void setEndAngle(JTextField cEndAngle, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_END_ANGLE__,
                Double.parseDouble(cEndAngle.getText()));
        } catch (NumberFormatException e) {
            cEndAngle.setText(Double.toString((Double) GraphicController.getController()
                                              .getProperty(UID, GraphicObjectProperties.__GO_END_ANGLE__)));
        }
    }
}