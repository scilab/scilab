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

package org.scilab.modules.xcos.utils;

import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.ConstBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;
import com.mxgraph.model.mxCell;

public class XcosCodec extends mxCodec {
    /**
     * Register usefull codecs and packages for encoding/decoding diagrams
     */
    static {
	// Add all xcos packages
	mxCodecRegistry.addPackage("org.scilab.modules.xcos");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
//	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.explicit");
//	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.implicit");
//	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.commandcontrol");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.input");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.output");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.command");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.control");

	// Add some hdf5 packages to have all scilab types known
//	mxCodecRegistry.addPackage("org.scilab.modules.hdf5.scilabTypes");
	
	
	String[] ignore = {"exprs",
		"realParameters",
		"integerParameters",
		"objectsParameters",
		"nbZerosCrossing",
		"nmode",
		"state",
		"dState",
		"oDState",
		"equations",
		"dependsOnU",
		"dependsOnT",
		"blockType",
		"ordering",
		"interfaceFunctionName",
		"simulationFunctionName",
		"simulationFunctionType",
		"SimulationFunctionType"};

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
	String[] diagramIgnore = {"parentTab", "viewPort", "viewPortMenu", "view", "selectionModel", "multiplicities"};
	mxObjectCodec diagramCodec = new mxObjectCodec(new XcosDiagram(), diagramIgnore, refs, null);
	mxCodecRegistry.register(diagramCodec);

	// Ports
	mxObjectCodec explicitOutputPortCodec = new mxObjectCodec(new ExplicitOutputPort(), null, refs, null);
	mxCodecRegistry.register(explicitOutputPortCodec);    
    }
    
    public XcosCodec() {
	super();
    }

    public XcosCodec(Document document) {
	super(document);
    }
    
}
