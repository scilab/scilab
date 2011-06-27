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
import javax.swing.JPanel;
import org.w3c.dom.Node;
import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import java.awt.Color;
import java.awt.BorderLayout;
import java.awt.Component;

public class TITLE extends JPanel implements XComponent {

	public String [] actuators() {
		String [] actuators = {};
		return actuators;
	}

	public Component getSlot() {
		return this;}

	public TITLE(Node peer) {
		super();
		String       text  = XConfigManager.getAttribute(peer , "text");
		TitledBorder title = BorderFactory.createTitledBorder(text);
		setBorder(title);
		XConfigManager.setDimension(this, peer);
		setLayout(new BorderLayout());

		String background = XConfigManager.getAttribute(peer, "background");
		if (!(background.equals(XConfigManager.NAV))) {
			Color color = XConfigManager.getColor(background);
			setOpaque(true);
			setBackground(color);
		}

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
		String signature = "TITLE";

		return signature;
	}

}

