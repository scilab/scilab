/*------------------------------------------------------------------------*/
/* file: ArcDrawerStrategy.java                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for arc drawing                                       */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.arcDrawing;

import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUnurbs;

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
	
	/**
	 * Draw part af circle starting from the point of angle start angle to angular region.
	 * The arc is centered on the origin.
	 * @param glu current GLU object/
	 * @param nurbsObj nurbsObj used to draw
	 * @param startAngle angle of the begining of the arc.
	 * @param sweepAngle size of the arc segment in radian. Should be lower than Pi and gt 0.
	 */
	void drawArcPart(GLU glu, GLUnurbs nurbsObj, double startAngle, double sweepAngle);
	
}
