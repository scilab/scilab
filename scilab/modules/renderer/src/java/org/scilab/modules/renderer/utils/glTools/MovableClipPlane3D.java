/*------------------------------------------------------------------------*/
/* file: MovableClipPlane3D.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Plane which can be used in different coodinates transformations */
/*------------------------------------------------------------------------*/


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
	private class ClipPlanePile extends LinkedList<Plane3D> {
		
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
		ClipPlane3DManager.addClippedPlane(this);
	}
	
	/**
	 * Remove the clipping
	 * @param gl current OpenGL pipeline
	 */
	@Override
	public void unClip(GL gl) {
		super.unClip(gl);
		ClipPlane3DManager.removeClippedPlane(this);
	}
	
	/**
	 * Save the current plane equation befaore a coordinate transformation
	 * @param gl current OpenGL pipeline
	 */
	public void pushPlane(GL gl) {

		Plane3D copyPlane = new Plane3D(this);
		pushedPlanes.push(copyPlane);
		
		
		getThreePointsOnPlane(point1, point2, point3);
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		point1 = transform.getCanvasCoordinates(gl, point1);
		point2 = transform.getCanvasCoordinates(gl, point2);
		point3 = transform.getCanvasCoordinates(gl, point3);
			
	}
	
	/**
	 * Recompute the plane using the current frame
	 * @param gl current OpenGL pipeline
	 */
	public void changeFrame(GL gl) {

		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		Vector3D newPoint1 = transform.retrieveSceneCoordinates(gl, point1);
		Vector3D newPoint2 = transform.retrieveSceneCoordinates(gl, point2);
		Vector3D newPoint3 = transform.retrieveSceneCoordinates(gl, point3);
		
		this.setEquation(newPoint1, newPoint2, newPoint3);
		super.clip(gl); // not just clip, otherwise the plane will be added to the list of clipped planes
	}
	
	/**
	 * Restore the previous plane when coming back to previous coordinate transformations
	 * @param gl current OpenGL pipeline
	 */
	public void popPlane(GL gl) {
		Plane3D restoredPlane = pushedPlanes.pop();
		this.setEquation(restoredPlane.getEquation());
		super.clip(gl); // not just clip, otherwise the plane will be added to the list of clipped planes
	}
	
}
