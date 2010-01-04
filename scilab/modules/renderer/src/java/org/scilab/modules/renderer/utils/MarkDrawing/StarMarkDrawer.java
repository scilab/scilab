/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing star marks    
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


/**
 * Class specialized in drawing star marks
 * @author Jean-Baptiste Silvy
 */
public class StarMarkDrawer extends CircleMarkDrawer {
	
	/**
	 * Default constructor
	 */
	public StarMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a star (outline circle, inside disk and a plus)
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		super.drawMark(gl, backColor, frontColor);
    // plus
    if(!isFrontTransparent)
    {
		  gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		  gl.glBegin(GL.GL_LINES);
		  gl.glVertex3d(-1.0,  0.0, 0.0);
		  gl.glVertex3d(1.0 ,  0.0, 0.0);
		  gl.glVertex3d(0.0 , -1.0, 0.0);
		  gl.glVertex3d(0.0 ,  1.0, 0.0);
		  gl.glEnd();
		}
	}

}
