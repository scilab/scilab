/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Plane which can be used in different coodinates transformations 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.glTools;
import java.util.LinkedList;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Plane3D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Plane which can be used in different coodinates transformations
 * @author Jean-Baptiste Silvy
 */
public class MovableClipPlane3D extends ClipPlane3D {

	
	/**
	 * Saved previous clipped planes to restore them after
	 */
	private static class ClipPlanePile extends LinkedList<Plane3D> {
		
		/**
		 * Needed
		 */
		private static final long serialVersionUID = 2435397256366L;

		/**
		 * Default constructor
		 */
		public ClipPlanePile() {
			super();
		}
	}
	
	/** normal of the plane */
	private Vector3D normal = new Vector3D();
	
	/** 3 points on the plane */
	private Vector3D point1 = new Vector3D();
	private Vector3D point2 = new Vector3D();
	private Vector3D point3 = new Vector3D();
	
	/** List of all pushed planes */
	private ClipPlanePile pushedPlanes = new ClipPlanePile();
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param a first equation parameter
	 * @param b second equation parameter
	 * @param c third equation parameter
	 * @param d fourth equation parameter
	 */
	public MovableClipPlane3D(double a, double b, double c, double d) {
		super(a, b, c, d);
	}
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param equation array of size 4 giving a, b, c and d
	 */
	public MovableClipPlane3D(double[] equation) {
		super(equation);
	}
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param copyPlane the plane to copy
	 */
	public MovableClipPlane3D(Plane3D copyPlane) {
		super(copyPlane.getEquation());
	}
	
	/**
	 * Define a plane by giving three of its non aligned points
	 * @param point1 first point
	 * @param point2 second point
	 * @param point3 third point
	 */
	public MovableClipPlane3D(Vector3D point1, Vector3D point2, Vector3D point3) {
		super(point1, point2, point3);
	}
	
	/**
	 * Default constructor. Should not be used, it does not create a valid plane.
	 */
	protected MovableClipPlane3D() {
		super();
	}
	
	/**
	 * Clip all points wich does not verify a.x + b.y + c.z + d >= 0;
	 * @param gl current OpenGL pipeline
	 */
	@Override
	public void clip(GL gl) {
		super.clip(gl);
	}
	
	/**
	 * Remove the clipping
	 * @param gl current OpenGL pipeline
	 */
	@Override
	public void unClip(GL gl) {
		super.unClip(gl);
	}
	
	/**
	 * Save the current plane equation before a coordinate transformation.
	 * @param gl current OpenGL pipeline
	 * @param transform current transfomrations between 3D, 2D a, dpixel coordinates
	 */
	public void pushPlane(GL gl, CoordinateTransformation transform) {

		Plane3D copyPlane = new Plane3D(this);
		pushedPlanes.addFirst(copyPlane);
		
		normal = getNormal();
		getThreePointsOnPlane(point1, point2, point3);
		
		// normal is a direction not a position
		normal = transform.getCanvasCoordinates3D(gl, normal);
		normal.normalize();
		
		point1 = transform.getCanvasCoordinates(gl, point1);
		point2 = transform.getCanvasCoordinates(gl, point2);
		point3 = transform.getCanvasCoordinates(gl, point3);
	}
	
	/**
	 * Recompute the plane using the current frame
	 * @param gl current OpenGL pipeline
	 */
	public void changeFrame(GL gl) {
		
		// I've tried to use setEquation with a point and normal, but it doesn'r work
		// for Y axis and I couldn't figure why. Otherwise, it seems to work with 3 points and a normal
		// with only 3 points normal can have the wrong direction (ie normal should be oppisite).
		this.setEquation(point1, point2, point3, normal);
		super.clip(gl); // not just clip, otherwise the plane will be added to the list of clipped planes
	}
	
	/**
	 * Restore the previous plane when coming back to previous coordinate transformations
	 * @param gl current OpenGL pipeline
	 */
	public void popPlane(GL gl) {
		Plane3D restoredPlane = pushedPlanes.removeFirst();
		this.setEquation(restoredPlane.getEquation());
		super.clip(gl); // not just clip, otherwise the plane will be added to the list of clipped planes
	}
	
}
