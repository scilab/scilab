/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for drawing different kind of axes box 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Parent class for drawing different kind of axes box
 * @author Jean-Baptiste Silvy
 */
public abstract class SubwinBoxDrawer extends DrawableObjectGL {

	/** Number of corners of the box */
	protected static final int BOX_NB_CORNERS = 8;
	
	/** For each corners, lines to draw for the back trihedron */
	protected static final int[][][] BACK_TRIHEDRON = {{{0, 1}, {0, 4}, {0, 2}},
													  {{1, 0}, {1, 3}, {1, 5}},
													  {{2, 0}, {2, 3}, {2, 6}},
													  {{3, 1}, {3, 2}, {3, 7}},
													  {{4, 0}, {4, 5}, {4, 6}},
												   	  {{5, 1}, {5, 4}, {5, 7}},
												   	  {{6, 4}, {6, 2}, {6, 7}},
												   	  {{7, 3}, {7, 5}, {7, 6}}};
	
	/** For each corners, corners of the facet to draw as back facet. */
	protected static final int[][][] BACK_FACETS = {{{0, 2, 3, 1}, {0, 4, 5, 1}, {0, 4, 6, 2}}, 
												   {{1, 0, 4, 5}, {1, 3, 2, 0}, {1, 5, 7, 3}},
												   {{2, 3, 1, 0}, {2, 6, 7, 3}, {2, 0, 4, 6}},
												   {{3, 7, 6, 2}, {3, 1, 0, 2}, {3, 7, 5, 1}},
												   {{4, 5, 1, 0}, {4, 5, 7, 6}, {4, 6, 2, 0}},
												   {{5, 1, 0, 4}, {5, 7, 6, 4}, {5, 7, 3, 1}},
												   {{6, 7, 5, 4}, {6, 2, 3, 7}, {6, 2, 0, 4}},
												   {{7, 5, 4, 6}, {7, 3, 2, 6}, {7, 3, 1, 5}}};
	
	/** For each corners, lines to draw between back trihedron and front one */
	protected static final int[][] HALF_BOX_LINES = {{1, 3, 2, 6, 4, 5},
								     			    {0, 2, 3, 7, 5, 4},
												    {3, 1, 0, 4, 6, 7},
												    {2, 0, 1, 5, 7, 6},
												    {5, 1, 0, 2, 6, 7},
												    {4, 0, 1, 3, 7, 6},
												    {7, 3, 2, 0, 4, 5},
												    {6, 2, 3, 1, 5, 4}};
	
	/** For each corners, lines to draw for the front trihedron */
	protected static final int[][][] FRONT_TRIHEDRON = {{{7, 3}, {7, 5}, {7, 6}},
		                                               {{6, 4}, {6, 2}, {6, 7}},
		                                               {{5, 1}, {5, 4}, {5, 7}},
		                                               {{4, 0}, {4, 5}, {4, 6}},
				 									   {{3, 1}, {3, 2}, {3, 7}},
				 									   {{2, 0}, {2, 3}, {2, 6}},
												       {{1, 0}, {1, 3}, {1, 5}},
												   	   {{0, 1}, {0, 4}, {0, 2}}};
	
	private int concealedCornerIndex;
	
	private Vector3D[] boxCorners;
	
	/**
	 * Default constructor
	 */
	public SubwinBoxDrawer() {
		super();
		boxCorners = null;
		concealedCornerIndex = 0;
	}
	
	/**
	 * Display the object using already given parameters.
	 * Display lists are not usable since hidden axis might change at every redraw.
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawBox(concealedCornerIndex);
		endDrawing();
	}
	
	/**
	 * @param index index of the corner
	 * @return corner of the box corresponding to the index
	 */
	protected Vector3D getBoxCorner(int index) {
		return boxCorners[index];
	}
	
	/**
	 * @param index index of the concealed corner
	 */
	protected void setConcealedCornerIndex(int index) {
		this.concealedCornerIndex = index;
	}
	
	/**
	 * Draw the box specified by its bounds.
	 * @param xMin minimun bounds on X axis.
	 * @param xMax maximum bounds on X axis.
	 * @param yMin minimun bounds on Y axis.
	 * @param yMax maximum bounds on Y axis.
	 * @param zMin minimun bounds on Z axis.
	 * @param zMax maximum bounds on Z axis.
	 * @param concealedCornerIndex index of the concealed corner
	 */
	public void drawBox(double xMin, double xMax,
			            double yMin, double yMax,
			            double zMin, double zMax,
			            int concealedCornerIndex) {
		Vector3D[] newBoxCorners = {new Vector3D(xMin, yMin, zMin),
					   				new Vector3D(xMin, yMin, zMax),
					   				new Vector3D(xMin, yMax, zMin),
									new Vector3D(xMin, yMax, zMax),
									new Vector3D(xMax, yMin, zMin),
									new Vector3D(xMax, yMin, zMax),
									new Vector3D(xMax, yMax, zMin),
									new Vector3D(xMax, yMax, zMax)};
		boxCorners = newBoxCorners;
		
		setConcealedCornerIndex(concealedCornerIndex);
		
		drawBox(concealedCornerIndex);
	}
	
	
	/**
	 * Draw the box specified by its bounds.
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected abstract void drawBox(int concealedCornerIndex);
	
}
