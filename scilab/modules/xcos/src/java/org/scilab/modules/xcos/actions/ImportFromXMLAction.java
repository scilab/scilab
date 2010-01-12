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
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.util.mxUtils;


/**
 * @author Bruno JOFRET
 *
 */
public class ImportFromXMLAction extends DefaultAction {
    
    /**
     * @param scilabGraph graph
     */
    public ImportFromXMLAction(final ScilabGraph scilabGraph) {
	super(XcosMessages.IMPORT_FROM_XML, scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(final ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.IMPORT_FROM_XML, null, new ImportFromXMLAction(scilabGraph), null);
    }

    public void actionPerformed(final ActionEvent e) {
	Document document = null;

	final String sci = System.getenv("SCI");

	try {
	    document = mxUtils.parse(mxUtils.readFile(sci + "/testexport.xml"));
	} catch (final IOException e1) {
	    e1.printStackTrace();
	}

	final XcosCodec codec2 = new XcosCodec(document);
	
	final XcosDiagram diagram = Xcos.createEmptyDiagram();
	//diagram.getModel().beginUpdate();
	codec2.decode(document.getDocumentElement(), diagram);
	//diagram.getModel().endUpdate();
	//diagram.refresh();
	if (diagram != null) {
	    System.out.println(diagram.getModel().getChildCount(diagram.getDefaultParent()));
	} else {
	    System.err.println("[FAILED] diagram is null");
	}

    }
}
