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

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.nio.ByteBuffer;

import org.scilab.forge.jlatexmath.ParseException;
import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.modules.renderer.utils.textRendering.SpecialTextException;

/**
 * LaTeX object
 * @author Calixte Denizet
 */
public class TeXObjectGL extends SpecialTextObjectGL {
        
    protected final static Component COMPONENT = (Component) new Canvas();
    protected TeXIcon texi;
    protected TeXFormula formula;
    private Color color;
    private float fontSize;

    /** 
     * Default constructor.
     * @param content the LaTeX code
     * @param color the color of the content
     * @param fontSize the size of the font
     * @throws SpecialTextException if the string is not a LaTeX expression
     */
    public TeXObjectGL(String content, Color color, float fontSize) throws SpecialTextException {
		if (content.endsWith("$")) {
			content = content.substring(1, content.length() - 1);
		} else {
			throw new SpecialTextException("Not a LaTeX expression");
		}
	
		try {
			formula = new TeXFormula(content);
		} catch (ParseException e) {
			throw new SpecialTextException("Not a LaTeX expression");
		}
 
		this.fontSize = fontSize;
		this.color = color;
		this.texi = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize);

		this.isColored = this.texi.isColored;

		COMPONENT.setForeground(color);
		makeImage();
    }

    /**
     * Copy constructor
     * @param t TeXObjectGL to copy 
     */
    public TeXObjectGL(TeXObjectGL t) {
	this.width = t.width;
	this.height = t.height;
	this.texi = t.texi;
	this.formula = t.formula;
	this.color = t.color;
	this.fontSize = t.fontSize;
    }
        
    /**
     * Set the color of the content
     * @param color the color of the content
     * @return true if the color changed
     */
    public boolean setColor(Color color) {
	        if (!color.equals(this.color)) {
		    this.color = color;
		    COMPONENT.setForeground(color);
		    makeImage();
		    return true;
		}
		
		return false;
    }
    
    /**
     * Set the font size of the content
     * @param fontSize the font size of the content
     * @return true if the font size changed
     */
    public boolean setFontSize(float fontSize) {
	        if (this.fontSize != fontSize) {
		    this.fontSize = fontSize;
		    this.texi = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize);
		    makeImage();
		    return true;
		}
		
		return false;
    }
    

    public void makeImage() {
		width = texi.getIconWidth();
		height = texi.getIconHeight();

		if (width <= 0 || height <= 0) {
			formula = new TeXFormula("An\\ error\\ occured,\\ please\\ contact\\ the\\ author\\ of\\ \\JLaTeXMath");
			this.texi = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, 10);
			width = texi.getIconWidth();
			height = texi.getIconHeight();
		}   

		BufferedImage bimg = new BufferedImage((int) width, (int) height, BufferedImage.TYPE_INT_ARGB);
	
		Graphics2D g2d = bimg.createGraphics();
	
		AffineTransform gt = new AffineTransform();
		gt.translate(0, height);
		gt.scale(1, -1d);
		g2d.transform(gt);
	
		texi.paintIcon(COMPONENT, (Graphics) g2d, 0, 0);
	
		int[] intData = ((DataBufferInt) bimg.getRaster().getDataBuffer()).getData();
		buffer = ByteBuffer.wrap(ARGBtoRGBA(intData));
		g2d.dispose();
    }
}
