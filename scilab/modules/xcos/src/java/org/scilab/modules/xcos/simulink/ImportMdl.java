package org.scilab.modules.xcos.simulink;

import java.io.File;
import java.io.IOException;
import java.util.Iterator;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.commons.logging.SimpleLogger;
import edu.tum.cs.simulink.builder.SimulinkModelBuilder;
import edu.tum.cs.simulink.builder.SimulinkModelBuildingException;
import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkLine;
import edu.tum.cs.simulink.model.SimulinkModel;
import edu.tum.cs.simulink.model.SimulinkOutPort;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.simulink.Logger;
import org.scilab.modules.xcos.simulink.DiagramElement;

public class ImportMdl {
	/**
	 * fromFile is a function that reads .mdl file, and logs all the model, blocks
	 * lines, ports parameters to file. This function gonna change, as Simulink import
	 * evolves.
	 * @param filename .mdl filename that we want to import
	 * @throws IOException
	 * @throws SimulinkModelBuildingException
	 */
	public static void fromFile(String filename) throws IOException,SimulinkModelBuildingException {
		SimulinkModelBuilder builder = new SimulinkModelBuilder(new File(
		        filename), new SimpleLogger());
	    SimulinkModel model = builder.buildModel();
	    DiagramElement diagram = new DiagramElement();
	    try {
	    	XcosDiagram into = diagram.decode(model, null);
	    } catch(SimulinkFormatException e1) {
	    	LogFactory.getLog(ImportMdl.class).error(e1);
	    	Logger.toFile("EE: diagram.decode exception");
	    }
	}
	/**
	 * 
	 * @param block
	 * 			block or model that will be added to Xcos Schema
	 */
	public static void readSimulinkBlock(SimulinkBlock block) {
		UnmodifiableIterator<SimulinkBlock> blockIter = block.getSubBlocks().iterator();
		while(blockIter.hasNext()) {
	    	readSimulinkBlock(blockIter.next());
    	}
	}
}
