/*------------------------------------------------------------------------*/
/* file: StarMarkDrawer.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing star marks                         */
/*------------------------------------------------------------------------*/



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
		
		gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		// plus
		gl.glBegin(GL.GL_LINES);
		gl.glVertex3d(-1.0,  0.0, 0.0);
		gl.glVertex3d(1.0 ,  0.0, 0.0);
		gl.glVertex3d(0.0 , -1.0, 0.0);
		gl.glVertex3d(0.0 ,  1.0, 0.0);
		gl.glEnd();
		

	}

}
