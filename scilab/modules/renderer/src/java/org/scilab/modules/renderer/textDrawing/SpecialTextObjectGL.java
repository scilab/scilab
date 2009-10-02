package org.scilab.modules.renderer.textDrawing;

import java.nio.Buffer;
import java.awt.Color;

public abstract class SpecialTextObjectGL {
    
    protected Buffer buffer;
    protected float height;
    protected float width;

    public SpecialTextObjectGL() {
    }

    /**
     * Return a byte-buffer used to draw content
     */
    public Buffer getBuffer() {
	return this.buffer;
    }
    
    /**
     * Return the height of the content
     */
    public float getHeight() {
	return this.height;
    }
    
    /**
     * Return the width of the content
     */
    public float getWidth() {
	return this.width;
    }
    
    /**
     * Set the color of the content
     * @param color the color of the content
     */
    public abstract void setColor(Color color);
    
    /**
     * Set the font size of the content
     * @param fontSize the font size of the content
     */
    public abstract void setFontSize(float fontSize);


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
            bytes[j] = (byte)r;
            bytes[j+1] = (byte)g;
            bytes[j+2] = (byte)b;
            bytes[j+3] = (byte)a;
            j += 4;
        }
        return bytes;
    }
}