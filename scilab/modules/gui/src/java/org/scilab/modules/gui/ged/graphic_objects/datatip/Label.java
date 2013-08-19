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
package org.scilab.modules.gui.ged.graphic_objects.datatip;

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
* Construction and startup of all components of the section: Label.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends Datatip implements SimpleSection {
    private String UID;
    private String parentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bLabel;
    private JLabel lLabel;
    private JSeparator sLabel;
    private static JPanel pLabel;
    private JLabel lBoxMode;
    private JComboBox cBoxMode;

    private JLabel lBackground;
    private JPanel pBackground;
    private JButton bBackground;
    private static JLabel cBackground;
    private static JDialog dialogBackground;
    private static JColorChooser chooserBackground;
    private static JButton okBackground;

    private JLabel lForeground;
    private JPanel pForeground;
    private JButton bForeground;
    private static JLabel cForeground;
    private static JDialog dialogForeground;
    private static JColorChooser chooserForeground;
    private static JButton okForeground;

    private JLabel lFontColor;
    private JPanel pFontColor;
    private JButton bFontColor;
    private static JLabel cFontColor;
    private static JDialog dialogFontColor;
    private static JColorChooser chooserFontColor;
    private static JButton okFontColor;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of datatip.
    */
    public Label(String objectID) {
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
        bLabel = new JToggleButton();
        lLabel = new JLabel();
        sLabel = new JSeparator();
        pLabel = new JPanel();
        lBoxMode = new JLabel();
        cBoxMode = new JComboBox();
        lBackground = new JLabel();
        pBackground = new JPanel();
        bBackground = new JButton();
        cBackground = new JLabel();
        lForeground = new JLabel();
        pForeground = new JPanel();
        bForeground = new JButton();
        cForeground = new JLabel();
        lFontColor = new JLabel();
        pFontColor = new JPanel();
        bFontColor = new JButton();
        cFontColor = new JLabel();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.label;
        this.setName(SECTIONNAME);
        layout.addHeader(this, pLabel, bLabel, lLabel, sLabel, SECTIONNAME);
        bLabel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pLabel.setVisible(!bLabel.isSelected());
                HideDatatip.checkAllButtons();
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

        //Components of the property: Tip Box Mode.
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        layout.addLabelComboBox(pLabel, lBoxMode, MessagesGED.box_mode,
                                cBoxMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cBoxMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_DATATIP_BOX_MODE__,
                    cBoxMode.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Background Color.
        dialogBackgroundColor();
        layout.addLabelColorField(pLabel, lBackground, MessagesGED.background_color,
                dialogBackground, cBackground, pBackground, bBackground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Foreground Color.
        dialogForegroundColor();
        layout.addLabelColorField(pLabel, lForeground, MessagesGED.foreground_color,
                dialogForeground, cForeground, pForeground, bForeground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Font Foreground.
        dialogFontColor();
        layout.addLabelColorField(pLabel, lFontColor, MessagesGED.font_color,
                dialogFontColor, cFontColor, pFontColor, bFontColor,
                LEFTMARGIN, COLUMN, ROW++);
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of datatip.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            UID = objectID;

            // Get the current status of the property: Tip Box Mode
            boolean enable = (Boolean) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_BOX_MODE__);
            cBoxMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Background Color
            Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                  .getProperty(UID, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
            cBackground.setBackground(new Color(rgbBackgroundColor[0].intValue(),
                                                rgbBackgroundColor[1].intValue(),
                                                rgbBackgroundColor[2].intValue()));

            // Get the current status of the property: Foreground Color
            Integer scilabForegroundColor = (Integer) GraphicController.getController()
                  .getProperty(UID, GraphicObjectProperties.__GO_LINE_COLOR__);
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeground.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                                rgbForegroundColor[1].intValue(),
                                                rgbForegroundColor[2].intValue()));

            // Get the current status of the property: Font Foreground
            Integer scilabFontColor = (Integer) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_FONT_COLOR__);
            Double[] rgbFontColor = ColorMapHandler.getRGBcolor(parentFigure, scilabFontColor);
            cFontColor.setBackground(new Color(rgbFontColor[0].intValue(),
                                               rgbFontColor[1].intValue(),
                                               rgbFontColor[2].intValue()));
        }
    }


    /**
    * JDialog - Selection of background colors.
    */
    public final void dialogBackgroundColor() {
        dialogBackground = new JDialog();
        chooserBackground = new JColorChooser();
        okBackground = new JButton();
        layout.addColorDialog(dialogBackground, chooserBackground, okBackground, cBackground,
                parentFigure, "datatip.Label", "setBackgroundColor", this);
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    public final void dialogForegroundColor() {
        dialogForeground = new JDialog();
        chooserForeground = new JColorChooser();
        okForeground = new JButton();
        layout.addColorDialog(dialogForeground, chooserForeground, okForeground, cForeground,
                parentFigure, "datatip.Label", "setForegroundColor", this);
    }

    /**
    * JDialog - Selection of font colors.
    */
    public final void dialogFontColor() {
        dialogFontColor = new JDialog();
        chooserFontColor = new JColorChooser();
        okFontColor = new JButton();
        layout.addColorDialog(dialogFontColor, chooserFontColor, okFontColor, cFontColor,
                parentFigure, "datatip.Label", "setFontColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setBackgroundColor(int scilabColor) {
        GraphicController.getController().setProperty(
                UID, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor) {
        GraphicController.getController().setProperty(
                UID, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public final void setFontColor(int scilabColor) {
        GraphicController.getController().setProperty(
                UID, GraphicObjectProperties.__GO_FONT_COLOR__, scilabColor);
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pLabel.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pLabel.setVisible(visible);
        bLabel.setSelected(!visible);
    }
}