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

import java.awt.Color;
import java.awt.Container;

import javax.swing.JLayeredPane;

import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;

public interface SwingScilabPanel extends SwingViewObject {

    public TextBox getInfoBar();
    public void setInfoBar(TextBox infoBar);
    public MenuBar getMenuBar();
    public void setMenuBar(MenuBar menuBar);
    public ToolBar getToolBar();
    public void setToolBar(ToolBar toolBar);

    public void setName(String name);
    public void setEventHandler(String name);
    public void setEventHandlerEnabled(boolean enabled);

    public void setParentWindowId(String parentWindowId);
    public String getParentWindowId();

    public SwingScilabWindow getParentWindow();
    public Container getContentPane();
    public JLayeredPane getWidgetPane();

    public void setVisible(boolean isVisible);
    public void setWindowIcon(String windowIcon);

    public void addMember(SwingViewObject member);
    public void removeMember(SwingViewObject member);

    public void revalidate();

    public void setCallback(CommonCallBack callback);

    public void close();
    public void setFigureBackground(Color color);
    public void setHasLayout(boolean hasLayout);

    public void applyDeltaSize();
    public void storeSizeDelta();

    public void disableResizeEvent();
    public void enableResizeEvent();
}
