/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
