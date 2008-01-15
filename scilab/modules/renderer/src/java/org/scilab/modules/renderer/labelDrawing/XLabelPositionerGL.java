/*------------------------------------------------------------------------*/
/* file: XLabelPositionerGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning X label                                       */
/*------------------------------------------------------------------------*/


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
