/*------------------------------------------------------------------------*/
/* file: GLTools.java                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions to ease GL calls                             */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.glTools;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
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
	public static final double MIN_PIXEL_Z = -1.0;
	/** Disctane max */
	public static final double MAX_PIXEL_Z = 1.0;
	
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
		if (lineStyle <= 1 || lineStyle >= STIPPLE_PATTERN.length) {
			// plain mode, no need to set dash style
			return;
		}
		
		// glLineStipple seems to need to be modified when GL_LINE_STIPPLE is enable
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
	}
	
	/**
	 * Change coordinates to pixel values (for x and y).
	 * To get back to user coordinates call endPixelCoordinates
	 * @param gl current OpenGL pipeline
	 */
	public static void usePixelCoordinates(GL gl) {
		int[] viewPort = new int[VIEWPORT_LENGTH];
		
		// clipping planes must be modified
		ClipPlane3DManager.pushPlanes(gl);
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glPushMatrix();
		gl.glLoadIdentity();
		gl.glGetIntegerv(GL.GL_VIEWPORT, viewPort, 0);
		gl.glOrtho(viewPort[0], viewPort[0] + viewPort[2], viewPort[1], viewPort[1] + viewPort[VIEWPORT_LENGTH - 1],
				   MIN_PIXEL_Z, MAX_PIXEL_Z);
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glPushMatrix();
		gl.glLoadIdentity();
		
		// Coordinates have changed, we need to update the transformation matrix.
		CoordinateTransformation.getTransformation(gl).update(gl);
		
		ClipPlane3DManager.changeAllPlanesFrame(gl);
	}
	
	/**
	 * To get back to pixels coordinates from user coordinates.
	 * To be called after a userPixelCoordinates call.
	 * @param gl current OpenGL pipeline
	 */
	public static void endPixelCoordinates(GL gl) {
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glPopMatrix();
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glPopMatrix();
		
		// Coordinates have changed, we need to update the transformation matrix.
		CoordinateTransformation.getTransformation(gl).update(gl);
		
		ClipPlane3DManager.popAllPlanes(gl);
		
	}
	
}
