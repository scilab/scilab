/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - Calixte Denizet
 * desc : TextRender to use with Scilab. Provides text rendering without aliasing
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
import java.awt.geom.Rectangle2D;

import javax.media.opengl.GL;

import java.lang.reflect.Field;
import com.sun.opengl.util.j2d.TextRenderer;


/**
 * Scilab adapted JOGL TextRenderer. Provides text rendering without aliasing.
 * @author Jean-Baptiste Silvy
 */
public class SciTextRenderer {

	private static boolean areMipmapsAvailable;
	
	private static boolean areMMsAvailableUpToDate;
	
	/** Size of the font to use */
	private float fontSize;
	
	/** Actual object used for text rendering */
	private TextRenderer renderer;
    
	/**  Object for MathML/LaTex rendering */
	private SpecialTextRenderer speRenderer;
	/* End */
	
	/** font size of the renderer object */
	private float scaleFactor;
	
	private boolean useFractionalMetrics;
	
	/**
	 * Constructor from a Font to use.
	 * @param fontSize font Size of the font
	 * @param renderer mapped text renderer
	 */
	public SciTextRenderer(TextRenderer renderer, float fontSize) {
		this.fontSize = fontSize;
		this.renderer = renderer;
		this.speRenderer = new SpecialTextRenderer(renderer, fontSize);

		setUseFractionalMetrics(true);
		updateScaleFactor();
	}
	
	/**
	 * Update the scale factor to use
	 */
	private void updateScaleFactor() {
		this.scaleFactor = fontSize / renderer.getFont().getSize2D();
	}
	
	public SpecialTextRenderer getSpeRenderer(){
		return speRenderer;
	}

	
	/**
	 * Display a string at the desired 3D location.
	 * (x,y,z) is the baseline of the leftmost character.
	 * @param gl current gl pipeline
	 * @param str string to draw
	 * @param x X coordinate of the text
	 * @param y Y coordinate of the text
	 * @param z Z coordinate of the text
	 * @param angle angle of the text to draw
	 */
	public void draw3D(GL gl, String str, double x, double y, double z, double angle) {

	    if (str.length() > 0 && (str.charAt(0) == '<' || str.charAt(0) == '$')) { /* MathML / LaTeX management */
		    speRenderer.draw3D(str, (float) x, (float) y, (float) z, useFractionalMetrics ? scaleFactor : 1.0f);
		    return;
		}

		// with OpenGL strings, angle is already set
		if (useFractionalMetrics) {
			renderer.draw3D(str, (float) x, (float) y, (float) z, scaleFactor);
		} else {
			// we need to add a little offset othrwise texture interpolation
			// sometimes (especially for title) leads to jaggy text.
			renderer.draw3D(str, (float) x, (float) y, (float) z, 1.0f);
		}
	}
	
	/**
	 * Specify wether the font size must stuck with Scilab font Size (integer from 0 to 6) or
	 * if intermediate sizes can be used.
	 * @param useFractionalMetrics if ture useFractionnal metrics
	 */
	public void setUseFractionalMetrics(boolean useFractionalMetrics) {
		this.useFractionalMetrics = useFractionalMetrics;
		if (useFractionalMetrics) {
			renderer.setSmoothing(true);
		} else {
			renderer.setSmoothing(false);
		}
		updateScaleFactor();
	}
	
	/**
	 * Begin a rendering process
	 * @param gl OpenGL pipeline
	 */
	public void begin3DRendering(GL gl) {
		
	    renderer.begin3DRendering();
		
		// HACK HACK HACK for Intel drivers
		// When text is rendered using normal texture mapping (no mipmap)
		// the result is sometime totally fuzzy or the text is simply not displayed.
		// Apparently setting mipmap use solves the problem on this cards.
		// Normally it should not break display on other GC, so let use it.
		fuzzyTextHack(gl);
		// END OF HACK
	}
	
	/**
	 * Terminate a drawing sequence
	 * @param gl OpenGL pipeline
	 */
	public void end3DRendering(GL gl) {
	    renderer.end3DRendering();
	}
	
	/**
	 * @return font used by the renderer.
	 */
	public Font getFont() {
		// renderer font is from the same family but does not have the same size.
		return renderer.getFont().deriveFont(fontSize);
	}
	
	/**
	 * @return real size of text produced by the renderer
	 */
	public double getDisplayedFontSize() {
		if (useFractionalMetrics) {
			return fontSize;
		} else {
			return renderer.getFont().getSize2D();
		}
	}
	
	/**
	 * modify the font size of the renderer
	 * To change family it is needed to create a new SciTextRenderer instance
	 * @param newFontSize font size to use
	 */
	public void setFontSize(float newFontSize) {
		this.fontSize = newFontSize;
		
		/* Set also for the MathML / LaTeX rendering */
		speRenderer.setFontSize(newFontSize);

		updateScaleFactor();
	}
	
	/**
	 * Redefine setColor with only three channels
	 * @param red red channel
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setColor(double red, double green, double blue) {
		/* Set also for the MathML / LaTeX rendering */
		speRenderer.setColor((float) red, (float) green, (float) blue, 1.0f);

		renderer.setColor((float) red, (float) green, (float) blue, 1.0f);
	}
	
	/**
	 * Redefine setColor with only three channels
	 * @param color array of size 3 containing the channels
	 */
	public void setColor(double[] color) {
		/* Set also for the MathML / LaTeX rendering */
		speRenderer.setColor((float) color[0], (float) color[1], (float) color[2], 1.0f);

		renderer.setColor((float) color[0], (float) color[1], (float) color[2], 1.0f);
	}
	
	/**
	 * Get the bounding box of the t
	 * @param str String of which we want the bounding box
	 * @return rectangle with the position, width and height.
	 */
	public Rectangle2D getBounds(String str) {
		Rectangle2D res; 
		
		/* Set also for the MathML / LaTeX rendering */
		if (str.length() > 0 && (str.charAt(0) == '<' || str.charAt(0) == '$')) {
		    res = speRenderer.getBounds(str);
		} else {
		    res = renderer.getBounds(str);
		}
		
		// apply scale factor to the bounds
		res.setRect(res.getX(), res.getY(),
					res.getWidth() * scaleFactor,
					res.getHeight() * scaleFactor);
		return res;
	}
	
	/**
	 * Force recomputation of mipmap availability.
	 * To be called when the OpenGL context is reinit since
	 * mipmap availability may change depending on the GLCapabilities.
	 */
	public static void forceMipMapRecomputationAvailabilty() {
		areMMsAvailableUpToDate = false;
	}
	
	/**
	 * @return true if mipmaps are enabled for the text renderer.
	 */
	private boolean getMipmapAvailability() {
		// The result of the function is actually the mipmap field
		// of the text renderer.
		// The result is the same for each renderers
		// So we just need to compute it once
		
		// The problem here is that the mipmap field is private
		// This following lines retrieve the field even if it is private.
		// This technique is inspired from the TextRenderHack:
		// http://www.javagaming.org/index.php/topic,19239.0.html
		if (!areMMsAvailableUpToDate) {
			// search for the mipmap field inside the renderer object
			Field mipmapField = null;
			try {
				mipmapField = renderer.getClass().getDeclaredField("mipmap");
			} catch (SecurityException e) {
				e.printStackTrace();
			} catch (NoSuchFieldException e) {
				e.printStackTrace();
			}
			
			// the field is private, we need to enable it
			mipmapField.setAccessible(true);
			
			// get the field value
			try {
				areMipmapsAvailable = mipmapField.getBoolean(renderer);
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			}
			areMMsAvailableUpToDate = true;
		}
		return areMipmapsAvailable;
	}

	/**
	 * HACK function for buggy drivers on Intel graphics.
	 * Always try to enable mipmaps since they seems less buggy than normal mode
	 * @param gl current GL pipeline
	 */
	private void fuzzyTextHack(GL gl) {
		// try to use mimaps if possible
		if (!useFractionalMetrics && getMipmapAvailability()) {
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST_MIPMAP_NEAREST);
		}
	}
	

}
