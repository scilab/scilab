/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
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

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * Scilab adapted JOGL TextRenderer. Provides text rendering without aliasing.
 * @author Jean-Baptiste Silvy
 */
public class SciTextRenderer {

	private static final float EPSILON = 1.0e-4f; 

	/** Size of the font to use */
	private float fontSize;
	
	/** Actual object used for text rendering */
	private TextRenderer renderer;
	
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
		setUseFractionalMetrics(true);
		updateScaleFactor();
	}
	
	/**
	 * Update the scale factor to use
	 */
	private void updateScaleFactor() {
		//if (useFractionalMetrics) {
			this.scaleFactor = fontSize / renderer.getFont().getSize2D();
//			
//		} else {
//			this.scaleFactor = 1.0f;
//		}
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
		// move position
		//gl.glPushMatrix();
		//gl.glTranslated(x, y, z);
//		gl.glScalef(fontSize / TextRendererManager.DEFAULT_FONT_SIZE,
//					fontSize / TextRendererManager.DEFAULT_FONT_SIZE,
//					fontSize / TextRendererManager.DEFAULT_FONT_SIZE);
		// with OpenGL strings, angle is already set
		if (useFractionalMetrics) {
			renderer.draw3D(str, (float) x, (float) y, (float) z, scaleFactor);
		} else {
			// we need to add a little offset othrwise texture interpolation
			// sometimes (especially for title) leads to jaggy text.
			renderer.draw3D(str, (float) x, (float) y, (float) z, 1.0f + EPSILON);
		}
		
		// flush since we moved rendering position
		//renderer.flush();
		//gl.glPopMatrix();
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
	 */
	public void begin3DRendering() {
		renderer.begin3DRendering();
	}
	
	/**
	 * Terminatr a drawing sequence
	 */
	public void end3DRendering() {
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
	 * modify the font size of the renderer
	 * To change family it is needed to create a new SciTextRenderer instance
	 * @param newFontSize font size to use
	 */
	public void setFontSize(float newFontSize) {
		this.fontSize = newFontSize;
		updateScaleFactor();
	}
	
	/**
	 * Redefine setColor with only three channels
	 * @param red red channel
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setColor(double red, double green, double blue) {
		renderer.setColor((float) red, (float) green, (float) blue, 1.0f);
	}
	
	/**
	 * Redefine setColor with only three channels
	 * @param color array of size 3 containing the channels
	 */
	public void setColor(double[] color) {
		renderer.setColor((float) color[0], (float) color[1], (float) color[2], 1.0f);
	}
	
	/**
	 * Get the bounding box of the t
	 * @param str String of which we want the bounding box
	 * @return rectangle with the position, width and height.
	 */
	public Rectangle2D getBounds(String str) {
		Rectangle2D res = renderer.getBounds(str);
		
		// apply scale factor to the bounds
		res.setRect(res.getX(), res.getY(),
					res.getWidth() * scaleFactor,
					res.getHeight() * scaleFactor);
		return res;
	}
	

}
