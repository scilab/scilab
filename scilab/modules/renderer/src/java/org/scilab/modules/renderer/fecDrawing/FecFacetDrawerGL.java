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

import com.sun.opengl.util.texture.Texture;

/**
 * Class drawing the facets of a fec object
 * @author Jean-Baptiste Silvy
 */
public class FecFacetDrawerGL extends AutoDrawableObjectGL {

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
	 * Set colMin and colMax
	 * @param colMin new colMin
	 * @param colMax new colMax
	 */
	public void setColMinMax(int colMin, int colMax) {
		this.colMin = colMin;
		this.colMax = colMax;
	}
	
	/**
	 * set zMin and zMax
	 * @param zMin new zMin
	 * @param zMax new zMax
	 */
	public void setZMinMax(double zMin, double zMax) {
		this.zMin = zMin;
		this.zMax = zMax;
	}
	
	/**
	 * set colors to use if value < zMin or value < zMax
	 * @param colOutLow color index to use for low values
	 * @param colOutUp color index to use for hight values
	 */
	public void setColOut(int colOutLow, int colOutUp) {
		this.colOutLow = colOutLow;
		this.colOutUp = colOutUp;
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
		setColMinMax(colMin, colMax);
		setZMinMax(zMin, zMax);
		setColOut(colOutLow, colOutUp);
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
		
		if (zMin == zMax) {
			zMin = minMax[0];
			zMax = minMax[1];
		}
	
		
		// create a color map for fec objects
		// create a special colormap to use with the fec object
		FecColorMap specialCM = new FecColorMap(getColorMap());
		specialCM.setRealBounds(minMax[0], minMax[1]);
		specialCM.setFecWeirdValues(zMin, zMax, colMin, colMax, colOutLow, colOutUp);
		
		
		// draw facets using the new colormap
		GL gl = getGL();
		int nbTriangles = firstPoints.length;
		
		Texture colormapTexture = specialCM.getTexture();
		colormapTexture.enable();
		colormapTexture.bind();
		
		// push back polygons from the box lines
		gl.glEnable(GL.GL_POLYGON_OFFSET_FILL);
		gl.glPolygonOffset(1.0f, 1.0f);
		
		gl.glColor3d(0.0, 1.0, 0.0);
		
		gl.glBegin(GL.GL_TRIANGLES);
		for (int i = 0; i < nbTriangles; i++) {
			specialCM.useValue(gl, values[firstPoints[i]]);
			gl.glVertex3d(xCoords[firstPoints[i]], yCoords[firstPoints[i]], 0.0);
			specialCM.useValue(gl, values[secondPoints[i]]);
			gl.glVertex3d(xCoords[secondPoints[i]], yCoords[secondPoints[i]], 0.0);
			specialCM.useValue(gl, values[thirdPoints[i]]);
			gl.glVertex3d(xCoords[thirdPoints[i]], yCoords[thirdPoints[i]], 0.0);
		}
		gl.glEnd();
		
		gl.glDisable(GL.GL_POLYGON_OFFSET_FILL);
		
		colormapTexture.disable();
		
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
	
}
