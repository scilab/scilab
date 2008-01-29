/*------------------------------------------------------------------------*/
/* file: SurfaceLineDrawerGL.java                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw marks    */
/*        on vertices of a surface object facets                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.surfaceDrawing;

import org.scilab.modules.renderer.drawers.MarkDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw marks
 * on vertices of a surface object facets
 * @author Jean-Baptiste Silvy
 */
public class SurfaceMarkDrawerGL extends MarkDrawerGL {

	/** positions of marks */
	private Vector3D[] marksPos;
	
	private SurfaceDecomposer decomposer;
	
	/**
	 * Default constructor
	 */
	public SurfaceMarkDrawerGL() {
		super();
	}
	
	
	/**
	 * Specify the kind of surface to use
	 * @param typeOfSurface type of surface (ie Plot3d or Fac3d)
	 */
	public void setSurfaceType(int typeOfSurface) {
		decomposer = SurfaceDecomposer.create(typeOfSurface);
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawSurface();
		endDrawing();
	}
	
	/**
	 * Draw the marks that have been computed
	 */
	public void drawSurface() {
		drawMarks(marksPos);
	}
	
	/**
	 * Specify the positions of each vertices and draw the surface
	 * @param xCoords array of size nbFacet * nbVertices
	 * @param yCoords array of size nbFacet * nbVertices
	 * @param zCoords array of size nbFacet * nbVertices
	 * @param nbVertices number of vertices for each facet
	 */
	public void drawSurface(double[] xCoords, double[] yCoords, double[] zCoords, int nbVertices) {
	
		decomposer.setFacetsPositions(xCoords, yCoords, zCoords, nbVertices);
		
		// create a new array of marks positions
		marksPos = new Vector3D[nbVertices * decomposer.getNbFacets()];
		
		int markIndex = 0;
		while (decomposer.hasNext()) {
			// find positions of the facet vertices
			Vector3D[] curFacetPos = decomposer.next();
			
			// copy the found values into the array of mark positions
			for (int i = 0; i < curFacetPos.length; i++) {
				marksPos[markIndex] = curFacetPos[i];
				markIndex++;
			}
		}
		
		drawSurface();
		
		// decomposer is no longer needed, we got the array of marks positions
		decomposer = null;
	}

}
