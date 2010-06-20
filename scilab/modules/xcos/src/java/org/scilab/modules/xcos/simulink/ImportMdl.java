/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Jerzy Zagorski
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.simulink.DiagramElement;

/**
 *  
 * main simulink entry point class
 * 
 */
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
			XcosDiagram xcosDiagram = Xcos.createANotShownDiagram();
			xcosDiagram = diagram.decode(model,xcosDiagram);
			XcosTab.showTabFromDiagram(xcosDiagram);
		} catch(SimulinkFormatException e1) {
			LogFactory.getLog(ImportMdl.class).error(e1);
		}
	}
}
