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

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.link.BasicLink;

import com.mxgraph.model.mxCell;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkAnnotation;
import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkLine;
import edu.tum.cs.simulink.model.SimulinkModel;

public class DiagramElement extends AbstractElement<XcosDiagram> {

	private SimulinkModel base;
	private static final Log LOG = LogFactory.getLog(DiagramElement.class);
	
	/** Map from index to blocks */
	private final Map<Integer, BasicBlock> blocks;
	/**
	 * Default constructor
	 */
	public DiagramElement() {
		blocks = new HashMap<Integer, BasicBlock>();
	}
	
	public XcosDiagram decode(SimulinkModel from, XcosDiagram into) 
			throws SimulinkFormatException {
		base = from;
        XcosDiagram diag = into;
        if (diag == null) {
             diag = new XcosDiagram();
        }

		diag.getModel().beginUpdate();
        decodeDiagram(diag);
		diag.getModel().endUpdate();
        
		return diag;
	}
	
	public void decodeDiagram(XcosDiagram diag) throws SimulinkFormatException {
		// fill all diagram parameters
		if (LOG.isTraceEnabled()) {
			LOG.trace("From " + base.getName() + ":");
			UnmodifiableIterator<String> paramNameIter = base.getParameterNames().iterator();
			while(paramNameIter.hasNext()){
				String paramName = paramNameIter.next();
				LOG.trace(paramName + ": " + base.getParameter(paramName));
			}
		}
		diag.setTitle(base.getName());
		try{
		DiagramParametersElement params = new DiagramParametersElement();
		params.decode(diag.getScicosParameters());
		} catch (ScicosFormatException sfe) {
			LOG.error(sfe);
		}
		decodeObjs(diag);
	}
	
	public void decodeObjs(XcosDiagram diag) throws SimulinkFormatException {
		final BlockElement blockElement = new BlockElement();
		final AnnotationElement annotationElement = new AnnotationElement();
		//final LinkElement linkElement = new LinkElement(blocks);
		
		double minimalYaxisValue = 0.0; 
		int i = 0;
		//recursively get to all blocks, links and ports
		/*
		 * Decode blocks
		 */
		UnmodifiableIterator<SimulinkBlock> blockIter = base.getSubBlocks().iterator();
		while(blockIter.hasNext()) {
			SimulinkBlock data = blockIter.next();
			Object cell = null;
			
			if (blockElement.canDecode(data)) {
				BasicBlock block = blockElement.decode(data, null);
				blocks.put(i, block);
				cell = block;
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			} 
			
			if (cell != null) {
				//FIXME: 
				diag.addCell(cell);
			}
			i++;
		}
		/*
		 * Decode links
		 */
		LineElement lineElement = new LineElement(blocks);
		blockIter = base.getSubBlocks().iterator();
		while(blockIter.hasNext()) {
			SimulinkBlock blockData = blockIter.next();
			Object cell = null;
			Iterator<SimulinkLine> dataIter = blockData.getInLines().iterator();
			while(dataIter.hasNext()){
				SimulinkLine data = dataIter.next();
				if (lineElement.canDecode(data)) {
					BasicLink link = lineElement.decode(data, null);
					cell = link;
					
					minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
				}
				
				if (cell != null) {
					diag.addCell(cell);
				}
			}
			/*
			 * no need for adding out lines when already added inLines
			dataIter = blockData.getOutLines().iterator();
			while(dataIter.hasNext()){
				SimulinkLine data = dataIter.next();
				if (lineElement.canDecode(data)) {
					BasicLink link = lineElement.decode(data, null);
					cell = link;
					
					minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
				}
				
				if (cell != null) {
					diag.addCell(cell);
				}
			}*/
		}
		/*
		 * Decode Annotations
		 */
		UnmodifiableIterator<SimulinkAnnotation> annotationIter = base.getAnnotations().iterator();
		while(annotationIter.hasNext()) {
			SimulinkAnnotation data = annotationIter.next();
			Object cell = null;
			
			if (annotationElement.canDecode(data)) {
				cell = annotationElement.decode(data, null);
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			} 
			
			if (cell != null) {
				//FIXME:
				diag.addCell(cell);
			}
		}
	}
}

