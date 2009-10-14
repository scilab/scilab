package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosCodec;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.util.mxUtils;

public class ImportFromXMLAction extends DefaultAction {
    public ImportFromXMLAction(ScilabGraph scilabGraph) {
	super(XcosMessages.IMPORT_FROM_XML,scilabGraph);
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.IMPORT_FROM_XML, null, new ImportFromXMLAction(scilabGraph), null);
    }

    public void actionPerformed(ActionEvent e) {
	Document document = null;
	try {
	    document = mxUtils.parse(mxUtils.readFile("/tmp/testexport.xml"));
	} catch (IOException e1) {
	    // TODO Auto-generated catch block
	    e1.printStackTrace();
	}

	XcosCodec codec2 = new XcosCodec(document);
	
	XcosDiagram diagram = Xcos.createEmptyDiagram();
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
