/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains functions to ease GL calls 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.glTools;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
/**
 * Class containing functions to ease Gl calls
 * @author Jean-Baptiste Silvy Silvy
 */
public final class GLTools {
	
	/** display lists index is always different than 0 */
	public static final int UNINIT_DL_INDEX = 0;
	
	/** 
	 * Depth range when using pixel values.
	 * Actually when using gluProject z may vary between 0 and 1 (relative to depth buffer)
	 * 0 is front clip plane and 1 back clip plane.
	 */
	public static final double MIN_PIXEL_Z = 0.0;
	
	/** Index to use fro drawing grid dash mode */
	public static final int GRID_DASH_INDEX = 3;
	
	/** Disctane max */
	public static final double MAX_PIXEL_Z = -1.0;
	
	/** Contains the different line stipple pattern */
	private static final short[] STIPPLE_PATTERN
	  = {(short) 0xFFFF, // 16 solids, unused equivalent to no stipple
		 (short) 0xFFFF, // 16 solids, unused equivalent to no stipple
		 (short) 0x07FF, // 5 blanks, 11 solid
		 (short) 0x0F0F, // 4 blanks, 4 solids, 4 blanks, 4 solidS
		 (short) 0x1FC2, // 3 blanks, 3 solids, 3 blnaks, 7 solids
		 (short) 0x3FC9, // 2 blanks, 8 solids, 2 blanks, 1 solid, 2 blanks, 1 solid
		 (short) 0x3FC6  // 3 blanks, 8 solids, 3 blanks, 2 solids
		};
	
	private static final int VIEWPORT_LENGTH = 4;
	
	/**
	 * Default contructor
	 * Should not be called
	 */
	private GLTools() { }
	
	/**
	 * Apply a specific line style for every instruction before a call
	 * to endDashMode
	 * @param gl        OpenGL pipeline to use
	 * @param lineStyle index of the line style
	 * @param thickness thickness of the line to draw
	 */
	public static void beginDashMode(GL gl, int lineStyle, float thickness) {
			
		// set thickness
		if (thickness <= 0) {
			// glLineWidth with size 0 or negative creates errors
			gl.glLineWidth(Float.MIN_VALUE);
		} else {
			gl.glLineWidth(thickness);
		}
		
		if (lineStyle <= 1 || lineStyle >= STIPPLE_PATTERN.length) {
			// plain mode, no need to set dash style
			return;
		}
		
		// workaround here
		// in GL2PS glLineStipple must be called before glEnable(GL.GL_LINE_STIPPLE)
		// and in OpenGL it must be called after
		// to test see http://bugzilla.scilab.org/show_bug.cgi?id=1669
		gl.glEnable(GL.GL_LINE_STIPPLE);
		gl.glLineStipple((int) thickness, STIPPLE_PATTERN[lineStyle]);
		gl.glEnable(GL.GL_LINE_STIPPLE);
		gl.glLineStipple((int) thickness, STIPPLE_PATTERN[lineStyle]);
	}
	
	/**
	 * To be called after a beginDahMode call to stop drawing dashes.
	 * @param gl        OpenGL pipeline to use
	 */
	public static void endDashMode(GL gl) {
		gl.glDisable(GL.GL_LINE_STIPPLE);
		gl.glLineWidth(1.0f);
	}
	
	/**
	 * Enable polygon offset
	 * @param gl current gl pipeline
	 */
	public static void pushPolygonsBack(GL gl) {
		gl.glEnable(GL.GL_POLYGON_OFFSET_FILL);
		gl.glPolygonOffset(1.0f, 1.0f);
		gl.glEnable(GL.GL_POLYGON_OFFSET_FILL);
		gl.glPolygonOffset(1.0f, 1.0f);
	}
	
	/**
	 * Disable polygon offset
	 * @param gl current gl pipeline
	 */
	public static void endPushPolygonsBack(GL gl) {
		gl.glDisable(GL.GL_POLYGON_OFFSET_FILL);
	}
	
	/**
	 * Change coordinates to pixel values (for x and y).
	 * To get back to user coordinates call endPixelCoordinates
	 * @param gl current OpenGL pipeline
	 * @param parentFigure figure on which to apply the new coordinates
	 */
	public static void usePixelCoordinates(GL gl, DrawableFigureGL parentFigure) {
		
		// clipping planes must be modified
		CoordinateTransformation transform = parentFigure.getCoordinateTransformation();
		ClipPlane3DManager manager = parentFigure.getClipPlaneManager();
		manager.pushPlanes(gl, transform);
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glPushMatrix();
		transform.loadIdentityWithAntialiasing(gl);
		
		double[] viewPort = transform.getViewPort();
		gl.glOrtho(0.0, viewPort[2], 0.0, viewPort[VIEWPORT_LENGTH - 1],
				   MIN_PIXEL_Z, MAX_PIXEL_Z);
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glPushMatrix();
		gl.glLoadIdentity();
		
		manager.changeAllPlanesFrame(gl);
		
		// transform additional translation to pixels
		Vector3D translation = transform.getAdditionalTranslationPix(gl);
		if (translation != null) {
			gl.glTranslated(translation.getX(), translation.getY(), translation.getZ());
		}
	}
	
	/**
	 * To get back to pixels coordinates from user coordinates.
	 * To be called after a userPixelCoordinates call.
	 * @param gl current OpenGL pipeline
	 * @param parentFigure figure on which to apply the new coordinates
	 */
	public static void endPixelCoordinates(GL gl, DrawableFigureGL parentFigure) {
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glPopMatrix();
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glPopMatrix();
		
		CoordinateTransformation transform = parentFigure.getCoordinateTransformation();
		ClipPlane3DManager manager = parentFigure.getClipPlaneManager();
		Vector3D translation = transform.getAdditionalTranslation();
		if (translation != null) {
			// we have an additional translation here
			// so clipping the plane with its previous equation is not valid here
			// the plane would be translated too. So remove the translation and use it again
			gl.glTranslated(-translation.getX(), -translation.getY(), -translation.getZ());
			manager.popAllPlanes(gl);
			gl.glTranslated(translation.getX(), translation.getY(), translation.getZ());
		} else {
			manager.popAllPlanes(gl);
		}
		
		
	}
	
}
