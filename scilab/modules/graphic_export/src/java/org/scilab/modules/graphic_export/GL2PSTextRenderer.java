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

import java.awt.Font;
import java.awt.geom.Rectangle2D;

import javax.media.opengl.GL;
import javax.media.opengl.GLContext;
import org.scilab.modules.renderer.textDrawing.SciTextRenderer;


/**
 * GL2PSTextRenderer
 * @author Sylvestre Koumar
 *
 */
public class GL2PSTextRenderer extends SciTextRenderer {
	

	/**
	 * Constructor
	 * @param font Font
	 */
	protected GL2PSTextRenderer(Font font) {
		super(font);
	}	
	
	/**
	 * Static factory
	 * @param font font to use for the display
	 * @return new instance of GL2PSTextRenderer
	 */
	static GL2PSTextRenderer create(Font font) {
		return new GL2PSTextRenderer(font);
	}
	
	/**
	 * Static factory
	 * @param font font to use for the display
	 * @param color initialize renderer with a certain font
	 * @return new instance of GL2PSTextRenderer
	 */
	public static GL2PSTextRenderer create(Font font, double[] color) {
		GL2PSTextRenderer res = create(font);
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
		//super.draw3D(str, (float) x, (float) y, (float) z, getAntiAliasingFactor());
		//Put the text on the figure
		GL gl = GLContext.getCurrent().getGL();
		
		GL2PS gl2ps = new GL2PS();
		gl.glRasterPos3d(x, y, z);
		gl2ps.gl2psText(str, getFont().getFontName(), (short) getFont().getSize());
		//gl2ps.gl2psTextOpt(str, getFont().getFontName(), (short) getFont().getSize(), GL2PS.GL2PS_TEXT_BL, 0);
	}	
	
	/**
	 * @param str String to get the bounding rectangle
	 * @return bounding box of the text.
	 */
	public Rectangle2D getBounds(String str) {
		Rectangle2D res = super.getBounds(str);
		res.setFrame(res.getX(), res.getY(),
					 res.getWidth(),
					 res.getHeight());
		return res;
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


}
