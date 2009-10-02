/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Calixte Denizet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils.textRendering;

import java.awt.geom.Rectangle2D;
import java.awt.Color;
import java.util.HashMap;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;
import com.sun.opengl.util.j2d.TextRenderer;

import org.scilab.modules.renderer.textDrawing.MathMLObjectGL;
import org.scilab.modules.renderer.textDrawing.TeXObjectGL;
import org.scilab.modules.renderer.textDrawing.SpecialTextObjectGL;

/**
 * Special Text Renderer (for MathML and LaTeX)
 * @author Calixte Denizet
 */
public class SpecialTextRenderer {

    private static HashMap<String, SpecialTextObjectGL> table = new HashMap<String, SpecialTextObjectGL>();
        
    /* I use the TextRenderer to render a string which isn't in mathml format
       although it starts with a '<' or '$'*/
    private TextRenderer textrenderer;

    private Color color = Color.black;
    private float fontSize;

    /**
     * Default constructor.
     * @param textrenderer a TextRenderer to display bad MathML code
     * @param fontSize The size of the font
     */
    public SpecialTextRenderer(TextRenderer textrenderer, float fontSize) {
		this.textrenderer = textrenderer;
		this.fontSize = fontSize + 4; /* @TODO: what is 4 ? */
    }
    
    /**
     * Construct and return a MathML object.
     * @param content the MathML code
	 * @return Returns the MathML object
     */
    public SpecialTextObjectGL getContent(final String content) {
		SpecialTextObjectGL spe;
		if (!table.containsKey(content)) {
			try {
				spe = getSpecialTextObjectGL(content);

				table.put(content, spe);
				return spe;
			} catch (RuntimeException e) { /* @TODO: Catcher l'exception 'RuntimeException' est prohibe. */
				table.put(content, null);
				return null;
			}
		}
    
		spe = table.get(content);
		if (spe != null) {
			spe.setColor(color);
			spe.setFontSize(fontSize);
		}
		return spe;
    }
    
    /**
     * Get the boundaries.
     * @param content the special code
	 * @return Returns the boundaries
     */
    public Rectangle2D getBounds(String content) {
		SpecialTextObjectGL spe = getContent(content);
		if (spe != null) {	
			return new Rectangle2D.Float(0, 0, spe.getWidth(), spe.getHeight());
		} else {
			return textrenderer.getBounds(content);
		}
    }
    
    /**
     * Redefine setColor with four channels.
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    public void setColor(float r, float g, float b, float a) {
		this.color = new Color(r, g, b, a);
    }
    
    /**
     * Modify the font size of the renderer.
     * @param fontSize font size to use
     */
    public void setFontSize(float fontSize) {
		this.fontSize = fontSize + 4; /* @TODO: what is 4 ? */
    }
    
    /**
     * Draw special content to the screen.
     * @param content the special code
     * @param x the x position
     * @param y the y position
     * @param z the z position
     * @param scaleFactor the scale factor used in the TextRenderer
     */
    public void draw3D(String content, float x, float y, float z, float scaleFactor) {
		SpecialTextObjectGL spe = getContent(content);
		if (spe == null) {
			textrenderer.draw3D(content, x, y, z, scaleFactor);
			return;
	    }
	
		GL gl = GLU.getCurrentGL();
	
		/* The method begin3DRendering of the object TextRenderer calls 
		   the method of the same name in object Texture and it enables 
		   texturing. When TEXTURE_2D is enabled, commands for drawing
		   don't work, so I disable it.
		*/
		gl.glDisable(GL.GL_TEXTURE_2D);
		gl.glRasterPos2f(x, y);
		gl.glDrawPixels((int) spe.getWidth(), (int) spe.getHeight(), gl.GL_RGBA, gl.GL_UNSIGNED_BYTE, spe.getBuffer());
		gl.glEnable(GL.GL_TEXTURE_2D);
    }

/**
 * Return the specialTextObjectGL
 *
 * @param content the message itself
 * @return The specialTextObjectGL
 */
    private SpecialTextObjectGL getSpecialTextObjectGL(String content) {
		switch (content.charAt(0)) {
			case '<': 
				return new MathMLObjectGL(content, color, fontSize);
			case '$':
				return new TeXObjectGL(content, color, fontSize);
			default:
				return null;
		}
    }
}
