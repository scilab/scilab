/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

import com.mxgraph.swing.mxGraphComponent;

public class DefaultAction extends CallBack {

    private ScilabGraph _scilabGraph = null;

    public DefaultAction(ScilabGraph scilabGraph) {
    	super("Default...");
    	_scilabGraph = scilabGraph;
    }

    protected DefaultAction(String label, ScilabGraph scilabGraph) {
    	super(label);
    	_scilabGraph = scilabGraph;
        }

    /**
     * 
     * @param e
     * @return Returns the graph for the given action event.
     */
    protected final ScilabGraph getGraph(ActionEvent e)
    {
	if (_scilabGraph != null) {
	    return _scilabGraph;
	}

	if (e.getSource() instanceof Component)
	{
	    Component component = (Component) e.getSource();

	    while (component != null
		    && !(component instanceof mxGraphComponent))
	    {
		component = component.getParent();
	    }

	    return (ScilabGraph) ((mxGraphComponent) component).getGraph();
	}

	return null;
    }

    
    protected static PushButton createButton(String title, String icon, ActionListener listener) {
	PushButton button = ScilabPushButton.createPushButton(); 
	((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(listener);
	if (icon == null) {
	    button.setText(title);
	    button.setToolTipText(title);
	}
	else {
	    ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(new ImageIcon(System.getenv("SCI")+"/modules/xcos/images/icons/"+icon));
	}
	return button;
    }

    protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
    	MenuItem menu = ScilabMenuItem.createMenuItem();
    	menu.setCallback(listener);
    	menu.setText(title);

    	if (keyStroke != null) {
    		((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
    	}
	
    	return menu;
    }
    
    public void doAction() {
    	JOptionPane.showMessageDialog(getGraph(null).getAsComponent(), "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
    }

    public void callBack() {
    	doAction();
        }

}
