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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.io.scicos.LinkElement;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.LinkPortMap;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;

import edu.tum.cs.simulink.model.SimulinkLine;

public class LineElement extends AbstractElement<BasicLink>{

	/*
	 * fields used for setting up start and end port of Link
	 */
	private BasicPort start;
	private BasicPort end;
	private static final Log LOG = LogFactory.getLog(LineElement.class);
	
	/** The already decoded block map */
	private final Map<Integer, BasicBlock> blocks;
	/**
	 * Default constructor
	 * 
	 * @param blocks
	 *            the already decoded blocks
	 */
	public LineElement(Map<Integer, BasicBlock> blocks) {
		this.blocks = blocks;
	}
	
	public BasicLink decode(SimulinkLine simulinkLine, BasicLink into) {
		BasicLink link = into;
		SimulinkLine from = simulinkLine;
		/*
		if (LOG.isTraceEnabled()) {
			LOG.trace("SRC: " + from.getSrcPort().toString());
			LOG.trace("DST: " + from.getDstPort().toString());
		}*/
		validate();
		
		if (into == null) {
			link = allocateLink();
		}
		/*
		 * Setting up points of line
		 */
		searchForPorts(from, link);
		List<mxPoint> points = getPoints(from);
		/*
		 * Fill the data
		 */
		link.setSource(start);
		link.setTarget(end);

		mxGeometry geom = link.getGeometry();
		if (geom == null) {
			geom = new mxGeometry();
		}
		geom.setPoints(points);
		link.setGeometry(geom);

		return link;
	}

	private List<mxPoint> getPoints(SimulinkLine from) {
		
		List<mxPoint> mxPoints = new ArrayList<mxPoint>();
		/*
		 * Set start and end point, should be SRC port posision
		 */
		double x = 0;
		double y = 0;
		if(start!=null) {
			/*
			 * get position of the parent block of start port, then add position of port
			 */
			x = start.getParent().getGeometry().getX() + start.getGeometry().getCenterX();
			y = start.getParent().getGeometry().getY() + start.getGeometry().getCenterY();
		}
		/*
		 * Set line breaking points
		 * Simulink line contains realative points positions
		 */
		/** 
		 * stripping from [,],;,, characters that exist in Points parameter string
		 * \\s+ to split string around whitespaces
		 */
		if (LOG.isTraceEnabled()) {
			LOG.trace(from.getParameter("Points"));
		}
		try {
		String[] points = from.getParameter("Points").replaceAll("[\\[\\];,]", " ").trim().split("\\s+");
		for(int i = 0 ; i<points.length ; i+=2) {
			x += Double.parseDouble(points[i]);
			y += Double.parseDouble(points[i+1]);
			mxPoints.add(new mxPoint(x, y));
		} 
		} catch (NullPointerException e) {
			LOG.trace("No points info available");
		}
		LOG.trace(from.getParameterNames().toString());
		
		return mxPoints;
	}

	private void searchForPorts(SimulinkLine simulinkLine,BasicLink link) {
		Iterator<Entry<Integer, BasicBlock>> blockIter = blocks.entrySet().iterator();
		while(blockIter.hasNext()){
			BasicBlock block = blockIter.next().getValue();
			List<OutputPort> portList = BasicBlockInfo.getAllTypedPorts(block, false, OutputPort.class);
			Iterator<OutputPort> portIter = portList.iterator();
			while(portIter.hasNext()){
				OutputPort port = portIter.next();
				if(port.getId().equals("Output" + simulinkLine.getSrcPort().toString())){
					start = port;
					//LOG.trace("start set to" + start.getId());
				}
			}
			
			List<InputPort> inPortList = BasicBlockInfo.getAllTypedPorts(block, false, InputPort.class);
			Iterator<InputPort> inPortIter = inPortList.iterator();
			while(inPortIter.hasNext()){
				InputPort port = inPortIter.next();
				if(port.getId().toString().equals("Input" + simulinkLine.getDstPort().toString())){
					end = port;
					//LOG.trace("end set to" + end.getId());
				}
			}
		}
	}

	private BasicLink allocateLink() {
		BasicLink link = null;
		//FIXME: type of port
		int type = 1;
		try {
			link = LinkPortMap.getLinkClass(type).newInstance();
		} catch (InstantiationException e) {
			LogFactory.getLog(LinkElement.class).error(e);
		} catch (IllegalAccessException e) {
			LogFactory.getLog(LinkElement.class).error(e);
		}

		return link;
	}

	private void validate() {
		// TODO Auto-generated method stub
		
	}

	public boolean canDecode(SimulinkLine data) {
		// TODO Auto-generated method stub
		return true;
	}

}
