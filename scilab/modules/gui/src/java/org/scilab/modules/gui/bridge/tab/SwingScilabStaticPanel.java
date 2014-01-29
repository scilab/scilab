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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFO_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagLayout;
import java.awt.GridLayout;

import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.events.callback.ScilabCloseCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.ToolBarBuilder;

public class SwingScilabStaticPanel extends JPanel implements SwingScilabPanel {

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

    /** Scroll the axes */
    private SwingScilabScrollPane scrolling;
    
    public SwingScilabStaticPanel(String figureTitle, Integer figureId, Figure figure) {
        super();
        setId(figureId);
        setVisible(true);
        setLayout(new BorderLayout());
        
        //layerdPane = new JLayeredPane();
        //layerdPane.setLayout(null);
        //layerdPane.add(canvas, JLayeredPane.FRAME_CONTENT_LAYER);
        uiContentPane = new JLayeredPane();
        uiContentPane.setOpaque(false);
        uiContentPane.setLayout(null);
        add(uiContentPane, BorderLayout.CENTER);

        //scrolling = new SwingScilabScrollPane(layerdPane, null, uiContentPane, figure);
        //add(scrolling, BorderLayout.CENTER);
        //scrolling.setSize(500, 500);
        //add(uiContentPane);
        //layerdPane.setVisible(true);
        //scrolling.setVisible(true);
        uiContentPane.setVisible(true);
        //setSize(500, 500);
        //setPreferredSize(new Dimension(500, 500));
        setVisible(true);
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
        return this;
    }
    
    public void close() {
    }
    
}
