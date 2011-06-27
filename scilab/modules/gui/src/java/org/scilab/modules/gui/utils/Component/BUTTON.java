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
import javax.swing.JButton;
import org.w3c.dom.Node;

public class BUTTON extends JButton implements XComponent {

	static String NAV = "<<none>>";

	public String [] actuators() {
		String [] actuators = {"text"};
		return actuators;
	}

	public BUTTON(Node peer) {
		super();
		String text = XConfigManager.getAttribute(peer , "text", NAV);
		setText(text);
	}

	public void refresh(Node peer) {
		String text = XConfigManager.getAttribute(peer , "text", NAV);
		if (!text.equals(text())) text(text);		
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S E N S O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String text() {
		return getText();
	}		

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       A C T U A T O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


	public void text(String xText) {
		setText(xText);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S I G N A T U R E
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String toString() {
		String signature = "BUTTON";

		if (!text().equals(XConfigManager.NAV))
			signature += " text='" + text() + "'";		

		return signature;
	}


}

