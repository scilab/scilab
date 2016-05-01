/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2014 Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.logging.Logger;

import org.scilab.modules.commons.ScilabConstants;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.util.mxUtils;
import com.mxgraph.util.mxXmlUtils;
import com.mxgraph.view.mxStylesheet;

/**
 * Contains useful method for managing files.
 */
public final class FileUtils {
    /**
     * Name of the xcos style configuration file.
     */
    public static final String STYLE_FILENAME = "Xcos-style.xml";

    /**
     * Default constructor
     */
    private FileUtils() {
    }

    /**
     * Copy in to out
     *
     * @param in
     *            the input file
     * @param out
     *            the output file
     * @throws IOException
     *             when an errors has occurred
     */
    public static void copy(File in, File out) throws IOException {
        FileInputStream fis = new FileInputStream(in);
        FileOutputStream fos = new FileOutputStream(out);
        FileChannel inChannel = fis.getChannel();
        FileChannel outChannel = fos.getChannel();

        try {
            inChannel.transferTo(0, inChannel.size(), outChannel);
        } catch (IOException e) {
            Logger.getLogger(FileUtils.class.getName()).warning(e.toString());
            throw e;
        } finally {
            inChannel.close();
            fis.close();

            outChannel.close();
            fos.close();
        }
    }

    /**
     * Force the copy from in to out
     *
     * @param in
     *            the input file
     * @param out
     *            the output file
     */
    public static void forceCopy(File in, File out) {
        FileChannel inChannel = null;
        FileChannel outChannel = null;

        if (!out.exists()) {
            try {
                out.createNewFile();
            } catch (IOException e) {
                Logger.getLogger(FileUtils.class.getName()).warning(e.toString());
            }
        }


        try {
            FileInputStream fis = new FileInputStream(in);
            FileOutputStream fos = new FileOutputStream(out);
            inChannel = fis.getChannel();
            outChannel = fos.getChannel();
            inChannel.transferTo(0, inChannel.size(), outChannel);
            fis.close();
            fos.close();
        } catch (IOException e) {
            Logger.getLogger(FileUtils.class.getName()).warning(e.toString());
        } finally {
            if (inChannel != null) {
                try {
                    inChannel.close();
                } catch (IOException e) {
                    Logger.getLogger(FileUtils.class.getName()).warning(e.toString());
                }
            }
            if (outChannel != null) {
                try {
                    outChannel.close();
                } catch (IOException e) {
                    Logger.getLogger(FileUtils.class.getName()).warning(e.toString());
                }
            }
        }
    }

    /**
     * Delete the file and log an error message if unable to do so.
     *
     * @param f
     *            the file to delete.
     */
    public static void delete(File f) {
        if (!f.delete()) {
            Logger.getLogger(FileUtils.class.getName()).severe(XcosMessages.UNABLE_TO_DELETE + f);
        }
    }

    /**
     * Delete the file and log an error message if unable to do so.
     *
     * @param f
     *            the file to delete.
     */
    public static void delete(String f) {
        if (!new File(f).delete()) {
            Logger.getLogger(FileUtils.class.getName()).severe(XcosMessages.UNABLE_TO_DELETE + f);
        }
    }

    /**
     * Test if the file exists.
     *
     * @param f
     *            the file check.
     * @return true if the file exists, false otherwise.
     */
    public static boolean exists(String f) {
        return new File(f).exists();
    }

    /**
     * Decode the style into the passed stylesheet.
     *
     * @param styleSheet
     *            the current stylesheet
     * @throws IOException
     *             on I/O errors
     */
    public static void decodeStyle(final mxStylesheet styleSheet) throws IOException {
        /*
         * Initialize constants
         */
        final String sciPath = ScilabConstants.SCI.getAbsolutePath();
        File baseStyleSheet = new File(sciPath + "/modules/xcos/etc/" + STYLE_FILENAME);

        final String homePath = ScilabConstants.SCIHOME.getAbsolutePath();
        final File userStyleSheet = new File(homePath + '/' + STYLE_FILENAME);

        final String sciURL = ScilabConstants.SCI.toURI().toURL().toString();
        final String homeURL = ScilabConstants.SCIHOME.toURI().toURL().toString();

        String xml;
        Document document;

        /*
         * Decode the base stylesheet
         */
        xml = mxUtils.readFile(baseStyleSheet.getAbsolutePath());
        xml = xml.replaceAll("\\$SCILAB", sciURL);
        xml = xml.replaceAll("\\$SCIHOME", homeURL);
        document = mxXmlUtils.parseXml(xml);
        new mxCodec().decode(document.getDocumentElement(), styleSheet);

        /*
         * Decode the user stylesheet if it exists
         */
        if (userStyleSheet.exists()) {
            xml = mxUtils.readFile(userStyleSheet.getAbsolutePath());
            xml = xml.replaceAll("\\$SCILAB", sciURL);
            xml = xml.replaceAll("\\$SCIHOME", homeURL);
            document = mxXmlUtils.parseXml(xml);
            new mxCodec().decode(document.getDocumentElement(), styleSheet);
        }
    }

    /**
     * Export an HTML label String to a valid C identifier String.
     *
     * @param label
     *            the HTML label
     * @return a valid C identifier String
     */
    public static String toValidCIdentifier(final String label) {
        final String text = mxUtils.getBodyMarkup(label, true);
        final StringBuilder cFunctionName = new StringBuilder();

        for (int i = 0; i < text.length(); i++) {
            final char ch = text.charAt(i);

            // Adding upper case chars
            if (ch >= 'A' && ch <= 'Z') {
                cFunctionName.append(ch);
            } else

                // Adding lower case chars
                if (ch >= 'a' && ch <= 'z') {
                    cFunctionName.append(ch);
                } else

                    // Adding number chars
                    if (ch >= '0' && ch <= '9') {
                        // do not append any first number
                        if (cFunctionName.length() > 0) {
                            cFunctionName.append(ch);
                        }
                    } else

                        // Specific chars
                        if (ch == '_' || ch == ' ') {
                            cFunctionName.append('_');
                        }
        }
        return cFunctionName.toString();
    }
}
