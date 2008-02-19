/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.figureDrawing;

import java.awt.Font;

import org.scilab.modules.renderer.textDrawing.SciTextRenderer;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * JOGLTextRendererFactory
 * @author Koumar Sylvestre
 *
 */
public class JOGLTextRendererFactory implements TextRendererFactory {
	
	private TextRenderer textRenderer;
	
	/**
	 * Constructor
	 */
	public JOGLTextRendererFactory() {
		
	}

	/**
	 * createTextRendererFactory
	 * @param font Font
	 * @param color double[]
	 * @return textRenderer
	 */
	public TextRenderer createTextRenderer(Font font,  double[] color) {	
		textRenderer = SciTextRenderer.create(font, color);		
		return textRenderer;
	}

}
