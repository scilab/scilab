/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to fill the area
 * enclosed by the polyline with interpolated colors 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;
import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

import com.sun.opengl.util.texture.Texture;


/**
 * Class containing the driver dependant routines to fill the area
 * enclosed by the polyline with interpolated colors.
 * Note that this class might be more generic by using a glu tesselator like
 * in fill drawing. 
 * @author Jean-Baptiste Silvy
 */
public class PolylineInterpColorDrawerGL extends FillDrawerGL {	
	
	private static final int TRIANGLE_NB_EDGE = 3;
	
	private boolean isZconstant;
	
	/**
	 * Default constructor
	 */
	public PolylineInterpColorDrawerGL() {
		super();
		isZconstant = false;
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 * @param colors colors index for each vertex
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords,
							 double[] zCoords, int[] colors) {
		
		// check if Z is constant
		this.isZconstant = checkIfZconstant(zCoords);
		
		GL gl = getGL();
		
		Vector3D[] vertices = new Vector3D[xCoords.length];
		for (int i = 0; i < xCoords.length; i++) {
			vertices[i] = new Vector3D(xCoords[i], yCoords[i], zCoords[i]);
			// colors are Scilab indices
			colors[i] = getColorMap().convertScilabToColorMapIndex(colors[i]);
		}
		
		ShadeFacetDrawer sfd = getParentFigureGL().getShadeFacetDrawer();
		
		// push polygons back if needed
		CoordinateTransformation transform = getParentFigureGL().getCoordinateTransformation();
		transform.pushPolygonsBack(gl, this);
		
		Texture colorMapTexture = getColorMap().getTexture();
		colorMapTexture.enable();
		colorMapTexture.bind();
		
		gl.glBegin(GL.GL_TRIANGLES);
		
		if (vertices.length == TRIANGLE_NB_EDGE) {
			// only a triangle
			sfd.paintPolygon(vertices, colors, gl, getColorMap());	
			
		} else {
			
			// its a quad, decompose it into 2 triangles
			Vector3D[] triangle1 =  new Vector3D[TRIANGLE_NB_EDGE];
			Vector3D[] triangle2 =  new Vector3D[TRIANGLE_NB_EDGE];
			int[] colorT1 = new int[TRIANGLE_NB_EDGE];
			int[] colorT2 = new int[TRIANGLE_NB_EDGE];
			GeomAlgos.decomposeQuad(vertices, colors, triangle1, colorT1, triangle2, colorT2);
			
			// draw the two decomposed triangles
			sfd.paintPolygon(triangle1, colorT1, gl, getColorMap());	
			sfd.paintPolygon(triangle2, colorT2, gl, getColorMap());
		}
		gl.glEnd();
		
		colorMapTexture.disable();
		
		transform.endPushPolygonsBack(gl, this);
	}
	
	/**
	 * Check if the z coordinate of the polyline is constant
	 * @param zCoords z coordinates of the polyline vertices
	 * @return true if z is constant, false otherwise
	 */
	private boolean checkIfZconstant(double[] zCoords) {
		for (int i = 0; i < zCoords.length; i++) {
			if (zCoords[0] != zCoords[i]) {
				return false;
			}
		}
		return true;
	}

	/**
	 * This method is used to know if polygon offset is needed in 2d mode.
	 * If this function returns true, then the polygon offset is not needed in 2d mode.
	 * If it returns false, polygon offset is always needed.
	 * @return true if the object is flat along Z coordinate
	 */
	public boolean isZConstant() {
		return isZconstant;
	}

}
