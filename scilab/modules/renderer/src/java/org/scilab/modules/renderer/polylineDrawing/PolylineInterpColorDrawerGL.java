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
import org.scilab.modules.renderer.utils.geom3D.ColoredTriangle;
import org.scilab.modules.renderer.utils.geom3D.TriangleDecomposition;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class containing the driver dependant routines to fill the area
 * enclosed by the polyline with interpolated colors.
 * Note that this class might be more generic by using a glu tesselator like
 * in fill drawing. 
 * @author Jean-Baptiste Silvy
 */
public class PolylineInterpColorDrawerGL extends FillDrawerGL {	
	
	//Coordinates of the triangle or of the polygon
	private Vector3D a;
	private Vector3D b;
	private Vector3D c;
	private Vector3D d;
	
	/**
	 * Default constructor
	 */
	public PolylineInterpColorDrawerGL() {
		
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

		if (xCoords.length == 3 && yCoords.length == 3 && zCoords.length == 3) { 
			//Coordinates of the triangle
			a = new Vector3D(xCoords[0], yCoords[0], zCoords[0]);
			b = new Vector3D(xCoords[1], yCoords[1], zCoords[1]);
			c = new Vector3D(xCoords[2], yCoords[2], zCoords[2]);
			
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			
		} else if (xCoords.length == 4 && yCoords.length == 4 && zCoords.length == 4) { 			
			//Coordinates of the square
			a = new Vector3D(xCoords[0], yCoords[0], zCoords[0]);
			b = new Vector3D(xCoords[1], yCoords[1], zCoords[1]);
			c = new Vector3D(xCoords[2], yCoords[2], zCoords[2]);
			d = new Vector3D(xCoords[3], yCoords[3], zCoords[3]);
			
			//we separate the square on 2 triangle then we work on each triangle
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			paintTriangle(c, d, a, colors[2], colors[1], colors[0]);
			
		} else if (xCoords.length >= 4 && yCoords.length >= 4 && zCoords.length >= 4) {
			//Coordinates of the square
			//closing the square case
			a = new Vector3D(xCoords[0], yCoords[0], zCoords[0]);
			b = new Vector3D(xCoords[1], yCoords[1], zCoords[1]);
			c = new Vector3D(xCoords[2], yCoords[2], zCoords[2]);
			d = new Vector3D(xCoords[3], yCoords[3], zCoords[3]);
			
			//we separate the square on 2 triangle then we work on each triangle
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			paintTriangle(c, d, a, colors[2], colors[1], colors[0]);
			
		}
		
		//GL gl = getGL();

		/*
		Texture colormapTexture = getColorMap().getTexture();
		colormapTexture.enable();
		colormapTexture.bind();

		gl.glColor3d(0.0, 1.0, 0.0);
		gl.glBegin(GL.GL_TRIANGLE_FAN); // works with triangles and quads
		for (int i = 0; i < xCoords.length; i++) {
			getColorMap().applyTexCoord(gl, colors[i]);
			gl.glVertex3d(xCoords[i], yCoords[i], zCoords[i]);
		}
		gl.glEnd();
		 */				

		//colormapTexture.disable();			
	}	
	
	/**
	 * Decompose a triangle in polygons then paint them
	 * @param a coordinates of the polylines vertices
	 * @param b coordinates of the polylines vertices
	 * @param c coordinates of the polylines vertices
	 * @param color1 color of the polylines vertices
	 * @param color2 color of the polylines vertices
	 * @param color3 color of the polylines vertices
	 */
	public void paintTriangle(Vector3D a, Vector3D b, Vector3D c, int color1, int color2, int color3) {
		
		GL gl = getGL();		

		//Calling this class will decompose triangle in colored polygons
		ColoredTriangle ct = new ColoredTriangle(a, b, c, color1, color2, color3);
		TriangleDecomposition td = ct.decomposeTriangle();	

		for(int i = 0 ; i < td.getNbPolygons() ; i++) {
			int color = td.getPolygonColor(i);	

			gl.glBegin(GL.GL_POLYGON);

			double[] polyColor = getColorMap().getColor(color);
			gl.glColor3d(polyColor[0], polyColor[1], polyColor[2]);
			Vector3D[] polygon = td.getPolygon(i);
			
			for(int j = 0 ; j < polygon.length ; j++) {
				gl.glVertex3d(polygon[j].getX(), polygon[j].getY(), polygon[j].getZ());
			}

			gl.glEnd();		
		}
	}

}
