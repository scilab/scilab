package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.ConstBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;
import com.mxgraph.model.mxCell;
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

	mxCodecRegistry.addPackage("org.scilab.modules.xcos");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.input");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.output");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.command");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.control");
	//mxObjectCodec constBlockCodec = new TextBlockCodec(BasicBlock.createBlock("CONST_m"));
	String[] ignore = {"exprs", "realParameters", "integerParameters", "nbZerosCrossing", "nmode", "state", "dState", "oDState",
		"simulationFunctionType", "SimulationFunctionType"};
	String[] refs = {"parent", "source", "target"};

	// Blocks
	mxObjectCodec textBlockCodec = new mxObjectCodec(new TextBlock(), ignore, refs, null);
	mxCodecRegistry.register(textBlockCodec);
	mxObjectCodec basicBlockCodec = new mxObjectCodec(new BasicBlock(), ignore, refs, null);
	mxCodecRegistry.register(basicBlockCodec);
	mxObjectCodec constBlockCodec = new mxObjectCodec(new ConstBlock(), ignore, refs, null);
	mxCodecRegistry.register(constBlockCodec);
	mxObjectCodec cellCodec = new mxObjectCodec(new mxCell(), null, refs, null);
	mxCodecRegistry.register(cellCodec);
	
	
	// Diagram
	String[] diagramIgnore = {"parentTab", "viewPort", "viewPortMenu", "view", "selectionModel", "wpar", "multiplicities"};
	mxObjectCodec diagramCodec = new mxObjectCodec(new XcosDiagram(), diagramIgnore, refs, null);
	mxCodecRegistry.register(diagramCodec);

	// Ports
	mxObjectCodec explicitOutputPortCodec = new mxObjectCodec(new ExplicitOutputPort(), null, refs, null);
	mxCodecRegistry.register(explicitOutputPortCodec);
	
	mxCodec codec = new mxCodec();
	String xml = mxUtils.getXml(codec.encode(getGraph(e)));

	try {
	    mxUtils.writeFile(xml, "/tmp/testexport.xml");
	} catch (IOException e1) {
	    // TODO Auto-generated catch block
	    e1.printStackTrace();
	}
    }

}
