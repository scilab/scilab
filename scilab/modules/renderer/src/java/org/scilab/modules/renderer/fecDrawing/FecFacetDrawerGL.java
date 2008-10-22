/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing the facets of a fec object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.fecDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.AutoDrawableObjectGL;
import org.scilab.modules.renderer.utils.geom3D.ColoredTriangle;
import org.scilab.modules.renderer.utils.geom3D.TriangleDecomposition;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;


/**
 * Class drawing the facets of a fec object
 * @author Jean-Baptiste Silvy
 */
public class FecFacetDrawerGL extends AutoDrawableObjectGL {

	private static final int TRIANGLE_NB_VERTEX = 3;
	
	private double zMin;
	private double zMax;
	private int colOutLow;
	private int colOutUp;
	private int colMin;
	private int colMax;
	
	/**
	 * Default consturctor
	 */
	public FecFacetDrawerGL() {
		super();
	}
	
	/**
	 * Set all the parameters in one function
	 * @param zMin new zMin
	 * @param zMax new zMax
	 * @param colMin new colMin
	 * @param colMax new colMax
	 * @param colOutLow new colOutLow
	 * @param colOutUp new colOutUp
	 */
	public void setFacetParameters(double zMin, double zMax, int colMin,
								   int colMax, int colOutLow, int colOutUp) {
		
	
		if (colMin == 0 && colMax == 0) {
			// default colmin and colmax
			// use the whole colormap
			this.colMin = 0;
			this.colMax = getColorMap().getSize() - 1;
		} else {
			// colMin and colMax are scilab indices
			this.colMin = getColorMap().convertScilabToColorMapIndex(colMin);
			this.colMax = getColorMap().convertScilabToColorMapIndex(colMax);
		}
		
		if (colOutLow <= -1) {
			// when outside of colormap use the lowest value in the colormap
			this.colOutLow = this.colMin;
		} else if (colOutLow == 0) {
			// don't display polygon
			this.colOutLow = -1;
		} else {
			// colOutLow is a valid colormap index
			this.colOutLow = getColorMap().convertScilabToColorMapIndex(colOutLow);
		}
		
		if (colOutUp <= -1) {
			// when outside of colormap use the highest value in the colormap
			this.colOutUp = this.colMax;
		} else if (colOutUp == 0) {
			// don't display polygon
			this.colOutUp = -1;
		} else {
			// colOutLow is a valid colormap index
			this.colOutUp = getColorMap().convertScilabToColorMapIndex(colOutUp);
		}
		
		
		
		
		this.zMin = zMin;
		this.zMax = zMax;
		
	}
	
	/**
	 * Convert fec value to color map index
	 * @param value double
	 * @param zMin double
	 * @param zMax double
	 * @return double
	 */
	protected double fecValueToColorMapIndex(double value, double zMin, double zMax) {
		//return ((value - zMin) / (zMax - zMin) * (colMax - colMin + 1.0) + colMin);
		return (colMax - colMin) / (zMax - zMin) * (value - zMax) + colMax;
		
		//return (colMax - colMin - 1) / (zMax - zMin) * (value - zMax) + colMax;
		//return (colMax - colMin - zMin) / (zMax - zMin) * (value - zMax) + colMax;
	}	
	
	/**
	 * Draw the triangles composing the fec object.
	 * @param xCoords X coordinates of nodes
	 * @param yCoords Y coordinates of nodes
	 * @param values value corresponding to each node
	 * @param firstPoints node indexes of triangles first point
	 * @param secondPoints node indexes of triangles second point
	 * @param thirdPoints node indexes of triangles third point
	 */
	public void drawFec(double[] xCoords, double[] yCoords, double[] values,
						int[] firstPoints, int[] secondPoints, int[] thirdPoints) {
		
		// find min and max of values
		double[] minMax = findValuesBounds(values);
		int nbTriangles = firstPoints.length;
		
		if (zMin == zMax) {
			zMin = minMax[0];
			zMax = minMax[1];
		}
		
		GL gl = getGL();

		Vector3D[] triangleCoords = new Vector3D[TRIANGLE_NB_VERTEX];
		double[] triangleColors = new double[TRIANGLE_NB_VERTEX];
		
		GLTools.pushPolygonsBack(gl);
	
		for (int i = 0; i < nbTriangles; i++) {
			triangleCoords[0] = new Vector3D(xCoords[firstPoints[i]], yCoords[firstPoints[i]], 0.0);
			triangleCoords[1] = new Vector3D(xCoords[secondPoints[i]], yCoords[secondPoints[i]], 0.0);
			triangleCoords[2] = new Vector3D(xCoords[thirdPoints[i]], yCoords[thirdPoints[i]], 0.0);
			
			triangleColors[0] = fecValueToColorMapIndex(values[firstPoints[i]], zMin, zMax);
			triangleColors[1] = fecValueToColorMapIndex(values[secondPoints[i]], zMin, zMax);
			triangleColors[2] = fecValueToColorMapIndex(values[thirdPoints[i]], zMin, zMax);
								
			ColoredTriangle ct = new ColoredTriangle(triangleCoords[0],
													 triangleCoords[1],
													 triangleCoords[2],
													 triangleColors[0],
													 triangleColors[1],
													 triangleColors[2]);
			TriangleDecomposition td = ct.decomposeTriangle();
			
			paintFec(td, gl);			
		}
		
		GLTools.endPushPolygonsBack(gl);
		
		
		// create a color map for fec objects
		// create a special colormap to use with the fec object
//		FecColorMap specialCM = new FecColorMap(getColorMap());
//		specialCM.setRealBounds(minMax[0], minMax[1]);
//		specialCM.setFecWeirdValues(zMin, zMax, colMin, colMax, colOutLow, colOutUp);
//		
//		
//		// draw facets using the new colormap
//		GL gl = getGL();
//		int nbTriangles = firstPoints.length;
//		
//		Texture colormapTexture = specialCM.getTexture();
//		colormapTexture.enable();
//		colormapTexture.bind();
//		
//		// push back polygons from the box lines
//		GLTools.pushPolygonsBack(gl);
//		
//		gl.glColor3d(0.0, 1.0, 0.0);
//		
//		gl.glBegin(GL.GL_TRIANGLES);
//		for (int i = 0; i < nbTriangles; i++) {
//			specialCM.useValue(gl, values[firstPoints[i]]);
//			gl.glVertex3d(xCoords[firstPoints[i]], yCoords[firstPoints[i]], 0.0);
//			specialCM.useValue(gl, values[secondPoints[i]]);
//			gl.glVertex3d(xCoords[secondPoints[i]], yCoords[secondPoints[i]], 0.0);
//			specialCM.useValue(gl, values[thirdPoints[i]]);
//			gl.glVertex3d(xCoords[thirdPoints[i]], yCoords[thirdPoints[i]], 0.0);
//		}
//		gl.glEnd();
//		
//		GLTools.endPushPolygonsBack(gl);
//		
//		colormapTexture.disable();
		
	}
	
	/**
	 * Find minimum and maximum bounds within the values
	 * @param values array containing the values for each nodes
	 * @return {min,max}
	 */
	private double[] findValuesBounds(double[] values) {
		double minVal = values[0];
		double maxVal = values[0];
		for (int i = 0; i < values.length; i++) {
			if (values[i] > maxVal) {
				maxVal = values[i];
			} else if (values[i] < minVal) {
				minVal = values[i];
			}
		}
		double[] res = {minVal, maxVal};
		return res;
	}
	
	/**
	 * Counting the color of every polygon in function of the color.
	 * @param td TriangleDecomposition
	 * @param gl GL
	 */
	private void paintFec(TriangleDecomposition td, GL gl) {
				
		gl.glBegin(GL.GL_TRIANGLES);
		
		for (int j = 0; j < td.getNbPolygons(); j++) {
			int color = td.getPolygonColor(j);
			double[] polyColor = getColorFecPolygon(color);
			
			if (polyColor != null) {				
				
				gl.glColor3d(polyColor[0], polyColor[1], polyColor[2]);
				Vector3D[] polygon = td.getPolygon(j);
				
				paintPolygone(gl, polygon);
				
//				// first triangle & following triangles
//				for (int k = 1; k < polygon.length - 1; k++) {
//					gl.glVertex3d(polygon[0].getX(), polygon[0].getY(), polygon[0].getZ());
//					gl.glVertex3d(polygon[k].getX(), polygon[k].getY(), polygon[k].getZ());
//					gl.glVertex3d(polygon[k + 1].getX(), polygon[k + 1].getY(), polygon[k + 1].getZ());
//				}				
			}
		}
		
		gl.glEnd();
		
	}
	
	/**
	 * getColorFecPolygon
	 * @param colorIndex int 
	 * @return polyColor
	 */
	private double[] getColorFecPolygon(int colorIndex) {
		
		double[] polyColor = null;
		
		if (colorIndex < colMin) {
			if (colOutLow >= 0) {
				polyColor = getColorMap().getColor(colOutLow);
			}
		} else if (colorIndex > colMax) {
			if (colOutUp >= 0) {
				polyColor = getColorMap().getColor(colOutUp);
			}
		} else {
			polyColor = getColorMap().getColor(colorIndex);
		}
		
		return polyColor;
	}
	
	/**
	 * This function paint the first triangle and the following triangles for the decomposed polygon
	 * @param gl GL
	 * @param polygon decomposed polygon 
	 */
	public void paintPolygone(GL gl, Vector3D[] polygon) {
		
		// first triangle & following triangles
		for (int k = 1; k < polygon.length - 1; k++) {
			gl.glVertex3d(polygon[0].getX(), polygon[0].getY(), polygon[0].getZ());
			gl.glVertex3d(polygon[k].getX(), polygon[k].getY(), polygon[k].getZ());
			gl.glVertex3d(polygon[k + 1].getX(), polygon[k + 1].getY(), polygon[k + 1].getZ());
		}
		
	}
	
}
