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

import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.graph.actions.GroupAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.UnGroupAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.handler.mxKeyboardHandler;
import com.mxgraph.swing.util.mxGraphActions;


public class XcosShortCut extends mxKeyboardHandler {

    public XcosShortCut(mxGraphComponent component) {
	super(component);
    }

    /**
     * Return JTree's input map.
     */
    protected InputMap getInputMap(int condition)
    {
	InputMap map = super.getInputMap(condition);

	if (condition == JComponent.WHEN_FOCUSED && map != null)
	{
	    map.put(KeyStroke.getKeyStroke("control S"), "save");
	    map.put(KeyStroke.getKeyStroke("control shift S"), "saveAs");
	    map.put(KeyStroke.getKeyStroke("control N"), "new");
	    map.put(KeyStroke.getKeyStroke("control O"), "open");

	    map.put(KeyStroke.getKeyStroke("control G"), "group");
	    map.put(KeyStroke.getKeyStroke("control shift G"), "ungroup");

	    map.put(KeyStroke.getKeyStroke("control Z"), "undo");
	    map.put(KeyStroke.getKeyStroke("control Y"), "redo");
	    map.put(KeyStroke.getKeyStroke("control A"), "selectAll");
	    map.put(KeyStroke.getKeyStroke("delete"), "deleteSelection");

	    map.put(KeyStroke.getKeyStroke("control +"), "zoomIn");
	    map.put(KeyStroke.getKeyStroke("control -"), "zoomOut");
	}

	return map;
    }   
  
    /**
     * Return the mapping between JTree's input map and JGraph's actions.
     */
    protected ActionMap createActionMap()
    {
	ActionMap map = super.createActionMap();
	map.put("save", new DefaultAction());
	map.put("saveAs", new DefaultAction());
	
	map.put("new", new DefaultAction());
	map.put("open", new DefaultAction());
	
	map.put("undo", new UndoAction());
	map.put("redo", new RedoAction());
	
	map.put("group", new GroupAction());
	map.put("ungroup", new UnGroupAction());
	
	map.put("selectAll", mxGraphActions.getSelectAllAction());
	map.put("delete", mxGraphActions.getDeleteAction());

	map.put("zoomIn", new ZoomInAction());
	map.put("zoomOut", new ZoomOutAction());
	return map;
    } 

}
