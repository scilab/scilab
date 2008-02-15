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

import org.scilab.modules.renderer.figureDrawing.TextRendererFactory;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * PSTextRenderer
 * @author Sylvestre Koumar
 *
 */
public class PSTextRendererFactory implements TextRendererFactory {

	/**
	 * Constructor
	 */
	public PSTextRendererFactory() {
		super();
	}
	
	/**
	 * createTextRendererFactory
	 * @param font Font
	 * @param color double[]
	 * @return textRenderer
	 */
	public TextRenderer createTextRenderer(Font font,  double[] color) {	
		TextRenderer textRenderer = GL2PSTextRenderer.create(font, color);		
		return textRenderer;
	}

}
