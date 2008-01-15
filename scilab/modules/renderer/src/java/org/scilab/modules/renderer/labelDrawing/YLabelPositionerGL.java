/*------------------------------------------------------------------------*/
/* file: YLabelPositionerGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Y label                                       */
/*------------------------------------------------------------------------*/


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

