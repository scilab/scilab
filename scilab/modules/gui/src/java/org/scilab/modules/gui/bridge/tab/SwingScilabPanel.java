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

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.SwingViewObject;

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
    public void setVisible(boolean isVisible);
    public void setWindowIcon(String windowIcon);
    public void addMember(SwingViewObject member);
    public void revalidate();
}
