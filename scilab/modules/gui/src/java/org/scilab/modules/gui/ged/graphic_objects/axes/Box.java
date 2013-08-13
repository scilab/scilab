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

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Box.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Box extends Axes implements SimpleSection {
    private ContentLayout layout = new ContentLayout();
    private String currentAxes;
    private String parentFigure;

    private static JToggleButton bBox;
    private static JPanel pBox;
    private JSeparator sBox;
    private JLabel lBox;
    private JButton bHiddenAxisColor;
    private JLabel cHiddenAxisColor;
    private JLabel lHiddenAxisColor;
    private JPanel pHiddenAxisColor;
    private JComboBox cAutoScale;
    private JTextField cDataBounds;
    private JComboBox cTightLimits;
    private JTextField cZoomBox;
    private JLabel lAutoScale;
    private JLabel lDataBounds;
    private JLabel lTightLimits;
    private JLabel lZoomBox;
    private static JDialog colorDialog;
    private static JColorChooser colorChooser;
    private static JButton okButton;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public Box(String objectID) {
        parentFigure = getFigueID(objectID);
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
        bBox = new JToggleButton();
        lBox = new JLabel();
        sBox = new JSeparator();
        pBox = new JPanel();
        lAutoScale = new JLabel();
        cAutoScale = new JComboBox();
        lDataBounds = new JLabel();
        cDataBounds = new JTextField();
        lHiddenAxisColor = new JLabel();
        pHiddenAxisColor = new JPanel();
        bHiddenAxisColor = new JButton();
        cHiddenAxisColor = new JLabel();
        lTightLimits = new JLabel();
        cTightLimits = new JComboBox();
        lZoomBox = new JLabel();
        cZoomBox = new JTextField();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.box;
        this.setName(SECTIONNAME);
        layout.addHeader(this, pBox, bBox, lBox, sBox, SECTIONNAME);
        bBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pBox.setVisible(!bBox.isSelected());
                HideAxes.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        int ROW = 0;
        int LEFTMARGIN = 16; //to inner components
        int COLUMN = 0; //first column

        //Components of the property: Auto Scale.
        layout.addLabelComboBox(pBox, lAutoScale, MessagesGED.auto_scale,
                                cAutoScale, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cAutoScale.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_AUTO_SCALE__,
                        cAutoScale.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Data Bounds.
        //not implemented yet.

        //Components of the property: Hidden Axis Color.
        dialogHiddenAxisColor();
        layout.addLabelColorField(pBox, lHiddenAxisColor, MessagesGED.hidden_axis_color,
                colorDialog, cHiddenAxisColor, pHiddenAxisColor, bHiddenAxisColor,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Tight Limits.
        layout.addLabelComboBox(pBox, lTightLimits, MessagesGED.tight_limits,
                                cTightLimits, new String[] {MessagesGED.off , MessagesGED.on},
                                LEFTMARGIN, COLUMN, ROW++);
        cTightLimits.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_TIGHT_LIMITS__,
                        cTightLimits.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Zoom Box.
        //not implemented yet.
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of axes.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentAxes = objectID;
            boolean enable;

            // Get the current status of the property: Auto Scale
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_AUTO_SCALE__);
            cAutoScale.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Hidden Axis Color
            Integer scilabColor = (Integer) GraphicController.getController()
                  .getProperty(currentAxes, GraphicObjectProperties.__GO_HIDDEN_AXIS_COLOR__);
            Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
            cHiddenAxisColor.setBackground(new Color(rgbColor[0].intValue(),
                                                     rgbColor[1].intValue(),
                                                     rgbColor[2].intValue()));

            //Get the current status of the property: Tight Limits
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_TIGHT_LIMITS__);
            cTightLimits.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Zoom Box
            //not implemented yet
        }
    }

    /**
    * JDialog - Selection of background colors.
    */
    public final void dialogHiddenAxisColor() {
        colorDialog = new JDialog();
        colorChooser = new JColorChooser();
        okButton = new JButton();
        layout.addColorDialog(colorDialog, colorChooser, okButton, cHiddenAxisColor,
                parentFigure, "axes.Box", "setHiddenAxisColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public final void setHiddenAxisColor(int scilabColor) {
        GraphicController.getController().setProperty(
                currentAxes, GraphicObjectProperties.__GO_HIDDEN_AXIS_COLOR__, scilabColor);
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pBox.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pBox.setVisible(visible);
        bBox.setSelected(!visible);
    }
}