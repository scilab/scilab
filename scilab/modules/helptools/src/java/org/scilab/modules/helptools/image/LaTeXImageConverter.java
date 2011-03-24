/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools.image;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.StringReader;
import java.io.IOException;
import java.util.Map;

import javax.swing.JLabel;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.jlatexmath.ParseException;

/**
 * A LaTeX to PNG converter
 * @author Calixte DENIZET
 */
public class LaTeXImageConverter implements ExternalImageConverter {

    private static LaTeXImageConverter instance;

    private LaTeXImageConverter() { }

    /**
     * {@inheritDoc}
     */
    public String getMimeType() {
        return "image/latex";
    }

    /**
     * Since it is a singleton class...
     * @return this
     */
    public static ExternalImageConverter getInstance() {
        if (instance == null) {
             instance = new LaTeXImageConverter();
        }
        return instance;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(String latex, Map<String, String> attributes, File imageFile, String imageName) {
        TeXIcon icon = null;
        try {
            TeXFormula formula = new TeXFormula(latex);
            String display = attributes.get("style");
            int style = TeXConstants.STYLE_DISPLAY;
            if (display != null && display.equals("text")) {
                style = TeXConstants.STYLE_TEXT;
            }

            icon = formula.createTeXIcon(style, Integer.parseInt(attributes.get("fontsize")));

            Image img = new Image(icon, icon.getIconWidth(), icon.getIconHeight(), icon.getIconHeight() - icon.getIconDepth(), icon.getIconDepth());
            if (img != null && ImageConverter.convertIconToPNG(img.icon, imageFile)) {
                return ImageConverter.generateCode(img, imageName, attributes);
            }

            return null;

        } catch (ParseException e) {
            System.err.println(e);
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(File latex, Map<String, String> attributes, File imageFile, String imageName) {
        BufferedReader input = null;
        StringBuilder buffer = new StringBuilder();
        boolean ok = false;
        try {
            input = new BufferedReader(new FileReader(latex));
            String line = null;
            while ((line = input.readLine()) != null) {
                buffer.append(line);
                buffer.append("\n");
            }
            ok = true;
        } catch (IOException e) {
            System.err.println(e);
        }
        if (input != null) {
            try {
                input.close();
            } catch (IOException e) {
                System.err.println(e);
            }
        }

        if (ok) {
            return convertToImage(buffer.toString(), attributes, imageFile, imageName);
        }

        return null;
    }
}