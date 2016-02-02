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
* Axes properties:
* AXISVISIBLE, AXISREVERSE, AXISGRIDCOLOR, AXISLABEL, AXISLOCATION, AXISLOGFLAG,
* AXISTICKS, AXISAUTOTICKS, AXISNUMBERTICKS, AXISTICKSLOCATIONS, AXISTICKSLABELS, AXISSUBTICKS,
* AUTOSUBTICKS,
* GRIDPOSITION, TITLE, AUTOCLEAR, FILLED, BACKGROUND,
* MARGINS, AXESBOUNDS,HIDDENCOLOR
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Axes extends ContentLayout {
    private JTextField cAxesBounds;
    private JTextField cAxesBoundsHeight;
    private JTextField cAxesBoundsWidth;
    private JTextField cAxesBoundsX;
    private JTextField cAxesBoundsY;
    private JTextField cMargins;
    private JTextField cMarginsBottom;
    private JTextField cMarginsLeft;
    private JTextField cMarginsRight;
    private JTextField cMarginsTop;
    private JTextField cXLabel;
    private JTextField cYLabel;
    private JTextField cZLabel;
    private JTextField cSubTicks;
    private JTextField cSubTicksX;
    private JTextField cSubTicksY;

    /**
     * Components of the property: Auto Clear
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void autoClear(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAutoClear = new JLabel();
        final JComboBox cAutoClear = new JComboBox();
        addLabelComboBox(panel, lAutoClear, MessagesGED.auto_clear,
                         cAutoClear, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cAutoClear.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_AUTO_CLEAR__,
                    cAutoClear.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Auto Clear
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_AUTO_CLEAR__);
        cAutoClear.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Auto Scale
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void autoScale(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAutoScale = new JLabel();
        final JComboBox cAutoScale = new JComboBox();

        addLabelComboBox(panel, lAutoScale, MessagesGED.auto_scale,
                         cAutoScale, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cAutoScale.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_AUTO_SCALE__,
                    cAutoScale.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Auto Scale
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_AUTO_SCALE__);
        cAutoScale.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Auto Ticks
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void autoTicks(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAutoTicks = new JLabel();
        JPanel cAutoTicks = new JPanel();
        final JCheckBox cAutoTicksX = new JCheckBox();
        final JCheckBox cAutoTicksY = new JCheckBox();
        final JCheckBox cAutoTicksZ = new JCheckBox();
        addLabel3CheckBox(panel, lAutoTicks, MessagesGED.auto_ticks,
                          cAutoTicks, cAutoTicksX, cAutoTicksY, cAutoTicksZ, LEFTMARGIN, COLUMN, ROW++);
        cAutoTicksX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_X_AXIS_AUTO_TICKS__,
                    cAutoTicksX.isSelected());
            }
        });
        cAutoTicksY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Y_AXIS_AUTO_TICKS__,
                    cAutoTicksY.isSelected());
            }
        });
        cAutoTicksZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Z_AXIS_AUTO_TICKS__,
                    cAutoTicksZ.isSelected());
            }
        });
        // Get the current status of the property: Auto Ticks
        cAutoTicksX.setSelected((Boolean) GraphicController.getController()
                                .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_AUTO_TICKS__));
        cAutoTicksY.setSelected((Boolean) GraphicController.getController()
                                .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_AUTO_TICKS__));
        cAutoTicksZ.setSelected((Boolean) GraphicController.getController()
                                .getProperty(UID, GraphicObjectProperties.__GO_Z_AXIS_AUTO_TICKS__));
    }

    /**
     * Components of the property: Axes Bounds
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void axesBounds(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lAxesBounds = new JLabel();
        final JPanel pAxesBounds = new JPanel();
        final JToggleButton bAxesBounds = new JToggleButton();
        final JComboBox cAutoScale = new JComboBox();
        JLabel lAxesBoundsX = new JLabel();
        JLabel lAxesBoundsY = new JLabel();
        JLabel lAxesBoundsHeight = new JLabel();
        JLabel lAxesBoundsWidth = new JLabel();
        cAxesBounds = new JTextField();
        cAxesBoundsHeight = new JTextField();
        cAxesBoundsWidth = new JTextField();
        cAxesBoundsX = new JTextField();
        cAxesBoundsY = new JTextField();

        addInnerPanel(panel, pAxesBounds, bAxesBounds, lAxesBounds,
                      cAxesBounds, MessagesGED.axes_bounds, ROW);
        bAxesBounds.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pAxesBounds.setVisible(!bAxesBounds.isSelected());
            }
        });
        int rowAxesBounds = 0;
        //Axes Bounds - X Left
        addLabelTextField(pAxesBounds, lAxesBoundsX, MessagesGED.x_left,
                          cAxesBoundsX, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds(UID);
            }
        });
        cAxesBoundsX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds(UID);
            }
        });
        //Axes Bounds - Y Up
        addLabelTextField(pAxesBounds, lAxesBoundsY, MessagesGED.y_up,
                          cAxesBoundsY, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds(UID);
            }
        });
        cAxesBoundsY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds(UID);
            }
        });
        //Axes Bounds - Height
        addLabelTextField(pAxesBounds, lAxesBoundsHeight, MessagesGED.height,
                          cAxesBoundsHeight, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsHeight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds(UID);
            }
        });
        cAxesBoundsHeight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds(UID);
            }
        });
        //Axes Bounds - Width
        addLabelTextField(pAxesBounds, lAxesBoundsWidth, MessagesGED.width,
                          cAxesBoundsWidth, true, LEFTMARGINIP, COLUMNIP, rowAxesBounds++);
        cAxesBoundsWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesBounds(UID);
            }
        });
        cAxesBoundsWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesBounds(UID);
            }
        });
        //Get the current status of the property: Axes Bounds
        Double[] currentAxesBounds = (Double []) GraphicController.getController()
                                     .getProperty(UID, GraphicObjectProperties.__GO_AXES_BOUNDS__);
        cAxesBoundsX.setText(currentAxesBounds[0].toString());
        cAxesBoundsY.setText(currentAxesBounds[1].toString());
        cAxesBoundsWidth.setText(currentAxesBounds[2].toString());
        cAxesBoundsHeight.setText(currentAxesBounds[3].toString());
        titleAxesBounds();
    }

    /**
    * Inserts the current state of functionality: AXES BOUNDS in main label.
    */
    public void titleAxesBounds() {
        cAxesBounds.setText(" [" + cAxesBoundsX.getText() + " , " + cAxesBoundsY.getText() + " , "
                            + cAxesBoundsWidth.getText() + " , " + cAxesBoundsHeight.getText() + "]");
    }

    /**
    * Updates the property: Axes Bounds.
    */
    private void updateAxesBounds(Integer UID) {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cAxesBoundsX.getText());
        value[1] = Double.parseDouble(cAxesBoundsY.getText());
        value[2] = Double.parseDouble(cAxesBoundsWidth.getText());
        value[3] = Double.parseDouble(cAxesBoundsHeight.getText());
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_AXES_BOUNDS__, value);
        titleAxesBounds();
    }

    /**
     * Components of the property: Axes Visible
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void axesVisible(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAxesVisible = new JLabel();
        JPanel cAxesVisible = new JPanel();
        final JCheckBox cAxesVisibleX = new JCheckBox();
        final JCheckBox cAxesVisibleY = new JCheckBox();
        final JCheckBox cAxesVisibleZ = new JCheckBox();
        addLabel3CheckBox(panel, lAxesVisible, MessagesGED.axes_visible,
                          cAxesVisible, cAxesVisibleX, cAxesVisibleY, cAxesVisibleZ, LEFTMARGIN, COLUMN, ROW++);
        cAxesVisibleX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__,
                    cAxesVisibleX.isSelected());
            }
        });
        cAxesVisibleY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__,
                    cAxesVisibleY.isSelected());
            }
        });
        cAxesVisibleZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__,
                    cAxesVisibleZ.isSelected());
            }
        });
        // Get the current status of the property: Axes Visible
        cAxesVisibleX.setSelected((Boolean) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__));
        cAxesVisibleY.setSelected((Boolean) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__));
        cAxesVisibleZ.setSelected((Boolean) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__));
    }

    /**
     * Components of the property: Axes Reverse
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void axesReverse(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAxesReverse = new JLabel();
        final JPanel cAxesReverse = new JPanel();
        final JCheckBox cAxesReverseX = new JCheckBox();
        final JCheckBox cAxesReverseY = new JCheckBox();
        final JCheckBox cAxesReverseZ = new JCheckBox();

        addLabel3CheckBox(panel, lAxesReverse, MessagesGED.axes_reverse,
                          cAxesReverse, cAxesReverseX, cAxesReverseY, cAxesReverseZ, LEFTMARGIN, COLUMN, ROW++);
        cAxesReverseX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_X_AXIS_REVERSE__,
                    cAxesReverseX.isSelected());
            }
        });
        cAxesReverseY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__,
                    cAxesReverseY.isSelected());
            }
        });
        cAxesReverseZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__,
                    cAxesReverseZ.isSelected());
            }
        });

        // Get the current status of the property: Axes Reverse
        cAxesReverseX.setSelected((Boolean) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_REVERSE__));
        cAxesReverseY.setSelected((Boolean) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__));
        cAxesReverseZ.setSelected((Boolean) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__));
    }

    /**
     * Components of the property: Axis Location
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void axisLocation(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        //x_location
        JLabel lLocationX = new JLabel();
        final JComboBox cLocationX = new JComboBox();
        addLabelComboBox(panel, lLocationX, MessagesGED.x_location, cLocationX,
                         new String[] {"bottom", "top", "middle", "origin"},
                         LEFTMARGIN, COLUMN, ROW++);
        cLocationX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_X_AXIS_LOCATION__,
                    cLocationX.getSelectedIndex());
            }
        });
        //y_location
        JLabel lLocationY = new JLabel();
        final JComboBox cLocationY = new JComboBox();
        addLabelComboBox(panel, lLocationY, MessagesGED.y_location, cLocationY,
                         new String[] {"middle", "origin", "left", "right"},
                         LEFTMARGIN, COLUMN, ROW++);
        cLocationY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__,
                    cLocationY.getSelectedIndex() + 2);
            }
        });

        // Get the current status of the property: Axis Location
        cLocationX.setSelectedIndex((Integer) GraphicController.getController()
                                    .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_LOCATION__));

        cLocationY.setSelectedIndex((Integer) GraphicController.getController()
                                    .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__) - 2);
    }

    /**
     * Components of the property: Box Type
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void box(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lBox = new JLabel();
        final JComboBox cBox = new JComboBox();

        addLabelComboBox(panel, lBox, MessagesGED.box,
                         cBox, new String[] {MessagesGED.off, MessagesGED.on,
                                             MessagesGED.hidden_axes, MessagesGED.back_half
                                            },
                         LEFTMARGIN, COLUMN, ROW++);
        cBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_BOX_TYPE__,
                    cBox.getSelectedIndex());
            }
        });
        //Get the current status of the property: Box Type
        cBox.setSelectedIndex((Integer) GraphicController.getController()
                              .getProperty(UID, GraphicObjectProperties.__GO_BOX_TYPE__));
    }

    /**
     * Components of the property: Filled
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void filled(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFilled = new JLabel();
        final JComboBox cFilled = new JComboBox();
        addLabelComboBox(panel, lFilled, MessagesGED.filled,
                         cFilled, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cFilled.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_FILLED__,
                    cFilled.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Filled
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_FILLED__);
        cFilled.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Grid Position
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void gridPosition(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lGridPosition = new JLabel();
        final JComboBox cGridPosition = new JComboBox();

        addLabelComboBox(panel, lGridPosition, MessagesGED.grid_position,
                         cGridPosition, new String[] {MessagesGED.background, MessagesGED.foreground},
                         LEFTMARGIN, COLUMN, ROW++);
        cGridPosition.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_GRID_POSITION__,
                    cGridPosition.getSelectedIndex());
            }
        });
        //Get the current status of the property: Grid Position
        cGridPosition.setSelectedIndex((Integer) GraphicController.getController()
                                       .getProperty(UID, GraphicObjectProperties.__GO_GRID_POSITION__));
    }

    /**
     * Components of the property: X Grid Color
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     * @param parentFigure
     */
    public void gridColorX(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lgridColor = new JLabel();
        JPanel pgridColor = new JPanel();
        JButton bgridColor = new JButton();
        JLabel cgridColor = new JLabel();
        JDialog dialog = new JDialog();
        JColorChooser chooser = new JColorChooser();
        JButton okButton = new JButton();

        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        addColorDialog(dialog, chooser, okButton, cgridColor, UID,
                       parentFigure, "properties.Axes", "setGridColorX", this);

        addLabelColorField(panel, lgridColor, MessagesGED.x_grid_color,
                           dialog, cgridColor, pgridColor, bgridColor,
                           LEFTMARGIN, COLUMN, ROW++);

        // Get the current status of the property: X Axis Grid Color
        Integer scilabColor = (Integer) GraphicController.getController()
                              .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_GRID_COLOR__);
        Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
        cgridColor.setBackground(new Color(rgbColor[0].intValue(),
                                           rgbColor[1].intValue(),
                                           rgbColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setGridColorX(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_X_AXIS_GRID_COLOR__, scilabColor);
    }

    /**
     * Components of the property: Y Grid Color
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void gridColorY(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lgridColor = new JLabel();
        JPanel pgridColor = new JPanel();
        JButton bgridColor = new JButton();
        JLabel cgridColor = new JLabel();
        JDialog dialog = new JDialog();
        JColorChooser chooser = new JColorChooser();
        JButton okButton = new JButton();

        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        addColorDialog(dialog, chooser, okButton, cgridColor, UID,
                       parentFigure, "properties.Axes", "setGridColorY", this);

        addLabelColorField(panel, lgridColor, MessagesGED.y_grid_color,
                           dialog, cgridColor, pgridColor, bgridColor,
                           LEFTMARGIN, COLUMN, ROW++);

        // Get the current status of the property: Y Axis Grid Color
        Integer scilabColor = (Integer) GraphicController.getController()
                              .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_GRID_COLOR__);
        Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
        cgridColor.setBackground(new Color(rgbColor[0].intValue(),
                                           rgbColor[1].intValue(),
                                           rgbColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setGridColorY(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_Y_AXIS_GRID_COLOR__, scilabColor);
    }

    /**
     * Components of the property: Hidden Axis Color
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     * @param parentFigure
     */
    public void hiddenAxisColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lHiddenAxisColor = new JLabel();
        JPanel pHiddenAxisColor = new JPanel();
        JButton bHiddenAxisColor = new JButton();
        JLabel cHiddenAxisColor = new JLabel();
        JDialog colorDialog = new JDialog();
        JColorChooser colorChooser = new JColorChooser();
        JButton okButton = new JButton();

        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        addColorDialog(colorDialog, colorChooser, okButton, cHiddenAxisColor, UID,
                       parentFigure, "properties.ContouredObject", "setHiddenAxisColor", this);

        addLabelColorField(panel, lHiddenAxisColor, MessagesGED.hidden_axis_color,
                           colorDialog, cHiddenAxisColor, pHiddenAxisColor, bHiddenAxisColor,
                           LEFTMARGIN, COLUMN, ROW++);

        // Get the current status of the property: Hidden Axis Color
        Integer scilabColor = (Integer) GraphicController.getController()
                              .getProperty(UID, GraphicObjectProperties.__GO_HIDDEN_AXIS_COLOR__);
        Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
        cHiddenAxisColor.setBackground(new Color(rgbColor[0].intValue(),
                                       rgbColor[1].intValue(),
                                       rgbColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setHiddenAxisColor(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_HIDDEN_AXIS_COLOR__, scilabColor);
    }

    /**
     * Used to check if the bounds are strictly positive
     * to use Log Scale
     *
     * @param UID the axes UID
     * @return boolean[3] (x, y, z), true if the bounds are positive false otherwise
     */
    private boolean[] checkBounds(Integer UID) {
        Double[] bounds = (Double[])GraphicController.getController().getProperty(
                              UID, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        boolean[] flag = new boolean[3];
        flag[0] = (bounds[0] > 0. && bounds[1] > 0.);
        flag[1] = (bounds[2] > 0. && bounds[3] > 0.);
        flag[2] = (bounds[4] > 0. && bounds[5] > 0.);
        return flag;
    }

    /**
     * Components of the property: Log Flags
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void logFlags(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lLogFlags = new JLabel();
        JPanel cLogFlags = new JPanel();
        final JCheckBox cLogFlagsX = new JCheckBox();
        final JCheckBox cLogFlagsY = new JCheckBox();
        final JCheckBox cLogFlagsZ = new JCheckBox();
        addLabel3CheckBox(panel, lLogFlags, MessagesGED.log_flags,
                          cLogFlags, cLogFlagsX, cLogFlagsY, cLogFlagsZ, LEFTMARGIN, COLUMN, ROW++);
        cLogFlagsX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                boolean[] check = checkBounds(UID);
                if (check[0]) {
                    GraphicController.getController().setProperty(
                        UID, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__,
                        cLogFlagsX.isSelected());
                } else {
                    JOptionPane.showConfirmDialog(null, MessagesGED.warning_logscale, MessagesGED.warning, JOptionPane.DEFAULT_OPTION);
                    cLogFlagsX.setSelected(false);
                }
            }
        });
        cLogFlagsY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                boolean[] check = checkBounds(UID);
                if (check[1]) {
                    GraphicController.getController().setProperty(
                        UID, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__,
                        cLogFlagsY.isSelected());
                } else {
                    JOptionPane.showConfirmDialog(null, MessagesGED.warning_logscale, MessagesGED.warning, JOptionPane.DEFAULT_OPTION);
                    cLogFlagsY.setSelected(false);
                }
            }
        });
        cLogFlagsZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                boolean[] check = checkBounds(UID);
                if (check[2]) {
                    GraphicController.getController().setProperty(
                        UID, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__,
                        cLogFlagsZ.isSelected());
                } else {
                    JOptionPane.showConfirmDialog(null, MessagesGED.warning_logscale, MessagesGED.warning, JOptionPane.DEFAULT_OPTION);
                    cLogFlagsZ.setSelected(false);
                }
            }
        });
        // Get the current status of the property: Log Flags
        cLogFlagsX.setSelected((Boolean) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__));
        cLogFlagsY.setSelected((Boolean) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__));
        cLogFlagsZ.setSelected((Boolean) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__));
    }

    /**
     * Components of the property: Label
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void label(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lXLabel = new JLabel();
        JLabel lYLabel = new JLabel();
        JLabel lZLabel = new JLabel();
        cXLabel = new JTextField();
        cYLabel = new JTextField();
        cZLabel = new JTextField();

        //Components of the property: X Label
        addLabelTextField(panel, lXLabel, MessagesGED.x_label,
                          cXLabel, true, LEFTMARGIN, COLUMN, ROW++);
        cXLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateLabel(0, UID);
            }
        });
        cXLabel.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateLabel(0, UID);
            }
        });

        //Components of the property: Y Label
        addLabelTextField(panel, lYLabel, MessagesGED.y_label,
                          cYLabel, true, LEFTMARGIN, COLUMN, ROW++);
        cYLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateLabel(1, UID);
            }
        });
        cYLabel.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateLabel(1, UID);
            }
        });

        //Components of the property: Z Label
        addLabelTextField(panel, lZLabel, MessagesGED.z_label,
                          cZLabel, true, LEFTMARGIN, COLUMN, ROW++);
        cZLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateLabel(2, UID);
            }
        });
        cZLabel.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateLabel(2, UID);
            }
        });

        // Get the current status of the property: X Label
        Integer labelID = (Integer) GraphicController.getController()
                          .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
        String[] label = (String[]) GraphicController.getController()
                         .getProperty(labelID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        cXLabel.setText(label[0]);

        // Get the current status of the property: Y Label
        labelID = (Integer) GraphicController.getController()
                  .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
        label = (String[]) GraphicController.getController()
                .getProperty(labelID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        cYLabel.setText(label[0]);

        // Get the current status of the property: Z Label
        labelID = (Integer) GraphicController.getController()
                  .getProperty(UID, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
        label = (String[]) GraphicController.getController()
                .getProperty(labelID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        cZLabel.setText(label[0]);
    }

    /**
    * Updates the property: Label.
    * @param axis x=0  y=1  z=2
    */
    private void updateLabel(int axis, Integer UID) {
        String[] text = new String[1];
        Integer axisLabel = null;
        switch (axis) {
            case 0:
                axisLabel = (Integer) GraphicController.getController()
                            .getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                text[0] = cXLabel.getText();
                break;
            case 1:
                axisLabel = (Integer) GraphicController.getController()
                            .getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                text[0] = cYLabel.getText();
                break;
            case 2:
                axisLabel = (Integer) GraphicController.getController()
                            .getProperty(UID, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                text[0] = cZLabel.getText();
                break;
        }
        GraphicController.getController()
        .setProperty(axisLabel, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
    }

    /**
     * Components of the property: Margins
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void margins(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lMargins = new JLabel();
        final JPanel pMargins = new JPanel();
        final JToggleButton bMargins = new JToggleButton();
        JLabel lMarginsLeft = new JLabel();
        JLabel lMarginsRight = new JLabel();
        JLabel lMarginsTop = new JLabel();
        JLabel lMarginsBottom = new JLabel();
        cMargins = new JTextField();
        cMarginsLeft = new JTextField();
        cMarginsRight = new JTextField();
        cMarginsTop = new JTextField();
        cMarginsBottom = new JTextField();

        addInnerPanel(panel, pMargins, bMargins, lMargins,
                      cMargins, MessagesGED.margins, ROW);
        bMargins.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pMargins.setVisible(!bMargins.isSelected());
            }
        });
        int rowAxesMargins = 0;
        //Margins - Left
        addLabelTextField(pMargins, lMarginsLeft, MessagesGED.left,
                          cMarginsLeft, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsLeft.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins(UID);
            }
        });
        cMarginsLeft.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins(UID);
            }
        });
        //Margins - Right
        addLabelTextField(pMargins, lMarginsRight, MessagesGED.right,
                          cMarginsRight, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsRight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins(UID);
            }
        });
        cMarginsRight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins(UID);
            }
        });
        //Margins - Top
        addLabelTextField(pMargins, lMarginsTop, MessagesGED.top,
                          cMarginsTop, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsTop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins(UID);
            }
        });
        cMarginsTop.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins(UID);
            }
        });
        //Margins - Bottom
        addLabelTextField(pMargins, lMarginsBottom, MessagesGED.bottom,
                          cMarginsBottom, true, LEFTMARGINIP, COLUMNIP, rowAxesMargins++);
        cMarginsBottom.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateMargins(UID);
            }
        });
        cMarginsBottom.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateMargins(UID);
            }
        });
        //Get the current status of the property: Margins
        Double[] currentMargins = (Double []) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_MARGINS__);
        cMarginsLeft.setText(currentMargins[0].toString());
        cMarginsRight.setText(currentMargins[1].toString());
        cMarginsTop.setText(currentMargins[2].toString());
        cMarginsBottom.setText(currentMargins[3].toString());
        titleMargins();
    }

    /**
    * Inserts the current state of functionality: MARGINS in main label.
    */
    public void titleMargins() {
        cMargins.setText(" [" + cMarginsLeft.getText() + " , " + cMarginsRight.getText() + " , "
                         + cMarginsTop.getText() + " , " + cMarginsBottom.getText() + "]");
    }

    /**
    * Updates the property: Margins.
    */
    private void updateMargins(Integer UID) {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cMarginsLeft.getText());
        value[1] = Double.parseDouble(cMarginsRight.getText());
        value[2] = Double.parseDouble(cMarginsTop.getText());
        value[3] = Double.parseDouble(cMarginsBottom.getText());
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_MARGINS__, value);
        titleMargins();
    }

    /**
     * Components of the property: Sub Ticks
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void subTicks(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lSubTicks = new JLabel();
        final JPanel pSubTicks = new JPanel();
        final JToggleButton bSubTicks = new JToggleButton();
        JLabel lSubTicksX = new JLabel();
        JLabel lSubTicksY = new JLabel();
        cSubTicks = new JTextField();
        cSubTicksX = new JTextField();
        cSubTicksY = new JTextField();

        addInnerPanel(panel, pSubTicks, bSubTicks, lSubTicks,
                      cSubTicks, MessagesGED.sub_ticks, ROW);
        bSubTicks.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pSubTicks.setVisible(!bSubTicks.isSelected());
            }
        });
        int rowSubTicks = 0;
        //SubTicks - X
        addLabelTextField(pSubTicks, lSubTicksX, "X",
                          cSubTicksX, true, LEFTMARGINIP, COLUMNIP, rowSubTicks++);
        cSubTicksX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateSubTicks(UID);
            }
        });
        cSubTicksX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateSubTicks(UID);
            }
        });
        //SubTicks - Y
        addLabelTextField(pSubTicks, lSubTicksY, "Y",
                          cSubTicksY, true, LEFTMARGINIP, COLUMNIP, rowSubTicks++);
        cSubTicksY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateSubTicks(UID);
            }
        });
        cSubTicksY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateSubTicks(UID);
            }
        });
        //Get the current status of the property: Sub Ticks
        getSubTicks(UID);
    }

    /**
    * Get the current status of the property: Sub Ticks.
    * @param UID objectID
    */
    public void getSubTicks(Integer UID) {
        Integer x = (Integer) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_X_AXIS_SUBTICKS__);
        Integer y = (Integer) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_Y_AXIS_SUBTICKS__);
        cSubTicksX.setText(x.toString());
        cSubTicksY.setText(y.toString());
        titleSubTicks();
    }

    /**
    * Inserts the current state of functionality: SUBTICKS in main label.
    */
    public void titleSubTicks() {
        cSubTicks.setText(" [" + cSubTicksX.getText() + " , "
                          + cSubTicksY.getText() + "]");
    }

    /**
    * Updates the property: SubTicks.
    */
    private void updateSubTicks(Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_X_AXIS_SUBTICKS__,
                Integer.parseInt(cSubTicksX.getText()));

            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_Y_AXIS_SUBTICKS__,
                Integer.parseInt(cSubTicksY.getText()));
        } catch (NumberFormatException e) {
            getSubTicks(UID);
        }
        titleSubTicks();
    }

    /**
     * Components of the property: Tight Limits
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void tightLimits(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        boolean enable = false;

        /* X axes */
        JLabel lXTightLimits = new JLabel();
        final JComboBox cXTightLimits = new JComboBox();

        addLabelComboBox(panel, lXTightLimits, MessagesGED.x_tight_limits,
                         cXTightLimits, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cXTightLimits.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_X_TIGHT_LIMITS__,
                    cXTightLimits.getSelectedIndex() == 0 ? false : true);
            }
        });
        //Get the current status of the property: Tight Limits
        enable = (Boolean) GraphicController.getController()
                 .getProperty(UID, GraphicObjectProperties.__GO_X_TIGHT_LIMITS__);
        cXTightLimits.setSelectedIndex(enable ? 1 : 0);

        /* Y axes */
        JLabel lYTightLimits = new JLabel();
        final JComboBox cYTightLimits = new JComboBox();

        addLabelComboBox(panel, lYTightLimits, MessagesGED.y_tight_limits,
                         cYTightLimits, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cYTightLimits.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Y_TIGHT_LIMITS__,
                    cYTightLimits.getSelectedIndex() == 0 ? false : true);
            }
        });
        //Get the current status of the property: Tight Limits
        enable = (Boolean) GraphicController.getController()
                 .getProperty(UID, GraphicObjectProperties.__GO_Y_TIGHT_LIMITS__);
        cYTightLimits.setSelectedIndex(enable ? 1 : 0);

        /* Z axes */
        JLabel lZTightLimits = new JLabel();
        final JComboBox cZTightLimits = new JComboBox();

        addLabelComboBox(panel, lZTightLimits, MessagesGED.z_tight_limits,
                         cZTightLimits, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cZTightLimits.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_Z_TIGHT_LIMITS__,
                    cZTightLimits.getSelectedIndex() == 0 ? false : true);
            }
        });
        //Get the current status of the property: Tight Limits
        enable = (Boolean) GraphicController.getController()
                 .getProperty(UID, GraphicObjectProperties.__GO_Z_TIGHT_LIMITS__);
        cZTightLimits.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Title
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID.
     */
    public void title(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lTitle = new JLabel();
        final JTextField cTitle = new JTextField();

        addLabelTextField(panel, lTitle, MessagesGED.title,
                          cTitle, true, LEFTMARGIN, COLUMN, ROW++);
        cTitle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTitle(UID, cTitle);
            }
        });
        cTitle.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTitle(UID, cTitle);
            }
        });
        // Get the current status of the property: Title
        Integer titleID = (Integer) GraphicController.getController()
                          .getProperty(UID, GraphicObjectProperties.__GO_TITLE__);
        String[] title = (String[]) GraphicController.getController()
                         .getProperty(titleID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        cTitle.setText(title[0]);
    }

    /**
    * Updates the property: Title.
    */
    private void updateTitle(Integer UID, JTextField cTitle) {
        Integer titleID = (Integer) GraphicController.getController()
                          .getProperty(UID, GraphicObjectProperties.__GO_TITLE__);
        String[] text = new String[] {cTitle.getText()};
        GraphicController.getController()
        .setProperty(titleID, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
    }
}