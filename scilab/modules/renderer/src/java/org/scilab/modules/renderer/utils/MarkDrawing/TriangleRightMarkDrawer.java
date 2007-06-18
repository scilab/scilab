/*------------------------------------------------------------------------*/
/* file: TriangleRightMarkDrawer.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing triangle right marks               */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;

/**
 * Class specialized in drawing triangle up marks
 * @author Jean-Baptiste Silvy
 */
public class TriangleRightMarkDrawer extends TriangleMarkDrawer {

	/** angle between up triangle and down triangle */
	public static final double ROTATION_ANGLE = 270.0;
	
	/**
	 * Default constructor
	 */
	public TriangleRightMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a, equilateral triangle pointing right
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		gl.glRotated(ROTATION_ANGLE, 0.0, 0.0, 1.0);
		super.drawMark(gl, backColor, frontColor);

	}

}
