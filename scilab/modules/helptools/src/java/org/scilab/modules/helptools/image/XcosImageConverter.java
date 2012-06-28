/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clément David
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools.image;

import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Map;

import javax.imageio.ImageIO;

import org.scilab.modules.helptools.HTMLDocbookTagConverter;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;

/**
 * Xcos schema to PNG converter
 */
public class XcosImageConverter implements ExternalImageConverter {

    private static XcosImageConverter instance;
    private final HTMLDocbookTagConverter.GenerationType type;

    private XcosImageConverter(HTMLDocbookTagConverter.GenerationType type) {
        this.type = type;
    }

    @Override
    public String getMimeType() {
        return "image/scilab-xcos";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean mustRegenerate() {
        return false;
    }

    /**
     * Since this a singleton class...
     *
     * @return this
     */
    public static XcosImageConverter getInstance(HTMLDocbookTagConverter.GenerationType type) {
        if (instance == null) {
            instance = new XcosImageConverter(type);
        }

        return instance;
    }

    public static XcosImageConverter getInstance() {
        return instance;
    }

    /**
     * {@inheritDoc}
     *
     * Redirect to the scilab code implementation.
     */
    @Override
    public String convertToImage(String currentFile, String code, Map<String, String> attributes, File imageFile, String imageName) {
        return ScilabImageConverter.getInstance().convertToImage(currentFile, code, attributes, imageFile, imageName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String convertToImage(File schema, Map<String, String> attributes, File imageFile, String imageName) {
        try {
            final String name = schema.getName();
            return convertToPNG(name, schema.getAbsolutePath(), imageFile, imageName);
        } catch (Exception e) {
            e.printStackTrace();
            System.err.printf("Problem when exporting Xcos schema %s !\n%s\n", schema, e.toString());
        }

        return null;
    }

    private final String convertToPNG(final String helpID, final String xcosFile, final File imageFile, final String imageName) throws Exception {
        final XcosDiagram diag = new XcosDiagram();
        diag.installListeners();

        final XcosFileType filetype = XcosFileType.findFileType(xcosFile);
        filetype.load(xcosFile, diag);

        final mxGraphComponent graphComponent = diag.getAsComponent();

        final BufferedImage image = mxCellRenderer.createBufferedImage(diag, null, 1, null, graphComponent.isAntiAlias(), null, graphComponent.getCanvas());
        ImageIO.write(image, "png", imageFile);

        return getHTMLCodeToReturn(helpID, "<img src=\'" + imageName + "\'/>");
    }

    public String getHTMLCodeToReturn(String filename, String imageTag) {
        if (type == HTMLDocbookTagConverter.GenerationType.WEB) {
            /* Provide a tooltip */
            return "<div rel='tooltip' title='" + filename + "'>" + imageTag + "</div>";
        } else {
            /*
             * No tooltip in the javahelp browser ... too limited html
             * capabilities
             */
            return imageTag;
        }
    }
}
