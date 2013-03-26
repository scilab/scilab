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
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;

import javax.swing.Box;
import javax.swing.Box.Filler;
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
import org.scilab.modules.gui.ged.componentRenderer.JComboBox.MarkStyleRenderer;
import org.scilab.modules.gui.ged.graphic_objects.contouredObject.MarkStyle;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends Position {
    protected static JToggleButton bStyle;
    private JLabel lStyle;
    private JSeparator sStyle;
    protected static JPanel pStyle;

    private JLabel lArrowSize;
    private JTextField cArrowSize;

    private JLabel lBarWidth;
    private JTextField cBarWidth;

    private JLabel lBackColor;
    private JPanel pBackColor;
    private JButton bBackColor;
    protected static JLabel cBackColor;
    protected static JDialog backcolorDialog;
    private static JColorChooser chooserBack;
    private static JButton okBack;

    private JLabel lForeColor;
    private JPanel pForeColor;
    private JButton bForeColor;
    protected static JLabel cForeColor;
    protected static JDialog forecolorDialog;
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
    protected static JLabel cMarkBackground;
    protected static JDialog markBackgroundDialog;
    private static JColorChooser chooserMarkBackground;
    private static JButton okMarkBackground;

    private JLabel lMarkForeground;
    private JPanel pMarkForeground;
    private JButton bMarkForeground;
    protected static JLabel cMarkForeground;
    protected static JDialog markForegroundDialog;
    private static JColorChooser chooserMarkForeground;
    private static JButton okMarkForeground;
    private final MarkStyleRenderer markStyleRenderer = new MarkStyleRenderer();

    private JLabel lMarkSize;
    private JTextField cMarkSize;

    private JLabel lMarkStyle;
    private JComboBox cMarkStyle;

    protected String parentFigure = (String) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_PARENT_FIGURE__);

    private Filler fillerVERTICAL;

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of polyline.
    */
    public Style(String objectID) {
        super(objectID);
        styleComponents();
        initPropertiesStyle(objectID);
        dialogBackgroundColor();
        dialogForegroundColor();
        dialogMarkBackground();
        dialogMarkForeground();
    }

    /**
    * It has all the components of the section Style/Appeareance.
    */
    public void styleComponents() {
        ContentLayout layout = new ContentLayout();

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
        fillerVERTICAL = new Box.Filler(new Dimension(1, 1), new java.awt.Dimension(1, 1), new java.awt.Dimension(1, 32767));

        //Components of the header: Style/Appeareance.
        bStyle.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bStyleActionPerformed(evt);
            }
        });
        layout.addSHbutton(this, bStyle, 0, 9);

        layout.addSectionTitle(this, lStyle, MessagesGED.style_appearance, 9);

        layout.addSeparator(this, sStyle, 10);

        pStyle.setLayout(new GridBagLayout());

        //Components of the property: Arrow Size Factor.
        layout.addJLabel(pStyle, lArrowSize, MessagesGED.arrow_size_factor, 1, 0, 16);

        layout.addJTextField(pStyle, cArrowSize, true, 2, 0, 4);
        cArrowSize.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cArrowSizeActionPerformed(evt);
            }
        });
        cArrowSize.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cArrowSizeFocusLost(evt);
            }
        });

        //Components of the property: Bar Width.
        layout.addJLabel(pStyle, lBarWidth, MessagesGED.bar_width, 1, 1, 16);

        layout.addJTextField(pStyle, cBarWidth, true, 2, 1, 4);
        cBarWidth.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cBarWidthActionPerformed(evt);
            }
        });
        cBarWidth.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cBarWidthFocusLost(evt);
            }
        });

        //Components of the property: Background Color.
        layout.addJLabel(pStyle, lBackColor, MessagesGED.background_color, 1, 2, 16);

        layout.addColorField(pStyle, pBackColor, bBackColor, cBackColor, 2, 2);
        bBackColor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bBackColorActionPerformed(evt);
            }
        });

        //Components of the property: Foreground Color.
        layout.addJLabel(pStyle, lForeColor, MessagesGED.foreground_color, 1, 3, 16);

        layout.addColorField(pStyle, pForeColor, bForeColor, cForeColor, 2, 3);
        bForeColor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bForeColorActionPerformed(evt);
            }
        });

        //Components of the property: Line Style.
        layout.addJLabel(pStyle, lLine, MessagesGED.line_style, 1, 4, 16);

        layout.addJComboBox(pStyle, cLine,
                new String[] { MessagesGED.solid, MessagesGED.dash, MessagesGED.dash_dot,
                    MessagesGED.longdash_dot, MessagesGED.bigdash_dot,
                    MessagesGED.bigdash_longdash, MessagesGED.dot, MessagesGED.double_dot }
                , 2, 4);
        cLine.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cLineActionPerformed(evt);
            }
        });

        //Components of the property: Polyline Style.
        layout.addJLabel(pStyle, lPolyline, MessagesGED.polyline_style, 1, 5, 16);

        layout.addJComboBox(pStyle, cPolyline,
                new String[] { MessagesGED.interpolated, MessagesGED.staircase, MessagesGED.barplot,
                    MessagesGED.bar, MessagesGED.arrowed, MessagesGED.filled }
                , 2, 5);
        cPolyline.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cPolylineActionPerformed(evt);
            }
        });

        //Components of the property: Thickness.
        layout.addJLabel(pStyle, lThickness, MessagesGED.thickness, 1, 6, 16);

        layout.addJTextField(pStyle, cThickness, true, 2, 6, 4);
        cThickness.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cThicknessActionPerformed(evt);
            }
        });
        cThickness.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cThicknessFocusLost(evt);
            }
        });

        //Components of the property: Mark Background.
        layout.addJLabel(pStyle, lMarkBackground, MessagesGED.mark_background, 1, 7, 16);

        layout.addColorField(pStyle, pMarkBackground, bMarkBackground, cMarkBackground, 2, 7);
        bMarkBackground.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bMarkBackgroundActionPerformed(evt);
            }
        });

        //Components of the property: Mark Foreground.
        layout.addJLabel(pStyle, lMarkForeground, MessagesGED.mark_foreground, 1, 8, 16);

        layout.addColorField(pStyle, pMarkForeground, bMarkForeground, cMarkForeground, 2, 8);
        bMarkForeground.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bMarkForegroundActionPerformed(evt);
            }
        });

        //Components of the property: Mark Size.
        layout.addJLabel(pStyle, lMarkSize, MessagesGED.mark_size, 1, 9, 16);

        layout.addJTextField(pStyle, cMarkSize, true, 2, 9, 4);
        cMarkSize.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarkSizeActionPerformed(evt);
            }
        });
        cMarkSize.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cMarkSizeFocusLost(evt);
            }
        });

        //Components of the property: Mark Style.
        layout.addJLabel(pStyle, lMarkStyle, MessagesGED.mark_style, 1, 10, 16);

        cMarkStyle.setRenderer(markStyleRenderer);
        cMarkStyle.setPreferredSize(new Dimension(5, 20));
        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = 2;
        gbc.gridy = 10;
        gbc.ipadx = 70;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 4, 5, 0);
        pStyle.add(cMarkStyle, gbc);
        cMarkStyle.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarkStyleActionPerformed(evt);
            }
        });

        //Positioning JPanel.
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 11;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 0, 12, 0);
        add(pStyle, gbc);

        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 12;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.VERTICAL;
        gbc.weighty = 0.1;
        add(fillerVERTICAL, gbc);
   }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param objectID Enters the identification of polyline.
    */
    public void initPropertiesStyle(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;

            // Get the current status of the property: Arrow Size Factor
            cArrowSize.setText( Double.toString((Double) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__)));

            // Get the current status of the property: Bar Width
            cBarWidth.setText( Double.toString((Double) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_BAR_WIDTH__)));

            // Get the current status of the property: Background Color
            Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                  .getProperty(currentpolyline, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
            cBackColor.setBackground(new Color(rgbBackgroundColor[0].intValue(), rgbBackgroundColor[1].intValue(), rgbBackgroundColor[2].intValue()));

            // Get the current status of the property: Foreground Color
            Integer scilabForegroundColor;
            if ((Boolean)GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__) == false) {
    		scilabForegroundColor = EditorManager.getFromUid(parentFigure).getOriColor();
            } else {
            	scilabForegroundColor = (Integer) GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_COLOR__);
            }
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeColor.setBackground(new Color(rgbForegroundColor[0].intValue(), rgbForegroundColor[1].intValue(), rgbForegroundColor[2].intValue()));

            // Get the current status of the property: Line Style
            int currentLineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_STYLE__);
            cLine.setSelectedIndex(currentLineStyle);

            // Get the current status of the property: Polyline Style
            int currentPolylineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            cPolyline.setSelectedIndex(currentPolylineStyle);

            // Get the current status of the property: Thickness
            cThickness.setText( Double.toString((Double) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_THICKNESS__)));

            // Get the current status of the property: Mark Background Color
            Integer scilabMarkBackground = (Integer) GraphicController.getController()
                  .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
            Double[] rgbMarkBackground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkBackground);
            Color markBackgroundCOLOR = new Color(rgbMarkBackground[0].intValue(), rgbMarkBackground[1].intValue(), rgbMarkBackground[2].intValue());
            cMarkBackground.setBackground(markBackgroundCOLOR);
            markStyleRenderer.setMarkBackground(markBackgroundCOLOR);

            // Get the current status of the property: Mark Foreground Color
            Integer scilabMarkForeground;
            if ((Boolean)GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__) == true) {
    		scilabMarkForeground = EditorManager.getFromUid(parentFigure).getOriColor();
            } else {
            	scilabMarkForeground = (Integer) GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
            }
            Double[] rgbMarkForeground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkForeground);
            Color markForegroundCOLOR = new Color(rgbMarkForeground[0].intValue(), rgbMarkForeground[1].intValue(), rgbMarkForeground[2].intValue());
            cMarkForeground.setBackground(markForegroundCOLOR);
            markStyleRenderer.setMarkForeground(markForegroundCOLOR);

            // Get the current status of the property: Mark Size
            cMarkSize.setText( Integer.toString((Integer) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE__)));

            // Get the current status of the property: Mark Style
            int currentMarkStyle = (Integer) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_STYLE__);
            cMarkStyle.setSelectedIndex(currentMarkStyle);
        }
    }

    /**
    * JDialog - Selection of background colors.
    */
    public void dialogBackgroundColor() {
        backcolorDialog = new JDialog();
        chooserBack = new JColorChooser();
        okBack = new JButton();
        ContentLayout layout = new ContentLayout();

        layout.addColorDialog(backcolorDialog, chooserBack, okBack, cBackColor,
                parentFigure, "polyline.Style", "setBackgroungColor", this);
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    public void dialogForegroundColor() {
        forecolorDialog = new JDialog();
        chooserFore = new JColorChooser();
        okFore = new JButton();
        ContentLayout layout = new ContentLayout();

        layout.addColorDialog(forecolorDialog, chooserFore, okFore, cForeColor,
                parentFigure, "polyline.Style", "setForegroundColor", this);
    }

    /**
    * JDialog - Selection of mark background colors.
    */
    public void dialogMarkBackground() {
        markBackgroundDialog = new JDialog();
        chooserMarkBackground = new JColorChooser();
        okMarkBackground = new JButton();
        ContentLayout layout = new ContentLayout();

        layout.addColorDialog(markBackgroundDialog, chooserMarkBackground, okMarkBackground,
                cMarkBackground, parentFigure, "polyline.Style", "setMarkBackground", this);
    }

    /**
    * JDialog - Selection of mark foreground colors.
    */
    public void dialogMarkForeground() {
        markForegroundDialog = new JDialog();
        chooserMarkForeground = new JColorChooser();
        okMarkForeground = new JButton();
        ContentLayout layout = new ContentLayout();

        layout.addColorDialog(markForegroundDialog, chooserMarkForeground, okMarkForeground,
                cMarkForeground, parentFigure, "polyline.Style", "setMarkForeground", this);
    }

    /**
    * Implement the action button to show/hide.
    * @param evt ActionEvent. 
    */
    private void bStyleActionPerformed(ActionEvent evt) {
        if (bStyle.isSelected()) {
            pStyle.setVisible(false);
            HidePolyline.checkAllButtons();
        } else {
            pStyle.setVisible(true);
            HidePolyline.checkAllButtons();
        }
    }

    /**
    * Updates the property: Arrow Size Factor.
    * @param evt ActionEvent.
    */
    private void cArrowSizeActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__,
                Double.parseDouble(cArrowSize.getText()));
    }

    /**
    * Updates the property: Arrow Size Factor.
    * @param evt FocusEvent.
    */
    private void cArrowSizeFocusLost(FocusEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__,
                Double.parseDouble(cArrowSize.getText()));
    }

    /**
    * Updates the property: Bar Width.
    * @param evt ActionEvent.
    */
    private void cBarWidthActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_BAR_WIDTH__,
                Double.parseDouble(cBarWidth.getText()));
    }

    /**
    * Updates the property: Bar Width.
    * @param evt FocusEvent.
    */
    private void cBarWidthFocusLost(FocusEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_BAR_WIDTH__,
                Double.parseDouble(cBarWidth.getText()));
    }

    /**
    * Updates the property: Background Color.
    * @param evt ActionEvent.
    */
    private void bBackColorActionPerformed(ActionEvent evt) {
        backcolorDialog.setVisible(true);
    }

    /**
    * Updates the property: Foreground Color.
    * @param evt ActionEvent.
    */
    private void bForeColorActionPerformed(ActionEvent evt) {
        forecolorDialog.setVisible(true);
    }

    /**
    * Updates the property: Line Style.
    * @param evt ActionEvent.
    */
    private void cLineActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_LINE_STYLE__,
                cLine.getSelectedIndex());
    }

    /**
    * Updates the property: Polyline Style.
    * @param evt ActionEvent.
    */
    private void cPolylineActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_POLYLINE_STYLE__,
                cPolyline.getSelectedIndex());
    }

    /**
    * Updates the property: Thickness.
    * @param evt ActionEvent.
    */
    private void cThicknessActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_LINE_THICKNESS__,
                Double.parseDouble(cThickness.getText()));
    }

    /**
    * Updates the property: Thickness.
    * @param evt FocusEvent.
    */
    private void cThicknessFocusLost(FocusEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_LINE_THICKNESS__,
                Double.parseDouble(cThickness.getText()));
    }

    /**
    * Updates the property: Mark Background Color.
    * @param evt ActionEvent.
    */
    private void bMarkBackgroundActionPerformed(ActionEvent evt) {
        markBackgroundDialog.setVisible(true);
    }

    /**
    * Updates the property: Mark Foreground Color.
    * @param evt ActionEvent.
    */
    private void bMarkForegroundActionPerformed(ActionEvent evt) {
        markForegroundDialog.setVisible(true);
    }

    /**
    * Updates the property: Mark Size.
    * @param evt ActionEvent.
    */
    private void cMarkSizeActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE__,
                Integer.parseInt(cMarkSize.getText()));
    }

    /**
    * Updates the property: Mark Size.
    * @param evt FocusEvent.
    */
    private void cMarkSizeFocusLost(FocusEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE__,
                Integer.parseInt(cMarkSize.getText()));
    }

    /**
    * Updates the property: Mark Style.
    * @param evt ActionEvent.
    */
    private void cMarkStyleActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_STYLE__, cMarkStyle.getSelectedIndex());
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    public void setBackgroungColor(int scilabColor) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor) {
    	if ((Boolean)GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__) == false) {
    		EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	} else {
    		GraphicController.getController().setProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    	}
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    public void setMarkBackground(int scilabColor) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__, scilabColor);
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkBackground(chooserMarkBackground.getColor());
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    public void setMarkForeground(int scilabColor) {
    	if ((Boolean)GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__) == true) {
    		EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	} else {
    		GraphicController.getController().setProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__, scilabColor);
    	}
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkForeground(chooserMarkForeground.getColor());
    }
}