/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte Denizet
 * desc : Class specialized in drawing circle marks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import javax.media.opengl.GL;
import org.scilab.modules.renderer.utils.MarkDrawing.CircleMarkDrawer;

/**
 * Class specialized in drawing circle marks
 * @author Calixte Denizet
 */
public class GL2PSCircleMarkDrawer extends GL2PSMarkDrawingStrategy {

	/**
	 * Default constructor
	 */
	public GL2PSCircleMarkDrawer() {
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
		    gl.glBegin(GL.GL_POINTS);
		    gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		    gl.glVertex3f(0f, 0f, 0f);
		    gl.glEnd();
		}
	    
	    // circle
	    if(!isFrontTransparent)
		{
		    gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		    gl.glBegin(GL.GL_LINE_LOOP);
		    for (int i = 0; i < CircleMarkDrawer.NB_SLICES; i++) {
			double angle = 2.0 * Math.PI * i / CircleMarkDrawer.NB_SLICES;
			gl.glVertex3d(Math.cos(angle), Math.sin(angle), 0.0);
		    }
		    gl.glEnd();
		}    
	}
}