/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.ImageIcon;

import org.scilab.modules.gui.utils.ScilabSwingUtilities;

/**
 * Few file tools
 *
 * @author Calixte DENIZET
 */
public final class FileUtils {

    private static final Icon SCILABICON = new ImageIcon(ScilabSwingUtilities.findIcon("scilab"));
    private static final Icon PDFICON = new ImageIcon(ScilabSwingUtilities.findIcon("application-pdf"));
    private static final Icon BINARYICON = new ImageIcon(ScilabSwingUtilities.findIcon("binary"));
    private static final Icon IMAGEICON = new ImageIcon(ScilabSwingUtilities.findIcon("image-x-generic"));
    private static final Icon TEXTICON = new ImageIcon(ScilabSwingUtilities.findIcon("text-x-generic"));
    private static final Icon HTMLICON = new ImageIcon(ScilabSwingUtilities.findIcon("text-html"));
    private static final Icon XMLICON = new ImageIcon(ScilabSwingUtilities.findIcon("text-xml"));
    private static final Icon CSVICON = new ImageIcon(ScilabSwingUtilities.findIcon("text-csv"));
    private static final Icon DIRICON = new ImageIcon(ScilabSwingUtilities.findIcon("folder"));
    private static final Icon OPENDIRICON = new ImageIcon(ScilabSwingUtilities.findIcon("folder-open"));
    private static final Icon READONLYICON = new ImageIcon(ScilabSwingUtilities.findIcon("emblem-readonly"));
    private static final Icon CLOSEDUSERHOMEICON = new ImageIcon(ScilabSwingUtilities.findIcon("user-home"));
    private static final Icon OPENUSERHOMEICON = new ImageIcon(ScilabSwingUtilities.findIcon("user-home-open"));
    private static final Icon UPICON = new ImageIcon(ScilabSwingUtilities.findIcon("dynamic-blue-up"));
    private static final Icon SCIICON = new ImageIcon(ScilabSwingUtilities.findIcon("scilab-folder"));
    private static final Icon OPENSCIICON = new ImageIcon(ScilabSwingUtilities.findIcon("scilab-folder-open"));

    private static final int CHARTOREAD = 128;

    private static final Map<String, Icon> extToIcon = new HashMap<String, Icon>();

    static {
        extToIcon.put("sce", SCILABICON);
        extToIcon.put("sci", SCILABICON);
        extToIcon.put("scg", SCILABICON);
        extToIcon.put("cos", SCILABICON);
        extToIcon.put("cosf", SCILABICON);
        extToIcon.put("xcos", SCILABICON);
        extToIcon.put("zcos", SCILABICON);
        extToIcon.put("bin", SCILABICON);
        extToIcon.put("sod", SCILABICON);
        extToIcon.put("dem", SCILABICON);
        extToIcon.put("graph", SCILABICON);
        extToIcon.put("sav", SCILABICON);
        extToIcon.put("tst", SCILABICON);
        extToIcon.put("pdf", PDFICON);
        extToIcon.put("png", IMAGEICON);
        extToIcon.put("gif", IMAGEICON);
        extToIcon.put("jpg", IMAGEICON);
        extToIcon.put("jpeg", IMAGEICON);
        extToIcon.put("html", HTMLICON);
        extToIcon.put("htm", HTMLICON);
        extToIcon.put("xml", XMLICON);
        extToIcon.put("csv", CSVICON);
    }

    /**
     * @param f
     *            the file
     * @return the extension of f
     */
    public static String getFileExtension(File f) {
        if (f.isFile()) {
            String name = f.getName();
            int pos = name.lastIndexOf(".");
            if (pos != -1) {
                return name.substring(pos + 1, name.length());
            }
        }

        return "";
    }

    /**
     * @param file
     *            the file to test
     * @return true if it is a binary file
     */
    public static boolean isBinaryFile(File f) {
        if (f.isFile() && f.canRead()) {
            try {
                BufferedReader reader = new BufferedReader(new FileReader(f));
                char[] buffer = new char[CHARTOREAD];
                int len = reader.read(buffer, 0, CHARTOREAD);
                reader.close();
                int i = 0;
                if (len != -1) {
                    for (; i < len && buffer[i] != '\0'; i++)
                        ;
                }

                return len != -1 && i != len;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return false;
    }

    /**
     * @return the up icon
     */
    public static Icon getUpDirIcon() {
        return UPICON;
    }

    /**
     * @return the SCI icon
     */
    public static Icon getSCIIcon() {
        return SCIICON;
    }

    /**
     * @return the open SCI icon
     */
    public static Icon getOpenSCIIcon() {
        return OPENSCIICON;
    }

    /**
     * @return the open dir icon
     */
    public static Icon getOpenDirIcon() {
        return OPENDIRICON;
    }

    /**
     * @return the closed dir icon
     */
    public static Icon getClosedDirIcon() {
        return DIRICON;
    }

    /**
     * @return the closed user-home dir icon
     */
    public static Icon getClosedUserHomeIcon() {
        return CLOSEDUSERHOMEICON;
    }

    /**
     * @return the open user-home dir icon
     */
    public static Icon getOpenUserHomeIcon() {
        return OPENUSERHOMEICON;
    }

    /**
     * @param f
     *            the file
     * @return an icon according to its extension
     */
    public static Icon getIconForFile(File f) {
        if (f.isDirectory()) {
            return DIRICON;
        }

        if (f.canRead() && !f.canWrite()) {
            return READONLYICON;
        }

        String ext = getFileExtension(f).toLowerCase();
        Icon icon = extToIcon.get(ext);
        if (icon != null) {
            return icon;
        }

        if (f.isFile() && isBinaryFile(f)) {
            return BINARYICON;
        }

        return TEXTICON;
    }
}
