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
package org.scilab.modules.gui.ged.graphic_objects.surface;

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
public class Style extends Surface implements SimpleSection {
    private String currentSurface;
    private String parentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bStyle;
    private JLabel lStyle;
    private JSeparator sStyle;
    private static JPanel pStyle;
    private JLabel lColorMode;
    private JTextField cColorMode;
    private JLabel lColorFlag;
    private JTextField cColorFlag;
    private JLabel lHiddenColor;
    private JPanel pHiddenColor;
    private JButton bHiddenColor;
    private static JLabel cHiddenColor;
    private static JDialog dialogHiddenColor;
    private static JColorChooser chooserHiddenColor;
    private static JButton okHiddenColor;
    private JLabel lForeground;
    private JPanel pForeground;
    private JButton bForeground;
    private static JLabel cForeground;
    private static JDialog dialogForeground;
    private static JColorChooser chooserForeground;
    private static JButton okForeground;
    private JLabel lThickness;
    private JTextField cThickness;

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of surface.
    */
    public Style(String objectID) {
        currentSurface = objectID;
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

        lColorMode = new JLabel();
        cColorMode = new JTextField();
        lColorFlag = new JLabel();
        cColorFlag = new JTextField();
        lHiddenColor = new JLabel();
        pHiddenColor = new JPanel();
        bHiddenColor = new JButton();
        cHiddenColor = new JLabel();
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
                HideSurface.checkAllButtons();
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

        //Components of the property: Color Flag.
        layout.addLabelTextField(pStyle, lColorFlag, MessagesGED.color_flag,
                                 cColorFlag, true, LEFTMARGIN, COLUMN, ROW++);
        cColorFlag.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setColorFlag(cColorFlag.getText());
            }
        });
        cColorFlag.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setColorFlag(cColorFlag.getText());
            }
        });

        //Components of the property: Color Mode.
        layout.addLabelTextField(pStyle, lColorMode, MessagesGED.color_mode,
                                 cColorMode, true, LEFTMARGIN, COLUMN, ROW++);
        cColorMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setColorMode(cColorMode.getText());
            }
        });
        cColorMode.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setColorMode(cColorMode.getText());
            }
        });

        //Components of the property: Hidden Color.
        dialogHiddenColor();
        layout.addLabelColorField(pStyle, lHiddenColor, MessagesGED.hidden_color,
                dialogHiddenColor, cHiddenColor, pHiddenColor, bHiddenColor,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Foreground Color.
        dialogForegroundColor();
        layout.addLabelColorField(pStyle, lForeground, MessagesGED.foreground_color,
                dialogForeground, cForeground, pForeground, bForeground,
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
    * @param objectID Enters the identification of surface.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentSurface = objectID;

            // Get the current status of the property: Color Flag
            cColorFlag.setText(GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_COLOR_FLAG__).toString());

            // Get the current status of the property: Color Mode
            cColorMode.setText(GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_COLOR_MODE__).toString());

            // Get the current status of the property: Hidden Color
            Integer scilabColor = (Integer) GraphicController.getController()
                  .getProperty(currentSurface, GraphicObjectProperties.__GO_HIDDEN_COLOR__);
            Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
            cHiddenColor.setBackground(new Color(rgbColor[0].intValue(),
                                                 rgbColor[1].intValue(),
                                                 rgbColor[2].intValue()));

            // Get the current status of the property: Foreground Color
            Integer scilabForegroundColor;
            if ((Boolean) GraphicController.getController().getProperty(currentSurface, GraphicObjectProperties.__GO_MARK_MODE__)) {
            	scilabForegroundColor = (Integer) GraphicController.getController()
                        .getProperty(currentSurface, GraphicObjectProperties.__GO_LINE_COLOR__);
            } else {
                scilabForegroundColor = EditorManager.getFromUid(parentFigure).getOriColor();
            }
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeground.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                                rgbForegroundColor[1].intValue(),
                                                rgbForegroundColor[2].intValue()));

            // Get the current status of the property: Thickness
            cThickness.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_LINE_THICKNESS__)));
        }
    }

    /**
    * JDialog - Selection of hidden color.
    */
    public final void dialogHiddenColor() {
        dialogHiddenColor = new JDialog();
        chooserHiddenColor = new JColorChooser();
        okHiddenColor = new JButton();
        layout.addColorDialog(dialogHiddenColor, chooserHiddenColor, okHiddenColor, cHiddenColor,
                              parentFigure, "surface.Style", "setHiddenColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public final void setHiddenColor(int scilabColor) {
        GraphicController.getController().setProperty(
                currentSurface, GraphicObjectProperties.__GO_HIDDEN_COLOR__, scilabColor);
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    public final void dialogForegroundColor() {
        dialogForeground = new JDialog();
        chooserForeground = new JColorChooser();
        okForeground = new JButton();
        layout.addColorDialog(dialogForeground, chooserForeground, okForeground, cForeground,
                parentFigure, "surface.Style", "setForegroundColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor) {
        if ((Boolean) GraphicController.getController()
                .getProperty(currentSurface, GraphicObjectProperties.__GO_MARK_MODE__))
        {
            GraphicController.getController().setProperty(
                    currentSurface, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    	} else {
            EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	}
    }

    /**
    * Set Color Flag.
    * @param flag Color flag.
    */
    public void setColorFlag(String flag) {
        try {
            GraphicController.getController()
                    .setProperty(currentSurface, GraphicObjectProperties.__GO_COLOR_FLAG__, Integer.parseInt(flag));
        } catch (NumberFormatException e) {
            cColorFlag.setText(GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_COLOR_FLAG__).toString());
        }
    }

    /**
    * Set Color Mode.
    * @param mode Color Mode.
    */
    public void setColorMode(String mode) {
        try {
            GraphicController.getController()
                    .setProperty(currentSurface, GraphicObjectProperties.__GO_COLOR_MODE__, Integer.parseInt(mode));
        } catch (NumberFormatException e) {
            cColorMode.setText(GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_COLOR_MODE__).toString());
        }
    }

    /**
    * Set Thickness.
    * @param thickness.
    */
    public void setThickness(String thickness) {
        try {
            GraphicController.getController().setProperty(
                    currentSurface,
                    GraphicObjectProperties.__GO_LINE_THICKNESS__,
                    Double.parseDouble(thickness));
        } catch (NumberFormatException e) {
            cThickness.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentSurface, GraphicObjectProperties.__GO_LINE_THICKNESS__)));
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