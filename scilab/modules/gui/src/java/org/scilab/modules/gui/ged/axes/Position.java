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
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Position.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Position extends Label {
    //Header - Label
    protected static JToggleButton bPosition;
    protected JLabel lPosition;
    protected JSeparator sPosition;
    protected static JPanel pPosition;
    //Components of the property: Axes Bounds
    protected static JToggleButton bAxesBounds;
    protected JLabel lAxesBounds;
    protected JLabel cAxesBounds;
    protected static JPanel pAxesBounds;
    protected JLabel lBoundsLeft;
    protected JTextField cBoundsLeft;
    protected JLabel lBoundsUp;
    protected JTextField cBoundsUp;
    protected JLabel lBoundsWidth;
    protected JTextField cBoundsWidth;
    protected JLabel lBoundsHeight;
    protected JTextField cBoundsHeight;
    //Components of the property: Margins
    protected static JToggleButton bMargins;
    protected JLabel lMargins;
    protected JLabel cMargins;
    protected static JPanel pMargins;
    protected JLabel lMarginsLeft;
    protected JTextField cMarginsLeft;
    protected JLabel lMarginsRight;
    protected JTextField cMarginsRight;
    protected JLabel lMarginsTop;
    protected JTextField cMarginsTop;
    protected JLabel lMarginsBottom;
    protected JTextField cMarginsBottom;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Axes.
    */
    public Position(String objectID) {
        super(objectID);
        position();
        setIconsPosition();
        initPropertiesPosition(objectID);
    }

    /**
    * It has all the components of the section Position.
    */
    @Override
    public void positionComponents() {
        //Header
        bPosition = new JToggleButton();
        lPosition = new JLabel();
        sPosition = new JSeparator();
        pPosition = new JPanel();
        //Axes bounds
        bAxesBounds = new JToggleButton();
        lAxesBounds = new JLabel();
        cAxesBounds = new JLabel();
        pAxesBounds = new JPanel();
        lBoundsLeft = new JLabel();
        cBoundsLeft = new JTextField();
        lBoundsUp = new JLabel();
        cBoundsUp = new JTextField();
        lBoundsWidth = new JLabel();
        cBoundsWidth = new JTextField();
        lBoundsHeight = new JLabel();
        cBoundsHeight = new JTextField();
        //Margins
        bMargins = new JToggleButton();
        lMargins = new JLabel();
        cMargins = new JLabel();
        pMargins = new JPanel();
        lMarginsLeft = new JLabel();
        cMarginsLeft = new JTextField();
        lMarginsRight = new JLabel();
        cMarginsRight = new JTextField();
        lMarginsTop = new JLabel();
        cMarginsTop = new JTextField();
        lMarginsBottom = new JLabel();
        cMarginsBottom = new JTextField();

        //Components of the header: Position
        bPosition.setBorder(null);
        bPosition.setBorderPainted(false);
        bPosition.setContentAreaFilled(false);
        bPosition.setMaximumSize(new Dimension(16, 16));
        bPosition.setMinimumSize(new Dimension(16, 16));
        bPosition.setPreferredSize(new Dimension(16, 16));
        bPosition.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bPositionActionPerformed(evt);
            }
        });

        lPosition.setText(MessagesGED.position);

        sPosition.setPreferredSize(new Dimension(50, 2));

        //Components of the property: Axes Bounds.
        pAxesBounds.setVisible(false);
        bAxesBounds.setSelected(true);
        bAxesBounds.setBorder(null);
        bAxesBounds.setBorderPainted(false);
        bAxesBounds.setContentAreaFilled(false);
        bAxesBounds.setMaximumSize(new Dimension(16, 16));
        bAxesBounds.setMinimumSize(new Dimension(16, 16));
        bAxesBounds.setPreferredSize(new Dimension(16, 16));
        bAxesBounds.setRolloverEnabled(false);
        bAxesBounds.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bAxesBoundsActionPerformed(evt);
            }
        });

        lAxesBounds.setBackground(new Color(255, 255, 255));
        lAxesBounds.setText(" " + MessagesGED.axes_bounds);
        lAxesBounds.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lAxesBounds.setOpaque(true);
        lAxesBounds.setPreferredSize(new Dimension(70, 20));

        cAxesBounds.setBackground(new Color(255, 255, 255));
        cAxesBounds.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cAxesBounds.setOpaque(true);
        cAxesBounds.setPreferredSize(new Dimension(70, 20));

        lBoundsLeft.setBackground(new Color(255, 255, 255));
        lBoundsLeft.setText(" " + MessagesGED.left);
        lBoundsLeft.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lBoundsLeft.setOpaque(true);
        lBoundsLeft.setPreferredSize(new Dimension(70, 20));

        cBoundsLeft.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cBoundsLeft.setPreferredSize(new Dimension(70, 20));
        cBoundsLeft.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cBoundsLeftActionPerformed(evt);
            }
        });

        lBoundsUp.setBackground(new Color(255, 255, 255));
        lBoundsUp.setText(" " + MessagesGED.up);
        lBoundsUp.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lBoundsUp.setOpaque(true);
        lBoundsUp.setPreferredSize(new Dimension(70, 20));

        cBoundsUp.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cBoundsUp.setPreferredSize(new Dimension(70, 20));
        cBoundsUp.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cBoundsUpActionPerformed(evt);
            }
        });

        lBoundsWidth.setBackground(new Color(255, 255, 255));
        lBoundsWidth.setText(" " + MessagesGED.width);
        lBoundsWidth.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lBoundsWidth.setOpaque(true);
        lBoundsWidth.setPreferredSize(new Dimension(70, 20));

        cBoundsWidth.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cBoundsWidth.setPreferredSize(new Dimension(70, 20));
        cBoundsWidth.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cBoundsWidthActionPerformed(evt);
            }
        });

        lBoundsHeight.setBackground(new Color(255, 255, 255));
        lBoundsHeight.setText(" " + MessagesGED.height);
        lBoundsHeight.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lBoundsHeight.setOpaque(true);
        lBoundsHeight.setPreferredSize(new Dimension(70, 20));

        cBoundsHeight.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cBoundsHeight.setPreferredSize(new Dimension(70, 20));
        cBoundsHeight.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cBoundsHeightActionPerformed(evt);
            }
        });

        titleBounds();

        //Positioning the components of the property: Axes Bounds.
        GroupLayout pAxesBoundsLayout = new GroupLayout(pAxesBounds);
        pAxesBounds.setLayout(pAxesBoundsLayout);
        pAxesBoundsLayout.setHorizontalGroup(
            pAxesBoundsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsLeft, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsLeft, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsUp, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsUp, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsWidth, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsWidth, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsHeight, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsHeight, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        pAxesBoundsLayout.setVerticalGroup(
            pAxesBoundsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addGroup(pAxesBoundsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsLeft, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsLeft, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxesBoundsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsUp, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsUp, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxesBoundsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsWidth, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsWidth, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxesBoundsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsHeight, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsHeight, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
        
        //Components of the property: Margins.
        bMargins.setSelected(true);
        bMargins.setBorder(null);
        bMargins.setBorderPainted(false);
        bMargins.setContentAreaFilled(false);
        bMargins.setMaximumSize(new Dimension(16, 16));
        bMargins.setMinimumSize(new Dimension(16, 16));
        bMargins.setPreferredSize(new Dimension(16, 16));
        bMargins.setRolloverEnabled(false);
        bMargins.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bMarginsActionPerformed(evt);
            }
        });

        pMargins.setVisible(false);

        lMargins.setBackground(new Color(255, 255, 255));
        lMargins.setText(" " + MessagesGED.margins);
        lMargins.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lMargins.setOpaque(true);
        lMargins.setPreferredSize(new Dimension(70, 20));

        cMargins.setBackground(new Color(255, 255, 255));
        cMargins.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cMargins.setOpaque(true);
        cMargins.setPreferredSize(new Dimension(70, 20));

        lMarginsLeft.setBackground(new Color(255, 255, 255));
        lMarginsLeft.setText(" " + MessagesGED.left);
        lMarginsLeft.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lMarginsLeft.setOpaque(true);
        lMarginsLeft.setPreferredSize(new Dimension(70, 20));

        cMarginsLeft.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cMarginsLeft.setPreferredSize(new Dimension(70, 20));
        cMarginsLeft.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarginsLeftActionPerformed(evt);
            }
        });

        lMarginsRight.setBackground(new Color(255, 255, 255));
        lMarginsRight.setText(" " + MessagesGED.right);
        lMarginsRight.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lMarginsRight.setOpaque(true);
        lMarginsRight.setPreferredSize(new Dimension(70, 20));

        cMarginsRight.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cMarginsRight.setPreferredSize(new Dimension(70, 20));
        cMarginsRight.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarginsRightActionPerformed(evt);
            }
        });

        lMarginsTop.setBackground(new Color(255, 255, 255));
        lMarginsTop.setText(" " + MessagesGED.top);
        lMarginsTop.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lMarginsTop.setOpaque(true);
        lMarginsTop.setPreferredSize(new Dimension(70, 20));

        cMarginsTop.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cMarginsTop.setPreferredSize(new Dimension(70, 20));
        cMarginsTop.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarginsTopActionPerformed(evt);
            }
        });

        lMarginsBottom.setBackground(new Color(255, 255, 255));
        lMarginsBottom.setText(" " + MessagesGED.bottom);
        lMarginsBottom.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lMarginsBottom.setOpaque(true);
        lMarginsBottom.setPreferredSize(new Dimension(70, 20));

        cMarginsBottom.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cMarginsBottom.setPreferredSize(new Dimension(70, 20));
        cMarginsBottom.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarginsBottomActionPerformed(evt);
            }
        });

        titleMargins();

        //Positioning the components of the property: Margins.
        GroupLayout pMarginsLayout = new GroupLayout(pMargins);
        pMargins.setLayout(pMarginsLayout);
        pMarginsLayout.setHorizontalGroup(
            pMarginsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsLeft, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsLeft, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsRight, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsRight, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsTop, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsTop, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsBottom, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsBottom, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        pMarginsLayout.setVerticalGroup(
            pMarginsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addGroup(pMarginsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsLeft, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsLeft, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pMarginsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsRight, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsRight, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pMarginsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsTop, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsTop, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pMarginsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsBottom, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsBottom, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Positioning all the components of the Position.
    */
    private void position() {
        //Positioning all components.
        GroupLayout pPositionLayout = new GroupLayout(pPosition);
        pPosition.setLayout(pPositionLayout);
        pPositionLayout.setHorizontalGroup(
            pPositionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pPositionLayout.createSequentialGroup()
                .addComponent(bAxesBounds, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lAxesBounds, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cAxesBounds, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pPositionLayout.createSequentialGroup()
                .addComponent(bMargins, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lMargins, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                .addGap(4, 4, 4)
                .addComponent(cMargins, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(pPositionLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pPositionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(pAxesBounds, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                    .addComponent(pMargins, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)))
        );
        pPositionLayout.setVerticalGroup(
            pPositionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pPositionLayout.createSequentialGroup()
                .addGroup(pPositionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(bAxesBounds, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(lAxesBounds, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAxesBounds, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addComponent(pAxesBounds, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addGroup(pPositionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(bMargins, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addGroup(pPositionLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addGroup(pPositionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                            .addComponent(lMargins, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
                            .addComponent(cMargins, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE))))
                .addGap(5, 5, 5)
                .addComponent(pMargins, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
        );
    }

    /**
    * Loads the current properties of the section Position.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesPosition(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Axes Bounds */
            Double[] currentBounds = (Double []) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_AXES_BOUNDS__);
            cBoundsLeft.setText(currentBounds[0].toString());
            cBoundsUp.setText(currentBounds[1].toString());
            cBoundsWidth.setText(currentBounds[2].toString());
            cBoundsHeight.setText(currentBounds[3].toString());
            titleBounds();

            /** Get the current status of the property: Margins */
            Double[] currentMargins = (Double []) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_MARGINS__);
            cMarginsLeft.setText(currentMargins[0].toString());
            cMarginsRight.setText(currentMargins[1].toString());
            cMarginsTop.setText(currentMargins[2].toString());
            cMarginsBottom.setText(currentMargins[3].toString());
            titleMargins();
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private final void setIconsPosition() {
        bPosition.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bPosition.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bMargins.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bMargins.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
        bAxesBounds.setIcon(new ImageIcon(SwingInspector.icon_collapse));
        bAxesBounds.setSelectedIcon(new ImageIcon(SwingInspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt ActionEvent.
    */
    private void bLabelActionPerformed(ActionEvent evt) {
        if (bLabel.isSelected()) {
            pLabel.setVisible(false);
            HideAxis.checkAllButtons();
        } else {
            pLabel.setVisible(true);
            HideAxis.checkAllButtons();
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt ActionEvent.
    */
    private void bPositionActionPerformed(ActionEvent evt) {
        if (bPosition.isSelected()) {
            pPosition.setVisible(false);
        } else {
            pPosition.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt ActionEvent.
    */
    private void bAxesBoundsActionPerformed(ActionEvent evt) {
        if (bAxesBounds.isSelected()) {
            pAxesBounds.setVisible(false);
        } else {
            pAxesBounds.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt ActionEvent.
    */
    private void bMarginsActionPerformed(ActionEvent evt) {
        if (bMargins.isSelected()) {
            pMargins.setVisible(false);
        } else {
            pMargins.setVisible(true);
        }
    }

    /**
    * Inserts the current state of functionality: AXES BOUNDS in main label.
    */
    public void titleBounds() {
        String titleBoundsLeft = cBoundsLeft.getText();
        String titleBoundsUp = cBoundsUp.getText();
        String titleBoundsWidth = cBoundsWidth.getText();
        String titleBoundsHeight = cBoundsHeight.getText();
        String titleBounds = titleBoundsLeft + " , " + titleBoundsUp + " , "
                             + titleBoundsWidth + " , " + titleBoundsHeight;
        cAxesBounds.setText(" [" + titleBounds + "]");
    }

    /**
    * Inserts the current state of functionality: MARGINS in main label.
    */
    public void titleMargins() {
        String titleMarginsLeft = cMarginsLeft.getText();
        String titleMarginsRight = cMarginsRight.getText();
        String titleMarginsTop = cMarginsTop.getText();
        String titleMarginsBottom = cMarginsBottom.getText();
        String titleMargins = titleMarginsLeft + " , " + titleMarginsRight + " , "
                             + titleMarginsTop + " , " + titleMarginsBottom;
        cMargins.setText(" [" + titleMargins + "]");
    }

    /**
    * Updates the property: Axes Bounds - Left.
    *
    * @param evt ActionEvent.
    */
    private void cBoundsLeftActionPerformed(ActionEvent evt) {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cBoundsLeft.getText());
        value[1] = Double.parseDouble(cBoundsUp.getText());
        value[2] = Double.parseDouble(cBoundsWidth.getText());
        value[3] = Double.parseDouble(cBoundsHeight.getText());
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_AXES_BOUNDS__, value);
        titleBounds();
    }

    /**
    * Updates the property: Axes Bounds - Up.
    *
    * @param evt ActionEvent.
    */
    private void cBoundsUpActionPerformed(ActionEvent evt) {
        cBoundsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Axes Bounds - Width.
    *
    * @param evt ActionEvent.
    */
    private void cBoundsWidthActionPerformed(ActionEvent evt) {
        cBoundsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Axes Bounds - Height.
    *
    * @param evt ActionEvent.
    */
    private void cBoundsHeightActionPerformed(ActionEvent evt) {
        cBoundsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Margins - Left.
    *
    * @param evt ActionEvent.
    */
    private void cMarginsLeftActionPerformed(ActionEvent evt) {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cMarginsLeft.getText());
        value[1] = Double.parseDouble(cMarginsRight.getText());
        value[2] = Double.parseDouble(cMarginsTop.getText());
        value[3] = Double.parseDouble(cMarginsBottom.getText());
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_MARGINS__, value);

        titleMargins();
    }

    /**
    * Updates the property: Margins - Right.
    *
    * @param evt ActionEvent.
    */
    private void cMarginsRightActionPerformed(ActionEvent evt) {
        cMarginsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Margins - Top.
    *
    * @param evt ActionEvent.
    */
    private void cMarginsTopActionPerformed(ActionEvent evt) {
        cMarginsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Margins - Bottom.
    *
    * @param evt ActionEvent.
    */
    private void cMarginsBottomActionPerformed(ActionEvent evt) {
        cMarginsLeftActionPerformed(evt);
    }
}