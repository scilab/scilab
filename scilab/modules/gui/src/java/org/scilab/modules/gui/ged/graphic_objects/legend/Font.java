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
* Construction and startup of all components of the section: Font.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Font extends Legend implements SimpleSection {
    private String currentLegend;
    private String parentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bFont;
    private JLabel lFont;
    private static JPanel pFont;
    private JSeparator sFont;
    private JLabel lFontSize;
    private JTextField cFontSize;
    private JLabel lFontStyle;
    private JComboBox cFontStyle;
    private JLabel lFractionalFont;
    private JComboBox cFractionalFont;

    private JButton bFontColor;
    private JLabel lFontColor;
    private JLabel cFontColor;
    private JPanel pFontColor;
    private static JDialog dialog;
    private static JColorChooser chooser;
    private static JButton okButton;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of legend.
    */
    public Font(String objectID) {
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
        bFont = new JToggleButton();
        lFont = new JLabel();
        sFont = new JSeparator();
        pFont = new JPanel();
        lFontColor = new JLabel();
        pFontColor = new JPanel();
        bFontColor = new JButton();
        cFontColor = new JLabel();
        lFontSize = new JLabel();
        cFontSize = new JTextField();
        lFontStyle = new JLabel();
        cFontStyle = new JComboBox();
        lFractionalFont = new JLabel();
        cFractionalFont = new JComboBox();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.font;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pFont, bFont, lFont, sFont, SECTIONNAME);
        bFont.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pFont.setVisible(!bFont.isSelected());
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

        //Components of the property: Font Color.
        dialogFontColor();
        layout.addLabelColorField(pFont, lFontColor, MessagesGED.font_color,
                dialog, cFontColor, pFontColor, bFontColor,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Font Size.
        layout.addLabelTextField(pFont, lFontSize, MessagesGED.font_size,
                                 cFontSize, true, LEFTMARGIN, COLUMN, ROW++);
        cFontSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setFontSize(cFontSize.getText());
            }
        });
        cFontSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setFontSize(cFontSize.getText());
            }
        });

        //Components of the property: Font Style.
        String[] options = new String[] { "Monospaced", "Symbol", "Serif", "Serif Italic", "Serif Bold",
                                          "Serif Bold Italic", "SansSerif", "SansSerif Italic",
                                          "SansSerif Bold", "SansSerif Bold Italic"};
        layout.addLabelComboBox(pFont, lFontStyle, MessagesGED.font_style,
                                 cFontStyle, options,
                                 LEFTMARGIN, COLUMN, ROW++);
        cFontStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentLegend, GraphicObjectProperties.__GO_FONT_STYLE__,
                    cFontStyle.getSelectedIndex());
            }
        });

        //Components of the property: Fractional Font.
        layout.addLabelComboBox(pFont, lFractionalFont, MessagesGED.fractional_font,
                                cFractionalFont, new String[] {MessagesGED.off , MessagesGED.on},
                                LEFTMARGIN, COLUMN, ROW++);
        cFractionalFont.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentLegend, GraphicObjectProperties.__GO_FONT_FRACTIONAL__,
                        cFractionalFont.getSelectedIndex() == 0 ? false : true);
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

            //Get the current status of the property: Font Color.
            Integer scilabColor = (Integer) GraphicController.getController()
                  .getProperty(currentLegend, GraphicObjectProperties.__GO_FONT_COLOR__);
            Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
            cFontColor.setBackground(new Color(rgbColor[0].intValue(),
                                               rgbColor[1].intValue(),
                                               rgbColor[2].intValue()));

            //Get the current status of the property: Font Size.
            cFontSize.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_FONT_SIZE__)));

            //Get the current status of the property: Font Style.
            cFontStyle.setSelectedIndex((Integer) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_FONT_STYLE__));

            // Get the current status of the property: Fractional Font.
            boolean enable = (Boolean) GraphicController.getController()
                                .getProperty(currentLegend, GraphicObjectProperties.__GO_FONT_FRACTIONAL__);
            cFractionalFont.setSelectedIndex(enable?1:0);
        }
    }

    /**
    * JDialog - Selection of font colors.
    */
    public final void dialogFontColor() {
        dialog = new JDialog();
        chooser = new JColorChooser();
        okButton = new JButton();
        layout.addColorDialog(dialog, chooser, okButton, cFontColor,
                parentFigure, "legend.Font", "setFontColor", this);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public final void setFontColor(int scilabColor) {
        GraphicController.getController().setProperty(
                currentLegend, GraphicObjectProperties.__GO_FONT_COLOR__, scilabColor);
    }

    /**
    * Set Font Size.
    * @param size Font Size.
    */
    private void setFontSize(String size) {
        try {
            GraphicController.getController().setProperty(
                    currentLegend,
                    GraphicObjectProperties.__GO_FONT_SIZE__,
                    Double.parseDouble(size));
        } catch (NumberFormatException e) {
            cFontSize.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentLegend, GraphicObjectProperties.__GO_FONT_SIZE__)));
        }
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pFont.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pFont.setVisible(visible);
        bFont.setSelected(!visible);
    }
}