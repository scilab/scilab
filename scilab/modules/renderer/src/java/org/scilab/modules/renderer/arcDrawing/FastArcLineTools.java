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

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * FastArcLineTools
 * @author Sylvestre Koumar
 *
 */
public class FastArcLineTools extends ArcLineTools {
	
	/**
	 * To avoid drawing the circle several times
	 */
	private static final double TWO_PI = 2 * Math.PI;


	/**
	 * Constructor
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 */
	protected FastArcLineTools(Vector3D center, Vector3D semiMinorAxis,
			Vector3D semiMajorAxis, double startAngle, double endAngle) {
		super(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
	}
	
	/**
	 * drawCircle
	 * Call gl routine to draw  a circle
	 * @param gl GL
	 */
	protected void drawCircle(GL gl) {		
		gl.glBegin(GL.GL_LINE_STRIP);
		
		double drawAngle;
		
		if (getSweepAngle() < -TWO_PI) {
			drawAngle = -TWO_PI;		
		} else if (getSweepAngle() >= -TWO_PI && getSweepAngle() <= TWO_PI) {
			drawAngle = getSweepAngle();		
		} else {
			drawAngle = TWO_PI;		
		}		
		
		for (int i = 0; i < NB_SLICES; i++) {
			double angle = (drawAngle * i) / ((double) (NB_SLICES - 1));
			gl.glVertex3d(Math.cos(angle), Math.sin(angle), 0.0);
		}
		gl.glEnd();	
		
	}

}
