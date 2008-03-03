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
	 * @param gl GL
	 */
	protected void drawCircle(GL gl) {		
		gl.glBegin(GL.GL_LINE_STRIP);
		for (int i = 0; i < NB_SLICES; i++) {
			double angle = (getSweepAngle() * i) / ((double) (NB_SLICES - 1));
			gl.glVertex3d(Math.cos(angle), Math.sin(angle), 0.0);
		}
		gl.glEnd();	
		
	}

}
