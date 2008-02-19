/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class positioning Y label
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
 * Class positioning Y label
 * @author Jean-Baptiste Silvy
 */
public class YLabelPositionerGL extends LabelPositionerGL {

	/**
	 * Defualt constructor
	 */
	public YLabelPositionerGL() {
		super();
	}
	
	/**
	 * Compute the position of the label for best display.
	 * @return array of size 3 containg the position 
	 */
	public Vector3D getLabelPosition() {
		
		double zCoordinate = findLowerZCoordinate();
		double xCoordinate = findFrontXCoordinate(zCoordinate);
		
		Vector3D axisStart = new Vector3D(xCoordinate, getYmin(), zCoordinate);
		Vector3D axisEnd = new Vector3D(xCoordinate, getYmax(), zCoordinate);
		
		// find ticks direction normalized
		Vector3D ticksDir = findYTicksDirection(xCoordinate, zCoordinate);
		
		return getLabelPosition(axisStart, axisEnd, ticksDir);
	}

}

