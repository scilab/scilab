/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Cube with edges of length one. Used to compute the screen size
 * of such a rotated cube
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



package org.scilab.modules.renderer.utils.glTools;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Cube with edges of length one. Used to compute the screen size
 * of such a rotated cube.
 * @author Jean-Baptiste Silvy
 */
public class UnitaryCubeGL {

	private static final int CUBE_NB_VERTICES = 8;
	
	private static final Vector3D[] CUBE_DEFAULT_VERTICES = {new Vector3D(0.0, 0.0, 0.0),
															 new Vector3D(0.0, 0.0, 1.0),
															 new Vector3D(0.0, 1.0, 0.0),
															 new Vector3D(0.0, 1.0, 1.0),
															 new Vector3D(1.0, 0.0, 0.0),
															 new Vector3D(1.0, 0.0, 1.0),
															 new Vector3D(1.0, 1.0, 0.0),
															 new Vector3D(1.0, 1.0, 1.0)};
	/**
	 * Default consturctor
	 */
	protected UnitaryCubeGL() { }
	
	/**
	 * Compute an axis aligned box whith specified bounds
	 * @param xMin min coordinate along X axis
	 * @param xMax max coordinate along X axis
	 * @param yMin min coordinate along Y axis
	 * @param yMax max coordinate along Y axis
	 * @param zMin min coordinate along Z axis
	 * @param zMax max coordinate along z axis
	 * @return Array of size 8 containing the vertices of the box
	 */
	public static Vector3D[] createCube(double xMin, double xMax,
			                            double yMin, double yMax,
			                            double zMin, double zMax) {
		Vector3D[] res = new Vector3D[CUBE_NB_VERTICES];
		for (int i = 0; i < CUBE_NB_VERTICES; i++) {
			res[i] = new Vector3D();
			
			// 0 => xMin, 1 => xMax
			if (CUBE_DEFAULT_VERTICES[i].getX() == 0.0) {
				res[i].setX(xMin);
			} else {
				res[i].setX(xMax);
			}
			
			if (CUBE_DEFAULT_VERTICES[i].getY() == 0.0) {
				res[i].setY(yMin);
			} else {
				res[i].setY(yMax);
			}
			
			if (CUBE_DEFAULT_VERTICES[i].getZ() == 0.0) {
				res[i].setZ(zMin);
			} else {
				res[i].setZ(zMax);
			}
		}
		return res;
	}
	
	/**
	 * Get the extent along x axis and y axis on the z = 0 plane.
	 * @param gl current GL pipeline.
	 * @return array of size 2 containing width and height.
	 * @param transform current transfomrations between 3D, 2D a, dpixel coordinates
	 */
	public static double[] getCubeScreenExtent(GL gl, CoordinateTransformation transform) {
		
		Vector3D[] screenCoordinates;
	
		// needed here since we are palcing the camera
		transform.update(gl);
		
		screenCoordinates = transform.getCanvasCoordinates(gl, CUBE_DEFAULT_VERTICES);
		
		// find the maximum distance along X and Y axis between each corners.
		double[] res = {0.0, 0.0};
		
		for (int i = 0; i < CUBE_NB_VERTICES; i++) {
			for (int j = 0; j < i; j++) {
				double curWidth = Math.abs(screenCoordinates[i].getX() - screenCoordinates[j].getX());
				double curHeight = Math.abs(screenCoordinates[i].getY() - screenCoordinates[j].getY());
				if (curWidth > res[0]) { res[0] = curWidth; }
				if (curHeight > res[1]) { res[1] = curHeight; }
			}
		}
		return res;
	}
	
	
	
}
