/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing ticks on of Y axis placed on the middle of the  axes box
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing ticks on of Y axis placed on the left of the axes box.
 * @author Jean-Baptiste Silvy
 */
public class MiddleYTicksDrawerGL extends YTicksDrawerGL {

	/**
	 * Default consturctor
	 */
	public MiddleYTicksDrawerGL() {
		super();
	}
	
	
	/**
	 * Compute the X coordinate of the X axis segment
	 * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	public double findXCoordinate(double zCoordinate) {
		return findMiddleXCoordinate(zCoordinate);
	}

	/**
	 * Find the Z coordinate of the Y axis segment.
	 * Y axis segment is always displayed on the bottom of the box.
	 * @return Z coordinate of the segment to draw
	 */
	public double findZCoordinate() {
		return findUpperZCoordinate();
	}

}

