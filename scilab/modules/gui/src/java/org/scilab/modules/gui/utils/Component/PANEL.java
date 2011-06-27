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
import javax.swing.JPanel;
import org.w3c.dom.Node;
import org.scilab.modules.gui.utils.XConfigManager;
import java.awt.Color;
import java.awt.BorderLayout;

import javax.swing.BorderFactory;
import javax.swing.border.Border;

public class PANEL extends JPanel implements XComponent {

	public String [] actuators() {
		String [] actuators = {};
		return actuators;
	}

	public PANEL(Node peer) {
		super();
		setLayout(new BorderLayout());
		XConfigManager.setDimension(this, peer);
		XConfigManager.drawConstructionBorders(this);
    }

	public void refresh(Node peer) {}

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
		String signature = "PANEL";

		return signature;
	}

}

