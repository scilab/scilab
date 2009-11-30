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

import java.util.List;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

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
    public static void updateWestPortsPosition(BasicBlock block, List ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX(- portGeom.getWidth());
	    portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (ports.size() + 1.0))
		    - (portGeom.getHeight() / 2.0));
	}
    }

    /**
     * Dispatch ports on Block's _NORTH_ side.
     * @param ports
     */
    public static void updateNorthPortsPosition(BasicBlock block, List ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (ports.size() + 1.0))
		    - (portGeom.getWidth() / 2.0));
	    portGeom.setY(- portGeom.getHeight());
	}
    }

    /**
     * Dispatch ports on Block's _EAST_ side.
     * @param ports
     */
    public static void updateEastPortsPosition(BasicBlock block, List ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX(blockGeom.getWidth());
	    portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (ports.size() + 1.0))
		    - (portGeom.getHeight() / 2.0));
	}
    }

    /**
     * Dispatch ports on Block's _SOUTH_ side.
     * @param ports
     */
    public static void updateSouthPortsPosition(BasicBlock block, List ports) {
	mxGeometry blockGeom = block.getGeometry();
	if (blockGeom == null) {
	    return;
	}
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (ports.size() + 1.0))
		    - (portGeom.getWidth() / 2.0));
	    portGeom.setY(blockGeom.getHeight());
	}
    }

    /**
     * @param block : block target
     * @param blockDirection : new block orientation
     */
    public static void updatePortsPosition(BasicBlock block, String blockDirection) {
	// Block -> EAST
	// East <=> Out / North <=> Control / West <=> In / South <=> Command
	if (blockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) {
	    if (block.getFlip()) {
		updateEastPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
		updateWestPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
		updateWestPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
	    }
	}
	// Block -> NORTH
	// East <=> Command / North <=> Out / West <=> Control / South <=> In
	if (blockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) {
	    if(block.getFlip()) {
		updateWestPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
		updateEastPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
		updateWestPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
	    }
	}
	// Block -> WEST
	// East <=> In / North <=> Command / West <=> Out / South <=> Control
	if (blockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) {
	    if (block.getFlip()) {
		updateEastPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
		updateWestPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
		updateWestPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
	    }
	}
	// Block -> SOUTH
	// East <=> Control / North <=> In / West <=> Command / South <=> Out
	if (blockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) {
	    if (block.getFlip()) {
		updateWestPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
		updateEastPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
	    } else {
		updateEastPortsPosition(block, BasicBlockInfo.getAllControlPorts(block));
		updateNorthPortsPosition(block, BasicBlockInfo.getAllInputPorts(block));
		updateWestPortsPosition(block, BasicBlockInfo.getAllCommandPorts(block));
		updateSouthPortsPosition(block, BasicBlockInfo.getAllOutputPorts(block));
	    }
	}
    }    

    /**
     * @param currentBlockDirection : current block orientation
     * @return : new orientation
     */
    public static String getNextAntiClockwiseDirection(String currentBlockDirection) {
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_NORTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_WEST; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_SOUTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_EAST; }
	return null;
    }

    /**
     * @param currentBlockDirection : current block orientation
     * @return : new orientation
     */
    public static String getNextClockwiseDirection(String currentBlockDirection) {
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_SOUTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_EAST; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_NORTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_WEST; }
	return null;
    }

    /**
     * @param currentBlockDirection : current block orientation
     * @return : new orientation
     */
    public static String getNextFlipDirection(String currentBlockDirection) {
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_WEST; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_SOUTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_EAST; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_NORTH; }
	return null;
    }

    /**
     * @param block : block target
     * @param currentBlockDirection : current block orientation
     * @return : event ports orientation
     */
    public static String getEventPortsDirection(BasicBlock block, String currentBlockDirection) {
	if (block.getFlip()) {
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_NORTH; }
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_WEST; }
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_SOUTH; }
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_EAST; }
	    return null;
	} else {
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_SOUTH; }
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_EAST; }
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_NORTH; }
	    if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_WEST; }
	    return null;
	}
    }


    /**
     * @param currentBlockDirection
     * @return block orientation
     */
    public static String getDataPortsDirection(String currentBlockDirection) {
	return currentBlockDirection;
    }

    /**
     * @param block
     * @param ports
     * @param portOrientation
     */
    public static void rotatePorts(BasicBlock block, List ports , String portOrientation) {
	for (int i = 0 ; i < ports.size() ; ++i) {
	    mxUtils.setCellStyles(block.getParentDiagram().getModel(), new Object[] {ports.get(i)}, mxConstants.STYLE_DIRECTION, portOrientation);
	}
    }

    /**
     * @param block
     * @param newBlockDirection
     */
    public static void updateBlockDirection(BasicBlock block, String newBlockDirection) {
	
	
	mxUtils.setCellStyles(block.getParentDiagram().getModel(), new Object[] {block}, mxConstants.STYLE_DIRECTION, newBlockDirection);
	
//	mxUtils.setCellStyles(block.getParentDiagram().getModel(), new Object[] {block},
//		mxConstants.STYLE_ROTATION, new Integer(angle).toString());

	rotatePorts(block, BasicBlockInfo.getAllInputPorts(block), getDataPortsDirection(newBlockDirection));
	rotatePorts(block, BasicBlockInfo.getAllOutputPorts(block), getDataPortsDirection(newBlockDirection));
	rotatePorts(block, BasicBlockInfo.getAllCommandPorts(block), getEventPortsDirection(block, newBlockDirection));
	rotatePorts(block, BasicBlockInfo.getAllControlPorts(block), getEventPortsDirection(block, newBlockDirection));
    }

    /**
     * @param block
     */
    public static void updateBlockView(BasicBlock block) {

	if (block != null 
		&& block.getParentDiagram() != null 
		&& block.getParentDiagram().getView() != null 
		&& block.getParentDiagram().getView().getState(block) != null) {
	    
	    mxCellState state = block.getParentDiagram().getView().getState(block);
	    String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);
	    //currentBlockDirection = mxConstants.DIRECTION_EAST;
	    BlockPositioning.updatePortsPosition(block, currentBlockDirection);
	    rotatePorts(block, BasicBlockInfo.getAllInputPorts(block), getDataPortsDirection(currentBlockDirection));
	    rotatePorts(block, BasicBlockInfo.getAllOutputPorts(block), getDataPortsDirection(currentBlockDirection));
	    rotatePorts(block, BasicBlockInfo.getAllCommandPorts(block), getEventPortsDirection(block, currentBlockDirection));
	    rotatePorts(block, BasicBlockInfo.getAllControlPorts(block), getEventPortsDirection(block, currentBlockDirection));
	}
    }

    /**
     * @param block
     */
    public static void toggleFlip(BasicBlock block) {

	block.setFlip(!block.getFlip());

	mxCellState state = block.getParentDiagram().getView().getState(block);
	String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);
	String  currentFlip = mxUtils.getString(state.getStyle(), XcosConstants.STYLE_FLIP, "false");
	if(currentFlip.compareTo("true") == 0) {
		currentFlip = "false";
	} else {
		currentFlip = "true";
	}
	mxUtils.setCellStyles(block.getParentDiagram().getModel(), new Object[] {block}, XcosConstants.STYLE_FLIP, currentFlip);
	
	updatePortsPosition(block, getNextFlipDirection(currentBlockDirection));
	updateBlockDirection(block, getNextFlipDirection(currentBlockDirection));
    }


    /**
     * @param block
     */
    public static void toggleAntiClockwiseRotation(BasicBlock block) {
	mxCellState state = block.getParentDiagram().getView().getState(block);
	String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);

	updatePortsPosition(block, getNextAntiClockwiseDirection(currentBlockDirection));
	updateBlockDirection(block, getNextAntiClockwiseDirection(currentBlockDirection));
    }
    
    public static int getAngleFromDirection(String direction) {
	    if (direction.compareTo(mxConstants.DIRECTION_EAST) == 0) { return 0; }
	    if (direction.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return 270; }
	    if (direction.compareTo(mxConstants.DIRECTION_WEST) == 0) { return 180; }
	    if (direction.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return 90; }
	    return 0;
    }
}
