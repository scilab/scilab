/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to get the list of facet to draw for a surface object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.surfaceDrawing;

/**
 * Class used to get the list of facet to draw for a surface object
 * @author Jean-Baptiste Silvy
 */
public abstract class SurfaceDecomposer implements FacetDecomposer, ColoredFacetDecomposer {
	
	/** number of vertices per facet */
	private int nbVertices;
	
	private double[] xCoords;
	private double[] yCoords;
	private double[] zCoords;
	
	/** To compute facet color if needed */
	private FacetColorComputer colorComputer;
	
	/**
	 * Default constructor
	 */
	public SurfaceDecomposer() {
		xCoords = null;
		yCoords = null;
		zCoords = null;
		nbVertices = -1;
	}
	
	/**
	 * Factory for surface decomposer
	 * @param typeOfSurface plot3d or fac3d
	 * @return new instance of surface decomposer
	 */
	public static SurfaceDecomposer create(int typeOfSurface) {
		switch (typeOfSurface) {
		case FacetDecomposer.FAC3D:
			return new Fac3dDecomposer();
		case FacetDecomposer.PLOT3D:
			return new Plot3dDecomposer();
		default:
			return null;
		}
	}
	
	/**
	 * Set computer
	 * @param computer computer
	 */
	public void setColorComputer(FacetColorComputer computer) {
		this.colorComputer = computer;
	}
	
	/**
	 * @return current colorFacet computer
	 */
	public FacetColorComputer getColorComputer() {
		return colorComputer;
	}
	
	/**
	 * Specify the positions of each vertices
	 * @param xCoords array of x cooridnates
	 * @param yCoords array of y cooridnates
	 * @param zCoords array of z cooridnates
	 * @param nbVertices number of facets in the plot
	 */
	public void setFacetsPositions(double[] xCoords, double[] yCoords, double[] zCoords, int nbVertices) {
		this.nbVertices = nbVertices;
		
		this.xCoords = xCoords;
		this.yCoords = yCoords;
		this.zCoords = zCoords;
		
	}
	
	/**
	 * @return number of vertex per facet
	 */
	protected int getNbVertices() {
		return nbVertices;
	}
	
	/**
	 * @param nbVertices new number of vertices per facet
	 */
	protected void setNbVertices(int nbVertices) {
		this.nbVertices = nbVertices;
	}
	
	/**
	 * @param i index
	 * @return ith value of xCoords
	 */
	protected double getXcoord(int i) {
		return xCoords[i];
	}
	
	/**
	 * @param i index
	 * @return ith value of yCoords
	 */
	protected double getYcoord(int i) {
		return yCoords[i];
	}
	
	/**
	 * @param i index
	 * @return ith value of zCoords
	 */
	protected double getZcoord(int i) {
		return zCoords[i];
	}
	
	/**
	 * compute min and max for zCoordinates
	 * @return array [min, max]
	 */
	protected double[] getZminAndMax() {
		double[] minMax = {zCoords[0], zCoords[0]};
		
		for (int i = 1; i < zCoords.length; i++) {
			if (zCoords[i] < minMax[0]) {
				minMax[0] = zCoords[i];
			} else if (zCoords[i] > minMax[1]) {
				minMax[1] = zCoords[i];
			}
		}
		return minMax;
		
	}
	
	/**
	 * @return number of facets in the surface
	 */
	protected abstract int getNbFacets();

}
