/*------------------------------------------------------------------------*/
/* file: CameraJoGL.java                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines position the     */
/*        camera                                                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.ObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.UnitaryCubeGL;

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
	
	/** Move viewPort to respect margins */
	private Vector3D viewPortTranslation = new Vector3D(); 
	
	/** Rescale viewport to respect margins */
	private Vector3D viewPortScale = new Vector3D();
	
	/** Scale to put all datas between 0 and 1 */
	private Vector3D normalizeScale = new Vector3D();
	
	/** Translation to put all datas between 0 and 1. */
	private Vector3D normalizeTranslation = new Vector3D();
	
	/** center of the axes box */
	private Vector3D rotationCenter = new Vector3D();
	
	/** Keep back camera parameters in oder to switch back to 2D mode if needed */
	private double alpha;
	private double theta;
	
	/** scale used to get the best fitting in the canvas for axes */
	private Vector3D fittingScale = new Vector3D();
	
	/** size of area enclosed by margins. */
	private double[] marginSize;
	
	/** viewPort size */
	private double viewPortWidth;
	private double viewPortHeight;
	
	/**
	 * Default constructor
	 */
	public CameraGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * Should not be called.
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) { }
	
	
	/**
	 * Set the viewPort for iso view.
	 */
	public void setIsoViewViewPort() {
		GL gl = getGL();
		setIsoViewViewPort(gl);
	}
	
	/**
	 * Set the view port for isoview.
	 * @param gl current GL pipeline
	 */
	public void setIsoViewViewPort(GL gl) {
		double[] viewPort = {0.0, 0.0, 0.0, 0.0};
		
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
	
	/**
	 * Set standard view port (ie wich follows window size).
	 */
	void setStandardViewPort() {
		GL gl = getGL();
		setStandardViewPort(gl);
	}
	
	/**
	 * Set standard view port (ie wich follows window size).
	 * @param gl current GL pipeline
	 */
	public void setStandardViewPort(GL gl) {
		// get width ad height of the viewPort
		viewPortWidth = 1.0;
		viewPortHeight = 1.0;
		
		// set projection
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();
		//		 with this the drawing view current scale for the view is [0,1]x[0,1]
	    // for perspective view, we need to use glFrustum, not glOrtho
		gl.glOrtho(0.0, 1.0, 0.0, 1.0, -FAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE);
		
	}
	
	/**
	 * Set the parameters for positionning viewing area (specified by axes margins)
	 * @param transX X translation of the viewing area
	 * @param transY Y translation of the viewing area
	 * @param scaleX X scale of the viewing area
	 * @param scaleY Y scale of the viewing area
	 */
	public void setViewingArea(double transX, double transY, double scaleX, double scaleY) {
		// we move it in 2D.
		viewPortTranslation.setValues(transX, transY, 0.0);
		viewPortScale.setValues(scaleX, scaleY, 1.0);
	}
	
	/**
	 * Position the viewing area in order to respect Subwin position.
	 */
	public void moveViewingArea() {
		GL gl = getGL();
		
		setIsoViewViewPort();
		
	    gl.glMatrixMode(GL.GL_MODELVIEW);
	    gl.glLoadIdentity();

	    gl.glScaled(viewPortWidth, viewPortHeight, 1.0);
		gl.glTranslated(viewPortTranslation.getX(), viewPortTranslation.getY(), viewPortTranslation.getZ());
		gl.glScaled(viewPortScale.getX(), viewPortScale.getY(), viewPortScale.getZ());
		
		// save pixel coordinates of the margin box, our axes box needs to fit inside.
		computeMarginsSize();
		
	}
	
	/**
	 * Compute width and height of the margins in pixels.
	 */
	public void computeMarginsSize() {
//		GL gl = getGL();
//		
//		Vector3D[] marginsCorners = {new Vector3D(0.0, 0.0, 0.0),
//									 new Vector3D(0.0, 1.0, 0.0),
//									 new Vector3D(1.0, 1.0, 0.0),
//									 new Vector3D(1.0, 0.0, 0.0)};
//		
//		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
//		transform.update(gl);
//		marginsCorners = transform.getCanvasCoordinates(gl, marginsCorners);
//		marginWidth = marginsCorners[2].getX() - marginsCorners[1].getX();
//		marginHeight = marginsCorners[1].getY() - marginsCorners[0].getY();
		
		// Margins are front size of the unitary cube.
		// And unitary cube is facing us.
		marginSize = UnitaryCubeGL.getCubeScreenExtent(getGL());
	}
	
	/**
	 * Set parameters so the axes box is map onto a the [0,1]x[0,1]x[0,1] cube.
	 * @param normalizeScaleX X scale to fit bounds within [0,1].
	 * @param normalizeScaleY Y scale to fit bounds within [0,1].
	 * @param normalizeScaleZ Z scale to fit bounds within [0,1].
	 * @param transX X translation to put the axes in view.
	 * @param transY Y translation to put the axes in view.
	 * @param transZ Z translation to put the axes in view.
	 */
	public void setNormalizationFactor(double normalizeScaleX, double normalizeScaleY, double normalizeScaleZ,
									   double transX, double transY, double transZ) {
		normalizeScale.setValues(normalizeScaleX, normalizeScaleY, normalizeScaleZ);
		normalizeTranslation.setValues(transX, transY, transZ);
	}
	
	/**
	 * Move the axes box so it map the onto a the [0,1]x[0,1]x[0,1] cube.
	 */
	public void moveAxesBox() {
		GL gl = getGL();
		gl.glScaled(normalizeScale.getX(), normalizeScale.getY(), normalizeScale.getZ());
		
		gl.glTranslated(normalizeTranslation.getX(), normalizeTranslation.getY(), normalizeTranslation.getZ());
		
	}
	
	/**
	 * Set the parameters for rotating the axes box.
	 * @param centerX X coordiantes of the rotation center 
	 * @param centerY Y coordinates of the rotation center
	 * @param centerZ Z coordinates of the rotation center
	 * @param alpha rotation angle around axe X
	 * @param theta rotation angle around axe Z
	 */
	public void setAxesRotationParameters(double centerX, double centerY, double centerZ,
									      double alpha, double theta) {
		rotationCenter.setValues(centerX, centerY, centerZ);
		this.alpha = alpha;
		this.theta = theta;
	}
	
	/**
	 * Set the scale used to best fit the margins.
	 * @param scaleX X coordinate of the scale
	 * @param scaleY Y coordinate of the scale
	 * @param scaleZ Z cordinate of the scale
	 */
	public void setFittingScale(double scaleX, double scaleY, double scaleZ) {
		fittingScale.setValues(scaleX, scaleY, scaleZ);
	}
	
	/**
	 * Move the box to the center of the screen.
	 */
	public void centerAxesBox() {
		GL gl = getGL();
		
		// rotate around the center of the box axes
		gl.glTranslated(rotationCenter.getX(),  rotationCenter.getY(),  rotationCenter.getZ());
		
		// reduction need to be performed on the center of the screen
		gl.glScaled(1.0 / (normalizeScale.getX() * viewPortWidth),
					1.0 / (normalizeScale.getY() * viewPortHeight),
					1.0 / normalizeScale.getZ());
	}
	
	/**
	 * Compute the bounding rectangle of the axes box projection on the screen. 
	 * @param gl current OpenGL pipeline.
	 * @return array of size 2: width and height in pixels.
	 */
	public double[] getBoxScreenExtent(GL gl) {
		// apply transformation a first time to 
		gl.glPushMatrix();
		applyRotation(gl, alpha, theta);
		
		gl.glScaled(fittingScale.getX() / normalizeScale.getX(),
					fittingScale.getY() / normalizeScale.getY(),
					fittingScale.getZ() / normalizeScale.getZ());
		
		// get the size of the viewing area on the screen.
		double[] screenExtent = UnitaryCubeGL.getCubeScreenExtent(gl);
		gl.glPopMatrix();
		return screenExtent;
	}
	
	/**
	 * Compute the scale wich will best fit the window in accordance with viewing angles.
	 */
	public void computeFittingScale() {
		GL gl = getGL();
		
		
		double[] screenExtent = getBoxScreenExtent(gl);
		// apply same factor to preserve isoview.
		double minScale = Math.min(marginSize[0] / screenExtent[0], Math.min(marginSize[1] / screenExtent[1], 1.0));
		gl.glScaled(minScale, minScale, minScale);
	}
	
	/**
	 * Rotate the axes in accordance with viewing angles.
	 */
	public void placeCamera() {
		
		GL gl = getGL();
		
		centerAxesBox();
		
		computeFittingScale();
		
		gl.glPushMatrix();
		applyRotation(gl, alpha, theta);
		gl.glScaled(fittingScale.getX(), fittingScale.getY(), fittingScale.getZ());
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ()); // translate origin back
		
		
		// compute the matrix for project and unproject.
		CoordinateTransformation.getTransformation(gl).update(gl);
		
	}
	
	/**
	 * Calls to turn the 
	 * @param gl current GL pipeline.
	 * @param alpha rotation angle around axe X in degree
	 * @param theta rotation angle around axe Z in degree
	 */
	protected void applyRotation(GL gl, double alpha, double theta) {
		gl.glRotated(DEFAULT_ALPHA - alpha, 1.0 , 0.0, 0.0); /* Seems we need to rotate counterclok-wise */
		gl.glRotated(DEFAULT_THETA - theta, 0.0 , 0.0, 1.0);
	}
	
	/**
	 * To be called at the end of camera use.
	 */
	public void replaceCamera() {
		GL gl = getGL();
		gl.glPopMatrix();
		CoordinateTransformation.getTransformation(gl).update(gl);
	}
	
	/**
	 * Convert scene coordinates to pixel coordinates.
	 * @param userCoordX X coordinate of the scene coordinate.
	 * @param userCoordY X coordinate of the scene coordinate.
	 * @param userCoordZ Z coordinate of the scene coordinate.
	 * @return array of size 2 containing the 2 coordinates.
	 */
	public int[] getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ) {
		GL gl = getGL();
		Vector3D userPos = new Vector3D(userCoordX, userCoordY, userCoordZ);
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		Vector3D screenCoordinate = transform.project(gl, userPos);
		
		// get only the pixel coordinates
		int[] res = {(int) screenCoordinate.getX(), (int) screenCoordinate.getY()};
		return res;
	}
	
	/**
	 * Convert scene coordinates to pixel coordinates using the 2D view.
	 * @param userCoordX X coordinate of the scene coordinate.
	 * @param userCoordY Y coordinate of the scene coordinate.
	 * @param userCoordZ Z coordinate of the scene coordinate.
	 * @return array of size 2 containing the 2 coordinates.
	 */
	public int[] get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ) {
		switchTo2DCoordinates();
		int[] res = getPixelCoordinates(userCoordX, userCoordY, userCoordZ);
		backTo3DCoordinates();
		return res;
	}
	
	/**
	 * Move the camera to the default 2D coordinates.
	 * Try to avoid to use this method, but it sometime needed
	 * for backward compatibility with old renderer.
	 */
	protected void switchTo2DCoordinates() {
		GL gl = getGL();
		gl.glPopMatrix();
		gl.glPushMatrix();
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ()); // translate origin back
		
		// update transformation
		CoordinateTransformation.getTransformation(gl).update(gl);
		
	}
	
	/**
	 * Return to the normal view after a call to switchTo2DCoordinates.
	 */
	protected void backTo3DCoordinates() {
		GL gl = getGL();
		gl.glPopMatrix();
		gl.glPushMatrix();
		applyRotation(gl, alpha, theta);
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ()); // translate origin back
		// update transformation
		CoordinateTransformation.getTransformation(gl).update(gl);
	}
	
	
	
	
}
