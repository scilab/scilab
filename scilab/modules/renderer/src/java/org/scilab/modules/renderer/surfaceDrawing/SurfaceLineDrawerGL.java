/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw lines 
 * surrounding facets of a surface object
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

import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing the driver dependant routines to draw lines
 * surrounding facets of a surface object.
 * @author Jean-Baptiste Silvy
 */
public class SurfaceLineDrawerGL extends LineDrawerGL {

	
	private SurfaceDecomposer decomposer;
	
	/** Number of vertice per facet */
	private int nbVertices;
	
	/**
	 * Default constructor
	 */
	public SurfaceLineDrawerGL() {
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
	 * Specify the positions of each vertices and raw the surface
	 * @param xCoords array of size nbFacet * nbVertices
	 * @param yCoords array of size nbFacet * nbVertices
	 * @param zCoords array of size nbFacet * nbVertices
	 * @param nbVertices number of vertices for each facet
	 */
	public void drawSurface(double[] xCoords, double[] yCoords, double[] zCoords, int nbVertices) {
	
		decomposer.setFacetsPositions(xCoords, yCoords, zCoords, nbVertices);
		this.nbVertices = nbVertices;
		drawSurface();
		decomposer = null;
	}
	
	/**
	 * Draw the lines of the surface
	 */
	public void drawSurface() {
		GL gl = getGL();
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		gl.glBegin(GL.GL_LINES);
		while (decomposer.hasNext()) {
			
			// find position of the facet
			Vector3D[] curFacetPos = decomposer.next();
			
			// draw the lines composing the facet
			if (curFacetPos != null) {
				for (int i = 0; i < nbVertices - 1; i++) {
					gl.glVertex3d(curFacetPos[i].getX(), curFacetPos[i].getY(), curFacetPos[i].getZ());
					gl.glVertex3d(curFacetPos[i + 1].getX(), curFacetPos[i + 1].getY(), curFacetPos[i + 1].getZ());
				}
			
				// last line
				gl.glVertex3d(curFacetPos[nbVertices - 1].getX(),
							  curFacetPos[nbVertices - 1].getY(),
							  curFacetPos[nbVertices - 1].getZ());
				gl.glVertex3d(curFacetPos[0].getX(), curFacetPos[0].getY(), curFacetPos[0].getZ());
			}
			
		}
		
		gl.glEnd();
		
		GLTools.endDashMode(gl);
	}
	
}
