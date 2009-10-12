package org.scilab.modules.renderer.textDrawing;

import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Component;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Insets;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.geom.AffineTransform;

import java.nio.ByteBuffer;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.jlatexmath.ParseException;

/**
 * LaTeX object
 * @author Calixte Denizet
 */
public class TeXObjectGL extends SpecialTextObjectGL {
        
    private TeXIcon texi;
    private TeXFormula formula;
    private final static Component c = (Component)new Canvas();

    /** 
     * Default constructor.
     * @param content the LaTeX code
     * @param color the color of the content
     * @param fontSize the size of the font
     */
    public TeXObjectGL(String content, Color color, float fontSize) throws RuntimeException {
	if (content.endsWith("$"))
	    content = content.substring(1, content.length()-1);
	else
	    throw new RuntimeException();
	
	try {
	    formula = new TeXFormula(content);
	} catch (ParseException e) {
	    throw new RuntimeException();
	}
 
	formula.setBackground(new Color(255, 255, 255, 0));
	this.texi = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize + 2);
	setColor(color);
	makeImage();
    }
        
    /**
     * Set the color of the content
     * @param color the color of the content
     */
    public void setColor(Color color) {
	c.setForeground(color);
    }
    
    /**
     * Set the font size of the content
     * @param fontSize the font size of the content
     */
    public void setFontSize(float fontSize) {
	this.texi = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize + 2);
	makeImage();
    }
        
    private void makeImage () {
	texi.setInsets(new Insets(1, 1, 1, 1));
	width = texi.getIconWidth();
	height = texi.getIconHeight();

	if (width <= 0 || height <= 0) {
	    formula = new TeXFormula("An\\ error\\ occured,\\ please\\ contact\\ the\\ author\\ of\\ J\\LaTeX Math");
	    this.texi = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, 10);
	    texi.setInsets(new Insets(1, 1, 1, 1));
	    width = texi.getIconWidth();
	    height = texi.getIconHeight();
	}   

	BufferedImage bimg = new BufferedImage((int)width, (int)height, BufferedImage.TYPE_INT_ARGB);
	
	Graphics2D g2d = bimg.createGraphics();
	
	AffineTransform gt = new AffineTransform();
	gt.translate (0, height);
	gt.scale (1, -1d);
	g2d.transform (gt);
	
	texi.paintIcon(c, (Graphics)g2d, 0, 0);
	
	int[] intData = ((DataBufferInt)bimg.getRaster().getDataBuffer()).getData();
	buffer = ByteBuffer.wrap(ARGBtoRGBA(intData));
    }
}
