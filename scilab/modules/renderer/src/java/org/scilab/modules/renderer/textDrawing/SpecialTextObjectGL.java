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

/**
 * Special text object 
 * @author Calixte Denizet
 */
public abstract class SpecialTextObjectGL {

    private static final int ALPHA_SHIFT = 24;
    private static final int RED_SHIFT = 16;
    private static final int GREEN_SHIFT = 8;
    private static final int BLUE_SHIFT = 0;
    private static final int COMPONENT_MASK = 0xFF;
    private static final int NB_COMPONENTS = 4;

    protected Buffer buffer;
    protected float height;
    protected float width;
    protected Texture texture;

    protected boolean isColored;

    /**
     * Default constructor
     */
    public SpecialTextObjectGL() {
    }

    /**
     * Return a byte-buffer used to draw content
     * @return byte-buffer 
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
     * @return height
     */
    public float getHeight() {
		return height;
    }
    
    /**
     * Return the width of the content
     * @return width
     */
    public float getWidth() {
		return width;
    }
    
    /**
     * Return the texture's name associated to this label
     * @return the texture object
     */
    public Texture getTexture() {
		return texture;
    }

    /**
     * Set the texture's name associated to this label
     * @param t texture used by GL
     */
    public void setTexture(Texture t) {
		texture = t;
		/* The buffer is set to null since GL put it into the buffer of the video card */
		this.buffer = null;
    }	

    /**
     * Return the isColored property
     * @return isColored property
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
     * @return true if the color changed
     */
    public abstract boolean setColor(Color color);
    
    /**
     * Set the font size of the content
     * @param fontSize the font size of the content
     * @return true if the font size changed
     */
    public abstract boolean setFontSize(float fontSize);

    /**
     * Render the label and set the pixels buffer
     */
    public abstract void makeImage();

    /**
     * Convert an ARGB pixmap into RGBA pixmap
     * @param pix pixmap ARGB data
     * @return pixmap RGBA data 
     */
    protected static byte[] ARGBtoRGBA(int[] pix) {
		byte[] bytes = new byte[pix.length * NB_COMPONENTS];
		int p;
		int [] tmpPix = new int[NB_COMPONENTS];
		int j = 0;
		for (int i = 0; i < pix.length; i++) {
		    p = pix[i];
		    tmpPix[0] = (p >> ALPHA_SHIFT) & COMPONENT_MASK;
		    tmpPix[1] = (p >> RED_SHIFT) & COMPONENT_MASK;
		    tmpPix[2] = (p >> GREEN_SHIFT) & COMPONENT_MASK;
		    tmpPix[NB_COMPONENTS - 1] = (p >> BLUE_SHIFT) & COMPONENT_MASK;
		    bytes[j] = (byte) tmpPix[1];
		    bytes[j + 1] = (byte) tmpPix[2];
		    bytes[j + 2] = (byte) tmpPix[NB_COMPONENTS - 1];
		    bytes[j + NB_COMPONENTS - 1] = (byte) tmpPix[0];
		    j += NB_COMPONENTS;
		}
		
		return bytes;
    }
}
