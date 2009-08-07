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

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;

import org.scilab.modules.graph.ScilabGraph;

import com.mxgraph.swing.mxGraphComponent;

public class DefaultAction extends AbstractAction {

    private ScilabGraph _scilabGraph = null;
    
    public DefaultAction() { }
    
    public DefaultAction(ScilabGraph scilabGraph) {
	_scilabGraph = scilabGraph;
    }
      
    public void actionPerformed(ActionEvent e) {
	  JOptionPane.showMessageDialog(null, "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
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

    protected static JButton createButton(String title, String icon, ActionListener listener) {
	JButton button = new JButton(); 
	button.addActionListener(listener);
	button.setText(title);
	button.setToolTipText(title);
	button.setIcon(new ImageIcon(ScilabGraph.class.getResource(icon)));
	
	return button;
    }
    
    protected static JMenuItem createMenu(String title, String icon, ActionListener listener) {
	JMenuItem menu = new JMenuItem();
	menu.addActionListener(listener);
	menu.setText(title);
	menu.setToolTipText(title);
	menu.setIcon(new ImageIcon(ScilabGraph.class.getResource(icon)));
	
	return menu;
    }
}
