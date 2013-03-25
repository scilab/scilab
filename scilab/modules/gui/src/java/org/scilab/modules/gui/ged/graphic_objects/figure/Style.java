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
package org.scilab.modules.gui.ged.graphic_objects.figure;

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

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends DataProperties {
    protected static JToggleButton bStyle;
    protected static JPanel pStyle;
    protected JPanel pColor;
    protected JButton bColor;
    protected JLabel lStyle;
    protected JSeparator sStyle;
    protected JLabel lColor;
    protected JLabel cColor;
    protected JComboBox cPixmap;
    protected JLabel lPixmap;
    protected JComboBox cPixelDrawing;
    protected JLabel lPixelDrawing;
    protected JComboBox cRotationStyle;
    protected JLabel lRotationStyle;
    protected JDialog colorDialog;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Figure.
    */
    public Style(String objectID) {
        super(objectID);
        position();
        setIconsStyle();
        initPropertiesStyle(objectID);
    }

    /**
    * It has all the components of the section Style/Appeareance.
    */
    @Override
    public void styleComponents() {
        pStyle = new JPanel();
        bStyle = new JToggleButton();
        lStyle = new JLabel();
        sStyle = new JSeparator();
        pColor = new JPanel();
        bColor = new JButton();
        cColor = new JLabel();
        lColor = new JLabel();
        lPixmap = new JLabel();
        cPixmap = new JComboBox();
        lPixelDrawing = new JLabel();
        cPixelDrawing = new JComboBox();
        lRotationStyle = new JLabel();
        cRotationStyle = new JComboBox();

        //Components of the header: Style/Appeareance.
        pStyle.setAlignmentX(0.0F);
        pStyle.setAlignmentY(0.0F);

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

	//Components of the property: Color.
        pColor.setBackground(new Color(255, 255, 255));
        pColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        pColor.setPreferredSize(new Dimension(70, 20));


        bColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        bColor.setContentAreaFilled(false);
        bColor.setPreferredSize(new Dimension(16, 16));
        bColor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bColorActionPerformed(evt);
            }
        });

        cColor.setText(" ");
        cColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cColor.setOpaque(true);

        lColor.setBackground(new Color(255, 255, 255));
        lColor.setText(" " + MessagesGED.background_color);
        lColor.setAlignmentX(0.5F);
        lColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lColor.setOpaque(true);
        lColor.setPreferredSize(new Dimension(70, 20));

	//Positioning the components of the property: Color.
        GroupLayout pColorLayout = new GroupLayout(pColor);
        pColor.setLayout(pColorLayout);
        pColorLayout.setHorizontalGroup(
            pColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cColor, GroupLayout.PREFERRED_SIZE, 30, GroupLayout.PREFERRED_SIZE))
        );
        pColorLayout.setVerticalGroup(
            pColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
            .addGroup(pColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cColor, GroupLayout.PREFERRED_SIZE, 11, GroupLayout.PREFERRED_SIZE))
        );

	//Components of the property: Pixmap.
        lPixmap.setBackground(new Color(255, 255, 255));
        lPixmap.setText(" " + MessagesGED.pixmap);
        lPixmap.setAlignmentX(0.5F);
        lPixmap.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lPixmap.setOpaque(true);
        lPixmap.setPreferredSize(new Dimension(70, 20));

        cPixmap.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cPixmap.setBorder(null);
        cPixmap.setEditor(null);
        cPixmap.setPreferredSize(new Dimension(70, 20));
        cPixmap.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cPixmapActionPerformed(evt);
            }
        });

	//Components of the property: Pixel Drawing Mode.
        lPixelDrawing.setBackground(new Color(255, 255, 255));
        lPixelDrawing.setText(" " + MessagesGED.pixel_drawing_mode);
        lPixelDrawing.setAlignmentX(0.5F);
        lPixelDrawing.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lPixelDrawing.setOpaque(true);
        lPixelDrawing.setPreferredSize(new Dimension(70, 20));

        cPixelDrawing.setModel(new DefaultComboBoxModel(new String[] { 
            "clear", "and", "andReverse", "copy", "andInverted",
            "noop", "xor", "or", "nor", "equiv", "invert", "orReverse",
            "copyInverted", "orInverted", "nand", "set" }));
        cPixelDrawing.setSelectedIndex(3);
        cPixelDrawing.setBorder(null);
        cPixelDrawing.setEditor(null);
        cPixelDrawing.setPreferredSize(new Dimension(70, 20));
        cPixelDrawing.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cPixelDrawingActionPerformed(evt);
            }
        });

	//Components of the property: Rotation Style.
        lRotationStyle.setBackground(new Color(255, 255, 255));
        lRotationStyle.setText(" " + MessagesGED.rotation_style);
        lRotationStyle.setAlignmentX(0.5F);
        lRotationStyle.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lRotationStyle.setOpaque(true);
        lRotationStyle.setPreferredSize(new Dimension(70, 20));

        cRotationStyle.setModel(new DefaultComboBoxModel(new String[] { 
            MessagesGED.unary, MessagesGED.multiple }));
        cRotationStyle.setBorder(null);
        cRotationStyle.setEditor(null);
        cRotationStyle.setPreferredSize(new Dimension(70, 20));
        cRotationStyle.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cRotationStyleActionPerformed(evt);
            }
        });
   }

    /**
    * Positioning all the components of the Style/Appearance.
    */
    private void position() {
        GroupLayout pStyleLayout = new GroupLayout(pStyle);
        pStyle.setLayout(pStyleLayout);
        pStyleLayout.setHorizontalGroup(
            pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lColor, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(pColor, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lPixmap, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(cPixmap, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lPixelDrawing, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(cPixelDrawing, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lRotationStyle, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                        .addGap(4, 4, 4)
                        .addComponent(cRotationStyle, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))))
        );
        pStyleLayout.setVerticalGroup(
            pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(7, 7, 7)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lPixmap, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                    .addComponent(cPixmap, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(7, 7, 7)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lPixelDrawing, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                    .addComponent(cPixelDrawing, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(7, 7, 7)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lRotationStyle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                    .addComponent(cRotationStyle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param figureUid Enters the identification of Figure.
    */
    public void initPropertiesStyle(String figureUid) {
        if (figureUid != null) {
            currentfigure = figureUid;

            /** Get the current status of the property: Background Color */
            Integer scilabColor = (Integer) GraphicController.getController()
                  .getProperty(currentfigure, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbColor = ColorMapHandler.getRGBcolor(currentfigure,scilabColor);
            cColor.setBackground(new Color(rgbColor[0].intValue(), rgbColor[1].intValue(), rgbColor[2].intValue()));

            /** Get the current status of the property: Pixmap */
            boolean currentPixmap = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_PIXMAP__);
            if (currentPixmap) {
                cPixmap.setSelectedIndex(1);
            } else {
		cPixmap.setSelectedIndex(0);
            }

            /** Get the current status of the property: Pixel Drawing Mode */
            Integer currentPixelDrawingMode = (Integer) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__);
            cPixelDrawing.setSelectedIndex(currentPixelDrawingMode);

            /** Get the current status of the property: Rotation Style */
            Integer currentRotationStyle = (Integer) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_ROTATION_TYPE__);
            cRotationStyle.setSelectedIndex(currentRotationStyle);
        }
    }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    public void dialogColor() { }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsStyle() {
        bStyle.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bStyle.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bColor.setIcon(new ImageIcon(SwingInspector.icon_color_fill));
    }

   /**
   * Implement the action button to show/hide.
   *
   * @param evt ActionEvent.
   */
    private void bStyleActionPerformed(ActionEvent evt) {
        if (bStyle.isSelected()) {
            pStyle.setVisible(false);
            HideFigure.checkAllButtons();
        } else {
            pStyle.setVisible(true);
            HideFigure.checkAllButtons();
        }
    }

    /**
    * Updates the property: Color.
    *
    * @param evt ActionEvent.
    */
    private void bColorActionPerformed(ActionEvent evt) {
        colorDialog.setVisible(true);
        colorDialog.setModal(true);
    }

    /**
    * Updates the property: Pixmap.
    *
    * @param evt ActionEvent.
    */
    private void cPixmapActionPerformed(ActionEvent evt) {
        boolean setPixmap = true;
        if (cPixmap.getSelectedIndex() == 0) {
            setPixmap = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_PIXMAP__, setPixmap);
    }

    /**
    * Updates the property: Pixel Drawing Mode.
    *
    * @param evt ActionEvent.
    */
    private void cPixelDrawingActionPerformed(ActionEvent evt) {
        int setPixelDrawing = cPixelDrawing.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__, setPixelDrawing);
    }

    /**
    * Updates the property: Rotation Style.
    *
    * @param evt ActionEvent.
    */
    private void cRotationStyleActionPerformed(ActionEvent evt) {
        int setRotationStyle = cRotationStyle.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_ROTATION_TYPE__, setRotationStyle);
    }
}
