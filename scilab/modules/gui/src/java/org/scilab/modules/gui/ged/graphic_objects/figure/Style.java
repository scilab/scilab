/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.figure;

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
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Style/Appeareance.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends Figure implements SimpleSection {
    private String currentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bStyle;
    private JLabel lStyle;
    private static JPanel pStyle;
    private JSeparator sStyle;
    private JButton bBackColor;
    private JLabel lBackColor;
    private JLabel cBackColor;
    private JPanel pBackColor;
    private JLabel lColorMap;
    private JComboBox cColorMap;
    private JLabel lPixelDrawingMode;
    private JComboBox cPixelDrawingMode;
    private JLabel lRotationStyle;
    private JComboBox cRotationStyle;
    private static JDialog backcolorDialog;
    private static JColorChooser chooserBack;
    private static JButton okBack;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of figure.
    */
    public Style(String objectID) {
        constructComponents();
        initMainPanel();
        initComponents();
        loadProperties(objectID);
        dialogBackgroundColor();
    }

    /**
    * Construct the Components.
    */
    @Override
    public final void constructComponents() {
        bStyle = new JToggleButton();
        lStyle = new JLabel();
        sStyle = new JSeparator();
        pStyle = new JPanel();
        lBackColor = new JLabel();
        pBackColor = new JPanel();
        bBackColor = new JButton();
        cBackColor = new JLabel();
        lColorMap = new JLabel();
        cColorMap = new JComboBox();
        lPixelDrawingMode = new JLabel();
        cPixelDrawingMode = new JComboBox();
        lRotationStyle = new JLabel();
        cRotationStyle = new JComboBox();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.style_appearance;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pStyle, bStyle, lStyle, sStyle, SECTIONNAME);
        bStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pStyle.setVisible(!bStyle.isSelected());
                HideFigure.checkAllButtons();
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

        //Components of the property: Background Color.
        layout.addLabelColorField(pStyle, lBackColor, MessagesGED.background_color,
                backcolorDialog, cBackColor, pBackColor, bBackColor,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Color Map.
        //not implemented yet

        //Components of the property: Pixel Drawing Mode.
        String[] options = new String[] {"clear", "and", "andReverse", "copy", "andInverted", "noop",
            "xor", "or", "nor", "equiv", "invert", "orReverse", "copyInverted", "orInverted", "nand", "set"};
        layout.addLabelComboBox(pStyle, lPixelDrawingMode, MessagesGED.pixel_drawing_mode,
                                 cPixelDrawingMode, options,
                                 LEFTMARGIN, COLUMN, ROW++);
        cPixelDrawingMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentFigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__,
                    cPixelDrawingMode.getSelectedIndex());
            }
        });

        //Components of the property: Rotation Style.
        layout.addLabelComboBox(pStyle, lRotationStyle, MessagesGED.rotation_style,
                                 cRotationStyle, new String[] {MessagesGED.unary, MessagesGED.multiple},
                                 LEFTMARGIN, COLUMN, ROW++);
        cRotationStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentFigure, GraphicObjectProperties.__GO_ROTATION_TYPE__,
                    cRotationStyle.getSelectedIndex());
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of Figure.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentFigure = objectID;

            //Get the current status of the property: Background Color.
            Integer scilabColor = (Integer) GraphicController.getController()
                  .getProperty(currentFigure, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbColor = ColorMapHandler.getRGBcolor(currentFigure, scilabColor);
            cBackColor.setBackground(new Color(rgbColor[0].intValue(), rgbColor[1].intValue(), rgbColor[2].intValue()));

            //Get the current status of the property: Color Map.
            //not implemented yet

            //Get the current status of the property: Pixel Drawing Mode.
            cPixelDrawingMode.setSelectedIndex((Integer) GraphicController.getController()
                    .getProperty(currentFigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__));

            //Get the current status of the property: Rotation Style
            cRotationStyle.setSelectedIndex((Integer) GraphicController.getController()
                    .getProperty(currentFigure, GraphicObjectProperties.__GO_ROTATION_TYPE__));
        }
    }

    /**
    * JDialog - Selection of background colors.
    */
    public final void dialogBackgroundColor() {
        backcolorDialog = new JDialog();
        chooserBack = new JColorChooser();
        okBack = new JButton();
        layout.addColorDialog(backcolorDialog, chooserBack, okBack, cBackColor,
                currentFigure, "figure.Style", "setBackgroundColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public final void setBackgroundColor(int scilabColor) {
        GraphicController.getController().setProperty(
                currentFigure, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pStyle.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pStyle.setVisible(visible);
        bStyle.setSelected(!visible);
    }
}