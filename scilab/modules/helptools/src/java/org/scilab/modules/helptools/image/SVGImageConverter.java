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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.io.StringReader;
import java.util.Map;

import org.apache.batik.transcoder.TranscoderInput;
import org.apache.batik.transcoder.TranscoderOutput;
import org.apache.batik.transcoder.Transcoder;
import org.apache.batik.transcoder.image.ImageTranscoder;
import org.apache.batik.transcoder.image.PNGTranscoder;
import org.scilab.modules.helptools.DocbookTagConverter;

/**
 * SVG to PNG converter
 */
public class SVGImageConverter implements ExternalImageConverter {
    private final DocbookTagConverter conv;

    public SVGImageConverter(DocbookTagConverter conv) {
        this.conv = conv;
    }

    /**
     * {@inheritDoc}
     */
    public String getMimeType() {
        return "image/svg";
    }

    /**
     * {@inheritDoc}
     */
    public boolean mustRegenerate() {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(String currentFile, String svg, Map<String, String> attributes, File imageFile, String imageName) {
        return convertToPNG(new TranscoderInput(new StringReader(svg)), attributes, imageFile, imageName);
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(File svg, Map<String, String> attributes, File imageFile, String imageName) {
        try {
            return convertToPNG(new TranscoderInput(new FileInputStream(svg)), attributes, imageFile, conv.getBaseImagePath() + imageName);
        } catch (FileNotFoundException e) {
            System.err.println("Problem when exporting SVG to " + imageFile + "!\n" + e.toString());
        }

        return null;
    }

    /**
     * Make really the conversion from svg to png
     */
    private String convertToPNG(TranscoderInput input, Map<String, String> attributes, File imageFile, String imageName) {
        Transcoder trans = new PNGTranscoder();
        trans.addTranscodingHint(ImageTranscoder.KEY_FORCE_TRANSPARENT_WHITE, Boolean.TRUE);

        try {
            OutputStream os = new FileOutputStream(imageFile);
            TranscoderOutput output = new TranscoderOutput(os);
            trans.transcode(input, output);
            os.flush();
            os.close();
            return conv.generateImageCode(conv.getBaseImagePath() + imageName, attributes);
        } catch (Exception e) {
            System.err.println("Problem when exporting SVG to " + imageFile + "!\n" + e.toString());
        }

        return null;
    }
}
