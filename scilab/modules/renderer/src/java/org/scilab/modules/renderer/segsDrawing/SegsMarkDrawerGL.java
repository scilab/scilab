/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing marks on the ends of segments 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.segsDrawing;

import org.scilab.modules.renderer.drawers.MarkDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class drawing marks on the ends of segments
 * @author Jean-Baptiste Silvy
 */
public class SegsMarkDrawerGL extends MarkDrawerGL {
	
	/**
	 * default constructor
	 */
	public SegsMarkDrawerGL() {
		super();
	}
	
	/**
	 * Display the object by displaying the already stored data
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	@Override
	public void show(int parentFigureIndex) {
		showMarks();
	}
	
	/**
	 * Draw a set of segment knowing their positions and colors
	 * @param startXCoords X coordinate of segments first point
	 * @param endXCoords X coordinate of segments end point
	 * @param startYCoords Y coordinate of segments first point
	 * @param endYCoords Y coordinate of segments end point
	 * @param startZCoords Z coordinate of segments first point
	 * @param endZCoords Z coordinate of segments end point
	 */
	public void drawSegs(double[] startXCoords, double[] endXCoords, double[] startYCoords, double[] endYCoords,
						 double[] startZCoords, double[] endZCoords) {
		int nbSegs = startXCoords.length;
		
		// we draw marks on both ends of segments
		Vector3D[] marksPositions = new Vector3D[2 * nbSegs];
		
		for (int i = 0; i < nbSegs; i++) {
			marksPositions[2 * i] = new Vector3D(startXCoords[i], startYCoords[i], startZCoords[i]);
			marksPositions[2 * i + 1] = new Vector3D(endXCoords[i], endYCoords[i], endZCoords[i]);
		}
		
		drawMarks(marksPositions);
		
	}
	
	/**
	 * Draw the segments
	 */
	public void drawSegs() {
		redrawMarks();
	}

}
