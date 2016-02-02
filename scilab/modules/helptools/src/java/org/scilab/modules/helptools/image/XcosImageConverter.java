/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clément David
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
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Map;

import org.scilab.modules.helptools.DocbookTagConverter;

/**
 * Xcos schema to PNG converter
 */
public class XcosImageConverter implements ExternalImageConverter {
    private final DocbookTagConverter conv;

    public XcosImageConverter(DocbookTagConverter conv) {
        this.conv = conv;
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
     * {@inheritDoc}
     *
     * Redirect to the scilab code implementation.
     */
    @Override
    public String convertToImage(String currentFile, String code, Map<String, String> attributes, File imageFile, String imageName) {
        return conv.getImageConverter().getScilabImageConverter().convertToImage(currentFile, code, attributes, imageFile, imageName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String convertToImage(File schema, Map<String, String> attributes, File imageFile, String imageName) {
        try {
            final String name = schema.getName();
            return convertToPNG(name, schema.getAbsolutePath(), attributes, imageFile, imageName);
        } catch (Exception e) {
            e.printStackTrace();
            System.err.printf("Problem when exporting Xcos schema %s !\n%s\n", schema, e.toString());
        }

        return null;
    }

    private final String convertToPNG(final String helpID, final String xcosFile, Map<String, String> attributes, final File imageFile, final String imageName) throws Exception {
        /*
         * use a delegate method to avoid a static dependency
         */
        final Class<?> export = Class.forName("org.scilab.modules.xcos.utils.XcosDelegates");
        final Method convertToPNG = export.getDeclaredMethod("convertToPNG", String.class, String.class, File.class, String.class);
        try {
            convertToPNG.invoke(null, helpID, xcosFile, imageFile, imageName);
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
        }

        return conv.generateImageCode(xcosFile, conv.getBaseImagePath() + imageName, attributes);
    }
}
