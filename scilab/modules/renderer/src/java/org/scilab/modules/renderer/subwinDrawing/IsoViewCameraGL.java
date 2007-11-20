/*------------------------------------------------------------------------*/
/* file: IsoViewCameraJoGL.java                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines position the     */
/*        camera with an isoview scale                                    */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

/**
 * Class containing the driver dependant routines position the
 * camera with an isoview scale
 * @author Jean-Baptiste Silvy
 */
public class IsoViewCameraGL extends CameraGL {

	
	private double viewPortHeight;
	private double viewPortWidth;

	/**
	 * Default constructor.
	 */
	public IsoViewCameraGL() {
		super();
	}
	
	/**
	 * Compute the scale wich will best fit the window in accordance with viewing angles.
	 */
	@Override
	protected void computeFittingScale() {
		GL gl = getGL();
		double[] screenExtent = getBoxScreenExtent(gl);
		// apply same factor to preserve isoview.
		double[] marginSize = getMarginSize();
		double minScale = Math.min(marginSize[0] / screenExtent[0], Math.min(marginSize[1] / screenExtent[1], 1.0));
		gl.glScaled(minScale, minScale, minScale);
	}

	/**
	 * @return view port height.
	 */
	@Override
	protected double getViewPortHeight() {
		return viewPortHeight;
	}

	/**
	 * @return view port width.
	 */
	@Override
	protected double getViewPortWidth() {
		return viewPortWidth;
	}

	/**
	 * Set the viewPort of the object.
	 */
	@Override
	protected void setViewPort() {
		double[] viewPort = {0.0, 0.0, 0.0, 0.0};
		
		GL gl = getGL();
		// get width ad height of the viewPort
		gl.glGetDoublev(GL.GL_VIEWPORT, viewPort, 0);
		viewPortWidth = viewPort[2];
		viewPortHeight = viewPort[2 + 1];
		
		// get minimum value between the two to set a close to 1 viewPort
		double minDim = Math.min(viewPortWidth, viewPortHeight);
		viewPortWidth /= minDim;
		viewPortHeight /= minDim;
		
		// set projection
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();
	    // for perspective view, we need to use glFrustum, not glOrtho
		gl.glOrtho(0.0, viewPortWidth, 0.0, viewPortHeight, -FAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE);

	}

}
