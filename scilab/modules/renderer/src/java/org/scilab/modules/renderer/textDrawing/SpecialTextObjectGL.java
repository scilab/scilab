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
package org.scilab.modules.renderer.textDrawing;

import java.nio.Buffer;
import java.awt.Color;

import com.sun.opengl.util.texture.Texture;

public abstract class SpecialTextObjectGL {
    
    protected Buffer buffer;
    protected float height;
    protected float width;
    protected Texture texture;

    protected boolean isColored;

    public SpecialTextObjectGL() {
    }

    /**
     * Return a byte-buffer used to draw content
     */
    public Buffer getBuffer() {
		if (buffer != null) {
		    return buffer;
		}
		makeImage();
		return buffer;
    }
    
    /**
     * Return the height of the content
     */
    public float getHeight() {
		return height;
    }
    
    /**
     * Return the width of the content
     */
    public float getWidth() {
		return width;
    }
    
    /**
     * Return the texture's name associated to this label
     */
    public Texture getTexture() {
		return texture;
    }

    /**
     * Set the texture's name associated to this label
     * @param id of the texture got with GL
     */
    public void setTexture(Texture t) {
		texture = t;
		/* The buffer is set to null since GL put it into the buffer of the video card */
		this.buffer = null;
    }	

    /**
     * Return the isColored property
     */
    public boolean getIsColored() {
		return isColored;
    }

    /**
     * Set the isColored property
     * @param isColored the isColored property of the content
     */
    public void setIsColored(boolean isColored) {
		this.isColored = isColored;
    }

    /**
     * Set the color of the content
     * @param color the color of the content
     * Return true if the color changed
     */
    public abstract boolean setColor(Color color);
    
    /**
     * Set the font size of the content
     * @param fontSize the font size of the content
     * Return true if the font size changed
     */
    public abstract boolean setFontSize(float fontSize);

    /**
     * Render the label and set the pixels buffer
     */
    public abstract void makeImage();

    /* Convert an ARGB pixmap into RGBA pixmap */
    protected static byte[] ARGBtoRGBA(int[] pix) {
		byte[] bytes = new byte[pix.length * 4];
		int p, r, g, b, a;
		int j = 0;
		for (int i = 0; i < pix.length; i++) {
		    p = pix[i];
		    a = (p >> 24) & 0xFF;
		    r = (p >> 16) & 0xFF;
		    g = (p >> 8) & 0xFF;
		    b = (p >> 0) & 0xFF;
		    bytes[j] = (byte) r;
		    bytes[j + 1] = (byte) g;
		    bytes[j + 2] = (byte) b;
		    bytes[j + 3] = (byte) a;
		    j += 4;
		}
		
		return bytes;
    }
}
