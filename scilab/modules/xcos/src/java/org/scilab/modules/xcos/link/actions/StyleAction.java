/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.link.actions;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.xcos.link.BasicLink;

/**
 * base class for changing the link style
 */
public abstract class StyleAction extends DefaultAction {
	/** Accelerator key for all children actions */
	public static final int ACCELERATOR_KEY = 0;

    /**
     * Default constructor
     * @param scilabGraph the graph to associate
     */
    public StyleAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    }
	
	/**
	 * @return the current selected links on the graph
	 */
	protected BasicLink[] getLinks() {
		Object[] cells = getGraph(null).getSelectionModel().getCells();
		List<BasicLink> links = new ArrayList<BasicLink>(cells.length);
		
		for (Object object : cells) {
			if (object instanceof BasicLink) {
				links.add((BasicLink) object);
			}
		}
		
		return links.toArray(new BasicLink[links.size()]);
	}
	
	/**
	 * Remove all point on the links
	 * @param links the links to work on
	 */
	protected void removePointsOnLinks(BasicLink[] links) {
		getGraph(null).getModel().beginUpdate();
		for (BasicLink link : links) {
			int numberOfPoints = link.getPointCount();
			for (int j = numberOfPoints - 1; j >= 0; j--) {
				link.removePoint(j);
			}
		}
		getGraph(null).getModel().endUpdate();
	}
}
