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
