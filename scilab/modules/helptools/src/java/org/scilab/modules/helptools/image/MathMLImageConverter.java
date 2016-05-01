/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.helptools.image;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.xml.parsers.ParserConfigurationException;

import net.sourceforge.jeuclid.MathMLParserSupport;
import net.sourceforge.jeuclid.context.LayoutContextImpl;
import net.sourceforge.jeuclid.context.Parameter;
import net.sourceforge.jeuclid.layout.JEuclidView;

import org.scilab.modules.helptools.DocbookTagConverter;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

/**
 * A MathML to PNG converter
 * @author Calixte DENIZET
 */
public class MathMLImageConverter implements ExternalImageConverter {

    private static final Graphics2D TEMPGRAPHIC = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB).createGraphics();
    private final DocbookTagConverter conv;

    public MathMLImageConverter(DocbookTagConverter conv) {
        this.conv = conv;
    }

    /**
     * {@inheritDoc}
     */
    public String getMimeType() {
        return "image/mathml";
    }

    /**
     * {@inheritDoc}
     */
    public boolean mustRegenerate() {
        return true;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(String currentFile, String mathml, Map<String, String> attributes, File imageFile, String imageName) {
        Document doc = null;
        try {
            doc = MathMLParserSupport.parseString(mathml);
        } catch (final SAXException e) {
            return null;
        } catch (final ParserConfigurationException e) {
            return null;
        } catch (final IOException e) {
            return null;
        }

        String fontsize = attributes.get("fontsize");
        int fs = 16;
        if (fontsize != null) {
            fs = Integer.parseInt(fontsize);
        }

        Image img = convertMathML(doc, fs);
        if (img != null && ImageConverter.convertIconToPNG(img.icon, imageFile)) {
            return conv.generateImageCode(img, conv.getBaseImagePath() + imageName, attributes);
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(File mathml, Map<String, String> attributes, File imageFile, String imageName) {
        Document doc = null;
        try {
            doc = MathMLParserSupport.parseFile(mathml);
        } catch (final SAXException e) {
            return null;
        } catch (final IOException e) {
            return null;
        }

        String fontsize = attributes.get("fontsize");
        int fs = 16;
        if (fontsize != null) {
            fs = Integer.parseInt(fontsize);
        }

        Image img = convertMathML(doc, fs);
        if (img != null && ImageConverter.convertIconToPNG(img.icon, imageFile)) {
            return conv.generateImageCode(img, conv.getBaseImagePath() + imageName, attributes);
        }

        return null;
    }

    /**
     * @param mathml the document to convert
     * @param fontSize the font size
     * @return an image information
     */
    private static Image convertMathML(Document mathml, int fontSize) {
        LayoutContextImpl parameters = new LayoutContextImpl(LayoutContextImpl.getDefaultLayoutContext());
        parameters.setParameter(Parameter.MATHSIZE, fontSize);
        JEuclidView jev = new JEuclidView((Node) mathml, parameters, TEMPGRAPHIC);

        int width = (int) Math.ceil(jev.getWidth());
        int ascent = (int) Math.ceil(jev.getAscentHeight());
        int height = (int) Math.ceil(jev.getDescentHeight()) + ascent;

        if (width <= 0 || height <= 0) {
            return null;
        }

        BufferedImage bimg = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = bimg.createGraphics();
        g2d.setColor(new Color(255, 255, 255, 0));
        g2d.fillRect(0, 0, width, height);

        jev.draw(g2d, 0, ascent);
        g2d.dispose();

        return new Image(new ImageIcon(bimg), width, height, ascent, height - ascent);
    }
}
