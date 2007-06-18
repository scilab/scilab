/*------------------------------------------------------------------------*/
/* file: CircleMarkDrawer.java                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing circle marks                       */
/*------------------------------------------------------------------------*/

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
		gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		GLU glu = new GLU();
		glu.gluDisk(glu.gluNewQuadric(), 0.0, 1.0, NB_SLICES, 1);
		
		
		// circle
		gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		gl.glBegin(GL.GL_LINE_LOOP);
		for (int i = 0; i < NB_SLICES; i++) {
			double angle = 2.0 * Math.PI * i / NB_SLICES;
			gl.glVertex3d(Math.cos(angle), Math.sin(angle), 0.0);
		}
		gl.glEnd();
		

	}

}
