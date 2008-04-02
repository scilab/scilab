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

package org.scilab.modules.renderer.textDrawing;

import java.awt.Font;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * TextRender to use with Scilab. Provides text rendering without aliasing.
 * @author Jean-Baptiste Silvy
 */
public class SciTextRenderer extends TextRenderer {

	private boolean isRendering;
	
	/**
	 * Constructor from a Font to use.
	 * @param font font to use.
	 */
	protected SciTextRenderer(Font font) {
		super(font, false, true, null, false);
		
		// no antialiasing
		this.setSmoothing(false);
		
		// apparently creates slow down on some computers if activated
		this.setUseVertexArrays(false);
		
		isRendering = false;
	}
	
	/**
	 * Static factory
	 * @param font font to use for the display
	 * @return new instance of SciTextRenderer
	 */
	public static SciTextRenderer create(Font font) {
		return new SciTextRenderer(font);
	}
	
	/**
	 * Static factory
	 * @param font font to use for the display
	 * @param color initialize renderer with a certain font
	 * @return new instance of SciTextRenderer
	 */
	public static SciTextRenderer create(Font font, double[] color) {
		SciTextRenderer res = create(font);
		res.setColor(color[0], color[1], color[2]);
		return res;
	}
	
	/**
	 * Begin a sequence of text drawing
	 */
	public void begin3DRendering() {
		isRendering = true;
		super.begin3DRendering();
	}
	
	/**
	 * End a sequence of text rendering
	 */
	public void end3DRendering() {
		super.end3DRendering();
		isRendering = false;
	}
	
	/**
	 * Free ressources used by this component
	 */
	public void dispose() {
		if (isRendering) {
			end3DRendering();
		}
		super.dispose();
	}
	
	/**
	 * Display a string at the desired 3D location.
	 * (x,y,z) is the baseline of the leftmost character.
	 * @param str string to draw
	 * @param x X coordinate of the text
	 * @param y Y coordinate of the text
	 * @param z Z coordinate of the text
	 */
	public void draw3D(String str, double x, double y, double z) {
		if (!isRendering) {
			begin3DRendering();
		}
		
		//long initTime = System.nanoTime();
		super.draw3D(str, (float) x, (float) y, (float) z, 1.0f);
//long elapsedTime = System.nanoTime() - initTime;
		
		//System.err.println("elapsedTime = " + (elapsedTime * 1.0e-6));
	}
	
	/**
	 * Redefine setColor with only three channels
	 * @param red red channel
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setColor(double red, double green, double blue) {
		super.setColor((float) red, (float) green, (float) blue, 1.0f);
	}
	
	
	/**
	 * Compare the font of the sciRenderer with an other
	 * @param font font to compare
	 * @return true if the fonts are the sames
	 */
	public boolean hasFont(Font font) {
		return getFont().equals(font);
	}

}
