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

import javax.media.opengl.GL;

/**
 * JOGLConstant
 * @author Sylvestre Koumar
 *
 */
public class JOGLConstant {
	
	/**
	 * Default constructor
	 */
	public JOGLConstant() {
		
	}
	
	/**
	 * get_GL_RGBA
	 * @return GL.GL_RGBA
	 */
	public static int get_GL_RGBA() {
	    return GL.GL_RGBA;
	}
	
	/**
	 * get_GL_RGB
	 * @return GL.GL_RGB
	 */
	public static int get_GL_RGB() {
	    return GL.GL_RGB;
	}
	
	/**
	 * get_GL_CURRENT_RASTER_POSITION_VALID
	 * @return GL.GL_CURRENT_RASTER_POSITION_VALID
	 */
	public static int get_GL_CURRENT_RASTER_POSITION_VALID() {
	    return GL.GL_CURRENT_RASTER_POSITION_VALID;
	}
	/**
	 * get_GL_CURRENT_RASTER_POSITION
	 * @return GL.GL_CURRENT_RASTER_POSITION
	 */
	public static int get_GL_CURRENT_RASTER_POSITION() {
	    return GL.GL_CURRENT_RASTER_POSITION;
	}
	
	/**
	 * get_GL_CURRENT_RASTER_COLOR
	 * @return GL.GL_CURRENT_RASTER_COLOR
	 */
	public static int get_GL_CURRENT_RASTER_COLOR() {
	    return GL.GL_CURRENT_RASTER_COLOR;
	}
	
	/**
	 * get_GL_SRC_ALPHA
	 * @return GL.GL_SRC_ALPHA
	 */
	public static int get_GL_SRC_ALPHA() {
	    return GL.GL_SRC_ALPHA;
	}
	
	/**
	 * get_GL_ONE_MINUS_SRC_ALPHA
	 * @return GL.GL_ONE_MINUS_SRC_ALPHA
	 */
	public static int get_GL_ONE_MINUS_SRC_ALPHA() {
	    return GL.GL_ONE_MINUS_SRC_ALPHA;
	}
	
	/**
	 * get_GL_ONE
	 * @return GL.GL_ONE
	 */
	public static int get_GL_ONE() {
	    return GL.GL_ONE;
	}
	
	/**
	 * get_GL_ZERO
	 * @return GL.GL_ZERO
	 */
	public static int get_GL_ZERO() {
	    return GL.GL_ZERO;
	}
	
	/**
	 * get_GL_COLOR_INDEX
	 * @return GL.GL_COLOR_INDEX
	 */
	public static int get_GL_COLOR_INDEX() {
	    return GL.GL_COLOR_INDEX;
	}
	
	/**
	 * get_GL_POINT_TOKEN
	 * @return GL.GL_POINT_TOKEN
	 */
	public static int get_GL_POINT_TOKEN() {
	    return GL.GL_POINT_TOKEN;
	}
	
	/**
	 * get_GL_LINE_TOKEN
	 * @return GL.GL_LINE_TOKEN
	 */
	public static int get_GL_LINE_TOKEN() {
	    return GL.GL_LINE_TOKEN;
	}
	
	/**
	 * get_GL_LINE_RESET_TOKEN
	 * @return GL.GL_LINE_RESET_TOKEN
	 */
	public static int get_GL_LINE_RESET_TOKEN() {
	    return GL.GL_LINE_RESET_TOKEN;
	}
	
	/**
	 * get_GL_POLYGON_TOKEN
	 * @return GL.GL_POLYGON_TOKEN
	 */
	public static int get_GL_POLYGON_TOKEN() {
	    return GL.GL_POLYGON_TOKEN;
	}
	
	/**
	 * get_GL_BITMAP_TOKEN
	 * @return GL.GL_BITMAP_TOKEN
	 */
	public static int get_GL_BITMAP_TOKEN() {
	    return GL.GL_BITMAP_TOKEN;
	}
	
	/**
	 * get_GL_DRAW_PIXEL_TOKEN
	 * @return GL_DRAW_PIXEL_TOKEN
	 */
	public static int get_GL_DRAW_PIXEL_TOKEN() {
	    return GL.GL_DRAW_PIXEL_TOKEN;
	}
	
	/**
	 * get_GL_COPY_PIXEL_TOKEN
	 * @return GL.GL_COPY_PIXEL_TOKEN
	 */
	public static int get_GL_COPY_PIXEL_TOKEN() {
	    return GL.GL_COPY_PIXEL_TOKEN;
	}
	
	/**
	 * get_GL_PASS_THROUGH_TOKEN
	 * @return GL.GL_PASS_THROUGH_TOKEN
	 */
	public static int get_GL_PASS_THROUGH_TOKEN() {
	    return GL.GL_PASS_THROUGH_TOKEN;
	}
	
	/**
	 * get_GL_FEEDBACK
	 * @return GL.GL_FEEDBACK
	 */
	public static int get_GL_FEEDBACK() {
	    return GL.GL_FEEDBACK;
	}
	
	/**
	 * get_GL_COLOR_CLEAR_VALUE
	 * @return GL.GL_COLOR_CLEAR_VALUE
	 */
	public static int get_GL_COLOR_CLEAR_VALUE() {
	    return GL.GL_COLOR_CLEAR_VALUE;
	}
	
	/**
	 * get_GL_INDEX_CLEAR_VALUE
	 * @return GL.GL_INDEX_CLEAR_VALUE
	 */
	public static int get_GL_INDEX_CLEAR_VALUE() {
	    return GL.GL_INDEX_CLEAR_VALUE;
	}
	
	/**
	 * get_GL_RENDER
	 * @return GL.GL_RENDER
	 */
	public static int get_GL_RENDER() {
	    return GL.GL_RENDER;
	}
	
	/**
	 * get_GL_VIEWPORT
	 * @return GL.GL_VIEWPORT
	 */
	public static int get_GL_VIEWPORT() {
	    return GL.GL_VIEWPORT;
	}
	
	/**
	 * get_GL_BLEND
	 * @return GL.GL_BLEND
	 */
	public static int get_GL_BLEND() {
	    return GL.GL_BLEND;
	}
	
	/**
	 * get_GL_BLEND_SRC
	 * @return GL.GL_BLEND_SRC
	 */
	public static int get_GL_BLEND_SRC() {
	    return GL.GL_BLEND_SRC;
	}
	
	/**
	 * get_GL_BLEND_DST
	 * @return GL.GL_BLEND_DST
	 */
	public static int get_GL_BLEND_DST() {
	    return GL.GL_BLEND_DST;
	}
	
	/**
	 * get_GL_3D_COLOR
	 * @return GL.GL_3D_COLOR
	 */
	public static int get_GL_3D_COLOR() {
	    return GL.GL_3D_COLOR;
	}
	
	/**
	 * get_GL_FLOAT
	 * @return GL.GL_FLOAT
	 */
	public static int get_GL_FLOAT() {
	    return GL.GL_FLOAT;
	}

        /**
	 * get_GL_UNSIGNED_BYTE
	 * @return GL.GL_UNSIGNED_BYTE
	 */
	public static int get_GL_UNSIGNED_BYTE() {
	    return GL.GL_UNSIGNED_BYTE;
	}
	
	/**
	 * get_GL_POINTS
	 * @return GL.GL_POINTS
	 */
	public static int get_GL_POINTS() {
	    return GL.GL_POINTS;
	}
	
	/**
	 * get_GL_POLYGON_OFFSET_FACTOR
	 * @return GL.GL_POLYGON_OFFSET_FACTOR
	 */
	public static int get_GL_POLYGON_OFFSET_FACTOR() {
	    return GL.GL_POLYGON_OFFSET_FACTOR;
	}
	
	/**
	 * get_GL_POLYGON_OFFSET_UNITS
	 * @return GL.GL_POLYGON_OFFSET_UNITS
	 */
	public static int get_GL_POLYGON_OFFSET_UNITS() {
	    return GL.GL_POLYGON_OFFSET_UNITS;
	}
	
	/**
	 * get_GL_LINE_STIPPLE_PATTERN
	 * @return GL.GL_LINE_STIPPLE_PATTERN
	 */
	public static int get_GL_LINE_STIPPLE_PATTERN() {
	    return GL.GL_LINE_STIPPLE_PATTERN;
	}
	
	/**
	 * get_GL_LINE_STIPPLE_REPEAT
	 * @return GL.GL_LINE_STIPPLE_REPEAT
	 */
	public static int get_GL_LINE_STIPPLE_REPEAT() {
	    return GL.GL_LINE_STIPPLE_REPEAT;
	}

}
