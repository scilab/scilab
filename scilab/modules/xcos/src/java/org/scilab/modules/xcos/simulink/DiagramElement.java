package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.simulink.BlockElement;
import org.scilab.modules.xcos.simulink.AnnotationElement;
import com.mxgraph.model.mxCell;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkAnnotation;
import edu.tum.cs.simulink.model.SimulinkBlock;
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
			LOG.trace(base.getParameterNames().toString());
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
				diag.addCell(cell);
			}
			i++;
		}
		/*
		 * Decode Annotations
		 */
		i=0;
		UnmodifiableIterator<SimulinkAnnotation> annotationIter = base.getAnnotations().iterator();
		while(annotationIter.hasNext()) {
			SimulinkAnnotation data = annotationIter.next();
			Object cell = null;
			
			if (annotationElement.canDecode(data)) {
				cell = annotationElement.decode(data, null);
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			} 
			
			if (cell != null) {
				diag.addCell(cell);
			}
			i++;
		}
		/*
		 * Decode Links
		 * TODO: This should be done differently then in io.scicos
		 * 		 there are inLines and outLines for each structure
		 * 		 just like ports.
		 */
		
	}
}

