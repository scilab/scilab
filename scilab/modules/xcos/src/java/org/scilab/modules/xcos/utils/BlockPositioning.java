/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.awt.Point;
import java.awt.Rectangle;
import java.util.List;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

public final class BlockPositioning {

    /**
     * Constructor
     */

    private BlockPositioning() {

    }
    /**
     * Dispatch ports on Block's _WEST_ side.
     * @param ports
     */
    public static void updateWestPortsPosition(BasicBlock block, List<? extends BasicPort> ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	
	beginUpdate(block);
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX(- portGeom.getWidth());
	    portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (ports.size() + 1.0))
		    - (portGeom.getHeight() / 2.0));
	}
	endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _NORTH_ side.
     * @param ports
     */
    public static void updateNorthPortsPosition(BasicBlock block, List<? extends BasicPort> ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	
	beginUpdate(block);
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (ports.size() + 1.0))
		    - (portGeom.getWidth() / 2.0));
	    portGeom.setY(- portGeom.getHeight());
	}
	endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _EAST_ side.
     * @param ports
     */
    public static void updateEastPortsPosition(BasicBlock block, List<? extends BasicPort> ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	
	beginUpdate(block);
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX(blockGeom.getWidth());
	    portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (ports.size() + 1.0))
		    - (portGeom.getHeight() / 2.0));
	}
	endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _SOUTH_ side.
     * @param ports
     */
    public static void updateSouthPortsPosition(BasicBlock block, List<? extends BasicPort> ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	
	beginUpdate(block);
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (ports.size() + 1.0))
		    - (portGeom.getWidth() / 2.0));
	    portGeom.setY(blockGeom.getHeight());
	}
	endUpdate(block);
    }

    /**
     * @param block : block target
     * @param blockDirection : new block orientation
     */
    public static void updatePortsPosition(BasicBlock block) {
	// Block -> EAST
	// East <=> Out / North <=> Control / West <=> In / South <=> Command
	if (block.getAngle() == 0) {
	    if(block.getMirror()) {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, block.getFlip()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, block.getFlip()));
	    } else {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, block.getFlip()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, block.getFlip()));
	    }
	    
	    if (block.getFlip()) {
		updateEastPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, block.getMirror()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, block.getMirror()));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, block.getMirror()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, block.getMirror()));
	    }
	}
	// Block -> NORTH
	// East <=> Command / North <=> Out / West <=> Control / South <=> In
	if (block.getAngle() == 270) {
	    if(block.getMirror()) {
		updateEastPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, !block.getFlip()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, !block.getFlip()));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, !block.getFlip()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, !block.getFlip()));
	    }
	    
	    if(block.getFlip()) {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, block.getMirror()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, block.getMirror()));
	    } else {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, block.getMirror()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, block.getMirror()));
	    }
	}
	// Block -> WEST
	// East <=> In / North <=> Command / West <=> Out / South <=> Control
	if (block.getAngle() == 180) {
	    if(block.getMirror()) {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, !block.getFlip()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, !block.getFlip()));
	    } else {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, !block.getFlip()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, !block.getFlip()));
	    }
	
	    if (block.getFlip()) {
		updateEastPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, !block.getMirror()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, !block.getMirror()));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, !block.getMirror()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, !block.getMirror()));
	    }
	}
	// Block -> SOUTH
	// East <=> Control / North <=> In / West <=> Command / South <=> Out
	if (block.getAngle() == 90) {
	    if(block.getMirror()) {
		updateEastPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, block.getFlip()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, block.getFlip()));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllControlPorts(block, block.getFlip()));
		updateWestPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block, block.getFlip()));
	    }

	    if (block.getFlip()) {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, !block.getMirror()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, !block.getMirror()));
	    } else {
		updateSouthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block, !block.getMirror()));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block, !block.getMirror()));
	    }
	}
    }    

    /**
     * @param block
     * @param ports
     * @param portOrientation
     */
    public static void rotatePorts(BasicBlock block, List<? extends BasicPort> ports , int angle) {

	int newAngle = angle;
	if(block.getFlip()) {
	    newAngle += 180;
	    newAngle %= 360;
	}

	if(block.getMirror()) {
	    newAngle += 180;
	    newAngle %= 360;
	}
	
	beginUpdate(block);
	for(Object obj : ports) {

	    if(obj instanceof BasicPort) {
		BasicPort port = (BasicPort) obj;
		if (port.getAngle() != newAngle)  {
		    port.setAngle(newAngle);
		    int newAngle2 = port.getAngle();
		    mxUtils.setCellStyles(block.getParentDiagram().getModel(), new Object[] {port}, mxConstants.STYLE_ROTATION, new Integer(newAngle2).toString());
		}
	    }
	}
	endUpdate(block);
    }

    public static int getDataPortsAngle(BasicBlock block) {
	if(block.getMirror()) {
	    return (block.getAngle() + 180) % 360;
	} else {
	    return block.getAngle();   
	}
    }

    public static int getEventPortsAngle(BasicBlock block) {
	if(block.getFlip()) {
	    return (block.getAngle() + 90) % 360;
	} else {
	    return (block.getAngle() + 270) % 360;
	}
    }
    
    public static void rotateAllPorts(BasicBlock block) {
	rotatePorts(block, BasicBlockInfo.getAllInputPorts(block, false), getDataPortsAngle(block));
	rotatePorts(block, BasicBlockInfo.getAllOutputPorts(block, false), getDataPortsAngle(block));
	rotatePorts(block, BasicBlockInfo.getAllCommandPorts(block, false), getEventPortsAngle(block));
	rotatePorts(block, BasicBlockInfo.getAllControlPorts(block, false), getEventPortsAngle(block));
    }

    /**
     * @param block
     */
    public static void updateBlockView(BasicBlock block) {

	if (block != null 
		&& block.getParentDiagram() != null 
		&& block.getParentDiagram().getView() != null 
		&& block.getParentDiagram().getView().getState(block) != null) {
	    block.getParentDiagram().getModel().beginUpdate();
	    updatePortsPosition(block);
	    rotateAllPorts(block);
	    block.getParentDiagram().getModel().endUpdate();
	}
    }

    /**
     * @param block
     */
    public static void toggleFlip(BasicBlock block) {

	block.setFlip(!block.getFlip());
	updateBlockView(block);
    }

    /**
     * @param block
     */
    public static void toggleMirror(BasicBlock block) {

	block.setMirror(!block.getMirror());
	updateBlockView(block);
    }

    /**
     * @param block
     */
    public static void toggleAntiClockwiseRotation(BasicBlock block) {
	block.setAngle(getNextAntiClockwiseAngle(block));
	updateBlockView(block);
    }
    
    public static int getNextAntiClockwiseAngle(BasicBlock block) {
	if (block.getAngle() == 0) { return 270; }
	if (block.getAngle() == 90) { return 0; }
	if (block.getAngle() == 180) { return 90; }
	if (block.getAngle() == 270) { return 180; }
	return 0;
    }

    public static int getNextClockwiseAngle(BasicBlock block) {
	if (block.getAngle() == 0) { return 90; }
	if (block.getAngle() == 90) { return 180; }
	if (block.getAngle() == 180) { return 270; }
	if (block.getAngle() == 270) { return 0; }
	return 0;
    }
    
    /**
     * Convert any angle value to a valid block value
     * @param angle the non valid value
     * @return the nearest graph valid value
     */
    public static int roundAngle(int angle) {
	if (angle < 0 || angle > 360)
	    angle = angle + 360 % 360;
	
	if (angle < (0 + 90)/2) { return 0; }
	if (angle < (90 + 180)/2) { return 90; }
	if (angle < (180 + 270)/2) { return 180; }
	if (angle < (270 + 360)/2) { return 270; }
	return 0;
    }
   
    public static Rectangle rotateRectangle(Rectangle rect, int angle) {
	Point tl = new Point(-rect.width / 2, -rect.height / 2); //top left
	Point tr = new Point(tl.x + rect.width, tl.y); //top right
	Point br = new Point(tr.x, tr.y + rect.height); //bottom right
	Point bl = new Point(tl.x, br.y); //bottom left 

	Point tl2 = rotatePoint(tl, angle);
	Point tr2 = rotatePoint(tr, angle);
	Point bl2 = rotatePoint(bl, angle);
	Point br2 = rotatePoint(br, angle);
	
	int x = Math.min(tl2.x, Math.min(tr2.x, Math.min(br2.x, bl2.x)));
	int y = Math.min(tl2.y, Math.min(tr2.y, Math.min(br2.y, bl2.y)));
	int width = Math.max(tl2.x, Math.max(tr2.x, Math.max(br2.x, bl2.x))) - x;
	int height = Math.max(tl2.y, Math.max(tr2.y, Math.max(br2.y, bl2.y))) - y;

	Rectangle result = new Rectangle(x,y,width, height);
	return result;
    }
    
    private static Point rotatePoint(Point point, int angle) {
    
	double angleRad = (angle * Math.PI ) / 180;
	int x = 0;
	int y = 0;
	
	x = (int)(point.getX() * Math.cos(angleRad) - point.getY() * Math.sin(angleRad));
	y = (int)(point.getX() * Math.sin(angleRad) + point.getY() * Math.cos(angleRad));
	Point result = new Point(x,y);
	return result;
    }
    
    private static void beginUpdate(BasicBlock block) {
	if (block != null && block.getParentDiagram() != null) {
	    block.getParentDiagram().getModel().beginUpdate();
	}
    }
    
    private static void endUpdate(BasicBlock block) {
	if (block != null && block.getParentDiagram() != null) {
	    block.getParentDiagram().getModel().endUpdate();
	}
    }
}
