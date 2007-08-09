/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw inside   */
/*        of an arc object                                                */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.arcDrawing;

import org.scilab.modules.renderer.drawers.FillDrawerGL;

/**
 * Class containing the driver dependant routines to draw inside of an arc object.
 * @author Jean-Baptiste Silvy
 */
public class ArcFillDrawerGL extends FillDrawerGL implements ArcDrawerStrategy {

	/**
	 * Default constructor
	 */
	public ArcFillDrawerGL() {
		super();
	}
	
	/**
	 * create the display list for the arc
	 * @param centerX X coordinate of the arc center
	 * @param centerY Y coordinate of the arc center
	 * @param centerZ Z coordinate of the arc center
	 * @param semiMinorAxisX X coordinate of the ellipse minor axis
	 * @param semiMinorAxisY Y coordinate of the ellipse minor axis
	 * @param semiMinorAxisZ Z coordinate of the ellipse minor axis
	 * @param semiMajorAxisX X coordinate of the ellipse major axis
	 * @param semiMajorAxisY Y coordinate of the ellipse minor axis
	 * @param semiMajorAxisZ Z coordinate of the ellipse minor axis
	 * @param startAngle lowest angle in radian, relative to the semi-major axis of the ellipse.
     *                   Start of the angular part to draw.
	 * @param endAngle highest angle in radian, relative to the semi-major axis of the ellipse.
     *                 End of the angular part to draw
	 */
	public void drawArc(double centerX, double centerY, double centerZ,
						double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
						double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
						double startAngle, double endAngle) {

		System.err.println("DrawArc fill.");
	}
	
}
