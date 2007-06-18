/*------------------------------------------------------------------------*/
/* file: FilledDiamondMarkDrawer.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing filled diamond marks               */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;

/**
 * Class specialized in drawing filled diamond marks
 * @author Jean-Baptiste Silvy
 */
public class FilledDiamondMarkDrawer extends MarkDrawingStrategy {

	/**
	 * Default constructor
	 */
	public FilledDiamondMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a filled diamond
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		gl.glBegin(GL.GL_QUADS);
		gl.glVertex3d(-1.0,  0.0, 0.0);
		gl.glVertex3d(0.0 ,  1.0, 0.0);
		gl.glVertex3d(1.0 ,  0.0, 0.0);
		gl.glVertex3d(0.0 , -1.0, 0.0);
		gl.glEnd();

	}

}
