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
import javax.swing.ImageIcon;
import org.w3c.dom.Node;

public class ICON extends JLabel implements XComponent {

    private static final   String   SCILAB_XICONS  = System.getenv("SCI") 
		+ "/modules/gui/images/icons/16x16/actions/";

	public String [] actuators() {
		String [] actuators = {"src"};
		return actuators;
	}

	public ICON(Node peer) {
		super();

		String src = XConfigManager.getAttribute(peer , "src");
		src(src);
	}


	public void refresh(Node peer) {

		String src = XConfigManager.getAttribute(peer , "src");
		if (!src.equals(src())) src(src);		

	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S E N S O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String src() {
		ImageIcon icon = (ImageIcon) getIcon();
		return icon.getDescription();
	}		

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       A C T U A T O R S
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public void src(String actuator) {
		ImageIcon icon = new ImageIcon( SCILAB_XICONS + actuator);
		icon.setDescription(actuator);
		setIcon(icon);
	}		

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//       S I G N A T U R E
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public String toString() {
		String signature = "ICON";

		return signature;
	}

}

