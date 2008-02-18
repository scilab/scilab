/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
 package org.scilab.modules.graphic_export;


import java.nio.FloatBuffer;
import javax.media.opengl.GL;
import javax.media.opengl.GLContext;
import com.sun.opengl.util.BufferUtil;


/**
 * 
 * @author Sylvestre Koumar
 *
 */
public class GL2PSToJOGL {
	
	/**
	 * Size of viewport for the glGetViewport function
	 */
	public static final int VIEWPORT_SIZE = 4;
	/**
	 * Size of color clear value for the glGetColorClearValue function
	 */
	public static final int COLOR_CLEAR_VALUE_SIZE = 4;
	/**
	 * Size of current raster position for the glGetCurrentRasterPosition function
	 */
	public static final int CURRENT_RASTER_POSITION_SIZE = 4;
	/**
	 * Size of current raster color for the glGetCurrentRasterColor function
	 */
	public static final int CURRENT_RASTER_COLOR_SIZE = 4;
	
	private static FloatBuffer gl2psFeedback;
	
	/**
	 * GL2PSToJOGL
	 */
	protected GL2PSToJOGL() {
		
	}
	
	/**
	 * glFeedBackBuffer
	 * @param size int
	 * @param type int
	 */
	public static void  glFeedbackBuffer(int size, int type) {
		GL gl = GLContext.getCurrent().getGL();		
		gl2psFeedback = BufferUtil.newFloatBuffer(size);
		gl.glFeedbackBuffer(size, type, gl2psFeedback);	
	}
	
	/**
	 * readFeedbackBuffer
	 * @param size int 
	 * @return res
	 */
	public static float[] readFeedbackBuffer(int size) {
		float[] res = new float[size];
		gl2psFeedback.get(res);		
		// desallocate  buffer
		gl2psFeedback = null;
		
		return res;
	}
	
	/**
	 * glIsEnabled
	 * @param cap int
	 * @return gl.glIsEnabled(arg0)
	 */
	public static boolean  glIsEnabled(int cap) {
		GL gl = GLContext.getCurrent().getGL();
		return gl.glIsEnabled(cap);
	}
	
	/**
	 * glBegin
	 * @param mode int 
	 */
	public static void  glBegin(int mode) {
		GL gl = GLContext.getCurrent().getGL();
		gl.glBegin(mode);
	}
	
	/**
	 * glVertex3f
	 * @param x float
	 * @param y float
	 * @param z float
	 */
	public static void  glVertex3f(float x, float y, float z) {
		GL gl = GLContext.getCurrent().getGL();
		gl.glVertex3f(x, y, z);
	}
	
	/**
	 * glEnd
	 */
	public static void  glEnd() {
		GL gl = GLContext.getCurrent().getGL();
		gl.glEnd();
	}
	
	/**
	 * glGetCurrentRasterPositionValid
	 * @return res size 1
	 */
	public static boolean glGetCurrentRasterPositionValid() {
		GL gl = GLContext.getCurrent().getGL();
		byte[] res = new byte[1];
		gl.glGetBooleanv(GL.GL_CURRENT_RASTER_POSITION_VALID, res, 0);		
		return res[0] != 0;		 
	}

	/**
	 * glRenderMode
	 * @param mode int
	 * @return res size 1
	 */
	public static int glRenderMode(int mode) {
		GL gl = GLContext.getCurrent().getGL();
		return gl.glRenderMode(mode);
	}
	
	/**
	 * glGetLineStipplePattern
	 * @return res size 1
	 */
	public static int glGetLineStipplePattern() {
		GL gl = GLContext.getCurrent().getGL();
		int[] res = new int[1];
		gl.glGetIntegerv(GL.GL_LINE_STIPPLE_PATTERN, res, 0);
		return res[0];
	}
	
	/**
	 * glGetLineStippleRepeat
	 * @return res size 1
	 */
	public static int glGetLineStippleRepeat() {
		GL gl = GLContext.getCurrent().getGL();
		int[] res = new int[1];
		gl.glGetIntegerv(GL.GL_LINE_STIPPLE_REPEAT, res, 0);
		return res[0];
	}
	
	
	/**
	 * glGetIndexClearValue
	 * @return res size 1
	 */
	public static int glGetIndexClearValue() {
		GL gl = GLContext.getCurrent().getGL();
		int[] res = new int[1];
		gl.glGetIntegerv(GL.GL_INDEX_CLEAR_VALUE, res, 0);
		return res[0];
	}
	
	/**
	 * glGetViewport
	 * @return res size 4
	 */
	public static int[] glGetViewport() {
		GL gl = GLContext.getCurrent().getGL();
		int[] res = new int[VIEWPORT_SIZE];
		gl.glGetIntegerv(GL.GL_VIEWPORT, res, 0);
		return res;
	}
	
	/**
	 * glGetBlendSrc
	 * @return res size 1
	 */
	public static int glGetBlendSrc() {
		GL gl = GLContext.getCurrent().getGL();
		int[] res = new int[1];
		gl.glGetIntegerv(GL.GL_BLEND_SRC, res, 0);
		return res[0];
	}
	
	/**
	 * glGetBlendDst
	 * @return res size 1
	 */
	public static int glGetBlendDst() {
		GL gl = GLContext.getCurrent().getGL();
		int[] res = new int[1];
		gl.glGetIntegerv(GL.GL_BLEND_DST, res, 0);
		return res[0];
	}
	
	/**
	 * glGetColorClearValue
	 * @return res size 4
	 */
	public static float[] glGetColorClearValue() {
		GL gl = GLContext.getCurrent().getGL();
		float[] res = new float[COLOR_CLEAR_VALUE_SIZE];
		gl.glGetFloatv(GL.GL_COLOR_CLEAR_VALUE, res, 0);
		return res;
	}
	
	/**
	 * glGetPolygonOffsetFactor
	 * @return res size 1
	 */
	public static float glGetPolygonOffsetFactor() {
		GL gl = GLContext.getCurrent().getGL();
		float[] res = new float[1];
		gl.glGetFloatv(GL.GL_POLYGON_OFFSET_FACTOR, res, 0);
		return res[0];
	}
	
	/**
	 * glGetPolygonOffsetUnits
	 * @return res size 1
	 */
	public static float glGetPolygonOffsetUnits() {
		GL gl = GLContext.getCurrent().getGL();
		float[] res = new float[1];
		gl.glGetFloatv(GL.GL_POLYGON_OFFSET_UNITS, res, 0);
		return res[0];
	}
	
	/**
	 * glGetCurrentRasterPosition
	 * @return res size 4
	 */
	public static float[] glGetCurrentRasterPosition() {
		GL gl = GLContext.getCurrent().getGL();
		float[] res = new float[CURRENT_RASTER_POSITION_SIZE];
		gl.glGetFloatv(GL.GL_CURRENT_RASTER_POSITION, res, 0);
		return res;
	}
	
	/**
	 * glGetCurrentRasterColor
	 * @return res size 4
	 */
	public static float[] glGetCurrentRasterColor() {
		GL gl = GLContext.getCurrent().getGL();
		float[] res = new float[CURRENT_RASTER_COLOR_SIZE];
		gl.glGetFloatv(GL.GL_CURRENT_RASTER_COLOR, res, 0);
		return res;
	}
	
	
	/**
	 * glPassThrough
	 * @param token float
	 */
	public static void  glPassThrough(float token) {
		GL gl = GLContext.getCurrent().getGL();
		gl.glPassThrough(token);
	}

}
