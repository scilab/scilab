/*------------------------------------------------------------------------*/
/* file: TextAlignmentStrategy.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : TextRender to use with Scilab. Provides text rendering without  */
/*        aliasing                                                        */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;

import java.awt.Font;
import java.awt.geom.Rectangle2D;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * TextRender to use with Scilab. Provides text rendering without aliasing.
 * @author Jean-Baptiste Silvy
 */
public class SciTextRenderer extends TextRenderer {

	/** Maximum supported fobnt size by awt */
	private static final float MAX_SIZE = 100.0f;
	
	private float antiAliasingFactor;
	
	/**
	 * Constructor from a Font to use.
	 * @param font font to use.
	 * @param aliasingFactor scale factor to apply to the font.
	 */
	protected SciTextRenderer(Font font, float aliasingFactor) {
		super(font, false, true, null, false);
		this.antiAliasingFactor = aliasingFactor;
	}
	
	/**
	 * Static factory
	 * @param font font to use for the display
	 * @return new instance of SciTextRenderer
	 */
	static SciTextRenderer create(Font font) {
		float aliasingFactor;
		Font newFont;
		if (font.getSize2D() > MAX_SIZE) {
			aliasingFactor = font.getSize2D() / MAX_SIZE;
			newFont = font.deriveFont(MAX_SIZE);
		} else {
			aliasingFactor = 1.0f;
			newFont = font;
		}
		return new SciTextRenderer(newFont, aliasingFactor);
	}
	
	/**
	 * Static factory
	 * @param font font to use for the display
	 * @param color initialize renderer with a certain font
	 * @return new instance of SciTextRenderer
	 */
	static SciTextRenderer create(Font font, double[] color) {
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
		super.draw3D(str, (float) x, (float) y, (float) z, antiAliasingFactor);
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
					 res.getWidth() * antiAliasingFactor,
					 res.getHeight() * antiAliasingFactor);
		return res;
	}
	
	
	
}
