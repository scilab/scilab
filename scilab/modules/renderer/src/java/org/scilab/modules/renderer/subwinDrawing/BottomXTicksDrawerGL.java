/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing ticks on of X axis placed on the bottom of the axes box
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
 * Class drawing ticks on of X axis placed on the bottom of the axes box
 * @author Jean-Baptiste Silvy
 */
public class BottomXTicksDrawerGL extends XTicksDrawerGL {

	/**
	 * Default constructor
	 */
	public BottomXTicksDrawerGL() {
		super();
	}
	
	/**
	 * Compute the Y coordinate of the X axis segment
	 * @param zCoordinate Z coordinate of the X axis segment alredy computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	public double findYCoordinate(double zCoordinate) {
		return findFrontYCoordinate(zCoordinate);
	}

	
	/**
	 * Find the Z coordinate of the X axis segment.
	 * X axis segment is always displayed on the bottom of tyhe box.
	 * @return Z coordinate of the segment to draw
	 */
	public double findZCoordinate() {
		return findLowerZCoordinate();
	}

}
