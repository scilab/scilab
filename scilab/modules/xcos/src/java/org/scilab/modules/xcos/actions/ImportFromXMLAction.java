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


public class ImportFromXMLAction extends DefaultAction {
    
    public ImportFromXMLAction(final ScilabGraph scilabGraph) {
	super(XcosMessages.IMPORT_FROM_XML,scilabGraph);
    }

    public static MenuItem createMenu(final ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.IMPORT_FROM_XML, null, new ImportFromXMLAction(scilabGraph), null);
    }

    public void actionPerformed(final ActionEvent e) {
	Document document = null;

	final String sci = System.getenv("SCI");

	try {
	    document = mxUtils.parse(mxUtils.readFile( sci + "/testexport.xml"));
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
	}
	else {
	    System.err.println("[FAILED] diagram is null");
	}

    }
}
