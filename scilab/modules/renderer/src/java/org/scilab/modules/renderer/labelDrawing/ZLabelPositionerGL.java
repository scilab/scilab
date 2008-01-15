/*------------------------------------------------------------------------*/
/* file: ZLabelPositionerGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Z label                                       */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.labelDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class positioning X label
 * @author Jean-Baptiste Silvy
 */
public class ZLabelPositionerGL extends LabelPositionerGL {

	/**
	 * Defualt constructor
	 */
	public ZLabelPositionerGL() {
		super();
	}
	
	/**
	 * Compute the position of the label for best display.
	 * @return array of size 3 containg the position 
	 */
	public Vector3D getLabelPosition() {
		
		double xCoordinate = findLeftMostXCoordinate();
		double yCoordinate = findLeftMostYCoordinate();
		
		Vector3D axisStart = new Vector3D(xCoordinate, yCoordinate, getZmin());
		Vector3D axisEnd = new Vector3D(xCoordinate, yCoordinate, getZmax());
		
		// find ticks direction normalized
		Vector3D ticksDir = findZTicksDirection(xCoordinate, yCoordinate);
		
		return getLabelPosition(axisStart, axisEnd, ticksDir);
	}

}
