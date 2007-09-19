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

import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
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
	
	/** Singleton */
	private static CoordinateTransformation transform;
	
	private Matrix4D projectMatrix;
	private Matrix4D unprojectMatrix;
	
	
	/**
	 * default constructor
	 */
	protected CoordinateTransformation() {
		projectMatrix = null;
		unprojectMatrix = null;
	}
	
	/**
	 * Factory of the singleton
	 * @param gl current OpenGL pipeline
	 * @return only instance of singleton
	 */
	public static synchronized CoordinateTransformation getTransformation(GL gl) {
		if (transform == null) {
			transform = new CoordinateTransformation();
		}
		transform.update(gl);
		return transform;
	}
	
	/**
	 * Update the projection data of the coordinates.
	 * @param gl current Gl pipeline
	 */
	public void update(GL gl) {
		
		// get OpenGL transformation matrices
		double[] oglModelViewMatrix = new double[MATRIX_4X4_SIZE];
		double[] oglProjectionMatrix = new double[MATRIX_4X4_SIZE];
		gl.glGetDoublev(GL.GL_MODELVIEW_MATRIX, oglModelViewMatrix, 0);
		gl.glGetDoublev(GL.GL_PROJECTION_MATRIX, oglProjectionMatrix, 0);
		
		// projection (without viewport is done by v' = P.M.v
		// where v' is the canvas coordinates and v scene coordinates
		projectMatrix = new Matrix4D(oglProjectionMatrix);
		projectMatrix = projectMatrix.mult(new Matrix4D(oglModelViewMatrix));
		
		// unproject is done by v = (P.M)^-1.v'
		unprojectMatrix = projectMatrix.getInverse();
	}
	
	/**
	 * Get the coordinate of a 3D position in a canvas frame from its user coordinates.
	 * @param gl current OpenGL pipeline
	 * @param pos coordinates of the position (size 3).
	 * @return array of size 3 containing the X, Y and Z positions in the canvas frame.
	 */
	public Vector3D getCanvasCoordinates(GL gl, Vector3D pos) {
		// I first used gluProject, but it is slower since it will always perform matrices multiplications and inverse.
		return projectMatrix.mult(pos);
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
		// I first used gluProject, but it is slower since it will always perform matrices multiplications and inverse.
		
		return unprojectMatrix.mult(canvasPos);
	}
	
}
