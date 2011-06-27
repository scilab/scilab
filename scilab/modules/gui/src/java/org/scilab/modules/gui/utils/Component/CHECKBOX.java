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
import org.scilab.modules.gui.utils.XChooser;
import org.scilab.modules.gui.utils.XConfigManager;
import javax.swing.JCheckBox;
import org.w3c.dom.Node;

public class CHECKBOX extends JCheckBox implements XComponent, XChooser {

	public String [] actuators() {
		String [] actuators = {};
		return actuators;
	}

	public CHECKBOX(Node peer) {
		super();
		refresh(peer);
	}
	public void refresh(Node peer) {
		String checked  = XConfigManager.getAttribute(peer , "checked");
		boolean state =  checked.equals("checked");
		setSelected(state);
	}

	public String choose() {
		if (isSelected())
			return "checked";
		return "unchecked";
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S E N S O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       A C T U A T O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S I G N A T U R E
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String toString() {
		String signature = "CHECKBOX";
		return signature;
	}


}

