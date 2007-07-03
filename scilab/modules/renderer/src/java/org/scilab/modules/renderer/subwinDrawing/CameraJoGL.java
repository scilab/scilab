/*------------------------------------------------------------------------*/
/* file: CameraJoGL.java                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines position the     */
/*        camera                                                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.ObjectJoGL;
import javax.media.opengl.GL;

/**
 * Class containing the driver dependant routines position the camera.
 * @author Jean-Baptiste Silvy
 */
public class CameraJoGL extends ObjectJoGL {

	/** Distance of the far clipping plane, should be greater than 1.0 and not to high */
	private static final double FAR_PLANE_DISTANCE = 100.0;
	
	/**
	 * Default constructor
	 */
	public CameraJoGL() {
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
		//gl.glTranslatef(0.0f, 0.0f, -1.0f); 
		
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
		System.out.println("scaleX = " + scaleX + "scaleY = " + scaleY + "scaleZ = " + scaleZ);
		gl.glScaled(scaleX, scaleY, scaleZ);
		System.out.println("transX = " + transX + "transY = " + transY + "transZ = " + transZ);
		gl.glTranslated(transX, transY, transZ);
	}
	
	
}
