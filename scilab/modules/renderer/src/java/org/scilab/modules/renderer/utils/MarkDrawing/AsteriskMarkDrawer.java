/*------------------------------------------------------------------------*/
/* file: AsteriskMarkDrawer.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing asterisk marks                     */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;

/**
 * Class specialized in drawing asterisk marks
 * @author Jean-Baptiste Silvy
 */
public class AsteriskMarkDrawer extends MarkDrawingStrategy {

	/** sqrt(2)/2 */
	private static final double DIAG_END = Math.sqrt(2.0) / 2.0;
	
	/**
	 * Default constructor
	 */
	public AsteriskMarkDrawer() {
		super();
	}
	
	/**
	 * Draw aan asterisk, a plus and a dot whose edges are on a circle
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		gl.glBegin(GL.GL_LINES);
		// plus
		gl.glVertex3d(-1.0,  0.0, 0.0);
		gl.glVertex3d(1.0 ,  0.0, 0.0);
		gl.glVertex3d(0.0 , -1.0, 0.0);
		gl.glVertex3d(0.0 ,  1.0, 0.0);
		
		// cross
		gl.glVertex3d(-DIAG_END,  DIAG_END, 0.0);
		gl.glVertex3d(DIAG_END , -DIAG_END, 0.0);
		gl.glVertex3d(-DIAG_END, -DIAG_END, 0.0);
		gl.glVertex3d(DIAG_END ,  DIAG_END, 0.0);
		gl.glEnd();
	}
	
}
