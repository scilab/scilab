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

import org.scilab.modules.gui.datatip.DatatipManagerMode;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.customComponents.JComboBox.MarkStyleRenderer;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.contouredObject.MarkStyle;

/**
* Construction and startup of all components of the section: Mark Properties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Mark extends Datatip implements SimpleSection {
    private String UID;
    private String parentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bMark;
    private JLabel lMark;
    private JSeparator sMark;
    private static JPanel pMark;
    private JLabel lMarkMode;
    private JComboBox cMarkMode;
    private JLabel lMarkBackground;
    private JPanel pMarkBackground;
    private JButton bMarkBackground;
    private static JLabel cMarkBackground;
    private static JDialog markBackgroundDialog;
    private static JColorChooser chooserMarkBackground;
    private static JButton okMarkBackground;
    private JLabel lMarkForeground;
    private JPanel pMarkForeground;
    private JButton bMarkForeground;
    private static JLabel cMarkForeground;
    private static JDialog markForegroundDialog;
    private static JColorChooser chooserMarkForeground;
    private static JButton okMarkForeground;
    private JLabel lMarkSize;
    private JTextField cMarkSize;
    private JLabel lMarkSizeUnit;
    private JComboBox cMarkSizeUnit;
    private JLabel lMarkStyle;
    private JComboBox cMarkStyle;
    private final MarkStyleRenderer markStyleRenderer = new MarkStyleRenderer();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of datatip.
    */
    public Mark(String objectID) {
        UID = objectID;
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
        bMark = new JToggleButton();
        lMark = new JLabel();
        sMark = new JSeparator();
        pMark = new JPanel();
        lMarkMode = new JLabel();
        cMarkMode = new JComboBox();
        lMarkBackground = new JLabel();
        pMarkBackground = new JPanel();
        bMarkBackground = new JButton();
        cMarkBackground = new JLabel();
        lMarkForeground = new JLabel();
        pMarkForeground = new JPanel();
        bMarkForeground = new JButton();
        cMarkForeground = new JLabel();
        lMarkSize = new JLabel();
        cMarkSize = new JTextField();
        lMarkSizeUnit = new JLabel();
        cMarkSizeUnit = new JComboBox();
        lMarkStyle = new JLabel();
        cMarkStyle = new JComboBox(MarkStyle.values());
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.mark;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pMark, bMark, lMark, sMark, SECTIONNAME);
        bMark.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pMark.setVisible(!bMark.isSelected());
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

        //Components of the property: Mark Mode.
        layout.addLabelComboBox(pMark, lMarkMode, MessagesGED.mark_mode,
                cMarkMode, new String[] {MessagesGED.off , MessagesGED.on},
                LEFTMARGIN, COLUMN, ROW++);
        cMarkMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_MARK_MODE__,
                    cMarkMode.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Mark Background.
        dialogMarkBackground();
        layout.addLabelColorField(pMark, lMarkBackground, MessagesGED.mark_background,
                markBackgroundDialog, cMarkBackground, pMarkBackground, bMarkBackground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Mark Foreground.
        dialogMarkForeground();
        layout.addLabelColorField(pMark, lMarkForeground, MessagesGED.mark_foreground,
                markForegroundDialog, cMarkForeground, pMarkForeground, bMarkForeground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Mark Size.
        layout.addLabelTextField(pMark, lMarkSize, MessagesGED.mark_size,
                                 cMarkSize, true, LEFTMARGIN, COLUMN, ROW++);
        cMarkSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setMarkSize(cMarkSize.getText());
            }
        });
        cMarkSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setMarkSize(cMarkSize.getText());
            }
        });

        //Components of the property: Mark Size Unit.
        layout.addLabelComboBox(pMark, lMarkSizeUnit, MessagesGED.mark_size_unit,
                cMarkSizeUnit, new String[] {MessagesGED.point , MessagesGED.tabulated},
                LEFTMARGIN, COLUMN, ROW++);
        cMarkSizeUnit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__,
                    cMarkSizeUnit.getSelectedIndex());
            }
        });

        //Components of the property: Mark Style.
        layout.addLabelComboBox(pMark, lMarkStyle, MessagesGED.mark_style,
                                 cMarkStyle, null,
                                 LEFTMARGIN, COLUMN, ROW++);
        cMarkStyle.setRenderer(markStyleRenderer);
        cMarkStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                if (cMarkMode.getSelectedIndex() == 0) //If mark_mode is off
                    cMarkMode.setSelectedIndex(1); //turn on

                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_MARK_STYLE__, cMarkStyle.getSelectedIndex());
            }
        });
   }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of datatip.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            UID = objectID;

            // Get the current status of the property: Mark Mode
            boolean enable = (Boolean) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_MARK_MODE__);
            cMarkMode.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Mark Background Color
            Integer scilabMarkBackground = DatatipManagerMode.getInstance().getMarkColor();
            Double[] rgbMarkBackground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkBackground);
            Color markBackgroundCOLOR = new Color(rgbMarkBackground[0].intValue(),
                                            rgbMarkBackground[1].intValue(),
                                            rgbMarkBackground[2].intValue());
            cMarkBackground.setBackground(markBackgroundCOLOR);
            markStyleRenderer.setMarkBackground(markBackgroundCOLOR);

            // Get the current status of the property: Mark Foreground Color
            Integer scilabMarkForeground = (Integer) GraphicController.getController()
                        .getProperty(UID, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
            Double[] rgbMarkForeground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkForeground);
            Color markForegroundCOLOR = new Color(rgbMarkForeground[0].intValue(),
                                            rgbMarkForeground[1].intValue(),
                                            rgbMarkForeground[2].intValue());
            cMarkForeground.setBackground(markForegroundCOLOR);
            markStyleRenderer.setMarkForeground(markForegroundCOLOR);

            // Get the current status of the property: Mark Size
            cMarkSize.setText(Integer.toString((Integer) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_MARK_SIZE__)));

            // Get the current status of the property: Mark Size Unit
            cMarkSizeUnit.setSelectedIndex((Integer) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__));

            // Get the current status of the property: Mark Style
            int currentMarkStyle = (Integer) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_MARK_STYLE__);
            if (cMarkMode.getSelectedIndex() == 1)
                cMarkStyle.setSelectedIndex(currentMarkStyle);
        }
    }

    /**
    * JDialog - Selection of mark background colors.
    */
    public final void dialogMarkBackground() {
        markBackgroundDialog = new JDialog();
        chooserMarkBackground = new JColorChooser();
        okMarkBackground = new JButton();
        layout.addColorDialog(markBackgroundDialog, chooserMarkBackground, okMarkBackground,
                cMarkBackground, parentFigure, "datatip.Mark", "setMarkBackground", this);
    }

    /**
    * JDialog - Selection of mark foreground colors.
    */
    public final void dialogMarkForeground() {
        markForegroundDialog = new JDialog();
        chooserMarkForeground = new JColorChooser();
        okMarkForeground = new JButton();
        layout.addColorDialog(markForegroundDialog, chooserMarkForeground, okMarkForeground,
                cMarkForeground, parentFigure, "datatip.Mark", "setMarkForeground", this);
    }

    /**
    * Set Mark Size.
    * @param size mark size.
    */
    public void setMarkSize(String size) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_MARK_SIZE__,
                Integer.parseInt(size));
        } catch (NumberFormatException e) {
            cMarkSize.setText(Integer.toString((Integer) GraphicController.getController()
                    .getProperty(UID, GraphicObjectProperties.__GO_MARK_SIZE__)));
        }
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setMarkBackground(int scilabColor) {
        DatatipManagerMode.getInstance().setMarkColor(scilabColor);
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkBackground(chooserMarkBackground.getColor());
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setMarkForeground(int scilabColor) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_MARK_FOREGROUND__, scilabColor);
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkForeground(chooserMarkForeground.getColor());
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pMark.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pMark.setVisible(visible);
        bMark.setSelected(!visible);
    }
}