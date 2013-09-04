/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
