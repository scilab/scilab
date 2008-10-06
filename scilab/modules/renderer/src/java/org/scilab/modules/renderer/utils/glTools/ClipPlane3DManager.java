/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Store all clipped planes and change everyone of them
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.glTools;

import java.util.Iterator;
import java.util.LinkedList;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;

/**
 * Store all cliped planes and change everyone of them
 * @author Jean-Baptiste Silvy
 */
public class ClipPlane3DManager {
	
	/**
	 * List of clipplane. So every one can access the clipped planes
	 */
	private static class ClipPlaneList extends LinkedList<MovableClipPlane3D> {
		
		/**
		 * Needed
		 */
		private static final long serialVersionUID = 2435315256366L;

		/**
		 * Default constructor
		 */
		public ClipPlaneList() {
			super();
		}
	}
	
	/** List of all active clip planes */
	private ClipPlaneList activeClipPlanes;
	
	/**
	 * Default constructor should not be used
	 */
	public ClipPlane3DManager() {
		activeClipPlanes = new ClipPlaneList();
	}
	
	/**
	 * Add a new plane to the list of clipped planes
	 * @param plane plane to add
	 */
	public void addClippedPlane(MovableClipPlane3D plane) {
		activeClipPlanes.add(plane);
	}
	
	/**
	 * Remove a plane from the list of clipped planes
	 * @param plane plane to remove
	 */
	public void removeClippedPlane(MovableClipPlane3D plane) {
		activeClipPlanes.remove(plane);
	}
	
	/**
	 * @return Iterator on the list of currently active clip planes
	 */
	public Iterator<MovableClipPlane3D> getActiveClipPlanes() {
		return activeClipPlanes.listIterator();
	}
	
	/**
	 * Push all planes.
	 * @param gl current GL pipeline
	 * @param transform current transfomrations between 3D, 2D and pixel coordinates
	 */
	public void pushPlanes(GL gl, CoordinateTransformation transform) {
		Iterator<MovableClipPlane3D> curPlaneIt = activeClipPlanes.iterator();
		while (curPlaneIt.hasNext()) {
			curPlaneIt.next().pushPlane(gl, transform);
		}
	}
	
	/**
	 * Pop all planes.
	 * @param gl current GL pipeline
	 */
	public void popAllPlanes(GL gl) {
		Iterator<MovableClipPlane3D> curPlaneIt = activeClipPlanes.iterator();
		while (curPlaneIt.hasNext()) {
			curPlaneIt.next().popPlane(gl);
		}
	}
	
	/**
	 * Change frame for all clipped planes
	 * @param gl current GL pipeline
	 */
	public void changeAllPlanesFrame(GL gl) {
		Iterator<MovableClipPlane3D> curPlaneIt = activeClipPlanes.iterator();
		while (curPlaneIt.hasNext()) {
			curPlaneIt.next().changeFrame(gl);
		}		
	}
	
	/**
	 * @return string representation of the class.
	 */
	public String getStringRepresentation() {
		StringBuffer res = new StringBuffer("");
		Iterator<MovableClipPlane3D> curPlaneIt = activeClipPlanes.iterator();
		while (curPlaneIt.hasNext()) {
			res.append(curPlaneIt.next().toString() + "\n");
		}
		return res.toString();
	}
}
