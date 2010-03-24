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

package org.scilab.modules.xcos.io;

import org.scilab.modules.graph.io.ScilabBooleanCodec;
import org.scilab.modules.graph.io.ScilabDoubleCodec;
import org.scilab.modules.graph.io.ScilabGraphCodec;
import org.scilab.modules.graph.io.ScilabIntegerCodec;
import org.scilab.modules.graph.io.ScilabListCodec;
import org.scilab.modules.graph.io.ScilabObjectCodec;
import org.scilab.modules.graph.io.ScilabStringCodec;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabInteger;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.codec.BasicBlockCodec;
import org.scilab.modules.xcos.io.codec.BasicPortCodec;
import org.scilab.modules.xcos.io.codec.OrientationCodec;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.model.mxCell;

/**
 * Root codec for Xcos diagram instance.
 * 
 * This class register all packages used by Xcos for 
 * serialization/deserialization. 
 */
public class XcosCodec extends mxCodec {
    /**
     * Register usefull codecs and packages for encoding/decoding diagrams
     */
    static {
	// Add all xcos packages
    mxCodecRegistry.addPackage("org.scilab.modules.graph");
    mxCodecRegistry.addPackage("org.scilab.modules.xcos");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.graph");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.block.positionning");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.commandcontrol");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.explicit");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.implicit");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.command");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.control");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.input");
	mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.output");
	// Add some hdf5 packages to have all scilab types known
	mxCodecRegistry.addPackage("org.scilab.modules.hdf5.scilabTypes");
	
	String[] ignore = {//"exprs",
		//"realParameters",
		//"integerParameters",
		//"objectsParameters",
		//"nbZerosCrossing",
		//"nmode",
		//"state",
		//"dState",
		//"oDState",
		//"equations",
		//"dependsOnU",
		//"dependsOnT",
		//"blockType",
		//"ordering",
		//"interfaceFunctionName",
		//"simulationFunctionName",

		"simulationFunctionType",
		"SimulationFunctionType"
			};
	
	String[] portIgnore = {
		"dataType"
	};

	
	String[] refs = {"parent", "source", "target", "container"};

	// Types
	ScilabObjectCodec scilabStringCodec = new ScilabStringCodec(new ScilabString(), null, null, null);
	mxCodecRegistry.register(scilabStringCodec);
	ScilabObjectCodec scilabBooleanCodec = new ScilabBooleanCodec(new ScilabBoolean(), null, null, null);
	mxCodecRegistry.register(scilabBooleanCodec);
	ScilabObjectCodec scilabDoubleCodec = new ScilabDoubleCodec(new ScilabDouble(), null, null, null);
	mxCodecRegistry.register(scilabDoubleCodec);
	ScilabObjectCodec scilabIntegerCodec = new ScilabIntegerCodec(new ScilabInteger(), null, null, null);
	mxCodecRegistry.register(scilabIntegerCodec);



    //
	ScilabObjectCodec scilabListCodec = new ScilabListCodec(new ScilabList(), new String[]{"scilabClass"}, null, null);
	mxCodecRegistry.register(scilabListCodec);


	
	// Blocks
	XcosObjectCodec textBlockCodec = new BasicBlockCodec(BlockInterFunction.TEXT_f.getSharedInstance(), ignore, refs, null);
	mxCodecRegistry.register(textBlockCodec);
	XcosObjectCodec basicBlockCodec = new BasicBlockCodec(new BasicBlock(), ignore, refs, null);
	mxCodecRegistry.register(basicBlockCodec);
	XcosObjectCodec constBlockCodec = new  BasicBlockCodec(BlockInterFunction.CONST.getSharedInstance(), ignore, refs, null);
	mxCodecRegistry.register(constBlockCodec);
	XcosObjectCodec afficheBlockCodec = new BasicBlockCodec(BlockInterFunction.AFFICH_f.getSharedInstance(), ignore, refs, null);
	mxCodecRegistry.register(afficheBlockCodec);
	XcosObjectCodec superBlockCodec = new BasicBlockCodec(BlockInterFunction.SUPER_f.getSharedInstance(), ignore, refs, null);
	mxCodecRegistry.register(superBlockCodec);
	XcosObjectCodec gainBlockCodec = new BasicBlockCodec(BlockInterFunction.GAIN_f.getSharedInstance(), ignore, refs, null);
	mxCodecRegistry.register(gainBlockCodec);
	XcosObjectCodec splitBlockCodec = new BasicBlockCodec(BlockInterFunction.SPLIT_f.getSharedInstance(), ignore, refs, null);
	mxCodecRegistry.register(splitBlockCodec);
	XcosObjectCodec cellCodec = new XcosObjectCodec(new mxCell(), null, refs, null);
	mxCodecRegistry.register(cellCodec);
	
	
	// Diagram
	String[] diagramIgnore = {
			"stylesheet",
			"parentTab",
			"viewPort",
			"viewPortMenu",
			"view",
			"selectionModel",
			"savedFile",
			"multiplicities",
			"opened"
		};
	ScilabGraphCodec diagramCodec = new ScilabGraphCodec(new XcosDiagram(), diagramIgnore, refs, null);
	mxCodecRegistry.register(diagramCodec);
	ScilabGraphCodec superBlockDiagramCodec = new ScilabGraphCodec(new SuperBlockDiagram(), diagramIgnore, refs, null);
	mxCodecRegistry.register(superBlockDiagramCodec);

	//Link 
	
	XcosObjectCodec explicitlinkCodec = new XcosObjectCodec(new ExplicitLink() , null , null , null);
	mxCodecRegistry.register(explicitlinkCodec);
	XcosObjectCodec implicitlinkCodec = new XcosObjectCodec(new ImplicitLink() , null , null , null);
	mxCodecRegistry.register(implicitlinkCodec);
	
	// Ports
	XcosObjectCodec explicitOutputPortCodec = new BasicPortCodec(new ExplicitOutputPort(), portIgnore, refs, null);
	mxCodecRegistry.register(explicitOutputPortCodec);
	XcosObjectCodec explicitInputPortCodec = new BasicPortCodec(new ExplicitInputPort(), portIgnore, refs, null);
	mxCodecRegistry.register(explicitInputPortCodec);    
	XcosObjectCodec implicitOutputPortCodec = new BasicPortCodec(new ImplicitOutputPort(), portIgnore, refs, null);
	mxCodecRegistry.register(implicitOutputPortCodec);
	XcosObjectCodec implicitInputPortCodec = new BasicPortCodec(new ImplicitInputPort(), portIgnore, refs, null);
	mxCodecRegistry.register(implicitInputPortCodec);
	XcosObjectCodec commandPortCodec = new BasicPortCodec(new CommandPort(), portIgnore, refs, null);
	mxCodecRegistry.register(commandPortCodec);
	XcosObjectCodec controltPortCodec = new BasicPortCodec(new ControlPort(), portIgnore, refs, null);
	mxCodecRegistry.register(controltPortCodec);
	
		// Orientation
		OrientationCodec.register();
    }
    
    /**
     * Default constructor
     */
    public XcosCodec() {
	super();
    }

    /**
     * Construct a new codec associated with the document.
     * @param document the document containing all the data.
     */
    public XcosCodec(Document document) {
	super(document);
    }
    
    
}
