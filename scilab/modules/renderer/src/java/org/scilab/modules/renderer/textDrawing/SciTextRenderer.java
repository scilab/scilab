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
import java.awt.geom.Rectangle2D;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * TextRender to use with Scilab. Provides text rendering without aliasing.
 * @author Jean-Baptiste Silvy
 */
public class SciTextRenderer extends TextRenderer {

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
	 * Display a string at the desired 3D location.
	 * (x,y,z) is the baseline of the leftmost character.
	 * @param str string to draw
	 * @param x X coordinate of the text
	 * @param y Y coordinate of the text
	 * @param z Z coordinate of the text
	 */
	public void draw3D(String str, double x, double y, double z) {
		super.draw3D(str, (float) x, (float) y, (float) z, 1.0f);
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
	 * @param str String to get the bounding rectangle
	 * @return bounding box of the text.
	 */
	@Override
	public Rectangle2D getBounds(String str) {
		Rectangle2D res = super.getBounds(str);
		res.setFrame(res.getX(), res.getY(),
					 res.getWidth(),
					 res.getHeight());
		return res;
	}
	
	/**
	 * Compare the font of the sciRenderer with an other
	 * @param font font to compare
	 * @return true if the fonts are the sames
	 */
	public boolean hasFont(Font font) {
		Font thisFont = getFont();
		
		return   (thisFont.getSize2D() == font.getSize2D())
			  && (thisFont.getFontName().equals(font.getFontName()));
	}

}
