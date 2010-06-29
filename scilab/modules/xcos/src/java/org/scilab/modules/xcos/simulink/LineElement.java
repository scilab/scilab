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

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.LinkElement;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.LinkPortMap;
import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkBlock;
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
		if (LOG.isTraceEnabled()) {
			LOG.trace("SRC: " + from.getSrcPort().toString());
			LOG.trace("DST: " + from.getDstPort().toString());
		}
		validate();
		
		if (into == null) {
			link = allocateLink();
		}
		/*
		 * Setting up points of line
		 */
		searchForPorts(link);
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
		 * Set start and end point
		 */
		double x = 100;
		double y = 100;
		mxPoints.add(new mxPoint(x, y));
		/*
		 * Set line breaking points
		 * Simulink line contains realative points positions
		 */
		/** 
		 * \\W is used to strip string from non-word characters 
		 * \\s+ to split string around whitespaces
		 */
		try{
		String[] points = from.getParameter("Points").replaceAll("\\W", " ").trim().split("\\s+");
		for(int i = 0 ; i<points.length ; i+=2){
			x+= Double.parseDouble(points[i]);
			y+= Double.parseDouble(points[i+1]);
			LOG.trace("Point: x:" + points[i]);
			LOG.trace("Point: y:" + points[i+1]);
			mxPoints.add(new mxPoint(x, y));
		}
		} catch (NullPointerException e) {
			LOG.trace("No points info available");
		}
		LOG.trace(from.getParameterNames().toString());
		
		mxPoints.add(new mxPoint(0, 10));
		
		return mxPoints;
	}

	private void searchForPorts(BasicLink link) {
		// TODO Auto-generated method stub
		
	}

	private BasicLink allocateLink() {
		BasicLink link = null;
		//FIXME: type of port
		int type = 2;
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
