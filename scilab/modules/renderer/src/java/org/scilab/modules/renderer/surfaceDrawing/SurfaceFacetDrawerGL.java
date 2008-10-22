/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw  
 * facets composing a surface object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.surfaceDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.AutoDrawableObjectGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw
 * facets composing a surface object with JoGL.
 * @author Jean-Baptiste Silvy
 */
public class SurfaceFacetDrawerGL extends AutoDrawableObjectGL {
	
	private SurfaceDecomposer decomposer;
	private FacetDrawerGL drawer;
	private FacetColorComputer colorComputer;
	
	/**
	 * Default constructor
	 */
	public SurfaceFacetDrawerGL() {
		super();
		decomposer = null;
		drawer = null;
		
	}
	
	/**
	 * Set the hidden color of the surface
	 * @param hiddenColorIndex color index
	 */
	public void setHiddenColor(int hiddenColorIndex) {
		drawer.setHiddenColor(hiddenColorIndex);
	}
	
	/**
	 * Set the default color for the surface
	 * @param defaultColorIndex color index
	 */
	public void setDefaultColor(int defaultColorIndex) {
		colorComputer.setDefaultColor(defaultColorIndex);
	}
	
	/**
	 * Set all the default colors at the same time
	 * @param defaultColor default color index of the surface
	 * @param hiddenColor hidden color index
	 */
	public void setDefaultColors(int defaultColor, int hiddenColor) {
		setDefaultColor(defaultColor);
		setHiddenColor(hiddenColor);
	}
	
	/**
	 * Specify the kind of surface and the colors to use
	 * @param typeOfSurface type of surface (ie Plot3d or Fac3d)
	 * @param colorFlag specify how to compute facets colors
	 */
	public void setSurfaceType(int typeOfSurface, int colorFlag) {
		decomposer = SurfaceDecomposer.create(typeOfSurface);
		
		colorComputer = FacetColorComputer.create(colorFlag, getColorMap(), decomposer);
		decomposer.setColorComputer(colorComputer);
		
		drawer = FacetDrawerGL.create(colorFlag, getColorMap(), getParentFigureGL().getShadeFacetDrawer());
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
		drawer.setNbVertices(nbVertices);
		drawSurface();
		decomposer = null;
		drawer = null;
		colorComputer = null;
	}
	
	/**
	 * Specify the colors to use to draw facets
	 * @param colors colors indices used to draw facets
	 */
	public void setFacetsColors(double[] colors) {
		colorComputer.setFacetsColor(colors);
	}
	
	/**
	 * Routine actually drawing the surface
	 */
	public void drawSurface() {
		
		GL gl = getGL();
		
		drawer.initializeDrawing(gl);
		
		while (decomposer.hasNext()) {
			
			// find position and color
			Vector3D[] curFacetPos = decomposer.next();
			
			// draw facet, if possible
			if (curFacetPos != null) {
				int[] curColors = decomposer.getCurFacetColors();
				drawer.drawFacet(gl, curFacetPos, curColors);
			}
		}
		
		drawer.endDrawing(gl);
		
		
	}
	
}
