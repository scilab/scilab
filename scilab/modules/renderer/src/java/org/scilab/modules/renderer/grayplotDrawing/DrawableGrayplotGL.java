/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a
 * grayplot object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.grayplotDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableClippedObjectGL;
import org.scilab.modules.renderer.utils.ColorMap;

/**
 * Class containing functions called by DrawableGrayplotJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableGrayplotGL extends DrawableClippedObjectGL {

	/**
	 * Default Constructor
	 */
	public DrawableGrayplotGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		displayDL();
	}
	
	/**
	 * Draw the grayplot or matplot object. The object is composed of set of facets.
	 * Each facet is a rectangle whose edges are between (xGrid[i], yGrid[j]) and
	 * (xGrid[i + 1], yGrid[j + 1]) and its color is color[i][j].
	 * @param xGrid array containing the abscissas of the grid
	 * @param yGrid array containing the ordinates of the grid
	 * @param zCoord zCoordinate used to draw the grayplot
	 * @param colors array considered as a matrix containing the color index for each facet
	 *               its size is (xGrid.length - 1) x (yGrid.length - 1)
	 */
	public void drawGrayplot(double[] xGrid, double[] yGrid, double zCoord, int[] colors) {
		GL gl  = getGL();
		
		int nbCol = xGrid.length - 1;
		int nbRow = yGrid.length - 1;
		
		ColorMap colMap = getColorMap();
		
		// rectangles are used to render each pixel
		// Using textures would be much faster
		// however it would not be compatible with GL2PS, so for now we keep the slow version
		
		startRecordDL();
		gl.glBegin(GL.GL_QUADS);
		for (int j = 0; j < nbRow; j++) {
			for (int i = 0; i < nbCol; i++) {
				int curColorIndex = colors[i + nbCol * j];
				if (colMap.isValidScilabIndex(curColorIndex)) {
					double[] curColor = colMap.getColor(colMap.convertScilabToColorMapIndex(curColorIndex));
					gl.glColor3d(curColor[0], curColor[1], curColor[2]);
					gl.glVertex3d(xGrid[i], yGrid[j], zCoord);
					gl.glVertex3d(xGrid[i], yGrid[j + 1], zCoord);
					gl.glVertex3d(xGrid[i + 1], yGrid[j + 1], zCoord);
					gl.glVertex3d(xGrid[i + 1], yGrid[j], zCoord);
				}
			}
		}
		gl.glEnd();
		endRecordDL();
		
	}
	
}
