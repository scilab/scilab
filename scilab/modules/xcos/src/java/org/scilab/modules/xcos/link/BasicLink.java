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

package org.scilab.modules.xcos.link;

import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.util.ArrayList;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxPoint;

public abstract class BasicLink extends mxCell {

	public BasicLink(String style) {
		super();
		setVertex(false);
		setEdge(true);
		setStyle(style);
	}

	public void setOrdering(int ordering) {
		((BasicPort) this.getSource()).setConnectedLinkId(ordering);
		((BasicPort) this.getTarget()).setConnectedLinkId(ordering);
	}

	public void addPoint(double x, double y) {
		mxPoint point = new mxPoint(x, y);
		if (getGeometry().getPoints() == null) {
			getGeometry().setPoints(new ArrayList());
		}
		getGeometry().getPoints().add(point);
	}

	public void insertPoint(double x, double y) {
		mxPoint point = new mxPoint(x, y);
		if (getGeometry().getPoints() == null) {
			getGeometry().setPoints(new ArrayList());
			getGeometry().getPoints().add(point);
		}
		else {
			// TODO : Must do the calculation to know where to insert it in List
			double startX = (getSource().getParent().getGeometry().getX() + getSource().getGeometry().getX());
			double startY = (getSource().getParent().getGeometry().getY() + getSource().getGeometry().getY());

			double endX = (getTarget().getParent().getGeometry().getX() + getTarget().getGeometry().getX());
			double endY = (getTarget().getParent().getGeometry().getY() + getTarget().getGeometry().getY());


			//increase placement window
			double saveDist = -1;
			int newPos = 0;

			for(int i = 0 ; i < getGeometry().getPoints().size() + 1; i++){
				Point2D.Double point1 = null;
				Point2D.Double point2 = null;

				if(i == getGeometry().getPoints().size()){
					point1 = new Point2D.Double(endX, endY);
				}else{
					point1 = new Point2D.Double((int)((mxPoint)getGeometry().getPoints().get(i)).getX(), (int)((mxPoint)getGeometry().getPoints().get(i)).getY());
				}

				if(i == 0){//first block
					point2 = new Point2D.Double(startX, startY);
				}else{
					point2 = new Point2D.Double((int) ((mxPoint)getGeometry().getPoints().get(i-1)).getX(), (int) ((mxPoint)getGeometry().getPoints().get(i-1)).getY());
				}

				Point2D.Double addPoint = new Point2D.Double(x,y);
				Line2D.Double line = new Line2D.Double(point1, point2);

				if(saveDist == -1){
					saveDist = line.ptLineDist(addPoint);
					newPos = i;
				}
				else{
					double dist = line.ptLineDist(addPoint);
					if(dist < saveDist && line.getBounds2D().contains(addPoint)){
						saveDist = dist;
						newPos = i;
					}
				}

				System.err.println("saveDist : " + saveDist);
				System.err.println("New Point : " + addPoint.toString());
				System.err.println("newPos : " + newPos);

			}	
			getGeometry().getPoints().add(newPos, point);
		}
	}

	public ScilabMList getAsScilabObj() {
		String[] fields = {"Link", "xx", "yy", "id", "thick", "ct", "from", "to"};
		ScilabMList data = new ScilabMList(fields);

//		double[][] xx = {{getSource().getGeometry().getCenterX() + getSource().getParent().getGeometry().getX()},
//				{getTarget().getGeometry().getCenterX() + getTarget().getParent().getGeometry().getX()}};
//		data.add(new ScilabDouble(xx)); // xx
//
//		double[][] yy = {{-(getSource().getGeometry().getCenterY() + getSource().getParent().getGeometry().getY() - getSource().getParent().getGeometry().getHeight())},
//				{-(getTarget().getGeometry().getCenterY() + getTarget().getParent().getGeometry().getY() - getSource().getParent().getGeometry().getHeight())}};
//		data.add(new ScilabDouble(yy)); // yy

		data.add(new ScilabDouble()); // xx
		
		data.add(new ScilabDouble()); // yy
		
		data.add(new ScilabString("drawlink")); // id

		double[][] thick = {{0, 0}};
		data.add(new ScilabDouble(thick)); // thick

		data.add(new ScilabDouble(getColorAndType())); //ct

		double fromBlockID = ((BasicBlock) getSource().getParent()).getOrdering();
		double fromPortID = ((BasicPort) getSource()).getOrdering();
		double fromType = 0;
		
		if(getSource() instanceof InputPort || getSource() instanceof ControlPort){
			fromType = 1;
		}
		double[][] fromData = {{fromBlockID, fromPortID, fromType}};
		data.add(new ScilabDouble(fromData)); // from

		double toBlockID = ((BasicBlock) getTarget().getParent()).getOrdering();
		double toPortID = ((BasicPort) getTarget()).getOrdering();
		double toType = 0;
		if(getTarget() instanceof InputPort || getTarget() instanceof ControlPort){
			toType = 1;
		}
		double[][] toData = {{toBlockID, toPortID, toType}};
		data.add(new ScilabDouble(toData)); // to

		return data;
	}

	public abstract double[][] getColorAndType();

}
