/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.polyline;

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

import org.scilab.modules.gui.editor.EditorManager;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.customComponents.JComboBox.MarkStyleRenderer;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.contouredObject.MarkStyle;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends Polyline implements SimpleSection {
    private String currentPolyline;
    private String parentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bStyle;
    private JLabel lStyle;
    private JSeparator sStyle;
    private static JPanel pStyle;
    private JLabel lArrowSize;
    private JTextField cArrowSize;
    private JLabel lBarWidth;
    private JTextField cBarWidth;
    private JLabel lBackColor;
    private JPanel pBackColor;
    private JButton bBackColor;
    private static JLabel cBackColor;
    private static JDialog backcolorDialog;
    private static JColorChooser chooserBack;
    private static JButton okBack;
    private JLabel lForeColor;
    private JPanel pForeColor;
    private JButton bForeColor;
    private static JLabel cForeColor;
    private static JDialog forecolorDialog;
    private static JColorChooser chooserFore;
    private static JButton okFore;
    private JLabel lLine;
    private JComboBox cLine;
    private JLabel lPolyline;
    private JComboBox cPolyline;
    private JLabel lThickness;
    private JTextField cThickness;
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
    private final MarkStyleRenderer markStyleRenderer = new MarkStyleRenderer();
    private JLabel lMarkSize;
    private JTextField cMarkSize;
    private JLabel lMarkStyle;
    private JComboBox cMarkStyle;

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of polyline.
    */
    public Style(String objectID) {
        currentPolyline = objectID;
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

        lArrowSize = new JLabel();
        cArrowSize = new JTextField();
        lBarWidth = new JLabel();
        cBarWidth = new JTextField();
        lBackColor = new JLabel();
        pBackColor = new JPanel();
        bBackColor = new JButton();
        cBackColor = new JLabel();
        lForeColor = new JLabel();
        pForeColor = new JPanel();
        bForeColor = new JButton();
        cForeColor = new JLabel();
        lLine = new JLabel();
        cLine = new JComboBox();
        lPolyline = new JLabel();
        cPolyline = new JComboBox();
        lThickness = new JLabel();
        cThickness = new JTextField();
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
        lMarkStyle = new JLabel();
        cMarkStyle = new JComboBox(MarkStyle.values());
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
                HidePolyline.checkAllButtons();
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

        //Components of the property: Arrow Size Factor.
        layout.addLabelTextField(pStyle, lArrowSize, MessagesGED.arrow_size_factor,
                                 cArrowSize, true, LEFTMARGIN, COLUMN, ROW++);
        cArrowSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setArrowSize(cArrowSize.getText());
            }
        });
        cArrowSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setArrowSize(cArrowSize.getText());
            }
        });

        //Components of the property: Bar Width.
        layout.addLabelTextField(pStyle, lBarWidth, MessagesGED.bar_width,
                                 cBarWidth, true, LEFTMARGIN, COLUMN, ROW++);
        cBarWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setBarWidth(cBarWidth.getText());
            }
        });
        cBarWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setBarWidth(cBarWidth.getText());
            }
        });

        //Components of the property: Background Color.
        dialogBackgroundColor();
        layout.addLabelColorField(pStyle, lBackColor, MessagesGED.background_color,
                backcolorDialog, cBackColor, pBackColor, bBackColor,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Foreground Color.
        dialogForegroundColor();
        layout.addLabelColorField(pStyle, lForeColor, MessagesGED.foreground_color,
                forecolorDialog, cForeColor, pForeColor, bForeColor,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Line Style.
        String[] options = new String[] {MessagesGED.dash, MessagesGED.dash_dot, MessagesGED.longdash_dot,
            MessagesGED.bigdash_dot, MessagesGED.bigdash_longdash, MessagesGED.dot, MessagesGED.double_dot};
        layout.addLabelComboBox(pStyle, lLine, MessagesGED.line_style,
                                 cLine, options,
                                 LEFTMARGIN, COLUMN, ROW++);
        cLine.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentPolyline, GraphicObjectProperties.__GO_LINE_STYLE__,
                    cLine.getSelectedIndex() + 1);
            }
        });

        //Components of the property: Polyline Style.
        options = new String[] {MessagesGED.interpolated, MessagesGED.staircase, MessagesGED.barplot,
                    MessagesGED.bar, MessagesGED.arrowed, MessagesGED.filled};
        layout.addLabelComboBox(pStyle, lPolyline, MessagesGED.polyline_style,
                                 cPolyline, options,
                                 LEFTMARGIN, COLUMN, ROW++);
        cPolyline.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    currentPolyline, GraphicObjectProperties.__GO_POLYLINE_STYLE__,
                    cPolyline.getSelectedIndex());
            }
        });

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

        //Components of the property: Mark Background.
        dialogMarkBackground();
        layout.addLabelColorField(pStyle, lMarkBackground, MessagesGED.mark_background,
                markBackgroundDialog, cMarkBackground, pMarkBackground, bMarkBackground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Mark Foreground.
        dialogMarkForeground();
        layout.addLabelColorField(pStyle, lMarkForeground, MessagesGED.mark_foreground,
                markForegroundDialog, cMarkForeground, pMarkForeground, bMarkForeground,
                LEFTMARGIN, COLUMN, ROW++);

        //Components of the property: Mark Size.
        layout.addLabelTextField(pStyle, lMarkSize, MessagesGED.mark_size,
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

        //Components of the property: Mark Style.
        layout.addLabelComboBox(pStyle, lMarkStyle, MessagesGED.mark_style,
                                 cMarkStyle, null,
                                 LEFTMARGIN, COLUMN, ROW++);
        cMarkStyle.setRenderer(markStyleRenderer);
        cMarkStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                boolean markMode = BaseProperties.getInstance().getMarkMode(); // Get the current status of the mark_mode
                if(!markMode) //If mark_mode is off
                    BaseProperties.getInstance().setMarkMode(!markMode);
                GraphicController.getController().setProperty(
                    currentPolyline, GraphicObjectProperties.__GO_MARK_STYLE__, cMarkStyle.getSelectedIndex());
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
            currentPolyline = objectID;

            // Get the current status of the property: Arrow Size Factor
            cArrowSize.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__)));

            // Get the current status of the property: Bar Width
            cBarWidth.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_BAR_WIDTH__)));

            // Get the current status of the property: Background Color
            Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                  .getProperty(currentPolyline, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
            cBackColor.setBackground(new Color(rgbBackgroundColor[0].intValue(),
                                               rgbBackgroundColor[1].intValue(),
                                               rgbBackgroundColor[2].intValue()));

            // Get the current status of the property: Foreground Color
            Integer scilabForegroundColor;
            if ((Boolean) GraphicController.getController().getProperty(currentPolyline, GraphicObjectProperties.__GO_MARK_MODE__)) {
            	scilabForegroundColor = (Integer) GraphicController.getController()
                        .getProperty(currentPolyline, GraphicObjectProperties.__GO_LINE_COLOR__);
            } else {
                scilabForegroundColor = EditorManager.getFromUid(parentFigure).getOriColor();
            }
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeColor.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                               rgbForegroundColor[1].intValue(),
                                               rgbForegroundColor[2].intValue()));

            // Get the current status of the property: Line Style
            int currentLineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_LINE_STYLE__);
            cLine.setSelectedIndex(currentLineStyle - 1);

            // Get the current status of the property: Polyline Style
            int currentPolylineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            cPolyline.setSelectedIndex(currentPolylineStyle);

            // Get the current status of the property: Thickness
            cThickness.setText(Double.toString((Double) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_LINE_THICKNESS__)));

            // Get the current status of the property: Mark Background Color
            Integer scilabMarkBackground = (Integer) GraphicController.getController()
                  .getProperty(currentPolyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
            Double[] rgbMarkBackground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkBackground);
            Color markBackgroundCOLOR = new Color(rgbMarkBackground[0].intValue(),
                                            rgbMarkBackground[1].intValue(),
                                            rgbMarkBackground[2].intValue());
            cMarkBackground.setBackground(markBackgroundCOLOR);
            markStyleRenderer.setMarkBackground(markBackgroundCOLOR);

            // Get the current status of the property: Mark Foreground Color
            Integer scilabMarkForeground;
            if ((Boolean) GraphicController.getController()
            .getProperty(currentPolyline, GraphicObjectProperties.__GO_MARK_MODE__)) {
    		scilabMarkForeground = EditorManager.getFromUid(parentFigure).getOriColor();
            } else {
            	scilabMarkForeground = (Integer) GraphicController.getController()
                        .getProperty(currentPolyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
            }
            Double[] rgbMarkForeground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkForeground);
            Color markForegroundCOLOR = new Color(rgbMarkForeground[0].intValue(),
                                            rgbMarkForeground[1].intValue(),
                                            rgbMarkForeground[2].intValue());
            cMarkForeground.setBackground(markForegroundCOLOR);
            markStyleRenderer.setMarkForeground(markForegroundCOLOR);

            // Get the current status of the property: Mark Size
            cMarkSize.setText(Integer.toString((Integer) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_MARK_SIZE__)));

            // Get the current status of the property: Mark Style
            int currentMarkStyle = (Integer) GraphicController.getController()
                    .getProperty(currentPolyline, GraphicObjectProperties.__GO_MARK_STYLE__);
            if(BaseProperties.getInstance().getMarkMode())
                cMarkStyle.setSelectedIndex(currentMarkStyle);
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
                parentFigure, "polyline.Style", "setBackgroungColor", this);
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    public final void dialogForegroundColor() {
        forecolorDialog = new JDialog();
        chooserFore = new JColorChooser();
        okFore = new JButton();
        layout.addColorDialog(forecolorDialog, chooserFore, okFore, cForeColor,
                parentFigure, "polyline.Style", "setForegroundColor", this);
    }

    /**
    * JDialog - Selection of mark background colors.
    */
    public final void dialogMarkBackground() {
        markBackgroundDialog = new JDialog();
        chooserMarkBackground = new JColorChooser();
        okMarkBackground = new JButton();
        layout.addColorDialog(markBackgroundDialog, chooserMarkBackground, okMarkBackground,
                cMarkBackground, parentFigure, "polyline.Style", "setMarkBackground", this);
    }

    /**
    * JDialog - Selection of mark foreground colors.
    */
    public final void dialogMarkForeground() {
        markForegroundDialog = new JDialog();
        chooserMarkForeground = new JColorChooser();
        okMarkForeground = new JButton();
        layout.addColorDialog(markForegroundDialog, chooserMarkForeground, okMarkForeground,
                cMarkForeground, parentFigure, "polyline.Style", "setMarkForeground", this);
    }

    /**
    * Set Arrow Size.
    * @param size Arrow Size.
    */
    public void setArrowSize(String size) {
        try {
            GraphicController.getController().setProperty(
                    currentPolyline,
                    GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__,
                    Double.parseDouble(size));
        } catch (NumberFormatException e) {
            cArrowSize.setText("1.0");
        }
    }

    /**
    * Set Bar Width.
    * @param width Bar Width.
    */
    public void setBarWidth(String width) {
        try {
            GraphicController.getController().setProperty(
                    currentPolyline,
                    GraphicObjectProperties.__GO_BAR_WIDTH__,
                    Double.parseDouble(width));
        } catch (NumberFormatException e) {
            cBarWidth.setText("0.0");
        }
    }

    /**
    * Set Thickness.
    * @param thickness.
    */
    public void setThickness(String thickness) {
        try {
            GraphicController.getController().setProperty(
                    currentPolyline,
                    GraphicObjectProperties.__GO_LINE_THICKNESS__,
                    Double.parseDouble(thickness));
        } catch (NumberFormatException e) {
            cThickness.setText("1.0");
        }
    }

    /**
    * Set Mark Size.
    * @param size mark size.
    */
    public void setMarkSize(String size) {
        try {
            GraphicController.getController().setProperty(
                currentPolyline,
                GraphicObjectProperties.__GO_MARK_SIZE__,
                Integer.parseInt(size));
        } catch (NumberFormatException e) {
            cMarkSize.setText("1");
        }
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setBackgroungColor(int scilabColor) {
        GraphicController.getController().setProperty(
                currentPolyline, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor) {
        if (BaseProperties.getInstance().getMarkMode()) {
            GraphicController.getController().setProperty(
                    currentPolyline, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    	} else {
            EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	}
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setMarkBackground(int scilabColor) {
        GraphicController.getController().setProperty(
                currentPolyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__, scilabColor);
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkBackground(chooserMarkBackground.getColor());
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setMarkForeground(int scilabColor) {
        if (BaseProperties.getInstance().getMarkMode()) {
            EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	} else {
            GraphicController.getController().setProperty(
                    currentPolyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__, scilabColor);
    	}
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkForeground(chooserMarkForeground.getColor());
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