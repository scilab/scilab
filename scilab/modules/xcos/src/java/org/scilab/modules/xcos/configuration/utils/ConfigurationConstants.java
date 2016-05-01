/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.configuration.utils;

import java.util.Comparator;

import org.scilab.modules.xcos.configuration.model.DocumentType;

/**
 * Contains useful methods and
 */
public final class ConfigurationConstants {

    /**
     * Compare {@link DocumentType} per date.
     */
    public static final Comparator<DocumentType> DATE_COMPARATOR = new Comparator<DocumentType>() {

        @Override
        public int compare(DocumentType o1, DocumentType o2) {
            return o1.getDate().compare(o2.getDate());
        }
    };

    /**
     * Compare {@link DocumentType} per file name.
     */
    public static final Comparator<DocumentType> FILENAME_COMPARATOR = new Comparator<DocumentType>() {

        @Override
        public int compare(DocumentType o1, DocumentType o2) {
            return o1.getUrl().compareTo(o2.getUrl());
        }
    };

    /*
     * Properties applicable to the ConfigurationManager change support
     */

    /**
     * Property fired when the configuration recent file list has changed.
     */
    public static final String RECENT_FILES_CHANGED = "recentFilesHasChanged";

    /**
     * This class is static
     */
    private ConfigurationConstants() {
    }
}
