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

import org.scilab.modules.graphic_export.jni.GL2PSConstant;
import org.scilab.modules.graphic_export.jni.GL2PSWrapping;
import org.scilab.modules.graphic_export.jni.GL2PSWrappingJNI;
/**
 * GL2PS Class
 * @author Sylvestre Koumar
 *
 */
public class GL2PS {

	/** Get GL2PS constants*/ 
	public static final int GL2PS_MAJOR_VERSION = GL2PSConstant.get_GL2PS_MAJOR_VERSION();
	public static final int GL2PS_MINOR_VERSION = GL2PSConstant.get_GL2PS_MINOR_VERSION();
	public static final int GL2PS_PATCH_VERSION = GL2PSConstant.get_GL2PS_PATCH_VERSION();
	public static final String GL2PS_EXTRA_VERSION = GL2PSConstant.get_GL2PS_EXTRA_VERSION();
	public static final int GL2PS_PS = GL2PSConstant.get_GL2PS_PS();
	public static final int GL2PS_EPS = GL2PSConstant.get_GL2PS_EPS();
	public static final int GL2PS_TEX = GL2PSConstant.get_GL2PS_TEX();
	public static final int GL2PS_PDF = GL2PSConstant.get_GL2PS_PDF();
	public static final int GL2PS_SVG = GL2PSConstant.get_GL2PS_SVG();
	public static final int GL2PS_PGF = GL2PSConstant.get_GL2PS_PGF();
	public static final int GL2PS_NO_SORT = GL2PSConstant.get_GL2PS_NO_SORT();
	public static final int GL2PS_SIMPLE_SORT = GL2PSConstant.get_GL2PS_SIMPLE_SORT();
	public static final int GL2PS_BSP_SORT = GL2PSConstant.get_GL2PS_BSP_SORT();
	public static final int GL2PS_SUCCESS = GL2PSConstant.get_GL2PS_SUCCESS();
	public static final int GL2PS_INFO = GL2PSConstant.get_GL2PS_INFO();
	public static final int GL2PS_WARNING = GL2PSConstant.get_GL2PS_WARNING();
	public static final int GL2PS_ERROR = GL2PSConstant.get_GL2PS_ERROR();
	public static final int GL2PS_NO_FEEDBACK = GL2PSConstant.get_GL2PS_NO_FEEDBACK();
	public static final int GL2PS_OVERFLOW = GL2PSConstant.get_GL2PS_OVERFLOW();
	public static final int GL2PS_UNINITIALIZED = GL2PSConstant.get_GL2PS_UNINITIALIZED();
	public static final int GL2PS_NONE = GL2PSConstant.get_GL2PS_NONE();
	public static final int GL2PS_DRAW_BACKGROUND = GL2PSConstant.get_GL2PS_DRAW_BACKGROUND();
	public static final int GL2PS_SIMPLE_LINE_OFFSET = GL2PSConstant.get_GL2PS_SIMPLE_LINE_OFFSET();
	public static final int GL2PS_SILENT = GL2PSConstant.get_GL2PS_SILENT();
	public static final int GL2PS_BEST_ROOT = GL2PSConstant.get_GL2PS_BEST_ROOT();
	public static final int GL2PS_OCCLUSION_CULL = GL2PSConstant.get_GL2PS_OCCLUSION_CULL();
	public static final int GL2PS_NO_TEXT = GL2PSConstant.get_GL2PS_NO_TEXT();
	public static final int GL2PS_LANDSCAPE = GL2PSConstant.get_GL2PS_LANDSCAPE();
	public static final int GL2PS_NO_PS3_SHADING = GL2PSConstant.get_GL2PS_NO_PS3_SHADING();
	public static final int GL2PS_NO_PIXMAP = GL2PSConstant.get_GL2PS_NO_PIXMAP();
	public static final int GL2PS_USE_CURRENT_VIEWPORT = GL2PSConstant.get_GL2PS_USE_CURRENT_VIEWPORT();
	public static final int GL2PS_COMPRESS = GL2PSConstant.get_GL2PS_COMPRESS();
	public static final int GL2PS_NO_BLENDING = GL2PSConstant.get_GL2PS_NO_BLENDING();
	public static final int GL2PS_TIGHT_BOUNDING_BOX = GL2PSConstant.get_GL2PS_TIGHT_BOUNDING_BOX();
	public static final int GL2PS_POLYGON_OFFSET_FILL = GL2PSConstant.get_GL2PS_POLYGON_OFFSET_FILL();
	public static final int GL2PS_POLYGON_BOUNDARY = GL2PSConstant.get_GL2PS_POLYGON_BOUNDARY();
	public static final int GL2PS_LINE_STIPPLE = GL2PSConstant.get_GL2PS_LINE_STIPPLE();
	public static final int GL2PS_BLEND = GL2PSConstant.get_GL2PS_BLEND();
	public static final int GL2PS_TEXT_C = GL2PSConstant.get_GL2PS_TEXT_C();
	public static final int GL2PS_TEXT_CL = GL2PSConstant.get_GL2PS_TEXT_CL();
	public static final int GL2PS_TEXT_CR = GL2PSConstant.get_GL2PS_TEXT_CR();
	public static final int GL2PS_TEXT_B = GL2PSConstant.get_GL2PS_TEXT_B();
	public static final int GL2PS_TEXT_BL = GL2PSConstant.get_GL2PS_TEXT_BL();
	public static final int GL2PS_TEXT_BR = GL2PSConstant.get_GL2PS_TEXT_BR();
	public static final int GL2PS_TEXT_T = GL2PSConstant.get_GL2PS_TEXT_T();
	public static final int GL2PS_TEXT_TL = GL2PSConstant.get_GL2PS_TEXT_TL();
	public static final int GL2PS_TEXT_TR = GL2PSConstant.get_GL2PS_TEXT_TR();

	public static final int RGBA_SIZE = 4;

	/**
	 * Constructor
	 */
	public GL2PS() {

	}


	/**
	 * Begin page
	 * @param title of the page
	 * @param producer of the page
	 * @param viewport of the page
	 * @param format of the page
	 * @param sort index of the figure to redraw
	 * @param options index of the figure to redraw
	 * @param colormode index of the figure to redraw
	 * @param colorsize index of the figure to redraw
	 * @param colormap_r index of the figure to redraw
	 * @param colormap_g index of the figure to redraw
	 * @param colormap_b index of the figure to redraw
	 * @param colormap_a index of the figure to redraw
	 * @param nr index of the figure to redraw
	 * @param ng index of the figure to redraw
	 * @param nb index of the figure to redraw
	 * @param buffersize index of the figure to redraw
	 * @param filename index of the figure to redraw
	 * @return GL2PSWrappingJNI.sci_gl2psBeginPage
	 */
	public int gl2psBeginPage(String title, String producer, int[] viewport, 
							  int format, int sort, int options, int colormode, int colorsize, 
							  float[] colormap_r, float[] colormap_g, float[] colormap_b, float[] colormap_a, 
							  int nr, int ng, int nb, int buffersize, String filename) {
		
		float[] colormap_r1 = colormap_r;
		float[] colormap_g1 = colormap_g;
		float[] colormap_b1 = colormap_b;
		float[] colormap_a1 = colormap_a;
		
		int[] viewport1 = viewport;
		if (colormap_r == null) {
			colormap_r1 = new float[RGBA_SIZE];
		}
		if (colormap_g == null) {
			colormap_g1 = new float[RGBA_SIZE];
		}
		if (colormap_b == null) {
			colormap_b1 = new float[RGBA_SIZE];
		}
		if (colormap_a == null) {
			colormap_a1 = new float[RGBA_SIZE];
		}
		if (viewport == null) {
			viewport1 = new int[RGBA_SIZE];
		}
		return GL2PSWrappingJNI.sci_gl2psBeginPage(title, producer, viewport1, format, sort, options, colormode, colorsize, colormap_r1, colormap_g1, colormap_b1, colormap_a1, nr, ng, nb, buffersize, filename);
	}	

	/**
	 * End of the page
	 * @return GL2PSWrappingJNI.sci_gl2psEndPage
	 */
	public int gl2psEndPage() {
		return GL2PSWrapping.sci_gl2psEndPage();
	}

	/**
	 * Option of the page
	 * @param options of the page
	 * @return GL2PSWrappingJNI.sci_gl2psSetOptions
	 */
	public int gl2psSetOptions(int options) {
		return GL2PSWrappingJNI.sci_gl2psSetOptions(options);
	}

	/**
	 * Beginning of the viewport
	 * @param viewport of the figure
	 * @return GL2PSWrappingJNI.sci_gl2psBeginViewport
	 */
	public int gl2psBeginViewport(int[] viewport) {
		return GL2PSWrappingJNI.sci_gl2psBeginViewport(viewport);
	}

	/**
	 * End of the viewport
	 * @return GL2PSWrappingJNI.sci_gl2psEndViewport
	 */
	public int gl2psEndViewport() {
		return GL2PSWrappingJNI.sci_gl2psEndViewport();
	}

	/**
	 * Text
	 * @param str text to display
	 * @param fontname of the text
	 * @param fontsize of the text
	 * @return GL2PSWrappingJNI.sci_gl2psText
	 */
	public int gl2psText(String str, String fontname, short fontsize) {
		return GL2PSWrappingJNI.sci_gl2psText(str, fontname, fontsize);
	}

	/**
	 * TextOPT
	 * @param str text to display
	 * @param fontname of the text
	 * @param fontsize of the text
	 * @param align of the text
	 * @param angle of the text
	 * @return GL2PSWrappingJNI.sci_gl2psTextOpt
	 */
	public int gl2psTextOpt(String str, String fontname, short fontsize, int align, float angle) {
		return GL2PSWrappingJNI.sci_gl2psTextOpt(str, fontname, fontsize, align, angle);
	}

	/**
	 * Special
	 * @param format of special
	 * @param str of special
	 * @return GL2PSWrappingJNI.sci_gl2psSpecial
	 */
	public int gl2psSpecial(int format, String str) {
		return GL2PSWrappingJNI.sci_gl2psSpecial(format, str);
	}

	/**
	 * Enable
	 * @param mode enable mode
	 * @return GL2PSWrappingJNI.sci_gl2psEnable
	 */
	public int gl2psEnable(int mode) {
		return GL2PSWrappingJNI.sci_gl2psEnable(mode);
	}

	/**
	 * Disable
	 * @param mode disable mode
	 * @return GL2PSWrappingJNI.sci_gl2psDisable
	 */
	public int gl2psDisable(int mode) {
		return GL2PSWrappingJNI.sci_gl2psDisable(mode);
	}

	/**
	 * Size of the point
	 * @param value value given for the size
	 * @return GL2PSWrappingJNI.sci_gl2psPointSize
	 */
	public int gl2psPointSize(float value) {
		return GL2PSWrappingJNI.sci_gl2psPointSize(value);
	}

	/**
	 * Width of the line
	 * @param value value value given for the width
	 * @return GL2PSWrappingJNI.sci_gl2psLineWidth
	 */
	public int gl2psLineWidth(float value) {
		return GL2PSWrappingJNI.sci_gl2psLineWidth(value);
	}

	/**
	 * Blend Function
	 * @param sfactor sfactor
	 * @param dfactor dfactor
	 * @return GL2PSWrappingJNI.sci_gl2psBlendFunc
	 */
	public int gl2psBlendFunc(int sfactor, int dfactor) {
		return GL2PSWrappingJNI.sci_gl2psBlendFunc(sfactor, dfactor);
	}

	/**
	 * Draw an image map (undocumented function)
	 * @param width of the image map
	 * @param height of the image map
	 * @param position of the image map
	 * @param imagemap imagemap
	 * @return GL2PSWrappingJNI.sci_gl2psDrawImageMap
	 */
	public int gl2psDrawImageMap(int width, int height, float[] position, String imagemap) {
		return GL2PSWrappingJNI.sci_gl2psDrawImageMap(width, height, position, imagemap);
	}

	/**
	 * Get the extension of the file (undocumented function)
	 * @param format file format
	 * @return GL2PSWrappingJNI.sci_gl2psGetFileExtension
	 */
	public String gl2psGetFileExtension(int format) {
		return GL2PSWrappingJNI.sci_gl2psGetFileExtension(format);
	}


	/**
	 * Get format description (undocumented function)
	 * @param format file format description
	 * @return GL2PSWrappingJNI.sci_gl2psGetFormatDescription
	 */
	public String gl2psGetFormatDescription(int format) {
		return GL2PSWrappingJNI.sci_gl2psGetFormatDescription(format);
	}

}
