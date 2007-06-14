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

/**
 * Class containing functions to switch between user and window coordinates.
 * This is a singleton class.
 * @author Jean-Baptiste Silvy
 */
public class CoordinateTransformation {

	/** transformation matrices sizes */
	private static final int PROJECTION_MATRIX_SIZE = 16;
	private static final int VIEWPORT_SIZE          = 4;
	
	/** Singleton */
	private static CoordinateTransformation transform;
	
	private double[] modelViewMatrix;
	private double[] projectionMatrix;
	private int[] viewPort;
	
	
	/**
	 * default constructor
	 */
	protected CoordinateTransformation() {
		modelViewMatrix = new double[PROJECTION_MATRIX_SIZE];
		projectionMatrix = new double[PROJECTION_MATRIX_SIZE];
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
	 * @param posX X coordinate of the position
	 * @param posY Y coordinate of the position
	 * @param posZ Z coordinate of the position
	 * @param canvasCoord Result, array of size 3 containing the X, Y and Z positions in the canavs frame.
	 */
	public void getCanvasCoordinates(GL gl, double posX, double posY, double posZ, double[] canvasCoord) {
		// get current matrices
		GLU glu = new GLU();
		updateModelViewMatrix(gl);
		updateProjectionMatrix(gl);
		updateViewPort(gl);
		
		glu.gluProject(posX, posY, posZ, modelViewMatrix, 0, projectionMatrix, 0, viewPort, 0, canvasCoord, 0);
		
	}
	
	/**
	 * Get the coordinate of a 3D position in a canvas frame from its user coordinates.
	 * @param gl current OpenGL pipeline
	 * @param pos coordinates of the position (size 3).
	 * @return array of size 3 containing the X, Y and Z positions in the canvas frame.
	 */
	public double[] getCanvasCoordinates(GL gl, double[] pos) {
		double[] canvasCoord = new double[pos.length];
		getCanvasCoordinates(gl, pos[0], pos[1], pos[2], canvasCoord);
		return canvasCoord;
	}
	
	/**
	 * Convert several user coordinates to the canvas one
	 * @param gl current OpenGL pipeline
	 * @param positions coordinates of the different position (size n x 3).
	 * @return array of size n x 3 containing the X, Y and Z positions in the canvas frame
	 * 					   corresponding to each 
	 */
	public double[][] getCanvasCoordinates(GL gl, double[][] positions) {
		int nbCoords = positions.length;
		double[][] canvasCoords = new double[nbCoords][];
		
		for (int i = 0; i < nbCoords; i++) {
			canvasCoords[i] = getCanvasCoordinates(gl, positions[i]);
		}
		return canvasCoords;
	}
	
}
