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

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.editor.EditorManager;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Style/Appeareance.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends Legend implements SimpleSection {
    private String currentLegend;
    private String parentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bStyle;
    private JLabel lStyle;
    private JSeparator sStyle;
    private static JPanel pStyle;

    private JLabel lBackground;
    private JPanel pBackground;
    private JButton bBackground;
    private static JLabel cBackground;
    private static JDialog backgroundDialog;
    private static JColorChooser chooserBackground;
    private static JButton okBackground;

    private JLabel lForeground;
    private JPanel pForeground;
    private JButton bForeground;
    private static JLabel cForeground;
    private static JDialog foregroundDialog;
    private static JColorChooser chooserForeground;
    private static JButton okForeground;

    private JLabel lThickness;
    private JTextField cThickness;

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of legend.
    */
    public Style(String objectID) {
        currentLegend = objectID;
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
        bStyle = new JToggleButton();
        lStyle = new JLabel();
        sStyle = new JSeparator();
        pStyle = new JPanel();

        lBackground = new JLabel();
        pBackground = new JPanel();
        bBackground = new JButton();
        cBackground = new JLabel();
        lForeground = new JLabel();
        pForeground = new JPanel();
        bForeground = new JButton();
        cForeground = new JLabel();
        lThickness = new JLabel();
        cThickness = new JTextField();
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
        int LEFTMARGIN = 16; //to inner components
        int COLUMN = 0; //first column

        //Components of the property: Background Color.
        dialogBackgroundColor();
        layout.addLabelColorField(pStyle, lBackground, MessagesGED.background_color,
                backgroundDialog, cBackground, pBackground, bBackground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Foreground Color.
        dialogForegroundColor();
        layout.addLabelColorField(pStyle, lForeground, MessagesGED.foreground_color,
                foregroundDialog, cForeground, pForeground, bForeground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Thickness.
        layout.addLabelTextField(pStyle, lThickness, MessagesGED.thickness,
                                 cThickness, true, LEFTMARGIN, COLUMN, ROW++);
        cThickness.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setThickness(cThickness.getText());
            }
        });
        cThickness.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setThickness(cThickness.getText());
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

            // Get the current status of the property: Background Color
            Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                  .getProperty(currentLegend, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
            cBackground.setBackground(new Color(rgbBackgroundColor[0].intValue(),
                                                rgbBackgroundColor[1].intValue(),
                                                rgbBackgroundColor[2].intValue()));

            // Get the current status of the property: Foreground Color
            Integer scilabForegroundColor = EditorManager.getFromUid(parentFigure).getOriColor();
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeground.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                                rgbForegroundColor[1].intValue(),
                                                rgbForegroundColor[2].intValue()));

            // Get the current status of the property: Thickness
            cThickness.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_LINE_THICKNESS__)));
        }
    }

    /**
    * JDialog - Selection of background colors.
    */
    public final void dialogBackgroundColor() {
        backgroundDialog = new JDialog();
        chooserBackground = new JColorChooser();
        okBackground = new JButton();
        layout.addColorDialog(backgroundDialog, chooserBackground, okBackground, cBackground,
                parentFigure, "legend.Style", "setBackgroundColor", this);
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    public final void dialogForegroundColor() {
        foregroundDialog = new JDialog();
        chooserForeground = new JColorChooser();
        okForeground = new JButton();
        layout.addColorDialog(foregroundDialog, chooserForeground, okForeground, cForeground,
                parentFigure, "legend.Style", "setForegroundColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setBackgroundColor(int scilabColor) {
        boolean fillMode = BaseProperties.getInstance().getFillMode(); // Get the current status of the fill_mode
        if(!fillMode) //If fill_mode is off
            BaseProperties.getInstance().setFillMode(!fillMode);

        GraphicController.getController().setProperty(
                currentLegend, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor) {
        EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    }

    /**
    * Set Thickness.
    * @param thickness.
    */
    public void setThickness(String thickness) {
        try {
            boolean lineMode = BaseProperties.getInstance().getLineMode(); // Get the current status of the line_mode
            if(!lineMode) //If line_mode is off
                BaseProperties.getInstance().setLineMode(!lineMode);
            GraphicController.getController().setProperty(
                    currentLegend,
                    GraphicObjectProperties.__GO_LINE_THICKNESS__,
                    Double.parseDouble(thickness));
        } catch (NumberFormatException e) {
            cThickness.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_LINE_THICKNESS__)));
        }
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