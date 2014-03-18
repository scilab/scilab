/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2014 Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import com.mxgraph.io.mxCodec;
import com.mxgraph.util.mxUtils;
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
        FileChannel inChannel = new FileInputStream(in).getChannel();
        FileChannel outChannel = new FileOutputStream(out).getChannel();
        try {
            inChannel.transferTo(0, inChannel.size(), outChannel);
        } catch (IOException e) {
            Logger.getLogger(FileUtils.class.getName()).warning(e.toString());
            throw e;
        } finally {
            if (inChannel != null) {
                inChannel.close();
            }
            if (outChannel != null) {
                outChannel.close();
            }
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
            inChannel = new FileInputStream(in).getChannel();
            outChannel = new FileOutputStream(out).getChannel();
            inChannel.transferTo(0, inChannel.size(), outChannel);
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
        document = mxUtils.parseXml(xml);
        new mxCodec().decode(document.getDocumentElement(), styleSheet);

        /*
         * Decode the user stylesheet if it exists
         */
        if (userStyleSheet.exists()) {
            xml = mxUtils.readFile(userStyleSheet.getAbsolutePath());
            xml = xml.replaceAll("\\$SCILAB", sciURL);
            xml = xml.replaceAll("\\$SCIHOME", homeURL);
            document = mxUtils.parseXml(xml);
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
