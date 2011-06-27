/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils.Component;

import org.scilab.modules.gui.utils.XComponent;
import org.scilab.modules.gui.utils.XConfigManager;
import javax.swing.JLabel;
import java.awt.Color;
import org.w3c.dom.Node;

public class LABEL extends JLabel implements XComponent {

	public String [] actuators() {
		String [] actuators = {"text", "foreground", "background"};
		return actuators;
	}

	public LABEL(Node peer) {
		super();

		String text = XConfigManager.getAttribute(peer , "text");
		text(text);

		String background = XConfigManager.getAttribute(peer , "background");
		background(background);

		String foreground = XConfigManager.getAttribute(peer , "foreground");
		foreground(foreground);
	}

	public void refresh(Node peer) {

		String text = XConfigManager.getAttribute(peer , "text");
		if (!text.equals(text())) text(text);		

		String background = XConfigManager.getAttribute(peer , "background");
		if (!background.equals(foreground())) background(background);		

		String foreground = XConfigManager.getAttribute(peer , "foreground");
		if (!foreground.equals(foreground())) foreground(foreground);		
		
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S E N S O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String text() {
		return getText();
	}		

	public String foreground() {
		if (getForeground().equals(getParent().getForeground())) {
			return XConfigManager.NAV;
		} else {
			return XConfigManager.getColor(getForeground());
		}
	}

	public String background() {
		if (getForeground().equals(getParent().getBackground())) {
			return XConfigManager.NAV;
		} else {
			return XConfigManager.getColor(getBackground());
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       A C T U A T O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public void text(String actuator) {
		setText(actuator);
	}		

	public void foreground(String actuator) {
		if (actuator.equals(XConfigManager.NAV)) {
			setForeground(null);
		} else {
			Color color = XConfigManager.getColor(actuator);
			//System.err.println("foreground:" + actuator + " = " + color);
			setForeground(color);			
		}
	}

	public void background(String actuator) {
		if (actuator.equals(XConfigManager.NAV)) {
			setOpaque(false);
			setBackground(null);
		} else {
			Color color = XConfigManager.getColor(actuator);
			//System.err.println("background:" + actuator + " = " + color);
			setOpaque(true);
			setBackground(color);
		}		
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S I G N A T U R E
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String toString() {
		String signature = "LABEL";

		if (!text().equals(XConfigManager.NAV))
			signature += " text='" + text() + "'";		

		if (!background().equals(XConfigManager.NAV))
			signature += " background='" + background() + "'";	
	
		if (!foreground().equals(XConfigManager.NAV))
			signature += " foreground='" + foreground() + "'";	
		
		return signature;
	}

}

