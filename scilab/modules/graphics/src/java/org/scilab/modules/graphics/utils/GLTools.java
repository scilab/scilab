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
public class GLTools {
	
	
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
	 */
	public static void beginDashMode(GL gl, int lineStyle, int thickness) {
		if ( lineStyle == 0 || lineStyle == 1) {
			// plain mode, no need to set dash style
			return;
		}
		
		switch(lineStyle) {
		case 2:
			// 5 blanks, 11 solid
			gl.glLineStipple(thickness, (short)0x07FF);
			break;
		case 3:
			// 4 blanks, 4 solids, 4 blanks, 4 solids
			gl.glLineStipple(thickness, (short)0x0F0F);
			break;
		case 4:
			// 3 blanks, 3 solids, 3 blnaks, 7 solids
			gl.glLineStipple(thickness, (short)0x1FC2);
			break;
		case 5:
			
			gl.glLineStipple(thickness, (short)0x3FC9);
			break;
		case 6:
			gl.glLineStipple(thickness, (short)0x3FC6);
			break;
		default:
			// should not happened
			break;
		}
		gl.glEnable(GL.GL_LINE_STIPPLE);
	}
	
	public static void endDashMode(GL gl) {
		gl.glDisable(GL.GL_LINE_STIPPLE);
	}
	
}
