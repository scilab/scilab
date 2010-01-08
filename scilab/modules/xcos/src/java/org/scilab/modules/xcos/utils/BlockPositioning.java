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
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Orientation;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxUtils;

/**
 * Helpers to place port on a block. 
 */
public final class BlockPositioning {

	/** This class is a static singleton, thus it must not be instantiated */
    private BlockPositioning() { }
    
    /**
     * Dispatch ports on Block's _WEST_ side.
     * @param block The block we have to work on.
     * @param ports The ports we have to move on the side.
     */
    public static void updateWestPortsPosition(BasicBlock block, List< ? extends BasicPort> ports) {
	final mxGeometry blockGeom = block.getGeometry();
	final int portsSize = ports.size();
	
	assert blockGeom != null;
	
	beginUpdate(block);
	for (int i = 0; i < portsSize; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX(-portGeom.getWidth());
	    portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (portsSize + 1.0))
		    - (portGeom.getHeight() / 2.0));
	}
	endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _NORTH_ side.
     * @param block The block we have to work on.
     * @param ports The ports we have to move on the side.
     */
    public static void updateNorthPortsPosition(BasicBlock block, List< ? extends BasicPort> ports) {
	final mxGeometry blockGeom = block.getGeometry();
	final int portsSize = ports.size();
	
	assert blockGeom != null;
	
	beginUpdate(block);
	for (int i = 0; i < portsSize; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (portsSize + 1.0))
		    - (portGeom.getWidth() / 2.0));
	    portGeom.setY(-portGeom.getHeight());
	}
	endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _EAST_ side.
     * @param block The block we have to work on.
     * @param ports The ports we have to move on the side.
     */
    public static void updateEastPortsPosition(BasicBlock block, List< ? extends BasicPort> ports) {
	final mxGeometry blockGeom = block.getGeometry();
	final int portsSize = ports.size();
	
	assert blockGeom != null;
	
	beginUpdate(block);
	for (int i = 0; i < portsSize; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX(blockGeom.getWidth());
	    portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (portsSize + 1.0))
		    - (portGeom.getHeight() / 2.0));
	}
	endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _SOUTH_ side.
     * @param block The block we have to work on.
     * @param ports The ports we have to move on the side.
     */
    public static void updateSouthPortsPosition(BasicBlock block, List< ? extends BasicPort> ports) {
	final mxGeometry blockGeom = block.getGeometry();
	final int portsSize = ports.size();
	
	assert blockGeom != null;
	
	beginUpdate(block);
	for (int i = 0; i < portsSize; ++i) {
	    mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
	    portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (portsSize + 1.0))
		    - (portGeom.getWidth() / 2.0));
	    portGeom.setY(blockGeom.getHeight());
	}
	endUpdate(block);
    }

    /**
     * Update all the port position of the block.
     * @param block The block we have to work on.
     */
    public static void updatePortsPosition(BasicBlock block) {
		final Map<BasicPort.Orientation, List<BasicPort>> ports = BasicBlockInfo
				.getAllOrientedPorts(block);

		beginUpdate(block);
		for (Orientation iter : Orientation.values()) {
			List<BasicPort> orientedPorts = ports.get(iter);
			if (orientedPorts != null && !orientedPorts.isEmpty()) {
				updatePortsPositions(block, orientedPorts, iter);
			}
		}
		endUpdate(block);
	}

	/**
	 * Update the port position for the specified orientation. This function
	 * manage the flip and mirror properties.
	 * 
	 * @param block
	 *            The block we are working on
	 * @param ports
	 *            The ports at with the specified orientation
	 * @param iter
	 *            The orientation.
	 */
	private static void updatePortsPositions(BasicBlock block,
			List<BasicPort> ports, Orientation iter) {
		final List<BasicPort> invertedPorts = new ArrayList<BasicPort>(ports) {
			{
				Collections.reverse(this);
			}
		};
		final boolean mirrored = block.getMirror();
		final boolean flipped = block.getFlip();
		final int angle = block.getAngle();
		List<BasicPort> working = ports;
		
		/* List order modification with the flip flag */
		if (flipped) {
			if (iter == Orientation.NORTH || iter == Orientation.SOUTH) {
				working = invertedPorts;
			}
		}

		/* List order modification with the mirror flag */
		if (mirrored) {
			if (iter == Orientation.EAST || iter == Orientation.WEST) {
				working = invertedPorts;
			}
		}
		
		/*
		 * Ugly modification of the iter to update at the right position
		 * Works only for 0 - 90 - 180 - 270 angles.
		 */
		final int nbOfOrientations = Orientation.values().length; // 4
		Orientation rotated = iter;
		
		/* Flip & Mirror management */
		if (flipped) {
			if (rotated == Orientation.EAST || rotated == Orientation.WEST) {
				rotated = Orientation.values()[(rotated.ordinal() + 2)
						% nbOfOrientations];
			}
		}
		if (mirrored) {
			if (rotated == Orientation.NORTH || rotated == Orientation.SOUTH) {
				rotated = Orientation.values()[(rotated.ordinal() + 2)
						% nbOfOrientations];
			}
		}
		
		updatePortsPosition(block, rotated, angle, working);
	}

	/**
	 * Update the ports positions according to the angle. This function doesn't handle order inversion. 
	 * @param block The block we are working on
	 * @param iter The current port orientation
	 * @param angle The angle we have to rotate
	 * @param working The ordered ports we are working on.
	 */
	private static void updatePortsPosition(BasicBlock block, Orientation iter,
			final int angle, List<BasicPort> working) {
		/*
		 * Ugly modification of the iter to update at the right position
		 * Works only for 0 - 90 - 180 - 270 angles.
		 */
		final int nbOfOrientations = Orientation.values().length; // 4
		Orientation rotated = iter;
		
		/* Angle management */
		if (angle == 90) {
			rotated = Orientation.values()[(rotated.ordinal() + 1)
					% nbOfOrientations];
		} else if (angle == 180) {
			rotated = Orientation.values()[(rotated.ordinal() + 2)
					% nbOfOrientations];
		} else if (angle == 270) {
			rotated = Orientation.values()[(rotated.ordinal() + 3)
					% nbOfOrientations];
		}

		/* Call the associated function */
		switch (rotated) {
		case NORTH:
			updateNorthPortsPosition(block, working);
			break;
		case SOUTH:
			updateSouthPortsPosition(block, working);
			break;
		case EAST:
			updateEastPortsPosition(block, working);
			break;
		case WEST:
			updateWestPortsPosition(block, working);
			break;

		default:
			break;
		}
	}
    
    /**
     * Rotate all the port of the block.
     * @param block The block to work on.
     */
	public static void rotateAllPorts(BasicBlock block) {
		final int angle = block.getAngle();
		final boolean flipped = block.getFlip();
		final boolean mirrored = block.getMirror();

		final int childrenCount = block.getChildCount();
		for (int i = 0; i < childrenCount; ++i) {
			final BasicPort port = (BasicPort) block.getChildAt(i);
			final Orientation orientation = port.getOrientation();

			beginUpdate(block);

			/* Apply angle */
			port.setAngle(orientation.getAngle(angle, flipped, mirrored));

			endUpdate(block);
		}
	}

    /**
     * Update the geometry of the block's ports.
     * @param block The block to work on
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
	    block.getParentDiagram().refresh();
	}
    }

    /**
     * Flip a block (horizontal inversion).
     * @param block The block to work on
     */
    public static void toggleFlip(BasicBlock block) {

	block.setFlip(!block.getFlip());
	updateBlockView(block);
    }

    /**
     * Mirror a block (vertical inversion).
     * @param block The block to work on
     */
    public static void toggleMirror(BasicBlock block) {

	block.setMirror(!block.getMirror());
	updateBlockView(block);
    }

    /**
     * Rotate a block with an anti-clockwise next value
     * @param block The block to work on
     */
    public static void toggleAntiClockwiseRotation(BasicBlock block) {
	block.setAngle(getNextAntiClockwiseAngle(block));
	updateBlockView(block);
    }
    
    /**
     * Get the next anti-clockwise rotation value
     * @param block The block to work on
     * @return The angle value
     */
    public static int getNextAntiClockwiseAngle(BasicBlock block) {
    	int angle = 0;
	if (block.getAngle() == 0) { angle = 270; }
	else if (block.getAngle() == 90) { angle = 0; }
	else if (block.getAngle() == 180) { angle = 90; }
	else if (block.getAngle() == 270) { angle = 180; }
	return angle;
    }

    /**
     * Get the next clockwise rotation value
     * @param block The block to work on
     * @return The angle value
     */
    public static int getNextClockwiseAngle(BasicBlock block) {
    	int angle = 0;
	if (block.getAngle() == 0) { angle = 90; }
	else if (block.getAngle() == 90) { angle = 180; }
	else if (block.getAngle() == 180) { angle = 270; }
	else if (block.getAngle() == 270) { angle = 0; }
	return angle;
    }
    
    /**
     * Convert any angle value to a valid block value
     * @param angle the non valid value
     * @return the nearest graph valid value
     */
    public static int roundAngle(int angle) {
    	int ret = 0;
	if (angle < 0 || angle > 360) {
	    angle = angle + 360 % 360;
	}
	
	if (angle < (0 + 90)/2) { ret = 0; }
	else if (angle < (90 + 180)/2) { ret = 90; }
	else if (angle < (180 + 270)/2) { ret = 180; }
	else if (angle < (270 + 360)/2) { ret = 270; }
	return ret;
    }
   
    @Deprecated
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
    
    @Deprecated
    private static Point rotatePoint(Point point, int angle) {
    
	double angleRad = (angle * Math.PI ) / 180;
	int x = 0;
	int y = 0;
	
	x = (int)(point.getX() * Math.cos(angleRad) - point.getY() * Math.sin(angleRad));
	y = (int)(point.getX() * Math.sin(angleRad) + point.getY() * Math.cos(angleRad));
	Point result = new Point(x,y);
	return result;
    }
    
    /**
     * Helper function that protect the block model.
     * @param block The block to protect
     */
    private static void beginUpdate(BasicBlock block) {
	if (block != null && block.getParentDiagram() != null) {
	    block.getParentDiagram().getModel().beginUpdate();
	}
    }
    
    /**
     * Helper function that end the protection of the block model.
     * @param block The block previously protected
     */
    private static void endUpdate(BasicBlock block) {
	if (block != null && block.getParentDiagram() != null) {
	    block.getParentDiagram().getModel().endUpdate();
	}
    }
}
