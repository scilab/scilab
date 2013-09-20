/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
