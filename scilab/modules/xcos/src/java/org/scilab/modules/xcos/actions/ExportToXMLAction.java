package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxUtils;

public class ExportToXMLAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    public ExportToXMLAction(ScilabGraph scilabGraph) {
	super(XcosMessages.EXPORT_TO_XML,scilabGraph);
    }

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
	    // TODO Auto-generated catch block
	    e1.printStackTrace();
	}
    }

}
