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
import javax.swing.JComboBox;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class SELECT extends JComboBox implements XComponent, XChooser {

	static String NAV = "<<none>>";

	public String [] actuators() {
		String [] actuators = {""};
		return actuators;
	}

	public SELECT(Node peer) {
		super();
		NodeList nodelist = peer.getChildNodes();
		int select = 0;
		int index = 0;
		for (int i=0; i < nodelist.getLength(); i++) {
			Node node = nodelist.item(i);
			if (node.getNodeName().equals("option")) {
				String value = XConfigManager.getAttribute(node , "value");
				addItem(value);
				if (XConfigManager.getAttribute(node , "selected").equals("selected")) {
					select = index;
				}
				index += 1;
			}
		}
		setSelectedIndex(select);
	}
	public void refresh(Node peer) {
	}

	public String choose() {
		return (String) getSelectedItem();
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
		String signature = "SELECT";
		return signature;
	}


}

