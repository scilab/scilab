/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing ticks for Axis objects
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.axesDrawing;

import org.scilab.modules.renderer.subwinDrawing.TicksDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class drawing ticks for Axis objects
 * @author Jean-Baptiste Silvy
 */
public abstract class AxesTicksDrawerGL extends TicksDrawerGL {

	private Vector3D ticksDirection;
	
	/**
	 * Default constructor
	 */
	public AxesTicksDrawerGL() {
		super();
	}
	
	/**
	 * @return always true, we don't check axes
	 */
	public boolean checkTicks() {
		return true;
	}

	/**
	 * Specify the direction to use for ticks
	 * @param dir scilab char defining direction of the ticks ('u', 'd', 'l', 'r').
	 */
	public void setTicksDirection(String dir) {
		char direction = dir.charAt(0);
		switch (direction) {
		case 'u':
			ticksDirection =  new Vector3D(0.0, 1.0, 0.0);
			break;
		case 'd':
			ticksDirection =  new Vector3D(0.0, -1.0, 0.0);
			break;
		case 'l':
			ticksDirection =  new Vector3D(-1.0, 0.0, 0.0);
			break;
		case 'r':
			ticksDirection =  new Vector3D(1.0, 0.0, 0.0);
			break;
		default:
			break;
		}
	}
	
	/**
	 * @return direction of the ticks
	 */
	public Vector3D getTicksDirection() {
		return setTicksDirectionLength(ticksDirection);
	}
	
	/**
	 * Draw ticks from the recorded data.
	 * @return maximum distance from ticks to the axis.
	 */
	public double drawTicks() {
		
		Vector3D[] ticksPosition = findTicksPositions();
		Vector3D[] subticksPosition = findSubticksPositions();
		Vector3D ticksDir = getTicksDirection();
		
		return drawTicks(ticksPosition, subticksPosition, ticksDir,
					getAxisSegmentStart(), getAxisSegmentEnd());
	}
	
	/**
	 * @return an array containing positions of ticks along the axis
	 */
	protected abstract Vector3D[] findTicksPositions();
	
	/**
	 * @return an array containing positions of subticks along the axis
	 */
	protected abstract Vector3D[] findSubticksPositions();
	
	/**
	 * @return position of the beginning of the axis
	 */
	protected abstract Vector3D getAxisSegmentStart();
	
	/**
	 * @return position of the end of the axis
	 */
	protected abstract Vector3D getAxisSegmentEnd();

}
