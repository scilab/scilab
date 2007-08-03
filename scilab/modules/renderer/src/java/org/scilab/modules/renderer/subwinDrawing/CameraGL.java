/*------------------------------------------------------------------------*/
/* file: CameraJoGL.java                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines position the     */
/*        camera                                                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.ObjectGL;
import javax.media.opengl.GL;

/**
 * Class containing the driver dependant routines position the camera.
 * @author Jean-Baptiste Silvy
 */
public class CameraGL extends ObjectGL {

	/** Distance of the far clipping plane, should be greater than 1.0 and not to high */
	public static final double FAR_PLANE_DISTANCE = 100.0;
	
	/** Default rotation angles, 2D view */
	private static final double DEFAULT_ALPHA = 0.0;
	private static final double DEFAULT_THETA = 270.0;
	
	/**
	 * Default constructor
	 */
	public CameraGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		super.endDrawing();
		
	}
	
	/**
	 * Position the viewing area in order to respect Subwin position.
	 * @param transX X translation of the viewing area
	 * @param transY Y translation of the viewing area
	 * @param scaleX X scale of the viewing area
	 * @param scaleY Y scale of the viewing area
	 */
	public void moveViewingArea(double transX, double transY, double scaleX, double scaleY) {
		GL gl = getGL();
		gl.glMatrixMode(GL.GL_PROJECTION);
	    gl.glLoadIdentity();
	    // with this the drawing view current scale for the view is [0,1]x[0,1]
	    // for perspective view, we need to use glFrustum, not gluPerpective
	    gl.glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, FAR_PLANE_DISTANCE);
	    gl.glMatrixMode(GL.GL_MODELVIEW);
	    gl.glLoadIdentity();

		gl.glTranslated(transX, transY, 0.0);
		gl.glScaled(scaleX, scaleY, 1.0);
		
	}
	
	/**
	 * Move the axes box so it fit the viewing area
	 * @param scaleX X scale to fit user axes coordinates
	 * @param scaleY Y scale to fit user axes coordinates
	 * @param scaleZ Z scale to fit user axes coordinates
	 * @param transX X translation to put the axes in view
	 * @param transY Y translation to put the axes in view
	 * @param transZ Z translation to put the axes in view
	 */
	public void moveAxesBox(double scaleX, double scaleY, double scaleZ,
							double transX, double transY, double transZ) {
		GL gl = getGL();
		gl.glScaled(scaleX, scaleY, scaleZ);
		gl.glTranslated(transX, transY, transZ);
	}
	
	/**
	 * Rotate the axes in accordance with viewing angles
	 * @param centerX X coordiantes of the rotation center 
	 * @param centerY Y coordinates of the rotation center
	 * @param centerZ Z coordinates of the rotation center
	 * @param alpha rotation angle around axe X
	 * @param theta rotation angle around axe Z
	 * @param reductionRatio graphic needs to be reduced a little to always fit the window
	 */
	public void rotateAxesBox(double centerX, double centerY, double centerZ,
						      double alpha, double theta, double reductionRatio) {
		GL gl = getGL();
		// rotate around the center of the box axes
		gl.glTranslated(centerX, centerY, centerZ);
		gl.glScaled(reductionRatio, reductionRatio, reductionRatio); // reduction need to be performed on the center of the screen
		gl.glRotated(DEFAULT_ALPHA - alpha, 1.0 , 0.0, 0.0); /* Seems we need to rotate counterclok-wise */
		gl.glRotated(DEFAULT_THETA - theta, 0.0 , 0.0, 1.0);
		gl.glTranslated(-centerX, -centerY, -centerZ); // translate origin back
	}
	
	
}
