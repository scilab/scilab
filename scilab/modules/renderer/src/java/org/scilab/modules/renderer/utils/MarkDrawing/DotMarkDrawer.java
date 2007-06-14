/*------------------------------------------------------------------------*/
/* file: DotMarkDrawer.java                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing dot marks                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.glu.GLU;
import javax.media.opengl.GL;


/**
 * Class specialized in drawing dot marks 
 * @author Jean-Baptiste Silvy
 */
public class DotMarkDrawer extends MarkDrawingStrategy  {

	
	private static final int NB_SLICES = 64;
	
	/**
	 * Default constructor
	 */
	public DotMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a dot
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		GLU glu = new GLU();
		glu.gluDisk(glu.gluNewQuadric(), 0.0, 1.0, NB_SLICES, 1);
	}
}
