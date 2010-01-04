/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing circle marks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

/**
 * Class specialized in drawing circle marks
 * @author Jean-Baptiste Silvy
 */
public class CircleMarkDrawer extends MarkDrawingStrategy {

	/** number of slice for the circle */
	private static final int NB_SLICES = 64;
	
	/**
	 * Default constructor
	 */
	public CircleMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a circle
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		// disk
    if(!isBackTransparent)
    {
		  gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		  GLU glu = new GLU();
		  glu.gluDisk(glu.gluNewQuadric(), 0.0, 1.0, NB_SLICES, 1);
		}
		
		// circle
    if(!isFrontTransparent)
    {
		  gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		  gl.glBegin(GL.GL_LINE_LOOP);
		  for (int i = 0; i < NB_SLICES; i++) {
			  double angle = 2.0 * Math.PI * i / NB_SLICES;
			  gl.glVertex3d(Math.cos(angle), Math.sin(angle), 0.0);
		  }
		  gl.glEnd();
		}

	}

}
