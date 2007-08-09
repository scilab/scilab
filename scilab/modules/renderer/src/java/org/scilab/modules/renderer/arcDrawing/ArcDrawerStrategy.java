/*------------------------------------------------------------------------*/
/* file: ArcDrawerStrategy.java                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for arc drawing                                       */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.arcDrawing;

/**
 * Interface for arc drawing
 * @author Jean-Baptiste Silvy
 */
public interface ArcDrawerStrategy {

	/**
	 * create the display list for the rectangle
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
	void drawArc(double centerX, double centerY, double centerZ,
		     	 double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
				 double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
				 double startAngle, double endAngle);
	
}
