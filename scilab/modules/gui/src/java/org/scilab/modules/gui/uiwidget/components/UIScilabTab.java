/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Color;
import java.awt.Font;
import java.util.Map;

import javax.swing.Action;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.flexdock.docking.DockingManager;
import org.flexdock.docking.event.DockingEvent;
import org.flexdock.view.Titlebar;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UILocator;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

public class UIScilabTab extends UIComponent {

    protected SwingScilabTab tab;
    private boolean isClosed;
    private boolean isUndocking;

    public UIScilabTab(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        tab = new MyTab("");

        return tab;
    }

    @UIComponentAnnotation(attributes = {"name"})
    public Object newInstance(String name) {
        tab = new MyTab(name == null ? "" : name);

        return tab;
    }

    public void add(final UIComponent comp) throws UIWidgetException {
        if (comp instanceof UIToolBar) {
            tab.setToolBar(((UIToolBar) comp).bar);
        } else if (comp instanceof UIInfoBar) {
            tab.setInfoBar(((UIInfoBar) comp).info);
        } else if (comp instanceof UIMenuBar) {
            tab.setMenuBar(((UIMenuBar) comp).bar);
        } else if (comp instanceof UIButton) {
            UIButton button = (UIButton) comp;
            Action action = button.getAction();
            tab.getTitlebar().removeAction((String) action.getValue(Action.NAME));
            tab.addAction(action);
            action.putValue("TITLEBAR", tab.getTitlebar());
        } else if (comp.getComponent() instanceof JComponent) {
            tab.setContentPane(comp.getJComponent());
        } else {
            super.add(comp);
        }
    }

    public void setVisible(boolean b) {
        SwingScilabWindow win = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, tab);
        if (win == null && b) {
            win = new SwingScilabWindow();
            win.addTab(tab);
            win.setVisible(true);
        } else {
            tab.setVisible(b);
        }
    }

    public void closeUIComponent() {
        if (!isClosed) {
            DockingManager.close(tab);
            tab.close();
        }
    }

    public void remove() {
        if (!isUndocking) {
            super.remove();
        }
    }

    private class MyTab extends SwingScilabTab {

        public MyTab(String name) {
            super(name);
        }

        /*	public void undockingComplete(DockingEvent e) {
        	    super.undockingComplete(e);
        	    isUndocking = true;
        	}

        	public void dockingComplete(DockingEvent e) {
        	    super.dockingComplete(e);
        	    if (UIScilabTab.this.getParent() instanceof UIScilabWindow) {
        		UIComponent ui = UILocator.findWindow(UIScilabTab.this);
        		if (ui != null && ui != UIScilabTab.this.getParent()) {
        		    try {
        			UIScilabTab.this.setParent(ui);
        		    } catch (UIWidgetException ee) {
        			System.err.println(ee);
        		    }
        		} else if (ui == null && isUndocking) {
        		    SwingScilabWindow win = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
        		    if (win != null) {
        			try {
        			    UIScilabTab.this.setParent((UIComponent) null);
        			} catch (UIWidgetException ee) {
        			    System.err.println(ee);
        			}
        		    } else {
        			isUndocking = false;
        	    		UIScilabTab.this.remove();
        		    }
        		}
        	    }

        	    isUndocking = false;
        	    }*/

        public void close() {
            super.close();
            isClosed = true;
            UIScilabTab.this.remove();
        }
    }
}
