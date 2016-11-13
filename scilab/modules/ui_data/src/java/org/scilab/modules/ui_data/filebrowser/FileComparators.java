/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser;

import java.util.Comparator;

import org.scilab.modules.commons.OS;

/**
 * Several file comparators
 * @author Calixte DENIZET
 */
public final class FileComparators {

    private static final boolean isWindows = (OS.get() == OS.WINDOWS);

    public static final Comparator<FileNode> ASCENDING_ALPHA = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            if (f1.isFile == f2.isFile) {
                int diff;
                if (isWindows) {
                    diff = f1.name.compareToIgnoreCase(f2.name);
                    if (diff == 0) {
                        diff = f1.name.compareTo(f2.name);
                    }
                } else {
                    diff = f1.name.compareTo(f2.name);
                }
                return diff;
            } else if (f1.isFile) {
                return 1;
            }
            return -1;
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> DESCENDING_ALPHA = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            return -ASCENDING_ALPHA.compare(f1, f2);
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> ASCENDING_DATE = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            long diff = f1.lastModified - f2.lastModified;
            if (diff == 0) {
                return ASCENDING_ALPHA.compare(f1, f2);
            } else {
                return diff > 0 ? 1 : -1;
            }
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> DESCENDING_DATE = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            return -ASCENDING_DATE.compare(f1, f2);
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> ASCENDING_SIZE = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            long diff = f1.length - f2.length;
            if (diff == 0) {
                return ASCENDING_ALPHA.compare(f1, f2);
            } else {
                return diff > 0 ? 1 : -1;
            }
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> DESCENDING_SIZE = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            return -ASCENDING_SIZE.compare(f1, f2);
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> ASCENDING_TYPE = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            int diff;
            if (isWindows) {
                diff = f1.extension.compareToIgnoreCase(f2.extension);
                if (diff == 0) {
                    diff = f1.extension.compareTo(f2.extension);
                }
            } else {
                diff = f1.extension.compareTo(f2.extension);
            }

            if (diff == 0) {
                return ASCENDING_ALPHA.compare(f1, f2);
            }

            return diff;
        }

        public boolean equals(Object obj) {
            return false;
        }
    };

    public static final Comparator<FileNode> DESCENDING_TYPE = new Comparator<FileNode>() {
        public int compare(FileNode f1, FileNode f2) {
            return -ASCENDING_TYPE.compare(f1, f2);
        }

        public boolean equals(Object obj) {
            return false;
        }
    };


}
