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
* Construction and startup of all components of the section: Position.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Position extends Axes implements SimpleSection {
    private String currentAxes;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bPosition;
    private JLabel lPosition;
    private JSeparator sPosition;
    private static JPanel pPosition;
    private JToggleButton bAxesBounds;
    private JToggleButton bMargins;
    private JTextField cAxesBounds;
    private JTextField cAxesBoundsHeight;
    private JTextField cAxesBoundsWidth;
    private JTextField cAxesBoundsX;
    private JTextField cAxesBoundsY;
    private JPanel cAxesReverse;
    private JCheckBox cAxesReverseX;
    private JCheckBox cAxesReverseY;
    private JCheckBox cAxesReverseZ;
    private JTextField cMargins;
    private JTextField cMarginsBottom;
    private JTextField cMarginsLeft;
    private JTextField cMarginsRight;
    private JTextField cMarginsTop;
    private JLabel lAxesBounds;
    private JLabel lAxesBoundsHeight;
    private JLabel lAxesBoundsWidth;
    private JLabel lAxesBoundsX;
    private JLabel lAxesBoundsY;
    private JLabel lAxesReverse;
    private JLabel lMargins;
    private JLabel lMarginsBottom;
    private JLabel lMarginsLeft;
    private JLabel lMarginsRight;
    private JLabel lMarginsTop;
    private JPanel pAxesBounds;
    private JPanel pMargins;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
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
        bAxesBounds = new JToggleButton();
        lAxesBounds = new JLabel();
        cAxesBounds = new JTextField();
        pAxesBounds = new JPanel();
        lAxesBoundsX = new JLabel();
        cAxesBoundsX = new JTextField();
        lAxesBoundsY = new JLabel();
        cAxesBoundsY = new JTextField();
        lAxesBoundsWidth = new JLabel();
        cAxesBoundsWidth = new JTextField();
        lAxesBoundsHeight = new JLabel();
        cAxesBoundsHeight = new JTextField();
        lAxesReverse = new JLabel();
        cAxesReverse = new JPanel();
        cAxesReverseX = new JCheckBox();
        cAxesReverseY = new JCheckBox();
        cAxesReverseZ = new JCheckBox();
        bMargins = new JToggleButton();
        lMargins = new JLabel();
        cMargins = new JTextField();
        pMargins = new JPanel();
        lMarginsLeft = new JLabel();
        cMarginsLeft = new JTextField();
        lMarginsRight = new JLabel();
        cMarginsRight = new JTextField();
        lMarginsTop = new JLabel();
        cMarginsTop = new JTextField();
        lMarginsBottom = new JLabel();
        cMarginsBottom = new JTextField();
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
        int LEFTMARGIN = 0; //to components
        int COLUMN = 1; //first column
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        //Components of the property: Axes Bounds.
        ROW = layout.addInnerPanel(pPosition, pAxesBounds, bAxesBounds, lAxesBounds,
                cAxesBounds, MessagesGED.axes_bounds, ROW);
        bAxesBounds.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pAxesBounds.setVisible(!bAxesBounds.isSelected());
                HideAxes.checkAllButtons();
            }
        });
        int rowAxesBounds = 0;
        //Axes Bounds - X Left
        layout.addLabelTextField(pAxesBounds, lAxesBoundsX, MessagesGED.x_left,
                                 cAxesBoundsX, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds();
            }
        });
        cAxesBoundsX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds();
            }
        });
        //Axes Bounds - Y Up
        layout.addLabelTextField(pAxesBounds, lAxesBoundsY, MessagesGED.y_up,
                                 cAxesBoundsY, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds();
            }
        });
        cAxesBoundsY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds();
            }
        });
        //Axes Bounds - Height
        layout.addLabelTextField(pAxesBounds, lAxesBoundsHeight, MessagesGED.height,
                                 cAxesBoundsHeight, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsHeight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds();
            }
        });
        cAxesBoundsHeight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds();
            }
        });
        //Axes Bounds - Width
        layout.addLabelTextField(pAxesBounds, lAxesBoundsWidth, MessagesGED.width,
                                 cAxesBoundsWidth, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds();
            }
        });
        cAxesBoundsWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds();
            }
        });

        //Components of the property: Axes Reverse.
        layout.addLabel3CheckBox(pPosition, lAxesReverse, MessagesGED.axes_reverse,
                cAxesReverse, cAxesReverseX, cAxesReverseY, cAxesReverseZ, LEFTMARGIN, COLUMN, ROW++);
        cAxesReverseX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_X_AXIS_REVERSE__,
                        cAxesReverseX.isSelected());
            }
        });
        cAxesReverseY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__,
                        cAxesReverseY.isSelected());
            }
        });
        cAxesReverseZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__,
                        cAxesReverseZ.isSelected());
            }
        });

        //Components of the property: Margins.
        ROW = layout.addInnerPanel(pPosition, pMargins, bMargins, lMargins,
                cMargins, MessagesGED.margins, ROW);
        bMargins.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pMargins.setVisible(!bMargins.isSelected());
                HideAxes.checkAllButtons();
            }
        });
        int rowAxesMargins = 0;
        //Margins - Left
        layout.addLabelTextField(pMargins, lMarginsLeft, MessagesGED.left,
                                 cMarginsLeft, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsLeft.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins();
            }
        });
        cMarginsLeft.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins();
            }
        });
        //Margins - Right
        layout.addLabelTextField(pMargins, lMarginsRight, MessagesGED.right,
                                 cMarginsRight, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsRight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins();
            }
        });
        cMarginsRight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins();
            }
        });
        //Margins - Top
        layout.addLabelTextField(pMargins, lMarginsTop, MessagesGED.top,
                                 cMarginsTop, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsTop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins();
            }
        });
        cMarginsTop.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins();
            }
        });
        //Margins - Bottom
        layout.addLabelTextField(pMargins, lMarginsBottom, MessagesGED.bottom,
                                 cMarginsBottom, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsBottom.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins();
            }
        });
        cMarginsBottom.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins();
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of polyline.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentAxes = objectID;

            //Get the current status of the property: Axes Bounds
            Double[] currentAxesBounds = (Double []) GraphicController.getController()
                    .getProperty(currentAxes, GraphicObjectProperties.__GO_AXES_BOUNDS__);
            cAxesBoundsX.setText(currentAxesBounds[0].toString());
            cAxesBoundsY.setText(currentAxesBounds[1].toString());
            cAxesBoundsWidth.setText(currentAxesBounds[2].toString());
            cAxesBoundsHeight.setText(currentAxesBounds[3].toString());
            titleAxesBounds();

            // Get the current status of the property: Axes Reverse
            cAxesReverseX.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_X_AXIS_REVERSE__));
            cAxesReverseY.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__));
            cAxesReverseZ.setSelected((Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__));

            //Get the current status of the property: Margins
            Double[] currentMargins = (Double []) GraphicController.getController()
                    .getProperty(currentAxes, GraphicObjectProperties.__GO_MARGINS__);
            cMarginsLeft.setText(currentMargins[0].toString());
            cMarginsRight.setText(currentMargins[1].toString());
            cMarginsTop.setText(currentMargins[2].toString());
            cMarginsBottom.setText(currentMargins[3].toString());
            titleMargins();
        }
    }

    /**
    * Inserts the current state of functionality: AXES BOUNDS in main label.
    */
    public void titleAxesBounds() {
        cAxesBounds.setText(" [" + cAxesBoundsX.getText() + " , " + cAxesBoundsY.getText() + " , "
                                 + cAxesBoundsWidth.getText() + " , " + cAxesBoundsHeight.getText() + "]");
    }

    /**
    * Inserts the current state of functionality: MARGINS in main label.
    */
    public void titleMargins() {
        cMargins.setText(" [" + cMarginsLeft.getText() + " , " + cMarginsRight.getText() + " , "
                             + cMarginsTop.getText() + " , " + cMarginsBottom.getText() + "]");
    }

    /**
    * Updates the property: Axes Bounds.
    */
    private void updateAxesBounds() {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cAxesBoundsX.getText());
        value[1] = Double.parseDouble(cAxesBoundsY.getText());
        value[2] = Double.parseDouble(cAxesBoundsWidth.getText());
        value[3] = Double.parseDouble(cAxesBoundsHeight.getText());
        GraphicController.getController()
                .setProperty(currentAxes, GraphicObjectProperties.__GO_AXES_BOUNDS__, value);
        titleAxesBounds();
    }

    /**
    * Updates the property: Margins.
    */
    private void updateMargins() {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cMarginsLeft.getText());
        value[1] = Double.parseDouble(cMarginsRight.getText());
        value[2] = Double.parseDouble(cMarginsTop.getText());
        value[3] = Double.parseDouble(cMarginsBottom.getText());
        GraphicController.getController()
                .setProperty(currentAxes, GraphicObjectProperties.__GO_MARGINS__, value);
        titleMargins();
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