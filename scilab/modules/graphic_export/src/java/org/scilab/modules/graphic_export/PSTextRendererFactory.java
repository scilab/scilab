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
