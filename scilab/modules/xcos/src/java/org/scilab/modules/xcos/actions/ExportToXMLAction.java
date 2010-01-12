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

import java.awt.event.ActionEvent;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxUtils;

/**
 * @author Bruno JOFRET
 *
 */
public class ExportToXMLAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    /**
     * @param scilabGraph graph
     */
    public ExportToXMLAction(ScilabGraph scilabGraph) {
	super(XcosMessages.EXPORT_TO_XML, scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.EXPORT_TO_XML, null, new ExportToXMLAction(scilabGraph), null);
    }

    public void actionPerformed(ActionEvent e) {

	XcosCodec codec = new XcosCodec();
	String xml = mxUtils.getXml(codec.encode(getGraph(e)));

	String sci = System.getenv("SCI");
	try {
	    mxUtils.writeFile(xml, sci + "/testexport.xml");
	} catch (IOException e1) {
	    e1.printStackTrace();
	}
    }

}
