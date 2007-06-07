/*------------------------------------------------------------------------*/
/* file: GLTools.java                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions to ease GL calls                             */
/*------------------------------------------------------------------------*/


package org.scilab.modules.graphics.utils;

import javax.media.opengl.GL;
/**
 * Class containing functions to ease Gl calls
 * @author Jean-Baptiste Silvy Silvy
 */
public final class GLTools {
	
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
	public static void beginDashMode(GL gl, int lineStyle, int thickness) {
		if (lineStyle <= 1 || lineStyle >= STIPPLE_PATTERN.length) {
			// plain mode, no need to set dash style
			return;
		}
		
		gl.glLineStipple(thickness, STIPPLE_PATTERN[lineStyle]);
		
		gl.glEnable(GL.GL_LINE_STIPPLE);
	}
	
	/**
	 * To be called after a beginDahMode call to stop drawing dashes.
	 * @param gl        OpenGL pipeline to use
	 */
	public static void endDashMode(GL gl) {
		gl.glDisable(GL.GL_LINE_STIPPLE);
	}
	
}
