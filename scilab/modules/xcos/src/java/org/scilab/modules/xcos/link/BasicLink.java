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

import java.util.ArrayList;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;

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
    
    public void insertPoint(int x, int y) {
	mxPoint point = new mxPoint(x, y);
	if (getGeometry().getPoints() == null) {
	    getGeometry().setPoints(new ArrayList());
	    getGeometry().getPoints().add(point);
	}
	else {
	    // TODO : Must do the calculation to know where to insert it in List
	    getGeometry().getPoints().add(point);
	}
    }

    public ScilabMList getAsScilabObj() {
	String[] fields = {"Link", "xx", "yy", "id", "thick", "ct", "from", "to"};
	ScilabMList data = new ScilabMList(fields);
	
	
	double[][] xx = {{getSource().getGeometry().getCenterX() + getSource().getParent().getGeometry().getX()},
			{getTarget().getGeometry().getCenterX() + getTarget().getParent().getGeometry().getX()}};
	data.add(new ScilabDouble(xx)); // xx
	
	double[][] yy = {{-(getSource().getGeometry().getCenterY() + getSource().getParent().getGeometry().getY() - getSource().getParent().getGeometry().getHeight())},
			{-(getTarget().getGeometry().getCenterY() + getTarget().getParent().getGeometry().getY() - getSource().getParent().getGeometry().getHeight())}};
	data.add(new ScilabDouble(yy)); // yy
	
	data.add(new ScilabString("drawlink")); // id
	
	double[][] thick = {{0, 0}};
	data.add(new ScilabDouble(thick)); // thick

	data.add(new ScilabDouble(getColorAndType())); //ct
	
	double fromBlockID = ((BasicBlock) getSource().getParent()).getOrdering();
	double fromPortID = ((BasicPort) getSource()).getOrdering();
	double fromType = 0; // ???
	double[][] fromData = {{fromBlockID, fromPortID, fromType}};
	data.add(new ScilabDouble(fromData)); // from
	
	double toBlockID = ((BasicBlock) getTarget().getParent()).getOrdering();
	double toPortID = ((BasicPort) getTarget()).getOrdering();
	double toType = 1; // ???
	double[][] toData = {{toBlockID, toPortID, toType}};
	data.add(new ScilabDouble(toData)); // to
	
	return data;
    }
    
    public abstract double[][] getColorAndType();
    
}
