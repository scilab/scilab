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
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.ScilabCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Default action for a Scilab Graph
 * @author Bruno JOFFRET
 */
public class DefaultAction extends CallBack {

	private static final long serialVersionUID = 1L;

	private ScilabGraph scilabGraph;

	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public DefaultAction(ScilabGraph scilabGraph) {
		super("Default...");
		this.scilabGraph = scilabGraph;
	}

	/**
	 * Constructor
	 * @param label action descriptor
	 * @param scilabGraph associated Scilab Graph
	 */
	protected DefaultAction(String label, ScilabGraph scilabGraph) {
		super(label);
		this.scilabGraph = scilabGraph;
	}

	/**
	 * Get associated graph
	 * @param e event
	 * @return Returns the graph for the given action event.
	 */
	protected final ScilabGraph getGraph(ActionEvent e) {
		if (this.scilabGraph != null) {
			return this.scilabGraph;
		}
		
		if (e == null) {
			return null;
		}

		if (e.getSource() instanceof Component) {
			Component component = (Component) e.getSource();

			while (component != null && !(component instanceof mxGraphComponent)) {
				component = component.getParent();
			}

			return (ScilabGraph) ((mxGraphComponent) component).getGraph();
		}

		return null;
	}


	/**
	 * Create a button for a graph toolbar
	 * @param title name/tooltip of the button
	 * @param icon the path the an icon file
	 * @param listener action listener associated
	 * @return the button
	 */
	protected static PushButton createButton(String title, String icon, ActionListener listener) {
		PushButton button = ScilabPushButton.createPushButton(); 
		((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(listener);
		if (icon == null) {
			button.setText(title);
			button.setToolTipText(title);
		} else {
			((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(
					new ImageIcon(System.getenv("SCI") + "/modules/xcos/images/icons/" + icon));
		}
		((SwingScilabPushButton) button.getAsSimplePushButton()).setToolTipText(title);
		return button;
	}

	/**
	 * Create a button for a graph toolbar
	 * @param title label of the menu
	 * @param icon the path the an icon file
	 * @param listener action listener associated
	 * @param keyStroke menu shortcut
	 * @return the button
	 */
	protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
		MenuItem menu = ScilabMenuItem.createMenuItem();
		menu.setCallback(listener);
		menu.setText(title);

		if (keyStroke != null) {
			((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
		}

		return menu;
	}
	
	/**
	 * Create a button for a graph toolbar
	 * @param title label of the menu
	 * @param icon the path the an icon file
	 * @param listener action listener associated
	 * @param keyStroke menu shortcut
	 * @return the button
	 */
	protected static CheckBoxMenuItem createCheckBoxMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
		CheckBoxMenuItem menu = ScilabCheckBoxMenuItem.createCheckBoxMenuItem();
		menu.setCallback(listener);
		menu.setText(title);

		if (keyStroke != null) {
			((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
		}

		return menu;
	}

	/**
	 * Action associated
	 */
	public void doAction() {
		JOptionPane.showMessageDialog(getGraph(null).getAsComponent(), "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * Action associated
	 */
	public void callBack() {
		doAction();
	}

}
