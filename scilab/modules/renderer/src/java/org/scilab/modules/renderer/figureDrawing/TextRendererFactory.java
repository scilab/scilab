package org.scilab.modules.renderer.figureDrawing;



import java.awt.Font;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * TextRendererFactory
 * @author Sylvestre Koumar
 *
 */
public interface TextRendererFactory {
	
	/**
	 * createTextRendererFactory
	 * @param font font
	 * @param color double[]
	 * @return TextRenderer
	 */
	TextRenderer createTextRenderer(Font font,  double[] color);

}
