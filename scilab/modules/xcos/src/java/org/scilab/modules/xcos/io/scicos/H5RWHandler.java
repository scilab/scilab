/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement useful methods to easily import or export Scicos data.
 */
//CSOFF: ClassDataAbstractionCoupling
public class H5RWHandler {
	private static final String CONTEXT = "context";
	private static final String SCS_M = "scs_m";
	
	private final File h5File;

	/**
	 * Constructor a new instance with a file.
	 * @param hdf5file the file path.
	 */
	public H5RWHandler(File hdf5file) {
		h5File = hdf5file;
	}
	
	/**
	 * Construct a new instance with the file path.
	 * @param hdf5file the file path.
	 */
	public H5RWHandler(String hdf5file) {
		h5File = new File(hdf5file);
	}
	
	/*
	 * Read methods
	 */
	
	/**
	 * Decode an Xcos block
	 * @return the decoded block
	 */
	public BasicBlock readBlock() {
		final ScilabMList data = new ScilabMList();
		final BlockElement element = new BlockElement();
		BasicBlock instance;

		try {
		    int fileId = H5Read.openFile(h5File.getAbsolutePath());
		    H5Read.readDataFromFile(fileId, data);
		    H5Read.closeFile(fileId);
		    
		    instance = element.decode(data, null);
		    StyleMap style = new StyleMap(instance.getStyle());
		    style.put(instance.getInterfaceFunctionName(), null);
		    instance.setStyle(style.toString());
		    
		} catch (ScicosFormatException e) {
		    LogFactory.getLog(H5RWHandler.class).error(e);
		    instance = null;
		} catch (HDF5Exception e) {
		    LogFactory.getLog(H5Read.class).error(e);
		    instance = null;
		}
		
		return instance;
	}
	
	/**
	 * Decode an evaluated Xcos context
	 * @return the decoded context
	 */
	public Map<String, String> readContext() {
		final ScilabList list = new ScilabList();
		final Map<String, String> result = new HashMap<String, String>();
		
		try {
			int handle = H5Read.openFile(h5File.getAbsolutePath());
			if (handle >= 0) {
				H5Read.readDataFromFile(handle, list);
			}
		} catch (HDF5Exception e) {
		    LogFactory.getLog(H5Read.class).error(e);
		    return result;
		}

		// We are starting at 2 because a struct is composed of
		//     - the fields names (ScilabString)
		//     - the dimension
		//     - variables values...
		for (int index = 2; index < list.size(); index++) {
			String key = ((ScilabString) list.get(0)).getData()[0][index];
			String value = list.get(index).toString();
			
			result.put(key, value);
		}
		
		return result;
	}
	
	/**
	 * Decode an Xcos diagram from an H5 file
	 * @return the decoded diagram
	 * @throws VersionMismatchException when the diagram version mismatch
	 */
	public XcosDiagram readDiagram() throws VersionMismatchException {
		return readDiagram(null);
	}
	
	/**
	 * Decode an Xcos diagram from an H5 file in place.
	 * @param instance the previously allocated diagram where to decode.
	 * @return the decoded diagram
	 */
	public XcosDiagram readDiagram(XcosDiagram instance) {
		final ScilabMList data = new ScilabMList();
		final DiagramElement element = new DiagramElement();
		
		XcosDiagram diagram;
		if (instance == null) {
			diagram = new XcosDiagram();
		} else {
			diagram = instance;
		}
		
		diagram.getModel().beginUpdate();

		try {
		    int fileId = H5Read.openFile(h5File.getAbsolutePath());

		    H5Read.readDataFromFile(fileId, data);
		    H5Read.closeFile(fileId);
	    	element.decode(data, diagram);
		} catch (ScicosFormatException e) {
			if (e instanceof VersionMismatchException) {
				/*
				 * On version mismatch alert the user but the current instance
				 * contains the partially decoded data so continue.
				 */
				diagram.error(XcosMessages.UNKNOW_VERSION
						+ ((VersionMismatchException) e).getWrongVersion());
			} else {
				LogFactory.getLog(H5RWHandler.class).error(e);
			}
		} catch (HDF5Exception e) {
		    LogFactory.getLog(H5Read.class).error(e);
		} finally {
			diagram.getModel().endUpdate();
		}
		
		return diagram;
	}
	
	/*
	 * Write methods
	 */
	
	/**
	 * Encode an Xcos block
	 * @param block the block
	 */
	public void writeBlock(BasicBlock block) {
		final BlockElement element = new BlockElement();

		try {
		    int fileId = H5Write.createFile(h5File.getAbsolutePath());
		    
		    H5Write.writeInDataSet(fileId, SCS_M, element.encode(block, null));
		    
		    H5Write.closeFile(fileId);
		} catch (HDF5Exception e) {
		   LogFactory.getLog(H5Read.class).error(e);
		}
	}
	
	/**
	 * Encode an Xcos context
	 * @param context the context
	 */
	public void writeContext(String[] context) {
		final ScilabString string = new ScilabString(context);

		try {
		    int fileId = H5Write.createFile(h5File.getAbsolutePath());
		    
		    H5Write.writeInDataSet(fileId, CONTEXT, string);
		    
		    H5Write.closeFile(fileId);
		} catch (HDF5Exception e) {
		   LogFactory.getLog(H5Read.class).error(e);
		}
	}
	
	/**
	 * Encode an Xcos diagram
	 * @param diagram the block
	 */
	public void writeDiagram(XcosDiagram diagram) {
		final DiagramElement element = new DiagramElement();

		try {
		    int fileId = H5Write.createFile(h5File.getAbsolutePath());
		    
		    H5Write.writeInDataSet(fileId, SCS_M, element.encode(diagram, null));
		    
		    H5Write.closeFile(fileId);
		} catch (HDF5Exception e) {
		   LogFactory.getLog(H5Read.class).error(e);
		}
	}
}
//CSON: ClassDataAbstractionCoupling
