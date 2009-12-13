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
import java.util.List;

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
import org.scilab.modules.xcos.XcosUIDObject;
import org.scilab.modules.xcos.block.actions.ColorAction;
import org.scilab.modules.xcos.actions.LinkStyleAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;

public abstract class BasicLink extends XcosUIDObject {

    private static final long serialVersionUID = 8557979393361216098L;

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

    public void removePoint(int index) { 
	if (getGeometry() == null || getGeometry().getPoints() == null) { 
	    return;
	}
	if (index < getGeometry().getPoints().size()) { 
	    getGeometry().getPoints().remove(index);
	}
    }

    private void removePoints() {
	getGeometry().setPoints(new ArrayList<mxPoint>());
    }

    public mxPoint[] getPoints(int index, boolean fromStart) { 

	if (getGeometry() == null || getGeometry().getPoints() == null) { 
	    return new mxPoint[0];
	}

	int start = 0;
	int size = 0;
	if (fromStart) { 
	    size = Math.min(getGeometry().getPoints().size(), index);
	    start = 0;
	} else {
	    start = index;
	    size = getGeometry().getPoints().size() - index;
	}

	if (size > 0) {
	    mxPoint[] points = new mxPoint[size];
	    for (int i = 0; i < size; i++) { 
		points[i] = (mxPoint) getGeometry().getPoints().get(start + i);
	    }
	    return points;
	}
	return null;
    }

    public int getPointCount() { 
	if (getGeometry() == null || getGeometry().getPoints() == null) { 
	    return 0;
	}
	return getGeometry().getPoints().size();
    }

    public int findNearestSegment(mxPoint point) { 

	if (getGeometry() == null || getGeometry().getPoints() == null) { 
	    return 0;
	}

	double startX = (getSource().getParent().getGeometry().getX() + getSource().getGeometry().getX());
	double startY = (getSource().getParent().getGeometry().getY() + getSource().getGeometry().getY());

	double endX = (getTarget().getParent().getGeometry().getX() + getTarget().getGeometry().getX());
	double endY = (getTarget().getParent().getGeometry().getY() + getTarget().getGeometry().getY());

	double saveDist = -1;
	int findPos = 0;

	for (int i = 0; i < getGeometry().getPoints().size() + 1; i++) { 
	    Point2D.Double point1 = null;
	    Point2D.Double point2 = null;

	    if (i == 0) { //first block
		point1 = new Point2D.Double(startX, startY);
	    } else {
		point1 = new Point2D.Double((int) ((mxPoint)getGeometry().getPoints().get(i-1)).getX(), (int) ((mxPoint)getGeometry().getPoints().get(i-1)).getY());
	    }

	    if (i == getGeometry().getPoints().size()) { 
		point2 = new Point2D.Double(endX, endY);
	    } else {
		point2 = new Point2D.Double((int)((mxPoint)getGeometry().getPoints().get(i)).getX(), (int)((mxPoint)getGeometry().getPoints().get(i)).getY());
	    }

	    Point2D.Double addPoint = new Point2D.Double(point.getX(), point.getY());
	    Line2D.Double line = new Line2D.Double(point1, point2);

	    if (saveDist == -1) { 
		saveDist = line.ptSegDist(addPoint);
		findPos = i;
	    }
	    else{
		double dist = line.ptSegDist(addPoint);
		if (dist < saveDist) { 
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
	    getGeometry().setPoints(new ArrayList<mxPoint>());
	}
	getGeometry().getPoints().add(point);
    }

    public void insertPoint(double x, double y) {

	//if it is a loop link, change coordinate origin to block instead of diagram
	mxPoint point = new mxPoint(x, y);
	if(isLoopLink() == true) {
	    mxGeometry geo = getSource().getParent().getGeometry();
	    point.setX(x - geo.getX());
	    point.setY(y - geo.getY());
	}

	if(getGeometry() == null) {
	    setGeometry(new mxGeometry(0, 0, 80, 80));
	}

	if (getGeometry().getPoints() == null) {
	    getGeometry().setPoints(new ArrayList<mxPoint>());
	    getGeometry().getPoints().add(point);
	} else {
	    //check to delete an old point before try to insert
	    for (int i = 0; i < getGeometry().getPoints().size(); i++) { 
		mxPoint oldPoint = (mxPoint) getGeometry().getPoints().get(i);
		mxRectangle rect = new mxRectangle(oldPoint.getX() - 5, oldPoint.getY() - 5, 10, 10);
		if (rect.contains(point.getX(), point.getY())) { 
		    getGeometry().getPoints().remove(i);
		    return;
		}
	    }			

	    int insertPos = findNearestSegment(point);
	    getGeometry().getPoints().add(insertPos, point);
	}
    }

    private boolean isLoopLink() {
	if(getSource() != null && getTarget() != null) {
	    if(getSource().getParent() == getParent() && getTarget().getParent() == getParent()) {
		return true;
	    }
	}
	return false;
    }

    public ScilabMList getAsScilabObj() {
	String[] fields = {"Link", "xx", "yy", "id", "thick", "ct", "from", "to"};
	ScilabMList data = new ScilabMList(fields);

	double[][] xx = new double[1][2 + getPointCount()];

	// xx
	xx[0][0] = getSource().getGeometry().getCenterX() + getSource().getParent().getGeometry().getX();
	for (int i = 0; i < getPointCount(); i++) { 
	    xx[0][i + 1] = ((mxPoint) getGeometry().getPoints().get(i)).getX();
	}
	xx[0][1 + getPointCount()] = getTarget().getGeometry().getCenterX() + getTarget().getParent().getGeometry().getX();
	data.add(new ScilabDouble(xx));

	// yy
	double[][] yy = new double[1][2 + getPointCount()];
	yy[0][0] = -(getSource().getGeometry().getCenterY() + getSource().getParent().getGeometry().getY() - getSource().getParent().getGeometry().getHeight());
	for (int i = 0; i < getPointCount(); i++) { 
	    yy[0][i+1] = - (((mxPoint) getGeometry().getPoints().get(i)).getY());
	}
	yy[0][1 + getPointCount()] = -(getTarget().getGeometry().getCenterY() + getTarget().getParent().getGeometry().getY() - getSource().getParent().getGeometry().getHeight());
	data.add(new ScilabDouble(yy));

	data.add(new ScilabString("drawlink")); // id

	double[][] thick = {{0, 0}};
	data.add(new ScilabDouble(thick)); // thick

	data.add(new ScilabDouble(getColorAndType())); //ct

	double fromBlockID = ((BasicBlock) getSource().getParent()).getOrdering();
	double fromPortID = ((BasicPort) getSource()).getOrdering();
	double fromType = 0;

	if (getSource() instanceof InputPort || getSource() instanceof ControlPort) { 
	    fromType = 1;
	}
	double[][] fromData = {{fromBlockID, fromPortID, fromType}};
	data.add(new ScilabDouble(fromData)); // from

	double toBlockID = ((BasicBlock) getTarget().getParent()).getOrdering();
	double toPortID = ((BasicPort) getTarget()).getOrdering();
	double toType = 0;
	if (getTarget() instanceof InputPort || getTarget() instanceof ControlPort) { 
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

    public static BasicLink createLinkFromPorts(BasicPort from, BasicPort to) {
	
	//from and to are clearly identify
	if (from instanceof ExplicitOutputPort && to instanceof ExplicitInputPort) {
	    return new ExplicitLink();
	} else if (from instanceof ImplicitOutputPort && to instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	} else if (from instanceof ImplicitOutputPort && to instanceof ImplicitOutputPort) {
	    return new ImplicitLink();
	} else if (from instanceof ImplicitInputPort && to instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	} else if (from instanceof ImplicitInputPort && to instanceof ImplicitOutputPort) {
	    return new ImplicitLink();
	} else if (from instanceof CommandPort && to instanceof ControlPort) {
	    return new CommandControlLink();
	} else if (to instanceof ExplicitOutputPort && from instanceof ExplicitInputPort) {
	    return new ExplicitLink();
	} else if (to instanceof ImplicitOutputPort && from instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	} else if (to instanceof ImplicitOutputPort && from instanceof ImplicitOutputPort) {
	    return new ImplicitLink();
	} else if (to instanceof ImplicitInputPort && from instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	} else if (to instanceof ImplicitInputPort && from instanceof ImplicitOutputPort) {
	    return new ImplicitLink();
	} else if (to instanceof CommandPort && from instanceof ControlPort) {
	    return new CommandControlLink();
	}
	
	//from is null, never happen
	if(from == null) {
	    if(to instanceof ExplicitInputPort) {
		return new ExplicitLink();
	    } else if(to instanceof ImplicitInputPort) {
		return new ImplicitLink();
	    } else if(to instanceof ControlPort) {
		return new CommandControlLink();
	    }
	} else if(to == null) {
	    if(from instanceof ExplicitOutputPort) {
		return new ExplicitLink();
	    } else if(from instanceof ImplicitOutputPort) {
		return new ImplicitLink();
	    } else if(from instanceof CommandPort) {
		return new CommandControlLink();
	    }
	}
	return new ExplicitLink();
    }

    public void invertDirection() {
	    //invert source and destination and all points.
	    mxICell linkSource = getSource();
	    mxICell linkTarget = getTarget();
	    List<mxPoint> points = getGeometry().getPoints();
	    
	    setSource(linkTarget);
	    setTarget(linkSource);

	    removePoints();
	    for(int i = points.size() - 1 ; i >= 0; i--) {
		addPoint(points.get(i).getX(), points.get(i).getY());
	    }
	    
    }

}
