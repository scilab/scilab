/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.arcDrawing;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * FastArcFillTools
 * @author Sylvestre Koumar
 *
 */
public class FastArcFillTools extends ArcFillTools {
	
	/**
	 * To avoid drawing the circle several times
	 */
	private static final double TWO_PI = 2 * Math.PI;
	
	/** Angle of the X axis in gluPartialDisk */
	private static final double X_AXIS_ANGLE = 90.0;

	/**
	 * Constructor
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 */
	protected FastArcFillTools(Vector3D center, Vector3D semiMinorAxis,
			Vector3D semiMajorAxis, double startAngle, double endAngle) {
		super(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
	}

	/**
	 * drawCircle
	 * Call gl routine to draw  a circle
	 * @param gl GL
	 */
	protected void drawCircle(GL gl) {	
		GLU glu = new GLU();
		double drawAngle;
				
		if (getSweepAngle() < -TWO_PI) {
			drawAngle = -TWO_PI;		
		} else if (getSweepAngle() <= TWO_PI) {
			drawAngle = getSweepAngle();		
		} else {
			drawAngle = TWO_PI;		
		}
		
		/* In gluPartialDisk, X axis as a start angle of 90 degrees and the sweepangle is clockwise */
		glu.gluPartialDisk(glu.gluNewQuadric(),  0.0, 1.0, NB_SLICES, 1, X_AXIS_ANGLE, -Math.toDegrees(drawAngle));
		
	}

}
