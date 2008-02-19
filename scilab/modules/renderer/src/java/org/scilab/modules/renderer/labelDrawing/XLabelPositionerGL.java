/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class positioning X label   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.labelDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class positioning X label
 * @author Jean-Baptiste Silvy
 */
public class XLabelPositionerGL extends LabelPositionerGL {

	/**
	 * Defualt constructor
	 */
	public XLabelPositionerGL() {
		super();
	}
	
	/**
	 * Compute the position of the label for best display.
	 * @return array of size 3 containg the position 
	 */
	public Vector3D getLabelPosition() {
		
		double zCoordinate = findLowerZCoordinate();
		double yCoordinate = findFrontYCoordinate(zCoordinate);
		
		Vector3D axisStart = new Vector3D(getXmin(), yCoordinate, zCoordinate);
		Vector3D axisEnd = new Vector3D(getXmax(), yCoordinate, zCoordinate);
		
		// find ticks direction normalized
		Vector3D ticksDir = findXTicksDirection(yCoordinate, zCoordinate);
		
		return getLabelPosition(axisStart, axisEnd, ticksDir);
	}

}
