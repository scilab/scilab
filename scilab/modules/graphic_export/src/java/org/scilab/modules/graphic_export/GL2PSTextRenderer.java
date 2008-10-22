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
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;

import com.sun.opengl.util.j2d.TextRenderer;


/**
 * GL2PSTextRenderer
 * @author Sylvestre Koumar
 *
 */
public class GL2PSTextRenderer extends SciTextRenderer {
	

	/**
	 * Constructor from a Font to use.
	 * @param fontSize font Size of the font
	 * @param renderer mapped text renderer
	 */
	public GL2PSTextRenderer(TextRenderer renderer, float fontSize) {
		super(renderer, fontSize);
	}
	
	/**
	 * Display a string at the desired 3D location.
	 * (x,y,z) is the baseline of the leftmost character.
	 * @param gl current OpenGL pipeline
	 * @param str string to draw
	 * @param x X coordinate of the text
	 * @param y Y coordinate of the text
	 * @param z Z coordinate of the text
	 * @param angle angle of the text to draw
	 */
	public void draw3D(GL gl, String str, double x, double y, double z, double angle) {
		//Put the text on the figure
		
		GL2PS gl2ps = new GL2PS();
		gl.glRasterPos3d(x, y, z);
		gl2ps.gl2psTextOpt(str, getFont().getPSName(),
				           (short) getFont().getSize(), GL2PS.GL2PS_TEXT_BL, (float) Math.toDegrees(angle));
	}	


}
