package org.scilab.modules.renderer.textDrawing;

import java.io.IOException;
import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.geom.AffineTransform;

import java.nio.ByteBuffer;

import javax.xml.parsers.ParserConfigurationException;

import net.sourceforge.jeuclid.MathMLParserSupport;
import net.sourceforge.jeuclid.MutableLayoutContext;
import net.sourceforge.jeuclid.layout.JEuclidView;
import net.sourceforge.jeuclid.context.LayoutContextImpl;
import net.sourceforge.jeuclid.context.Parameter;

import org.w3c.dom.Node;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

/**
 * MathML object
 * @author Calixte Denizet
 */
public class MathMLObjectGL extends SpecialTextObjectGL {
    
    private Document doc;
    private JEuclidView jev;
    private MutableLayoutContext parameters;
    private final static Graphics2D tmpgrph = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB).createGraphics();

    private final static String mmlBeg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE math PUBLIC \"-//W3C//DTD MathML 2.0//EN\" \"http://www.w3.org/Math/DTD/mathml2/mathml2.dtd\"><math mode=\"display\" xmlns=\"http://www.w3.org/1998/Math/MathML\">";
    private final static String mmlEnd = "</math>";

    /** 
     * Default constructor.
     * @param content the MathML code
     * @param color the color of the content
     * @param fontSize the size of the font
     */
    public MathMLObjectGL(String content, Color color, float fontSize) {
	this.parameters = new LayoutContextImpl(LayoutContextImpl.getDefaultLayoutContext());
	this.parameters.setParameter(Parameter.MATHCOLOR, color);
	this.parameters.setParameter(Parameter.MATHSIZE, fontSize);
	this.jev = new JEuclidView((Node)contentToDocument(mmlBeg + content + mmlEnd), parameters, tmpgrph); 
	makeImage();
    }
        
    /**
     * Set the color of the content
     * @param color the color of the content
     */
    public void setColor(Color color) {
	if (!parameters.getParameter(Parameter.MATHCOLOR).equals(color)) {
	    parameters.setParameter(Parameter.MATHCOLOR, color);
	    update();
	}
    }
    
    /**
     * Set the font size of the content
     * @param fontSize the font size of the content
     */
    public void setFontSize(float fontSize) {
	if ((Float) parameters.getParameter(Parameter.MATHSIZE) != fontSize) {
	    parameters.setParameter(Parameter.MATHSIZE, fontSize);
	    update();
	}
    }
    
    private void update() {
	this.jev = new JEuclidView((Node)doc, parameters, tmpgrph);
	makeImage();
    }
    
    private Document contentToDocument(final String content) {
	try {
	    doc = MathMLParserSupport.parseString(content);
	} catch (final SAXException e) {
	    throw new RuntimeException(e);
	} catch (final ParserConfigurationException e) {
	    throw new RuntimeException(e);
	} catch (final IOException e) {
	    throw new RuntimeException(e);
	}
	return doc;
    }

    private void makeImage () {
	width = (int)Math.ceil(jev.getWidth()) + 2;
	int W = (int)width;
        final int ascent = (int)Math.ceil(jev.getAscentHeight());
        height = (int)Math.ceil(jev.getDescentHeight()) + ascent;
	int H = (int)height;
	
	BufferedImage bimg = new BufferedImage(W, H, BufferedImage.TYPE_INT_ARGB);
	
	Graphics2D g2d = bimg.createGraphics();
	
	AffineTransform gt = new AffineTransform();
	gt.translate (0, H);
	gt.scale (1, -1d);
	g2d.transform (gt);
	
	g2d.setColor(new Color(255, 255, 255, 0));
	g2d.fillRect(0, 0, W, H);
	
	jev.draw(g2d, 0, ascent);
	
	int[] intData = ((DataBufferInt)bimg.getRaster().getDataBuffer()).getData();
	buffer = ByteBuffer.wrap(ARGBtoRGBA(intData));
    }
}
