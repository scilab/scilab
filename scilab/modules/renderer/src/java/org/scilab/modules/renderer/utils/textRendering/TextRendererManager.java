/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Singleton class used to set font of text objects   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.textRendering;

import java.awt.Font;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;


import com.sun.opengl.util.j2d.TextRenderer;

/**
 * Class storing the textRender objects used by a figure
 * @author Jean-Baptiste Silvy
 */
public class TextRendererManager extends HashMap<String, TextRenderer[]> {
	
	/** default font size for textRenderer */
	public static final float DEFAULT_FONT_SIZE = 10.0f;
	
	private static final long serialVersionUID = 1L;
	
	/**
	 * List of all font sizes the created text renderer may use.
	 * The 8, 10, 12, 14, 18 and 24, correspond to the 6 font sizes used in in Scilab 4.x.
	 */
	private static final float[] TEXT_RENDERER_FONT_SIZES = {2.0f,
															 4.0f,
															 6.0f,
															 8.0f,
															 10.0f,
															 12.0f,
															 14.0f,
															 18.0f,
															 24.0f,
															 30.0f,
															 36.0f,
															 42.0f,
															 48.0f,
															 54.0f,
															 75.0f,
															 90.0f};
	
	/** choose between JOGL factory and GL2PS one */
	private TextRendererFactory factory;
	
	/**
	 * Default constructor
	 */
	public TextRendererManager() {
		super();
		factory = null;
	}
	
	/**
	 * @param factory new factory to use
	 */
	public void setTextRendererFactory(TextRendererFactory factory) {
		this.factory = factory;
	}
	
	/**
	 * Get the text actual textRenderer thnat should be used to render the Font font.
	 * @param font font to render
	 * @param useFractionalMetrics if true the generated renderer will be able to use fractional metrics
	 * @return instance of TextRender that should be used by the SciTextRenderer
	 */
	protected TextRenderer getTextRenderer(Font font, boolean useFractionalMetrics) {
		TextRenderer[] usedRenderedList = getRendererList(font);
		
		int fontSizeIndex;
		
		// find the font size we will use in the created TexRenderer
		if (useFractionalMetrics) {
			// use a greater font size and then reduce the size
			fontSizeIndex = getRendererUpperSize(font.getSize2D());
		} else {
			// use the font which is just above like in Scilab 4
			fontSizeIndex = getRendererLowerSize(font.getSize2D());
		}
		
		TextRenderer res = usedRenderedList[fontSizeIndex];
		
		if (res == null) {
			// this textRenderer has not yet been use, we need to create it
			float fontSize = TEXT_RENDERER_FONT_SIZES[fontSizeIndex];
			// create a textRenderer with the a font of the same fmaily has the requested font
			// but with a font size within the array
			Font defaultFont = font.deriveFont(fontSize);
			res = new TextRenderer(defaultFont, true, false, null, true);
			// direct rendering. Some report says that enabling
			// vertex arrays may sometime slow down text rendering.
			res.setUseVertexArrays(false);
			
			// add it to the list of textRenderer from this fotn family
			usedRenderedList[fontSizeIndex] = res;
		}
		
		return res;
	}
	
	/**
	 * Get the list of renderer associated with the family of the font.
	 * @param font font of which we wants to get the family.
	 * @return array containing the list of text renderer able to render the font
	 */
	protected TextRenderer[] getRendererList(Font font) {
		TextRenderer[] usedRenderedList = get(font.getFontName());
		
		if (usedRenderedList == null) {
			// this font has not already been used, we need to create a new TextRenderer list
			usedRenderedList = new TextRenderer[TEXT_RENDERER_FONT_SIZES.length];
			
			// add this array to the hashmap
			// we need to use getFontName and not getName here
			// getFontName is more precise
			put(font.getFontName(), usedRenderedList);
		}
		
		return usedRenderedList;
	}
	
	/**
	 * Compute the index in the default font size which is just above the requested font size
	 * @param displayFontSize size of the font to display
	 * @return index of the font in the font size array
	 */
	protected int getRendererUpperSize(float displayFontSize) {
		// get the font size which is just above displayFont size in thte array
		// or one of the bounds if size is out of bounds
		int res;
		for (res = 0; res < TEXT_RENDERER_FONT_SIZES.length; res++) {
			if (TEXT_RENDERER_FONT_SIZES[res] >= displayFontSize) {
				break;
			}
		}
		
		if (res < TEXT_RENDERER_FONT_SIZES.length) {
			return res;
		} else {
			return TEXT_RENDERER_FONT_SIZES.length - 1;
		}
	}
	
	/**
	 * Compute the index in the default font size which is just below the requested font size
	 * @param displayFontSize size of the font to display
	 * @return index of the font in the font size array
	 */
	protected int getRendererLowerSize(float displayFontSize) {
		// get the font size which is just below displayFont size in thte array
		// or one of the bounds if size is out of bounds
		int res;
		for (res = 0; res < TEXT_RENDERER_FONT_SIZES.length; res++) {
			if (TEXT_RENDERER_FONT_SIZES[res] > displayFontSize) {
				break;
			}
		}
		
		if (res == 0) {
			return res;
		} else {
			return res - 1;
		}
	}
	
	/**
	 * Create a new instance of SciTextRenderer displaying the font font.
	 * @param font that will be used in the created SciTextRenderer
	 * @param useFractionalMetrics if true the generated renderer will be able to use fractional metrics
	 * @return new instance of SciTextRenderer
	 */
	public SciTextRenderer createTextRenderer(Font font, boolean useFractionalMetrics) {
		SciTextRenderer res = factory.createTextRenderer(getTextRenderer(font, useFractionalMetrics), font);
		res.setUseFractionalMetrics(useFractionalMetrics);
		return res;
	}
	
	/**
	 * Clear all the mapping.
	 */
	public void clear() {
		// we should call dispose on every entry
		Collection<TextRenderer[]> allTextRenderers = this.values();
		Iterator<TextRenderer[]> it = allTextRenderers.iterator();
		while (it.hasNext()) {
			TextRenderer[] renderers = it.next();
			for (int i = 0; i < renderers.length; i++) {
				if (renderers[i] != null) {
					renderers[i].dispose();
				}
			}
		}
		
		// clear everything
		super.clear();
	}
	
}
