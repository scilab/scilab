/*------------------------------------------------------------------------*/
/* file: CoordinateTransformation.java                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing functions to switch between user and window    */
/*        coordinates.                                                    */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing functions to switch between user and window coordinates.
 * This is a singleton class.
 * @author Jean-Baptiste Silvy
 */
public class CoordinateTransformation {

	/** transformation matrices sizes */
	private static final int MATRIX_4X4_SIZE = 16;
	private static final int VIEWPORT_SIZE   = 4;
	
	/** Identity matrix of size 4x4 */
	private static final double[] IDENTITY_4X4 = {1.0, 0.0, 0.0, 0.0,
												  0.0, 1.0, 0.0, 0.0,
												  0.0, 0.0, 1.0, 0.0,
												  0.0, 0.0, 0.0, 1.0};
	
	/** The six translation indices in an frame changing matrix */
	private static final int[] TRANSLATION_INDICES = {3, 7, 11, 12, 13, 14};
	
	/** Singleton */
	private static CoordinateTransformation transform;
	
	private double[] modelViewMatrix;
	private double[] projectionMatrix;
	private int[] viewPort;
	
	
	/**
	 * default constructor
	 */
	protected CoordinateTransformation() {
		modelViewMatrix = new double[MATRIX_4X4_SIZE];
		projectionMatrix = new double[MATRIX_4X4_SIZE];
		viewPort = new int[VIEWPORT_SIZE];
	}
	
	/**
	 * Factory of the singleton
	 * @return only instance of singleton
	 */
	public static synchronized CoordinateTransformation getTransformation() {
		if (transform == null) {
			transform = new CoordinateTransformation();
		}
		return transform;
	}
	
	/**
	 * Update the model view matrix from the current transformation.
	 * @param gl current OpenGL pipeline
	 */
	public void updateModelViewMatrix(GL gl) {
		gl.glGetDoublev(GL.GL_MODELVIEW_MATRIX, modelViewMatrix, 0);
	}
	
	/**
	 * Update the projection view matrix from the current transformation.
	 * @param gl current OpenGL pipeline
	 */
	public void updateProjectionMatrix(GL gl) {
		gl.glGetDoublev(GL.GL_PROJECTION_MATRIX, projectionMatrix, 0);
	}
	
	/**
	 * Update view port vector from current transformation.
	 * @param gl current OpenGL pipeline
	 */
	public void updateViewPort(GL gl) {
		gl.glGetIntegerv(GL.GL_VIEWPORT, viewPort, 0);
	}
	
	/**
	 * Get the coordinate of a 3D position in a canvas frame from its user coordinates.
	 * @param gl current OpenGL pipeline
	 * @param pos coordinates of the position (size 3).
	 * @return array of size 3 containing the X, Y and Z positions in the canvas frame.
	 */
	public Vector3D getCanvasCoordinates(GL gl, Vector3D pos) {
		double[] canvasCoord = {0.0, 0.0, 0.0};
		GLU glu = new GLU();
		updateModelViewMatrix(gl);
		updateProjectionMatrix(gl);
		updateViewPort(gl);
		
		glu.gluProject(pos.getX(), pos.getY(), pos.getZ(), modelViewMatrix, 0, projectionMatrix, 0, viewPort, 0, canvasCoord, 0);
		return new Vector3D(canvasCoord);
	}
	
	/**
	 * Convert several user coordinates to the canvas one
	 * @param gl current OpenGL pipeline
	 * @param positions coordinates of the different position (size n x 3).
	 * @return array of size n x 3 containing the X, Y and Z positions in the canvas frame
	 * 					   corresponding to each 
	 */
	public Vector3D[] getCanvasCoordinates(GL gl, Vector3D[] positions) {
		int nbCoords = positions.length;
		Vector3D[] canvasCoords = new Vector3D[nbCoords];
		
		for (int i = 0; i < nbCoords; i++) {
			canvasCoords[i] = getCanvasCoordinates(gl, positions[i]);
		}
		return canvasCoords;
	}
	
	/**
	 * Opposite transformation od getCanvasCoordinates.
	 * Compute back scene coordinates from window coordinates.
	 * @param gl current OpengL pipeline
	 * @param canvasPos coordinates of the point in the canvas frame
	 * @return coodinates in object frame
	 */
	public Vector3D retrieveSceneCoordinates(GL gl, Vector3D canvasPos) {
		double[] objectPos = {0.0, 0.0, 0.0};
		// get current matrices
		GLU glu = new GLU();
		updateModelViewMatrix(gl);
		updateProjectionMatrix(gl);
		updateViewPort(gl);
		
		glu.gluUnProject(canvasPos.getX(), canvasPos.getY(), canvasPos.getZ(),
						 modelViewMatrix, 0, projectionMatrix, 0, viewPort, 0, objectPos, 0);
		
		return new Vector3D(objectPos);
	}
	
}
