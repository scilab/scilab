/*------------------------------------------------------------------------*/
/* file: TriangleUpMarkDrawer.java                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing triangle up marks                  */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;

/**
 * Class specialized in drawing triangle up marks
 * @author Jean-Baptiste Silvy
 *
 */
public class SquareMarkDrawer extends MarkDrawingStrategy {


	/**
	 * Default constructor
	 */
	public SquareMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a, equilateral triangle pointing upward
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {

		// inside
		gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		gl.glBegin(GL.GL_QUADS);
		gl.glVertex3d(-1.0,  1.0, 0.0);
		gl.glVertex3d(-1.0, -1.0, 0.0);
		gl.glVertex3d(1.0 , -1.0, 0.0);
		gl.glVertex3d(1.0 ,  1.0, 0.0);
		gl.glEnd();
		
		// outline
		gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		gl.glBegin(GL.GL_LINE_LOOP);
		gl.glVertex3d(-1.0,  1.0, 0.0);
		gl.glVertex3d(-1.0, -1.0, 0.0);
		gl.glVertex3d(1.0 , -1.0, 0.0);
		gl.glVertex3d(1.0 ,  1.0, 0.0);
		gl.glEnd();

	}

	
}
