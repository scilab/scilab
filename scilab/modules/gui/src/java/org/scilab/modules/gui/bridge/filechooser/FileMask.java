/*
 * Copyright (c) 2002 Sun Microsystems, Inc. All  Rights Reserved.
 * Largely modified by Sylvestre Ledru - 2009 - Digiteo
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * -Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduct the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of Sun Microsystems, Inc. or the names of contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT
 * BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
 * OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN
 * IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or intended for
 * use in the design, construction, operation or maintenance of any nuclear
 * facility.
 */



package org.scilab.modules.gui.bridge.filechooser;

import java.io.File;
import java.util.Enumeration;
import java.util.Vector;

import javax.swing.filechooser.FileFilter;

/**
 * This Class manages file mask for the graphic export filechooser
 */
public class FileMask extends FileFilter {

    private Vector<String> filters = null;
    private String description = null;
    private String fullDescription = null;

    /**
     * Create a file filter
     * If there is no filter added, display all files
     */
    public FileMask() {
        this.filters = new Vector<String>();
    }

    /**
     * Creates a file filter that accepts the given file type.
     * Ex: "jpg", "JPEG Images"
     */
    public FileMask(String extension, String description) {
        this();
        filters.add(extension.toLowerCase());
        this.description = description;
    }


    /**
     * Creates a file filter that accepts the given file type.
     * with several extensions
     * Ex: "jpg", "JPEG Images"
     */
    public FileMask(String extensions[], String description) {
        this();

        for (int i = 0; i < extensions.length; i++) {
            filters.add(extensions[i]);
        }
        this.description = description;
    }

    /**
     * Allow the file to be display
     * @param f a File
     */
    @Override
    public boolean accept(File f) {
        if (f != null) {
            if (f.isDirectory() || filters.size() == 0) {
                return true;
            }
            String extension = getExtension(f);
            if (extension != null && filters.contains(extension)) {
                return true;
            };
        }
        return false;
    }

    /**
     * Get the file's extension
     * @param f a File
     * @return the extension without the dot
     */
    public static String getExtension(File f) {
        if (f != null) {
            String filename = f.getName();
            int i = filename.lastIndexOf('.');
            if (i > 0 && i < filename.length() - 1) {
                return filename.substring(i + 1).toLowerCase();
            };
        }
        return null;
    }

    /**
     * Get the extension from the filter
     */
    public String getExtensionFromFilter() {
        if (filters != null && filters.size() > 0) {
            /* If exists, retrieve the actual extension from the filter */
            return filters.elementAt(0);
        }
        return null;
    }

    /**
     * Get description of the mask
     */
    @Override
    public String getDescription() {
        if (fullDescription == null) {
            if (description == null) {
                fullDescription = description == null ? "(" : description + " (";
                Enumeration extensions = filters.elements();
                if (extensions != null && extensions.hasMoreElements()) {
                    fullDescription += "." + (String) extensions.nextElement();
                    while (extensions.hasMoreElements()) {
                        fullDescription += ", ." + (String) extensions.nextElement();
                    }
                }
                return fullDescription + ")";
            }
        }
        return description;
    }

    public void clearExtensions() {
        filters.clear();
    }


}
