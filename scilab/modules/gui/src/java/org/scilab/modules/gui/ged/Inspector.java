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
package org.scilab.modules.gui.ged;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JToggleButton;
import javax.swing.LayoutStyle;
import javax.swing.SwingConstants;
import javax.swing.ScrollPaneConstants;
import javax.swing.WindowConstants;

import org.scilab.modules.gui.ged.axes.HideAxis;
import org.scilab.modules.gui.ged.curve.HideCurve;
import org.scilab.modules.gui.ged.figure.HideFigure;

/**
* Main properties window - Light GED.
* Receive the classes of graphic objects (JPanels).
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Inspector extends JFrame {
    public static String selectaux;
    public static Inspector window;
    public static JPanel pReceive;
    public static JToggleButton bHideShow;
    private JScrollPane desktop;
    private JPanel pMenu;
    private static String imagepath = System.getenv("SCI") + "/modules/gui/images/icons/";
    public static String icon_color_fill = imagepath + "16x16/actions/color-fill.png";
    public static String icon_collapse = imagepath + "16x16/actions/tree-collapse.png";
    public static String icon_expand = imagepath + "16x16/actions/media-playback-start.png";
    public static String icon_collapse_all = imagepath + "32x32/actions/tree-diagramm.png";
    public static String icon_expand_all = imagepath + "32x32/actions/tree-diagramm-delete.png";

    /**
    * Constructor - Initializes the inspection window.
    *
    * @param select Indicates which property window will open.
    * @param objectID Enters the identification of object.
    */
    public Inspector(String select , String objectID) {
        guiComponents();
        setIcons();
        position();
        initLayout();
        new SwapObject(select, objectID, 1, 1);
        selectaux = select;
    }

    /**
    * If lightGED not yet started it creates a JFrame.
    *
    * @param select Indicates which property window will open initially.
    * @param objectID Enters the identification of object.
    * @return Shows the lightGED was generated.
    */
    public static Inspector createGuiInspector(String select , String objectID) {
        if (window == null) {
            window = new Inspector(select, objectID);
        }
        return window;
    }

    /**
    * Manages the operation of the button show/hide for the current object.
    *
    * @param evt ActionEvent
    */
    private void bHideShowActionPerformed(ActionEvent evt) {
        if (selectaux.equals("axes")) {
            if (bHideShow.isSelected()) {
                new HideAxis(true);
            } else {
                new HideAxis(false);
            }
        } else {
            if (selectaux.equals("curve")) {
                if (bHideShow.isSelected()) {
                    new HideCurve(true);
                } else {
                    new HideCurve(false);
                }
            } else {
                if (bHideShow.isSelected()) {
                    new HideFigure(true);
                } else {
                    new HideFigure(false);
                }
            }
        }
    }

    /**
    * It has all the components of the main window.
    */
    private void guiComponents() {
        pMenu = new JPanel();
        bHideShow = new JToggleButton();
        desktop = new JScrollPane();
        pReceive = new JPanel();

        setVisible(true);
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        /**
        * Implements an event button to close the window.
        * Resets the variable responsible for identifying if the window is open.
        */
        addWindowListener(new WindowListener() {
        public void windowClosing(WindowEvent evt) {
            window = null;
        }
        public void windowClosed(WindowEvent evt) {
            window = null;
        }
        public void windowOpened(WindowEvent evt) { }
        public void windowIconified(WindowEvent evt) { }
        public void windowDeiconified(WindowEvent evt) { }
        public void windowActivated(WindowEvent evt) { }
        public void windowDeactivated(WindowEvent evt) { }
        });

        setResizable(false);

        pMenu.setBorder(BorderFactory.createEtchedBorder());
        pMenu.setMaximumSize(new java.awt.Dimension(370, 30));
        pMenu.setMinimumSize(new java.awt.Dimension(370, 30));
        pMenu.setPreferredSize(new java.awt.Dimension(370, 30));

        bHideShow.setToolTipText(MessagesGED.collapse_expand);
        bHideShow.setAlignmentY(0.0F);
        bHideShow.setContentAreaFilled(false);
        bHideShow.setHorizontalTextPosition(SwingConstants.CENTER);
        bHideShow.setMaximumSize(new Dimension(20, 20));
        bHideShow.setMinimumSize(new Dimension(20, 20));
        bHideShow.setPreferredSize(new Dimension(20, 20));
        bHideShow.setRolloverEnabled(false);
        bHideShow.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bHideShowActionPerformed(evt);
            }
        });

        desktop.setBorder(null);
        desktop.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        desktop.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        desktop.setAlignmentX(0.0F);
        desktop.setAlignmentY(0.0F);
        desktop.setPreferredSize(new java.awt.Dimension(370, 584));

        pReceive.setAlignmentX(0.0F);
        pReceive.setAlignmentY(0.0F);
        pReceive.setPreferredSize(new Dimension(370, 780));

        desktop.setViewportView(pReceive);
    }

    /**
    * Positioning all the components of the Inspector.
    */
    private void position() {
        //Positioning the components of the menu.
        GroupLayout pMenuLayout = new GroupLayout(pMenu);
        pMenu.setLayout(pMenuLayout);
        pMenuLayout.setHorizontalGroup(
            pMenuLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pMenuLayout.createSequentialGroup()
                .addGap(4, 4, 4)
                .addComponent(bHideShow, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
        );
        pMenuLayout.setVerticalGroup(
            pMenuLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(pMenuLayout.createSequentialGroup()
                .addGap(2, 2, 2)
                .addComponent(bHideShow, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
        );

        //Positioning the inner panel
        GroupLayout pReceiveLayout = new GroupLayout(pReceive);
        pReceive.setLayout(pReceiveLayout);
        pReceiveLayout.setHorizontalGroup(
            pReceiveLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGap(0, 370, Short.MAX_VALUE)
        );
        pReceiveLayout.setVerticalGroup(
            pReceiveLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGap(0, 780, Short.MAX_VALUE)
        );
    }

    /**
    * Creates layout of all components.
    */
    private void initLayout() {
        GroupLayout layout = new GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addComponent(pMenu, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(desktop, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(pMenu, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(desktop, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        pack();
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIcons() {
        bHideShow.setIcon(new ImageIcon(icon_collapse_all));
        bHideShow.setSelectedIcon(new ImageIcon(icon_expand_all));
    }
}