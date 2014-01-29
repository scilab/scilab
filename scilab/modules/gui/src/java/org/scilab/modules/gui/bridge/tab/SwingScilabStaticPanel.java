/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.JLayeredPane;
import javax.swing.SwingUtilities;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Size;

public class SwingScilabStaticPanel extends SwingScilabScrollPane implements SwingScilabPanel {

    private Integer id;
    private TextBox infoBar;
    private MenuBar menuBar;
    private ToolBar toolBar;
    private String eventHandler;
    private boolean eventHandlerEnabled;
    private String parentWindowId;
    private String windowIcon;
    
    private JLayeredPane uiContentPane;
    private JLayeredPane layerdPane;

    public SwingScilabStaticPanel(String figureTitle, Integer figureId, Figure figure) {
        super(new JLayeredPane(), new JLayeredPane(), figure);
        uiContentPane = (JLayeredPane) getUIComponent();
        layerdPane = (JLayeredPane) getGlobalComponent();
        setVisible(true);
        
        layerdPane.setLayout(null);
        layerdPane.setOpaque(false);
        
        uiContentPane.setOpaque(true);
        uiContentPane.setLayout(null);
        layerdPane.add(uiContentPane, JLayeredPane.DEFAULT_LAYER + 1, 0);

        layerdPane.setVisible(true);
        uiContentPane.setVisible(true);
        
        /* Manage figure_size property */
        addComponentListener(new ComponentListener() {

            public void componentShown(ComponentEvent arg0) {
            }

            public void componentResized(ComponentEvent arg0) {

                /* Update the figure_size property */
                Size parentSize =  SwingScilabWindow.allScilabWindows.get(parentWindowId).getDims();
                Integer[] newSize = new Integer[] {parentSize.getWidth(), parentSize.getHeight()};

                GraphicController.getController().setProperty(id, __GO_SIZE__, newSize);

                Boolean autoreSize = (Boolean) GraphicController.getController().getProperty(id, __GO_AUTORESIZE__);

                if (autoreSize != null && autoreSize) {
                    /* Update the axes_size property */
                    Integer[] newAxesSize = new Integer[] {getContentPane().getWidth(), getContentPane().getHeight()};
                    GraphicController.getController().setProperty(id, __GO_AXES_SIZE__, newAxesSize);
                }
            }

            public void componentMoved(ComponentEvent arg0) {
            }

            public void componentHidden(ComponentEvent arg0) {
            }
        });

    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getId() {
        return id;
    }

    public void update(int property, Object value) {
        SwingScilabCommonPanel.update(this, property, value);
    }

    public TextBox getInfoBar() {
        return infoBar;
    }

    public void setInfoBar(TextBox infoBar) {
        this.infoBar = infoBar;
    }

    public MenuBar getMenuBar() {
        return menuBar;
    }

    public void setMenuBar(MenuBar menuBar) {
        this.menuBar = menuBar;
    }

    public ToolBar getToolBar() {
        return toolBar;
    }

    public void setToolBar(ToolBar toolBar) {
        this.toolBar = toolBar;
    }

    public void setEventHandler(String eventHandler) {
        this.eventHandler = eventHandler;
    }

    public void setEventHandlerEnabled(boolean enabled) {
        this.eventHandlerEnabled = enabled;
    }

    public void setParentWindowId(String parentWindowId) {
        this.parentWindowId = parentWindowId;
    }

    public void setWindowIcon(String windowIcon) {
        this.windowIcon = windowIcon;
    }

    public void addMember(SwingViewObject member) {
        SwingScilabCommonPanel.addMember(this, member);
    }

    public String getParentWindowId() {
        return parentWindowId;
    }

    public SwingScilabWindow getParentWindow() {
        return (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
    }


    public JLayeredPane getWidgetPane() {
        return uiContentPane;
    }

    public void setCallback(CommonCallBack callback) {
        // TODO Auto-generated method stub
        
    }

    public Container getContentPane() {
        return this.getAsContainer();
    }
    
    public void close() {
    }

    
}
