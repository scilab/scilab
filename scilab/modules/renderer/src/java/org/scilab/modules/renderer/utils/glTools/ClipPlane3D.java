/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Plane which can be use to clip the scene
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.glTools;

import org.scilab.modules.renderer.utils.geom3D.Plane3D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

import javax.media.opengl.GL;

/**
 * Plane which can be use to clip the scene
 * @author Jean-Baptiste Silvy
 */
public class ClipPlane3D extends Plane3D {

	private static final int CLIP_PLANE_NOT_AVAILABLE = 0;
	
	
	/** OpenGL index for the clip plane */
	private int clipPlaneIndex = CLIP_PLANE_NOT_AVAILABLE;
	
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param a first equation parameter
	 * @param b second equation parameter
	 * @param c third equation parameter
	 * @param d fourth equation parameter
	 */
	public ClipPlane3D(double a, double b, double c, double d) {
		super(a, b, c, d);
	}
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param equation array of size 4 giving a, b, c and d
	 */
	public ClipPlane3D(double[] equation) {
		super(equation);
	}
	
	/**
	 * Define a plane by giving three of its non aligned points
	 * @param point1 first point
	 * @param point2 second point
	 * @param point3 third point
	 */
	public ClipPlane3D(Vector3D point1, Vector3D point2, Vector3D point3) {
		super(point1, point2, point3);
	}
	
	/**
	 * Default constructor. Should not be used, it does not create a valid plane.
	 */
	protected ClipPlane3D() {
		super();
	}
	
	/**
	 * Clip all points wich does not verify a.x + b.y + c.z + d >= 0;
	 * @param gl current OpenGL pipeline
	 */
	public void clip(GL gl) {
		
		if (clipPlaneIndex == CLIP_PLANE_NOT_AVAILABLE) {
			clipPlaneIndex = getAvailableClipPlane(gl);
			if (clipPlaneIndex == CLIP_PLANE_NOT_AVAILABLE) { return; }
			gl.glEnable(clipPlaneIndex);
		}
		
		gl.glClipPlane(clipPlaneIndex, getEquation(), 0);
	}
	
	
	/**
	 * Remove the clipping
	 * @param gl current OpenGL pipeline
	 */
	public void unClip(GL gl) {
		gl.glDisable(clipPlaneIndex);
		clipPlaneIndex = CLIP_PLANE_NOT_AVAILABLE;
	}
	
	/**
	 * To know if there is a clipplane available
	 * @param gl current OpenGL pipeline
	 * @return true if there is a clip plane available, false otherwise
	 */
	protected int getAvailableClipPlane(GL gl) {
		for (int i = 0; i < GL.GL_MAX_CLIP_PLANES; i++) {
			if (!gl.glIsEnabled(GL.GL_CLIP_PLANE0 + i)) {
				// note that GL_CLIP_PLANEi == GL_CLIP_PLANE0 + i
				return GL.GL_CLIP_PLANE0 + i;
			}
		}
		return CLIP_PLANE_NOT_AVAILABLE;
	}	
}
