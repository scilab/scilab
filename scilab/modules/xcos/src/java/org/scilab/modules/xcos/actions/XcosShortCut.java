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

package org.scilab.modules.xcos.actions;

import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.GroupAction;
import org.scilab.modules.graph.actions.UnGroupAction;

import com.mxgraph.swing.handler.mxKeyboardHandler;


public class XcosShortCut extends mxKeyboardHandler {

	private ScilabGraph component;

	public XcosShortCut(ScilabGraph component) {
		super(component.getAsComponent());
		this.component = component;
	}

	/**
	 * Return JTree's input map.
	 */
	protected InputMap getInputMap(int condition) {
		InputMap map = super.getInputMap(condition);

		if (condition == JComponent.WHEN_FOCUSED && map != null) {
			map.put(KeyStroke.getKeyStroke("control G"), "group");
			map.put(KeyStroke.getKeyStroke("control shift G"), "ungroup");
		}

		return map;
	}   

	/**
	 * Return the mapping between JTree's input map and JGraph's actions.
	 */
	protected ActionMap createActionMap() {
		ActionMap map = super.createActionMap();

		map.put("group", new GroupAction(component));
		map.put("ungroup", new UnGroupAction(component));
		return map;
	} 

}
