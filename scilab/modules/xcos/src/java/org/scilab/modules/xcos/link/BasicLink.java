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

import java.awt.MouseInfo;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.util.ArrayList;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.actions.ColorAction;
import org.scilab.modules.xcos.actions.LinkStyleAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;

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

	public void removePoint(int index){
		if(getGeometry() == null || getGeometry().getPoints() == null){
			return;
		}
		if(index < getGeometry().getPoints().size()){
			getGeometry().getPoints().remove(index);
		}
	}
	
	public mxPoint[] getPoints(int index, boolean fromStart){
		
		if(getGeometry() == null || getGeometry().getPoints() == null){
			return new mxPoint[0];
		}
		
		int start = 0;
		int size = 0;
		if(fromStart){
			size = Math.min(getGeometry().getPoints().size(), index);
			start = 0;
		}else{
			start = index;
			size = getGeometry().getPoints().size() - index;
		}
		
		if(size > 0)
		{	
			mxPoint points[] = new mxPoint[size];
			for(int i = 0 ; i < size ; i++){
				points[i] = (mxPoint)getGeometry().getPoints().get(start + i);
			}
			return points;
		}
		return null;
	}
	
	public int getPointCount(){
		if(getGeometry() == null || getGeometry().getPoints() == null){
			return 0;
		}
		return getGeometry().getPoints().size();
	}

	public int findNearestSegment(mxPoint point){

		if(getGeometry() == null || getGeometry().getPoints() == null){
			return 0;
		}
		
		double startX = (getSource().getParent().getGeometry().getX() + getSource().getGeometry().getX());
		double startY = (getSource().getParent().getGeometry().getY() + getSource().getGeometry().getY());

		double endX = (getTarget().getParent().getGeometry().getX() + getTarget().getGeometry().getX());
		double endY = (getTarget().getParent().getGeometry().getY() + getTarget().getGeometry().getY());

		double saveDist = -1;
		int findPos = 0;

		for(int i = 0 ; i < getGeometry().getPoints().size() + 1; i++){
			Point2D.Double point1 = null;
			Point2D.Double point2 = null;

			if(i == 0){//first block
				point1 = new Point2D.Double(startX, startY);
			}else{
				point1 = new Point2D.Double((int) ((mxPoint)getGeometry().getPoints().get(i-1)).getX(), (int) ((mxPoint)getGeometry().getPoints().get(i-1)).getY());
			}

			if(i == getGeometry().getPoints().size()){
				point2 = new Point2D.Double(endX, endY);
			}else{
				point2 = new Point2D.Double((int)((mxPoint)getGeometry().getPoints().get(i)).getX(), (int)((mxPoint)getGeometry().getPoints().get(i)).getY());
			}

			Point2D.Double addPoint = new Point2D.Double(point.getX(),point.getY());
			Line2D.Double line = new Line2D.Double(point1, point2);

			if(saveDist == -1){
				saveDist = line.ptSegDist(addPoint);
				findPos = i;
			}
			else{
				double dist = line.ptSegDist(addPoint);
				if(dist < saveDist){
					saveDist = dist;
					findPos = i;
				}
			}
		}
		return findPos;
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
			//check to delete an old point before try to insert
			for(int i = 0 ; i < getGeometry().getPoints().size(); i++){
				mxPoint oldPoint = (mxPoint)getGeometry().getPoints().get(i);
				mxRectangle rect = new mxRectangle(oldPoint.getX() - 5, oldPoint.getY() - 5, 10, 10);
				if(rect.contains(x, y)){
					getGeometry().getPoints().remove(i);
					return;
				}else{
				}
			}			

			int insertPos = findNearestSegment(point);
			getGeometry().getPoints().add(insertPos, point);
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

	public void openContextMenu(ScilabGraph graph) {
		ContextMenu menu = ScilabContextMenu.createContextMenu();
		menu.add(DeleteAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		Menu format = ScilabMenu.createMenu();
		format.setText(XcosMessages.FORMAT);
		format.add(ColorAction.createMenu(graph, XcosMessages.BORDER_COLOR, mxConstants.STYLE_STROKECOLOR));
		menu.add(format);
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		Menu linkStyle = ScilabMenu.createMenu();
		linkStyle.setText(XcosMessages.LINK_STYLE);
		linkStyle.add(LinkStyleAction.createMenu(graph, XcosMessages.LINK_STYLE_STRAIGHT, mxConstants.SHAPE_CONNECTOR));
		linkStyle.add(LinkStyleAction.createMenu(graph, XcosMessages.LINK_STYLE_HORIZONTAL, mxConstants.ELBOW_HORIZONTAL));
		linkStyle.add(LinkStyleAction.createMenu(graph, XcosMessages.LINK_STYLE_VERTICAL, mxConstants.ELBOW_VERTICAL));
		
		
		menu.add(linkStyle);
		
		((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);		

		menu.setVisible(true);
	}

}
