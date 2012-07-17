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
package org.scilab.modules.gui.ged.figure;

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
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Control.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Control extends BaseProperties {
    protected static JToggleButton bControl;
    protected static JPanel pControl;
    protected JLabel lControl;
    protected JSeparator sControl;
    protected JComboBox cAutoResize;
    protected JLabel lAutoResize;
    protected JLabel lxposition;
    protected JTextField cxposition;
    protected JLabel lyposition;
    protected JTextField cyposition;
    protected JTextField cxsize;
    protected JLabel lxsize;
    protected JTextField cysize;
    protected JLabel lysize;


    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of Figure.
    */
    public Control(String objectID) {
        super(objectID);
        position();
        setIconsControl();
        initPropertiesControl(objectID);
    }

    /**
    * It has all the components of the section Control.
    */
    @Override
    public void controlComponents() {
        bControl = new JToggleButton();
        lControl = new JLabel();
        sControl = new JSeparator();
        pControl = new JPanel();
        lAutoResize = new JLabel();
        cAutoResize = new JComboBox();
        lxposition = new JLabel();
        cxposition = new JTextField();
        lyposition = new JLabel();
        cyposition = new JTextField();
        lxsize = new JLabel();
        cxsize = new JTextField();
        lysize = new JLabel();
        cysize = new JTextField();

        //Components of the header: Control.
        pControl.setAlignmentX(0.0F);
        pControl.setAlignmentY(0.0F);

        bControl.setBorder(null);
        bControl.setBorderPainted(false);
        bControl.setContentAreaFilled(false);
        bControl.setMaximumSize(new Dimension(16, 16));
        bControl.setMinimumSize(new Dimension(16, 16));
        bControl.setPreferredSize(new Dimension(16, 16));
        bControl.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bControlActionPerformed(evt);
            }
        });

        lControl.setText(MessagesGED.control);

        sControl.setPreferredSize(new Dimension(50, 2));

        //Components of the property: Auto Resize.
        lAutoResize.setBackground(new Color(255, 255, 255));
        lAutoResize.setText(" " + MessagesGED.auto_resize);
        lAutoResize.setAlignmentX(0.5F);
        lAutoResize.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lAutoResize.setOpaque(true);
        lAutoResize.setPreferredSize(new Dimension(153, 20));

        cAutoResize.setModel(new DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cAutoResize.setBorder(null);
        cAutoResize.setEditor(null);
        cAutoResize.setPreferredSize(new Dimension(150, 20));
        cAutoResize.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cAutoResizeActionPerformed(evt);
            }
        });

	//Components of the property: Position X.
        lxposition.setBackground(new Color(255, 255, 255));
        lxposition.setText(" " + MessagesGED.x_position);
        lxposition.setAlignmentX(0.5F);
        lxposition.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lxposition.setOpaque(true);
        lxposition.setPreferredSize(new Dimension(153, 20));

        cxposition.setToolTipText(MessagesGED.x_position_tooltip);
        cxposition.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cxposition.setPreferredSize(new Dimension(150, 20));
        cxposition.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cxpositionActionPerformed(evt);
            }
        });

        //Components of the property: Position Y.
        lyposition.setBackground(new Color(255, 255, 255));
        lyposition.setText(" " + MessagesGED.y_position);
        lyposition.setAlignmentX(0.5F);
        lyposition.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lyposition.setOpaque(true);
        lyposition.setPreferredSize(new Dimension(153, 20));

        cyposition.setToolTipText(MessagesGED.y_position_tooltip);
        cyposition.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cyposition.setPreferredSize(new Dimension(150, 20));
        cyposition.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cypositionActionPerformed(evt);
            }
        });

	//Components of the property: Size X.
        lxsize.setBackground(new Color(255, 255, 255));
        lxsize.setText(" " + MessagesGED.x_size);
        lxsize.setAlignmentX(0.5F);
        lxsize.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lxsize.setOpaque(true);
        lxsize.setPreferredSize(new Dimension(153, 20));

        cxsize.setToolTipText(MessagesGED.x_size_tooltip);
        cxsize.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cxsize.setPreferredSize(new Dimension(150, 20));
        cxsize.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cxsizeActionPerformed(evt);
            }
        });

	//Components of the property: Size Y.
        lysize.setBackground(new Color(255, 255, 255));
        lysize.setText(" " + MessagesGED.y_size);
        lysize.setAlignmentX(0.5F);
        lysize.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        lysize.setOpaque(true);
        lysize.setPreferredSize(new Dimension(153, 20));

        cysize.setToolTipText(MessagesGED.y_size_tooltip);
        cysize.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0)));
        cysize.setPreferredSize(new Dimension(150, 20));
        cysize.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cysizeActionPerformed(evt);
            }
        });
   }

    /**
    * Positioning all the components of the Control.
    */
    private void position() {
        GroupLayout pControlLayout = new GroupLayout(pControl);
        pControl.setLayout(pControlLayout);
        pControlLayout.setHorizontalGroup(
            pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pControlLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lAutoResize, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cAutoResize, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lxposition, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cxposition, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lyposition, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cyposition, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lxsize, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cxsize, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lysize, GroupLayout.PREFERRED_SIZE, 159, GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cysize, GroupLayout.PREFERRED_SIZE, 157, GroupLayout.PREFERRED_SIZE))))
        );
        pControlLayout.setVerticalGroup(
            pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pControlLayout.createSequentialGroup()
                .addGroup(pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lAutoResize, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAutoResize, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lxposition, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cxposition, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lyposition, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cyposition, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lxsize, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cxsize, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addComponent(lysize, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(cysize, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Loads the current properties of the section Control.
    *
    * @param objectID Enters the identification of Figure.
    */
    public void initPropertiesControl(String objectID) {
        if (objectID != null) {
            currentfigure = objectID;
            /** Get the current status of the property: Auto Resize */
            boolean isAutoResize = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_AUTORESIZE__);
            if (isAutoResize) {
                cAutoResize.setSelectedIndex(1);
            } else {
		cAutoResize.setSelectedIndex(0);
            }
            /** Get the current status of the property: Position */
            Integer[] currentPosition = (Integer[]) GraphicController.getController()
                        .getProperty(currentfigure, GraphicObjectProperties.__GO_POSITION__);
            String currentXposition;
            currentXposition = currentPosition[0].toString();
            cxposition.setText(currentXposition);
            String currentYposition;
            currentYposition = currentPosition[1].toString();
            cyposition.setText(currentXposition);
            /** Get the current status of the property: Size */
            Integer[] currentSize = (Integer[]) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_SIZE__);
            String currentXsize;
            currentXsize = currentSize[0].toString();
            cxsize.setText(currentXsize);
            String currentYsize;
            currentYsize = currentSize[1].toString();
            cysize.setText(currentXsize);
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsControl() {
        bControl.setIcon(new ImageIcon(Inspector.icon_collapse));
        bControl.setSelectedIcon(new ImageIcon(Inspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt ActionEvent.
    */
    private void bControlActionPerformed(ActionEvent evt) {
        if (bControl.isSelected()) {
            pControl.setVisible(false);
        } else {
            pControl.setVisible(true);
        }
    }

    /**
    * Updates the property: Auto Resize.
    *
    * @param evt ActionEvent.
    */
    private void cAutoResizeActionPerformed(ActionEvent evt) {
        boolean setAutoResize = true;
        if (cAutoResize.getSelectedIndex() == 0) {
            setAutoResize = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_AUTORESIZE__, setAutoResize);
    }

    /**
    * Updates the property: xPosition.
    *
    * @param evt ActionEvent.
    */
    private void cxpositionActionPerformed(ActionEvent evt) {
        int setXposition = Integer.parseInt(cxposition.getText());
        int setYposition = Integer.parseInt(cyposition.getText());
        Integer[] setPosition = new Integer[2];
        setPosition[0] = setXposition;
        setPosition[1] = setYposition;
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_POSITION__, setPosition);
    }

    /**
    * Updates the property: yPosition.
    *
    * @param evt ActionEvent.
    */
    private void cypositionActionPerformed(ActionEvent evt) {
        cxpositionActionPerformed(evt);
    }

    /**
    * Updates the property: xSize.
    *
    * @param evt ActionEvent.
    */
    private void cxsizeActionPerformed(ActionEvent evt) {
        int setXsize = Integer.parseInt(cxsize.getText());
        int setYsize = Integer.parseInt(cysize.getText());
        Integer[] setSize = new Integer[2];
        setSize[0] = setXsize;
        setSize[1] = setYsize;
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_SIZE__, setSize);
    }

    /**
    * Updates the property: ySize.
    *
    * @param evt ActionEvent.
    */
    private void cysizeActionPerformed(ActionEvent evt) {
        cxsizeActionPerformed(evt);
    }
}