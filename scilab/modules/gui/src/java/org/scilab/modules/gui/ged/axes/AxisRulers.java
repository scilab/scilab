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
package org.scilab.modules.gui.ged.axes;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Axis Rulers.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class AxisRulers extends Roof {
    //Header - Axis Rulers
    protected static JToggleButton bAxisRulers;
    protected JLabel lAxisRulers;
    protected JSeparator sAxisRulers;
    public static JPanel pAxisRulers;
    //Components of the property: Location
    protected static JToggleButton bLocation;
    protected JLabel lLocation;
    protected JLabel cLocation;
    protected static JPanel pLocation;
    protected JLabel lLocationX;
    protected JComboBox cLocationX;
    protected JLabel lLocationY;
    protected JComboBox cLocationY;
    //Components of the property: Reverse
    protected static JToggleButton bReverse;
    protected JLabel lReverse;
    protected JLabel cReverse;
    protected static JPanel pReverse;
    protected JLabel lReverseX;
    protected JComboBox cReverseX;
    protected JLabel lReverseY;
    protected JComboBox cReverseY;
    protected JLabel lReverseZ;
    protected JComboBox cReverseZ;

    protected String currentaxes = null;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Axes.
    */
    public AxisRulers(String objectID) {
        position();
        setIconsAxisRulers();
        initPropertiesAxisRulers(objectID);
    }

    /**
    * It has all the components of the section Axis Rulers.
    */
    @Override
    public void axisRulersComponents() {
        //Header
        bAxisRulers = new JToggleButton();
        lAxisRulers = new JLabel();
        sAxisRulers = new JSeparator();
        pAxisRulers = new JPanel();
        //Location
        bLocation = new JToggleButton();
        lLocation = new JLabel();
        cLocation = new JLabel();
        pLocation = new JPanel();
        lLocationX = new JLabel();
        cLocationX = new JComboBox();
        lLocationY = new JLabel();
        cLocationY = new JComboBox();
        //Reverse
        bReverse = new JToggleButton();
        lReverse = new JLabel();
        cReverse = new JLabel();
        pReverse = new JPanel();
        lReverseX = new JLabel();
        cReverseX = new JComboBox();
        lReverseY = new JLabel();
        cReverseY = new JComboBox();
        lReverseZ = new JLabel();
        cReverseZ = new JComboBox();

        //Components of the header: Axis Rulers
        bAxisRulers.setBorder(null);
        bAxisRulers.setBorderPainted(false);
        bAxisRulers.setContentAreaFilled(false);
        bAxisRulers.setMaximumSize(new Dimension(16, 16));
        bAxisRulers.setMinimumSize(new Dimension(16, 16));
        bAxisRulers.setPreferredSize(new Dimension(16, 16));
        bAxisRulers.setRolloverEnabled(false);
        bAxisRulers.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bAxisRulersActionPerformed(evt);
            }
        });

        lAxisRulers.setText(MessagesGED.axis_rulers);

        sAxisRulers.setPreferredSize(new Dimension(50, 2));

        //Components of the property: Location.
        lLocation.setBackground(new Color(255, 255, 255));
        lLocation.setText(" " + MessagesGED.location);
        lLocation.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lLocation.setOpaque(true);
        lLocation.setPreferredSize(new Dimension(70, 20));

        cLocation.setBackground(new Color(255, 255, 255));
        cLocation.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cLocation.setOpaque(true);
        cLocation.setPreferredSize(new Dimension(70, 20));

        bLocation.setSelected(true);
        bLocation.setBorder(null);
        bLocation.setBorderPainted(false);
        bLocation.setContentAreaFilled(false);
        bLocation.setMaximumSize(new Dimension(16, 16));
        bLocation.setMinimumSize(new Dimension(16, 16));
        bLocation.setPreferredSize(new Dimension(16, 16));
        bLocation.setRolloverEnabled(false);
        bLocation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bLocationActionPerformed(evt);
            }
        });

        pLocation.setVisible(false);

        lLocationX.setBackground(new Color(255, 255, 255));
        lLocationX.setText(" X");
        lLocationX.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lLocationX.setOpaque(true);
        lLocationX.setPreferredSize(new Dimension(70, 20));

        cLocationX.setModel(new DefaultComboBoxModel(new String[] {
                                MessagesGED.bottom, MessagesGED.top, MessagesGED.middle, MessagesGED.origin
                            }));
        cLocationX.setPreferredSize(new Dimension(70, 20));
        cLocationX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cLocationXActionPerformed(evt);
            }
        });

        lLocationY.setBackground(new Color(255, 255, 255));
        lLocationY.setText(" Y");
        lLocationY.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lLocationY.setOpaque(true);
        lLocationY.setPreferredSize(new Dimension(70, 20));

        cLocationY.setModel(new DefaultComboBoxModel(new String[] {
                                MessagesGED.middle, MessagesGED.origin, MessagesGED.left, MessagesGED.right
                            }));
        cLocationY.setPreferredSize(new Dimension(70, 20));
        cLocationY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cLocationYActionPerformed(evt);
            }
        });

        titleLocation();

        //Positioning the components of the property: Location.
        GroupLayout pLocationLayout = new GroupLayout(pLocation);
        pLocation.setLayout(pLocationLayout);
        pLocationLayout.setHorizontalGroup(
            pLocationLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pLocationLayout.createSequentialGroup()
                      .addComponent(lLocationX, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cLocationX, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pLocationLayout.createSequentialGroup()
                      .addComponent(lLocationY, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cLocationY, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        pLocationLayout.setVerticalGroup(
            pLocationLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pLocationLayout.createSequentialGroup()
                      .addGroup(pLocationLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lLocationX, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cLocationX, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                      .addGap(5, 5, 5)
                      .addGroup(pLocationLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lLocationY, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cLocationY, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );


        //Components of the property: Reverse.
        lReverse.setBackground(new Color(255, 255, 255));
        lReverse.setText(" " + MessagesGED.reverse);
        lReverse.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lReverse.setOpaque(true);
        lReverse.setPreferredSize(new Dimension(70, 20));

        cReverse.setBackground(new Color(255, 255, 255));
        cReverse.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cReverse.setOpaque(true);
        cReverse.setPreferredSize(new Dimension(70, 20));

        bReverse.setSelected(true);
        bReverse.setBorder(null);
        bReverse.setBorderPainted(false);
        bReverse.setContentAreaFilled(false);
        bReverse.setMaximumSize(new Dimension(16, 16));
        bReverse.setMinimumSize(new Dimension(16, 16));
        bReverse.setPreferredSize(new Dimension(16, 16));
        bReverse.setRolloverEnabled(false);
        bReverse.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bReverseActionPerformed(evt);
            }
        });

        pReverse.setVisible(false);

        lReverseX.setBackground(new Color(255, 255, 255));
        lReverseX.setText(" X");
        lReverseX.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lReverseX.setOpaque(true);
        lReverseX.setPreferredSize(new Dimension(70, 20));

        cReverseX.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cReverseX.setPreferredSize(new Dimension(70, 20));
        cReverseX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cReverseXActionPerformed(evt);
            }
        });

        lReverseY.setBackground(new Color(255, 255, 255));
        lReverseY.setText(" Y");
        lReverseY.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lReverseY.setOpaque(true);
        lReverseY.setPreferredSize(new Dimension(70, 20));

        cReverseY.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cReverseY.setPreferredSize(new Dimension(70, 20));
        cReverseY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cReverseYActionPerformed(evt);
            }
        });

        lReverseZ.setBackground(new Color(255, 255, 255));
        lReverseZ.setText(" Z");
        lReverseZ.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lReverseZ.setOpaque(true);
        lReverseZ.setPreferredSize(new Dimension(70, 20));

        cReverseZ.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cReverseZ.setPreferredSize(new Dimension(70, 20));
        cReverseZ.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cReverseZActionPerformed(evt);
            }
        });

        titleReverse();

        //Positioning the components of the property: Reverse.
        GroupLayout pReverseLayout = new GroupLayout(pReverse);
        pReverse.setLayout(pReverseLayout);
        pReverseLayout.setHorizontalGroup(
            pReverseLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pReverseLayout.createSequentialGroup()
                      .addComponent(lReverseX, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cReverseX, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pReverseLayout.createSequentialGroup()
                      .addComponent(lReverseY, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cReverseY, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pReverseLayout.createSequentialGroup()
                      .addComponent(lReverseZ, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cReverseZ, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        pReverseLayout.setVerticalGroup(
            pReverseLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pReverseLayout.createSequentialGroup()
                      .addGroup(pReverseLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lReverseX, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cReverseX, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                      .addGap(5, 5, 5)
                      .addGroup(pReverseLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lReverseY, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cReverseY, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                      .addGap(5, 5, 5)
                      .addGroup(pReverseLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(lReverseZ, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cReverseZ, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Positioning all the components of the Axis Rulers.
    */
    private void position() {
        //Positioning all components.
        GroupLayout pAxisRulersLayout = new GroupLayout(pAxisRulers);
        pAxisRulers.setLayout(pAxisRulersLayout);
        pAxisRulersLayout.setHorizontalGroup(
            pAxisRulersLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                      .addComponent(bLocation, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                      .addGap(2, 2, 2)
                      .addComponent(lLocation, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cLocation, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                      .addComponent(bReverse, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                      .addGap(2, 2, 2)
                      .addComponent(lReverse, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                      .addGap(4, 4, 4)
                      .addComponent(cReverse, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                      .addGap(18, 18, 18)
                      .addGroup(pAxisRulersLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(pLocation, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                                .addComponent(pReverse, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)))
        );
        pAxisRulersLayout.setVerticalGroup(
            pAxisRulersLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                      .addGroup(pAxisRulersLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addGroup(pAxisRulersLayout.createSequentialGroup()
                                          .addGap(3, 3, 3)
                                          .addComponent(bLocation, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                                .addComponent(lLocation, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cLocation, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE))
                      .addGap(5, 5, 5)
                      .addComponent(pLocation, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                      .addGap(5, 5, 5)
                      .addGroup(pAxisRulersLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addGroup(pAxisRulersLayout.createSequentialGroup()
                                          .addGap(3, 3, 3)
                                          .addComponent(bReverse, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                                .addComponent(lReverse, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                                .addComponent(cReverse, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE))
                      .addGap(5, 5, 5)
                      .addComponent(pReverse, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
        );
    }

    /**
    * Loads the current properties of the section Axis Rulers.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesAxisRulers(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Location */
            int currentLocationX = (Integer) GraphicController.getController()
                                   .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LOCATION__);
            cLocationX.setSelectedIndex(currentLocationX);
            int currentLocationY = (Integer) GraphicController.getController()
                                   .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__);
            cLocationY.setSelectedIndex(currentLocationY - 2);

            /** Get the current status of the property: Reverse */
            boolean currentReverseX = (Boolean) GraphicController.getController()
                                      .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_REVERSE__);
            if (currentReverseX) {
                cReverseX.setSelectedIndex(1);
            } else {
                cReverseX.setSelectedIndex(0);
            }
            boolean currentReverseY = (Boolean) GraphicController.getController()
                                      .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__);
            if (currentReverseX) {
                cReverseY.setSelectedIndex(1);
            } else {
                cReverseY.setSelectedIndex(0);
            }
            boolean currentReverseZ = (Boolean) GraphicController.getController()
                                      .getProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__);
            if (currentReverseX) {
                cReverseZ.setSelectedIndex(1);
            } else {
                cReverseZ.setSelectedIndex(0);
            }
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsAxisRulers() {
        bAxisRulers.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bAxisRulers.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bLocation.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bLocation.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bReverse.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bReverse.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bAxisRulersActionPerformed(ActionEvent evt) {
        if (bAxisRulers.isSelected()) {
            pAxisRulers.setVisible(false);
            HideAxis.checkAllButtons();
        } else {
            pAxisRulers.setVisible(true);
            HideAxis.checkAllButtons();
        }
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bLocationActionPerformed(ActionEvent evt) {
        if (bLocation.isSelected()) {
            pLocation.setVisible(false);
        } else {
            pLocation.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bReverseActionPerformed(ActionEvent evt) {
        if (bReverse.isSelected()) {
            pReverse.setVisible(false);
        } else {
            pReverse.setVisible(true);
        }
    }

    /**
    * Inserts the current situation of the location of the axis on the main label.
    */
    public void titleLocation() {
        String titleLocationx = cLocationX.getSelectedItem().toString();
        String titleLocationy = cLocationY.getSelectedItem().toString();
        String titleLocation = titleLocationx + " , " + titleLocationy;
        cLocation.setText(" [" + titleLocation + "]");
    }

    /**
    * Inserts the current state of functionality: REVERSE in main label.
    */
    public void titleReverse() {
        String titleReverseX = cReverseX.getSelectedItem().toString();
        String titleReverseY = cReverseY.getSelectedItem().toString();
        String titleReverseZ = cReverseZ.getSelectedItem().toString();
        String titleReverse = titleReverseX + " , " + titleReverseY + " , " + titleReverseZ;
        cReverse.setText(" [" + titleReverse + "]");
    }

    /**
    * Updates the property: x Location.
    *
    * @param evt ActionEvent.
    */
    private void cLocationXActionPerformed(ActionEvent evt) {
        int setLocationX = cLocationX.getSelectedIndex();
        GraphicController.getController()
        .setProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LOCATION__, setLocationX);
        titleLocation();
    }

    /**
    * Updates the property: y Location.
    *
    * @param evt ActionEvent.
    */
    private void cLocationYActionPerformed(ActionEvent evt) {
        int setLocationY = cLocationY.getSelectedIndex();
        GraphicController.getController()
        .setProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__, setLocationY + 2);
        titleLocation();
    }

    /**
    * Updates the property: x Reverse.
    *
    * @param evt ActionEvent.
    */
    private void cReverseXActionPerformed(ActionEvent evt) {
        boolean setReverseX = true;
        if (cReverseX.getSelectedIndex() == 0) {
            setReverseX = false;
        }
        GraphicController.getController()
        .setProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_REVERSE__, setReverseX);
        titleReverse();
    }

    /**
    * Updates the property: y Reverse.
    *
    * @param evt ActionEvent.
    */
    private void cReverseYActionPerformed(ActionEvent evt) {
        boolean setReverseY = true;
        if (cReverseY.getSelectedIndex() == 0) {
            setReverseY = false;
        }
        GraphicController.getController()
        .setProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__, setReverseY);
        titleReverse();
    }

    /**
    * Updates the property: z Reverse.
    *
    * @param evt ActionEvent.
    */
    private void cReverseZActionPerformed(ActionEvent evt) {
        boolean setReverseZ = true;
        if (cReverseZ.getSelectedIndex() == 0) {
            setReverseZ = false;
        }
        GraphicController.getController()
        .setProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__, setReverseZ);
        titleReverse();
    }
}