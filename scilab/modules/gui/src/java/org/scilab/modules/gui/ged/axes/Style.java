/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.axes;

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
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Style extends Position {
    //Header - Style/Appearance
    protected static JToggleButton bStyle;
    protected JLabel lStyle;
    protected JSeparator sStyle;
    protected static JPanel pStyle;
    //Components of the property: Background Color
    protected JLabel lBackColor;
    protected static JPanel pBackColor;
    protected JLabel cBackColor;
    protected static JButton bBackColor;
    protected JDialog backColorDialog;
    //Components of the property: Foreground Color
    protected JLabel lForeColor;
    protected static JPanel pForeColor;
    protected JLabel cForeColor;
    protected static JButton bForeColor;
    protected JDialog foreColorDialog;
    //Components of the property: Fontground Color
    protected JLabel lFontColor;
    protected static JPanel pFontColor;
    protected JLabel cFontColor;
    protected static JButton bFontColor;
    protected JDialog fontColorDialog;
    //Components of the property: Font Style
    protected JLabel lFontStyle;
    protected JComboBox cFontStyle;
    //Components of the property: Visible
    protected JLabel lVisible;
    protected JComboBox cVisible;
    protected String parentFigure = (String) GraphicController.getController()
                                .getProperty(currentaxes, GraphicObjectProperties.__GO_PARENT_FIGURE__);

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Axes.
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
        //Header
        bStyle = new JToggleButton();
        lStyle = new JLabel();
        sStyle = new JSeparator();
        pStyle = new JPanel();
        //Background Color
        lBackColor = new JLabel();
        pBackColor = new JPanel();
        bBackColor = new JButton();
        cBackColor = new JLabel();
        //Foreground Color
        lForeColor = new JLabel();
        pForeColor = new JPanel();
        bForeColor = new JButton();
        cForeColor = new JLabel();
        //Font Color
        lFontColor = new JLabel();
        pFontColor = new JPanel();
        bFontColor = new JButton();
        cFontColor = new JLabel();
        //Font Style
        lFontStyle = new JLabel();
        cFontStyle = new JComboBox();
        //Visible
        lVisible = new JLabel();
        cVisible = new JComboBox();

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
        lBackColor.setPreferredSize(new Dimension(153, 20));

        pBackColor.setBackground(new Color(255, 255, 255));
        pBackColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        pBackColor.setPreferredSize(new Dimension(157, 20));

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
        lForeColor.setPreferredSize(new Dimension(153, 20));

        pForeColor.setBackground(new Color(255, 255, 255));
        pForeColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        pForeColor.setPreferredSize(new Dimension(157, 20));

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

        //Components of the property: Font Color.
        lFontColor.setBackground(new Color(255, 255, 255));
        lFontColor.setText(" " + MessagesGED.font_color);
        lFontColor.setAlignmentX(0.5F);
        lFontColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lFontColor.setOpaque(true);
        lFontColor.setPreferredSize(new Dimension(153, 20));

        pFontColor.setBackground(new Color(255, 255, 255));
        pFontColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        pFontColor.setPreferredSize(new Dimension(157, 20));

        bFontColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        bFontColor.setContentAreaFilled(false);
        bFontColor.setPreferredSize(new Dimension(16, 16));
        bFontColor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bFontColorActionPerformed(evt);
            }
        });

        cFontColor.setText(" ");
        cFontColor.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cFontColor.setOpaque(true);

        //Positioning the components of the property: Font Color.
        GroupLayout pFontColorLayout = new GroupLayout(pFontColor);
        pFontColor.setLayout(pFontColorLayout);
        pFontColorLayout.setHorizontalGroup(
            pFontColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bFontColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cFontColor, GroupLayout.PREFERRED_SIZE, 30, GroupLayout.PREFERRED_SIZE))
        );
        pFontColorLayout.setVerticalGroup(
            pFontColorLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bFontColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cFontColor, GroupLayout.PREFERRED_SIZE, 11, GroupLayout.PREFERRED_SIZE))
        );

        //Components of the property: Font Style.
        lFontStyle.setBackground(new Color(255, 255, 255));
        lFontStyle.setText(" " + MessagesGED.font_style);
        lFontStyle.setAlignmentX(0.5F);
        lFontStyle.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lFontStyle.setOpaque(true);
        lFontStyle.setPreferredSize(new Dimension(153, 20));

        cFontStyle.setModel(new DefaultComboBoxModel(new String[] { 
            "Monospaced", "Symbol", "Serif", "Serif Italic", "Serif Bold", 
            "Serif Bold Italic", "SansSerif", "SansSerif Italic", 
            "SansSerif Bold", "SansSerif Bold Italic" 
        }));
        cFontStyle.setBorder(null);
        cFontStyle.setEditor(null);
        cFontStyle.setPreferredSize(new Dimension(150, 20));
        cFontStyle.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cFontStyleActionPerformed(evt);
            }
        });

        //Components of the property: Visible.
        lVisible.setBackground(new Color(255, 255, 255));
        lVisible.setText(" " + MessagesGED.visible);
        lVisible.setAlignmentX(0.5F);
        lVisible.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lVisible.setOpaque(true);
        lVisible.setPreferredSize(new Dimension(153, 20));

        cVisible.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cVisible.setBorder(null);
        cVisible.setEditor(null);
        cVisible.setPreferredSize(new Dimension(150, 20));
        cVisible.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cVisibleActionPerformed(evt);
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
                        .addComponent(lBackColor, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pBackColor, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lForeColor, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pForeColor, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lFontColor, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pFontColor, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lFontStyle, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cFontStyle, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lVisible, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cVisible, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))))
        );
        pStyleLayout.setVerticalGroup(
            pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pBackColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pForeColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lFontColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pFontColor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(4, 4, 4)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lFontStyle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cFontStyle, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lVisible, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cVisible, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesStyle(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;

            /** Get the current status of the property: Background Color */
            Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                  .getProperty(currentaxes, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
            cBackColor.setBackground(new Color(rgbBackgroundColor[0].intValue(), rgbBackgroundColor[1].intValue(), rgbBackgroundColor[2].intValue()));

            /** Get the current status of the property: Foreground Color */
            Integer scilabForegroundColor = (Integer) GraphicController.getController()
                  .getProperty(currentaxes, GraphicObjectProperties.__GO_LINE_COLOR__);
            Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
            cForeColor.setBackground(new Color(rgbForegroundColor[0].intValue(), rgbForegroundColor[1].intValue(), rgbForegroundColor[2].intValue()));

            /** Get the current status of the property: Font Color */
            Integer scilabFontColor = (Integer) GraphicController.getController()
                  .getProperty(currentaxes, GraphicObjectProperties.__GO_FONT_COLOR__);
            Double[] rgbFontColor = ColorMapHandler.getRGBcolor(parentFigure, scilabFontColor);
            cFontColor.setBackground(new Color(rgbFontColor[0].intValue(), rgbFontColor[1].intValue(), rgbFontColor[2].intValue()));

            /** Get the current status of the property: Font Style */
            int currentFontStyle = (Integer) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_FONT_STYLE__);
            cFontStyle.setSelectedIndex(currentFontStyle);

            /** Get the current status of the property: Visible */
            boolean isVisible = (Boolean) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
        }
    }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    @Override
    public void dialogBackgroundColor() { }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    @Override
    public void dialogForegroundColor() { }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    @Override
    public void dialogFontColor() { }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsStyle() {
        bStyle.setIcon(new ImageIcon(Inspector.icon_collapse));
        bStyle.setSelectedIcon(new ImageIcon(Inspector.icon_expand));
        bBackColor.setIcon(new ImageIcon(Inspector.icon_color_fill));
        bForeColor.setIcon(new ImageIcon(Inspector.icon_color_fill));
        bFontColor.setIcon(new ImageIcon(Inspector.icon_color_fill));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bStyleActionPerformed(ActionEvent evt) {
        if (bStyle.isSelected()) {
            pStyle.setVisible(false);
        } else {
            pStyle.setVisible(true);
        }
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    *
    * @param evt ActionEvent.
    */
    private void bBackColorActionPerformed(ActionEvent evt) {
        backColorDialog.setVisible(true);
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    *
    * @param evt ActionEvent.
    */
    private void bForeColorActionPerformed(ActionEvent evt) {
        foreColorDialog.setVisible(true);
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    *
    * @param evt ActionEvent.
    */
    private void bFontColorActionPerformed(ActionEvent evt) {
        fontColorDialog.setVisible(true);
    }

    /**
    * Updates the property: Font Style.
    *
    * @param evt ActionEvent.
    */
    private void cFontStyleActionPerformed(ActionEvent evt) {
        int setFontStyle = cFontStyle.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_FONT_STYLE__, setFontStyle);
    }

    /**
    * Updates the property: Visible.
    *
    * @param evt ActionEvent.
    */
    private void cVisibleActionPerformed(ActionEvent evt) {
        boolean setVisible = true;
        if (cVisible.getSelectedIndex() == 0) {
            setVisible = false;
        }
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_VISIBLE__, setVisible);
    }
}