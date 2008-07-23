/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing functions to switch between user and window coordinates
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class containing functions to switch between user and window coordinates.
 * This is a singleton class.
 * @author Jean-Baptiste Silvy
 */
public class CoordinateTransformation {

	/** transformation matrices sizes */
	public static final int MATRIX_4X4_SIZE = 16;
	
	/** Size of the view port */
	public static final int VIEW_PORT_SIZE = 4;
	
	private Matrix4D projectionMatrix;
	private Matrix4D modelViewMatrix;
	
	private Matrix4D completeProjectMatrix;
	private Matrix4D unprojectMatrix;
	private double[] viewPort;
	
	private Vector3D additionalTranslation;
	private Vector3D additionalTranslationPix;
	
	/**
	 * default constructor
	 */
	public CoordinateTransformation() {
		completeProjectMatrix = null;
		unprojectMatrix = null;
		projectionMatrix = new Matrix4D();
		modelViewMatrix = new Matrix4D();
		viewPort = new double[VIEW_PORT_SIZE];
		additionalTranslation = null;
		additionalTranslationPix = null;
	}
	
	/**
	 * @return current projection matrix
	 */
	public Matrix4D getCompleteProjectionMatrix() {
		return completeProjectMatrix;
	}
	
	/**
	 * @return current model view matrix
	 */
	public Matrix4D getModelViewMatrix() {
		return modelViewMatrix;
	}
	
	/**
	 * @return current inverse of projection matrix
	 */
	public Matrix4D getUnprojectMatrix() {
		return unprojectMatrix;
	}
	
	/**
	 * @return current viewPort
	 */
	public double[] getViewPort() {
		return viewPort;
	}
	
	/**
	 * @param trans translation to apply
	 */
	public void setAdditionalTranslation(Vector3D trans) {
		this.additionalTranslation = trans;
		this.additionalTranslationPix = null; // not up to date
		
	}
	
	/**
	 * @return additional translation
	 */
	public Vector3D getAdditionalTranslation() {
		return additionalTranslation;
	}
	
	/**
	 * @param gl current OpenGL pipeline
	 * @return additional translation
	 */
	public Vector3D getAdditionalTranslationPix(GL gl) {
		if (additionalTranslation == null) {
			return null;
		} else if (additionalTranslationPix == null) {
			additionalTranslationPix = getCanvasDirection(gl, additionalTranslation);
			return additionalTranslationPix;
		} else {
			return additionalTranslationPix;
		}
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
		gl.glGetDoublev(GL.GL_VIEWPORT, viewPort, 0);
		// force values to 0 to be compatible when
		// OpenGL pipeline is enable. The viewport position
		// is modified. See http://www.javagaming.org/forums/index.php?topic=16414.0.
		viewPort[0] = 0;
		viewPort[1] = 0;
		
		// projection (without viewport is done by v' = P.M.v
		// where v' is the canvas coordinates and v scene coordinates
		projectionMatrix.setFromOpenGLRepresentation(oglProjectionMatrix);
		modelViewMatrix.setFromOpenGLRepresentation(oglModelViewMatrix);
		completeProjectMatrix = projectionMatrix.mult(modelViewMatrix);
		
		// unproject is done by v = (P.M)^-1.v'
		unprojectMatrix = completeProjectMatrix.getInverse();
	}
	
	/**
	 * Get the coordinate of a 3D position in a canvas frame from its user coordinates.
	 * @param gl current OpenGL pipeline
	 * @param pos coordinates of the position.
	 * @return Vector the X, Y and Z positions in the canvas frame.
	 */
	public Vector3D getCanvasCoordinates(GL gl, Vector3D pos) {
		// I first used gluProject, but it is slower since it will always perform matrices multiplications and inverse.
		//return projectMatrix.mult(pos);
		return project(gl, pos);
	}
	
	/**
	 * Perform the same operation as gluProject.
	 * @param gl unused
	 * @param pos scene position
	 * @return pixel coordinate of the point.
	 */
	public Vector3D project(GL gl, Vector3D pos) {
		Vector3D canvasCoord = completeProjectMatrix.mult(pos);
		canvasCoord.setX(viewPort[0] + viewPort[2] * (canvasCoord.getX() + 1.0) / 2.0);
		canvasCoord.setY(viewPort[1] + viewPort[VIEW_PORT_SIZE - 1] * (canvasCoord.getY() + 1.0) / 2.0);
		canvasCoord.setZ((canvasCoord.getZ() + 1.0) / 2.0);
		return canvasCoord;
	}
	
	/**
	 * Perform the same operation as gluUnproject.
	 * @param gl unused
	 * @param canvasPos canvas position
	 * @return scene coordinate of the point
	 */
	public Vector3D unProject(GL gl, Vector3D canvasPos) {
		Vector3D sceneCoord 
			= new Vector3D(2.0 * (canvasPos.getX() - viewPort[0]) / viewPort[2] - 1.0,
						   2.0 * (canvasPos.getY() - viewPort[1]) / viewPort[VIEW_PORT_SIZE - 1] - 1.0,
						   2.0 * canvasPos.getZ() - 1.0);
		return unprojectMatrix.mult(sceneCoord);
		
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
	 * Convert a direction in pixels
	 * @param gl current GL pipeline
	 * @param direction direction to transform
	 * @return direcyion in pixels
	 */
	public Vector3D getCanvasDirection(GL gl, Vector3D direction) {
		Vector3D originPix = getCanvasCoordinates(gl, Vector3D.ORIGIN);
		Vector3D res = getCanvasCoordinates(gl, direction);
		return res.substract(originPix);
	}
	
	/**
	 * Opposite transformation od getCanvasCoordinates.
	 * Compute back scene coordinates from window coordinates.
	 * @param gl current OpengL pipeline
	 * @param canvasPos coordinates of the point in the canvas frame
	 * @return coodinates in object frame
	 */
	public Vector3D retrieveSceneCoordinates(GL gl, Vector3D canvasPos) {
		// I first used gluUnproject, but it is slower since it will always perform matrices multiplications and inverse.
		
		//return unprojectMatrix.mult(canvasPos);
		return unProject(gl, canvasPos);
	}
	
	/**
	 * Transform a direction vector from current coordinates system to canvas position.
	 * In this mode, translation are discared.
	 * @param gl current GL pipeline unused.
	 * @param dir direction in the current coordinates system.
	 * @return direction in the canvas coordinate system.
	 */
	public Vector3D getCanvasCoordinates3D(GL gl, Vector3D dir) {
		return completeProjectMatrix.mult3D(dir);
	}
	
	/**
	 * Opposite transformation of getCanvasCoordinates3D.
	 * @param gl current GL pipeline unused.
	 * @param canvasDir direction in canvas coordinates
	 * @return direction in the current coordinates system
	 */
	public Vector3D retrieveSceneCoordinates3D(GL gl, Vector3D canvasDir) {
		return unprojectMatrix.mult3D(canvasDir);
	}
	
	/**
	 * @return String representation of the transformationException.
	 */
	@Override
	public String toString() {
		return completeProjectMatrix.toString();
	}
	
}
