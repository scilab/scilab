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
package org.scilab.modules.gui.ged.curve;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.editor.Editor;
import org.scilab.modules.gui.editor.EditorManager;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends BaseProperties {
    protected JButton bBackColor;
    protected JButton bForeColor;
    protected static JToggleButton bStyle;
    protected static JLabel cBackColor;
    protected static JLabel cForeColor;
    protected JComboBox cLine;
    protected JComboBox cPolyline;
    protected JLabel lBackColor;
    protected JLabel lForeColor;
    protected JLabel lLine;
    protected JLabel lPolyline;
    protected JLabel lStyle;
    protected JPanel pBackColor;
    protected JPanel pForeColor;
    protected static JPanel pStyle;
    protected JSeparator sStyle;
    protected static JDialog backcolorDialog;
    protected static JDialog forecolorDialog;
    protected String parentFigure = (String) GraphicController.getController()
                                .getProperty(currentcurve, GraphicObjectProperties.__GO_PARENT_FIGURE__);

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of curve.
    */
    public Style(String objectID) {
        super(objectID);
        initPropertiesStyle(objectID);
        position();
        setIconsStyle();
    }

    /**
    * It has all the components of the section Style/Appeareance.
    */
    @Override
    public void styleComponents() {
        bStyle = new JToggleButton();
        lStyle = new JLabel();
        sStyle = new JSeparator();
        pStyle = new JPanel();
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

        //Components of the header: Style/Appeareance.
        bStyle.setBorder(null);
        bStyle.setBorderPainted(false);
        bStyle.setContentAreaFilled(false);
        bStyle.setMaximumSize(new Dimension(16, 16));
        bStyle.setMinimumSize(new Dimension(16, 16));
        bStyle.setPreferredSize(new Dimension(16, 16));
        bStyle.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bStyleActionPerformed(evt);
            }
        });

        lStyle.setText(MessagesGED.style_appearance);

        sStyle.setPreferredSize(new Dimension(50, 2));

        pStyle.setAlignmentX(0.0F);
        pStyle.setAlignmentY(0.0F);

        //Components of the property: Background Color.
        lBackColor.setBackground(new Color(255, 255, 255));
        lBackColor.setText(" " + MessagesGED.background_color);
        lBackColor.setAlignmentX(0.5F);
        lBackColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lBackColor.setOpaque(true);
        lBackColor.setPreferredSize(new Dimension(70, 20));

        pBackColor.setBackground(new Color(255, 255, 255));
        pBackColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        pBackColor.setPreferredSize(new Dimension(70, 20));

        bBackColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        bBackColor.setContentAreaFilled(false);
        bBackColor.setPreferredSize(new Dimension(16, 16));
        bBackColor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bBackColorActionPerformed(evt);
            }
        });

        cBackColor.setText(" ");
        cBackColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cBackColor.setOpaque(true);

        //Positioning the components of the property: Background Color.
        GroupLayout pBackColorLayout = new GroupLayout(pBackColor);
        pBackColor.setLayout(pBackColorLayout);
        pBackColorLayout.setHorizontalGroup(
            pBackColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cBackColor, GroupLayout.PREFERRED_SIZE, 30, GroupLayout.PREFERRED_SIZE))
        );
        pBackColorLayout.setVerticalGroup(
            pBackColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cBackColor, GroupLayout.PREFERRED_SIZE, 11, GroupLayout.PREFERRED_SIZE))
        );

        //Components of the property: Foreground Color.
        lForeColor.setBackground(new Color(255, 255, 255));
        lForeColor.setText(" " + MessagesGED.foreground_color);
        lForeColor.setAlignmentX(0.5F);
        lForeColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lForeColor.setOpaque(true);
        lForeColor.setPreferredSize(new Dimension(70, 20));

        pForeColor.setBackground(new Color(255, 255, 255));
        pForeColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        pForeColor.setPreferredSize(new Dimension(70, 20));

        bForeColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        bForeColor.setContentAreaFilled(false);
        bForeColor.setPreferredSize(new Dimension(16, 16));
        bForeColor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bForeColorActionPerformed(evt);
            }
        });

        cForeColor.setText(" ");
        cForeColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cForeColor.setOpaque(true);

	//Positioning the components of the property: Foreground Color.
        GroupLayout pForeColorLayout = new GroupLayout(pForeColor);
        pForeColor.setLayout(pForeColorLayout);
        pForeColorLayout.setHorizontalGroup(
            pForeColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cForeColor, GroupLayout.PREFERRED_SIZE, 30, GroupLayout.PREFERRED_SIZE))
        );
        pForeColorLayout.setVerticalGroup(
            pForeColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cForeColor, GroupLayout.PREFERRED_SIZE, 11, GroupLayout.PREFERRED_SIZE))
        );

	//Components of the property: Line Style.
        lLine.setBackground(new Color(255, 255, 255));
        lLine.setText(" " + MessagesGED.line_style);
        lLine.setAlignmentX(0.5F);
        lLine.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lLine.setOpaque(true);
        lLine.setPreferredSize(new Dimension(70, 20));

        cLine.setModel(new DefaultComboBoxModel(new String[] { 
            MessagesGED.solid, MessagesGED.dash, MessagesGED.dash_dot, 
            MessagesGED.longdash_dot, MessagesGED.bigdash_dot, 
            MessagesGED.bigdash_longdash, MessagesGED.dot, MessagesGED.double_dot }));
        cLine.setBorder(null);
        cLine.setEditor(null);
        cLine.setPreferredSize(new Dimension(70, 20));
        cLine.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cLineActionPerformed(evt);
            }
        });

	//Components of the property: Polyline Style.
        lPolyline.setBackground(new Color(255, 255, 255));
        lPolyline.setText(" " + MessagesGED.polyline_style);
        lPolyline.setAlignmentX(0.5F);
        lPolyline.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lPolyline.setOpaque(true);
        lPolyline.setPreferredSize(new Dimension(70, 20));

        cPolyline.setModel(new DefaultComboBoxModel(new String[] { 
            MessagesGED.interpolated, MessagesGED.staircase, MessagesGED.barplot, 
            MessagesGED.bar, MessagesGED.arrowed, MessagesGED.filled }));
        cPolyline.setBorder(null);
        cPolyline.setEditor(null);
        cPolyline.setPreferredSize(new Dimension(70, 20));
        cPolyline.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cPolylineActionPerformed(evt);
            }
        });
   }

    /**
    * Positioning all the components of the Style/Appearance.
    */
    private void position() {
	//Positioning all components.
        GroupLayout pStyleLayout = new GroupLayout(pStyle);
        pStyle.setLayout(pStyleLayout);
        pStyleLayout.setHorizontalGroup(
            pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(pBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(pForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lLine, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(cLine, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lPolyline, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(cPolyline, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))))
        );
        pStyleLayout.setVerticalGroup(
            pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lLine, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cLine, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lPolyline, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cPolyline, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param objectID Enters the identification of curve.
    */
    public void initPropertiesStyle(String objectID) {
        if (objectID != null) {
            currentcurve = objectID;

            // Get the current status of the property: Background Color
            Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                  .getProperty(currentcurve, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
            cBackColor.setBackground(new Color(rgbBackgroundColor[0].intValue(), rgbBackgroundColor[1].intValue(), rgbBackgroundColor[2].intValue()));

            // Get the current status of the property: Foreground Color
            Integer scilabForegroundColor = EditorManager.getFromUid(parentFigure).getOriColor();
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeColor.setBackground(new Color(rgbForegroundColor[0].intValue(), rgbForegroundColor[1].intValue(), rgbForegroundColor[2].intValue()));

            // Get the current status of the property: Line Style
            int currentLineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_LINE_STYLE__);
            cLine.setSelectedIndex(currentLineStyle);

            // Get the current status of the property: Polyline Style
            int currentPolylineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            cPolyline.setSelectedIndex(currentPolylineStyle);
        }
    }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    public void dialogBackgroundColor() { }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    public void dialogForegroungColor() { }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsStyle() {
        bStyle.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bStyle.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bBackColor.setIcon(new ImageIcon(SwingInspector.icon_color_fill));
        bForeColor.setIcon(new ImageIcon(SwingInspector.icon_color_fill));
    }

    /**
    * Implement the action button to show/hide.
    * @param evt ActionEvent. 
    */
    private void bStyleActionPerformed(ActionEvent evt) {
        if (bStyle.isSelected()) {
            pStyle.setVisible(false);
            HideCurve.checkAllButtons();
        } else {
            pStyle.setVisible(true);
            HideCurve.checkAllButtons();
        }
    }

    /**
    * Updates the property: Line Style.
    * @param evt ActionEvent.
    */
    private void cLineActionPerformed(ActionEvent evt) {
        int setLine = cLine.getSelectedIndex();
        GraphicController.getController().setProperty(currentcurve, GraphicObjectProperties.__GO_LINE_STYLE__, setLine);
    }

    /**
    * Updates the property: Polyline Style.
    * @param evt ActionEvent.
    */
    private void cPolylineActionPerformed(ActionEvent evt) {
        int setPolyline = cPolyline.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentcurve, GraphicObjectProperties.__GO_POLYLINE_STYLE__, setPolyline);
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
}
