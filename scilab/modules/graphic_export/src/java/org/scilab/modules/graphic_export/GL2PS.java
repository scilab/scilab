/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
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
 * 		   Jean-Baptiste Silvy
 *
 */
public class GL2PS {

	/* GL2PS constants*/ 
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

	private static final int VIEWPORT_SIZE = 4;
	
	/**
	 * Constructor
	 */
	public GL2PS() {

	}


	/**
	 * Interface to gl2psBeginPage (http://geuz.org/gl2ps/#tth_sEc2.1)
	 * @param title Specifies the plot title.
	 * @param producer Specifies the plot producer.
	 * @param viewport Array of size 4. Specifies the plot viewport.
	 * @param format Specifies the output format.
	 * @param sort Specifies the sorting algorithm.
	 * @param options Sets global plot options.
	 * @param colormode Specifies the color mode.
	 * @param colorsize Specifies the size of the colormap.
	 * @param colortable_r Red channel of colormap.
	 * @param colortable_g Green channel of colormap.
	 * @param colortable_b Blue channel of colormap.
	 * @param colortable_a Alpha channel of colormap.
	 * @param nr Controls the number of flat-shaded (sub-)triangles used to approximate a smooth-shaded triangle.
	 * @param ng Controls the number of flat-shaded (sub-)triangles used to approximate a smooth-shaded triangle.
	 * @param nb Controls the number of flat-shaded (sub-)triangles used to approximate a smooth-shaded triangle.
	 * @param buffersize Specifies the size of the feedback buffer.
	 * @param filename Path of the exported file. Unlike standard GL2PS, there is only one parameter to select the
	 *                 output file.
	 * @return GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psBeginPage(String title, String producer, int[] viewport, 
							  int format, int sort, int options, int colormode, int colorsize, 
							  float[] colortable_r, float[] colortable_g, float[] colortable_b, float[] colortable_a, 
							  int nr, int ng, int nb, int buffersize, String filename) {
		
		// Swig does not like null arrays
		float[] colormapR = colortable_r;
		float[] colormapG = colortable_g;
		float[] colormapB = colortable_b;
		float[] colormapA = colortable_a;
		
		int[] viewport1 = viewport;
		
		if (colormapR == null) {
			colormapR = new float[0];
		}
		if (colormapG == null) {
			colormapG = new float[0];
		}
		if (colormapB == null) {
			colormapB = new float[0];
		}
		if (colormapA == null) {
			colormapA = new float[0];
		}
		
		if (viewport == null) {
			// Swig requests an array of size 4
			viewport1 = new int[VIEWPORT_SIZE];
		}
		return GL2PSWrappingJNI.sci_gl2psBeginPage(title, producer, viewport1,
												  format, sort, options, colormode, colorsize,
												  colormapR, colormapG, colormapB, colormapA,
												  nr, ng, nb, buffersize, filename);
	}	

	/**
	 * Interface to gl2psEndPage (http://geuz.org/gl2ps/#tth_sEc2.1)
	 * @return GL2PS_NO_FEEDBACK, GL2PS_OVERFLOW, GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psEndPage() {
		return GL2PSWrapping.sci_gl2psEndPage();
	}

	/**
	 * Interface to gl2psSetOptions (http://geuz.org/gl2ps/#tth_sEc2.9)
	 * @param options global option
	 * @return GL2PS_UNINITIALIZED or GL2PS_SUCCESS
	 */
	public int gl2psSetOptions(int options) {
		return GL2PSWrappingJNI.sci_gl2psSetOptions(options);
	}

	/**
	 * Interface to gl2psBeginViewport (http://geuz.org/gl2ps/#tth_sEc2.8)
	 * @param viewport Array of size 4. New viewport.
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psBeginViewport(int[] viewport) {
		return GL2PSWrappingJNI.sci_gl2psBeginViewport(viewport);
	}

	/**
	 * Interface to gl2psEndViewport (http://geuz.org/gl2ps/#tth_sEc2.8)
	 * @return GL2PS_OVERFLOW, GL2PS_NO_FEEDBACK, GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psEndViewport() {
		return GL2PSWrappingJNI.sci_gl2psEndViewport();
	}

	/**
	 * Interface to gl2psText (http://geuz.org/gl2ps/#tth_sEc2.2)
	 * @param str Specifies the text string to print.
	 * @param fontname Specifies the PostScript name of a valid Type 1 font.
	 * @param fontsize Specifies the size of the font.
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psText(String str, String fontname, short fontsize) {
		return GL2PSWrappingJNI.sci_gl2psText(str, fontname, fontsize);
	}

	/**
	 * Interface to gl2psTextOpt (http://geuz.org/gl2ps/#tth_sEc2.2)
	 * @param str Specifies the text string to print.
	 * @param fontname Specifies the PostScript name of a valid Type 1 font.
	 * @param fontsize Specifies the size of the font.
	 * @param align Specifies the text string alignment with respect to the current raster position.
	 * @param angle Specifies a rotation angle for the text string.
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psTextOpt(String str, String fontname, short fontsize, int align, float angle) {
		return GL2PSWrappingJNI.sci_gl2psTextOpt(str, fontname, fontsize, align, angle);
	}

	/**
	 * Interface to gl2psSpecial (http://geuz.org/gl2ps/#tth_sEc2.4)
	 * @param format Specifies the output format for which the special string will be printed.
	 * @param str Specifies the string to print.
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psSpecial(int format, String str) {
		return GL2PSWrappingJNI.sci_gl2psSpecial(format, str);
	}

	/**
	 * Interface to gl2psEnable (http://geuz.org/gl2ps/#tth_sEc2.5)
	 * @param mode GL2PS_LINE_STIPPLE, GL2PS_POLYGON_OFFSET_FILL, GL2PS_BLEND or GL2PS_POLYGON_BOUNDARY
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psEnable(int mode) {
		return GL2PSWrappingJNI.sci_gl2psEnable(mode);
	}

	/**
	 * Interface to gl2psDisable (http://geuz.org/gl2ps/#tth_sEc2.5)
	 * @param mode GL2PS_LINE_STIPPLE, GL2PS_POLYGON_OFFSET_FILL, GL2PS_BLEND or GL2PS_POLYGON_BOUNDARY
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psDisable(int mode) {
		return GL2PSWrappingJNI.sci_gl2psDisable(mode);
	}

	/**
	 * Interface to gl2psPointSize (http://geuz.org/gl2ps/#tth_sEc2.6)
	 * @param value point size
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psPointSize(float value) {
		return GL2PSWrappingJNI.sci_gl2psPointSize(value);
	}

	/**
	 * Interface to gl2psLineWidth (http://geuz.org/gl2ps/#tth_sEc2.6)
	 * @param value line width
	 * @return GL2PS_UNINITIALIZED, GL2PS_ERROR or GL2PS_SUCCESS
	 */
	public int gl2psLineWidth(float value) {
		return GL2PSWrappingJNI.sci_gl2psLineWidth(value);
	}

	/**
	 * Interface to gl2psBlendFunc (http://geuz.org/gl2ps/#tth_sEc2.7)
	 * @param sfactor sfactor of glBlendFunc
	 * @param dfactor dfactor of glBlendFunc
	 * @return GL2PS_UNINITIALIZED, GL2PS_WARNING or GL2PS_SUCCESS
	 */
	public int gl2psBlendFunc(int sfactor, int dfactor) {
		return GL2PSWrappingJNI.sci_gl2psBlendFunc(sfactor, dfactor);
	}

	/**
	 * Draw an image map (undocumented function)
	 * @param width width of the image map in pixels
	 * @param height height of the image map in pixels
	 * @param position Array of size 3. Position of the image map
	 * @param imagemap A string?
	 * @return GL2PS_UNINITIALIZED or GL2PS_SUCCESS
	 */
	public int gl2psDrawImageMap(int width, int height, float[] position, String imagemap) {
		return GL2PSWrappingJNI.sci_gl2psDrawImageMap(width, height, position, imagemap);
	}

	/**
	 * Get the extension of the file (undocumented function)
	 * @param format file format
	 * @return format
	 */
	public String gl2psGetFileExtension(int format) {
		return GL2PSWrappingJNI.sci_gl2psGetFileExtension(format);
	}


	/**
	 * Get format description (undocumented function)
	 * @param format file format description
	 * @return description
	 */
	public String gl2psGetFormatDescription(int format) {
		return GL2PSWrappingJNI.sci_gl2psGetFormatDescription(format);
	}

    /* Added by Calixte */
    	/**
	 * Draw a pixel buffer
	 * @param w width of image
	 * @param h height of image
	 * @param format specifies the format of the pixel data. GL_RGB and GL_RGBA are the only values accepted at the moment.
	 * @param type specifies the data type for pixels
	 * @return GL2PSWrappingJNI.sci_gl2psGetFormatDescription
	 */
        public static int gl2psDrawPixels(int w, int h, int format, int type, java.nio.Buffer buffer) {
	    return GL2PSWrappingJNI.sci_gl2psDrawPixels(w, h, format, type, buffer);
	}
    /* End */
}
