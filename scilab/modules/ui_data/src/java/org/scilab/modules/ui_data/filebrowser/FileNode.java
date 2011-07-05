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

import java.io.File;
import java.io.FileFilter;
import java.util.Arrays;
import java.util.Comparator;
import java.util.regex.Pattern;

import javax.swing.Icon;
import javax.swing.SortOrder;

import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.core.Scilab;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Handle a File as a node in the JTree
 * @author Calixte DENIZET
 */
public class FileNode {

    public static final int NAMEORDER = 1;
    public static final int SIZEORDER = 2;
    public static final int DATEORDER = 3;
    public static final int TYPEORDER = 4;

    private final FileFilter FILTER = new FileFilter() {
            public boolean accept(File f) {
                return !f.isHidden() && f.canRead() && (f.isFile() || f.isDirectory()) && (pat == null || pat.matcher(f.getName()).matches());
            }
        };

    private static final File userHomeDir = new File(ScilabConstants.USERHOME);
    private static final File SCIDIR = new File(System.getenv("SCI"));
    private static boolean isWindows = Scilab.isWindowsPlateform();

    protected File file;
    protected Icon icon;
    protected Object[] children;
    protected File[] childrenFiles;
    protected long lastModified;
    protected boolean isFile;
    protected boolean canRead;
    protected boolean canWrite;
    protected boolean isEmpty;
    private boolean userHome;
    private boolean sci;
    private int order = 1;
    private Pattern pat;
    private int position;

    /**
     * Default constructor
     * @param file the file in this node
     */
    public FileNode(File file, int position) {
        this.file = file;
        this.position = position;
        init();
    }

    public int getPosition() {
        return position;
    }

    /**
     * @param pat the pattern to filter the files
     */
    public void setFilter(Pattern pat) {
        this.pat = pat;
        reset();
    }

    /**
     * @param order the order to use to sort the file. If order is positive, then ascending order is used.
     */
    public void setOrder(int order) {
        this.order = order;
    }

    /**
     * @param order the order to use to sort the file.
     * @param ascending if true, the ascending order is used
     */
    public void setOrder(int order, boolean ascending) {
        int sign = ascending ? +1 : -1;
        setOrder(sign * order);
    }

    /**
     * @return the used order
     */
    public SortOrder getOrder() {
        return order > 0 ? SortOrder.ASCENDING : SortOrder.DESCENDING;
    }

    /**
     * Sort a column according to the natural order for its.
     * @param nameColumn the column name
     */
    public void toggleSortOrder(String nameColumn) {
        if (nameColumn.equals(UiDataMessages.NAME_COLUMN)) {
            if (Math.abs(order) == NAMEORDER) {
                setOrder(-order);
            } else {
                setOrder(NAMEORDER);
            }
        } else if (nameColumn.equals(UiDataMessages.TYPE_COLUMN)) {
            if (Math.abs(order) == TYPEORDER) {
                setOrder(-order);
            } else {
                setOrder(TYPEORDER);
            }
        } else if (nameColumn.equals(UiDataMessages.SIZE_COLUMN)) {
            if (Math.abs(order) == SIZEORDER) {
                setOrder(-order);
            } else {
                setOrder(SIZEORDER);
            }
        } else if (nameColumn.equals(UiDataMessages.LASTMODIF_COLUMN)) {
            if (Math.abs(order) == DATEORDER) {
                setOrder(-order);
            } else {
                setOrder(DATEORDER);
            }
        }
    }

    /**
     * Sort the files
     * @param order the order to use
     * @param files the files to order
     * @return the ordered FileNodes
     */
    protected static FileNode[] orderFiles(int order, File[] files) {
        final Comparator alpha;
        if (isWindows) {
            alpha = new Comparator<File>() {
                public int compare(File f1, File f2) {
                    if ((f1.isFile() && f2.isFile()) || (f1.isDirectory() && f2.isDirectory())) {
                        int diff = f1.getName().compareToIgnoreCase(f2.getName());
                        if (diff == 0) {
                            diff = f1.getName().compareTo(f2.getName());
                        }
                        return diff;
                    } else if (f1.isFile()) {
                        return 1;
                    }
                    return -1;
                }

                public boolean equals(Object obj) {
                    return false;
                }
            };
        } else {
            alpha = new Comparator<File>() {
                public int compare(File f1, File f2) {
                    if ((f1.isFile() && f2.isFile()) || (f1.isDirectory() && f2.isDirectory())) {
                        return f1.getName().compareTo(f2.getName());
                    } else if (f1.isFile()) {
                        return 1;
                    }
                    return -1;
                }

                public boolean equals(Object obj) {
                    return false;
                }
            };
        }

        switch (Math.abs(order)) {
        case SIZEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        int diff = new Long(f1.length()).compareTo(f2.length());
                        if (diff == 0 || f1.isDirectory() || f2.isDirectory()) {
                            return alpha.compare(f1, f2);
                        } else {
                            return diff;
                        }
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            break;
        case DATEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        int diff = new Long(f1.lastModified()).compareTo(f2.lastModified());
                        if (diff == 0) {
                            return alpha.compare(f1, f2);
                        } else {
                            return diff;
                        }
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            break;
        case TYPEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        int diff = FileUtils.getFileExtension(f1).compareTo(FileUtils.getFileExtension(f2));
                        if (diff == 0) {
                            return alpha.compare(f1, f2);
                        } else {
                            return diff;
                        }
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            break;
        default :
            Arrays.sort(files, alpha);
        }

        FileNode[] nodes = new FileNode[files.length];
        if (order > 0) {
            for (int i = 0; i < files.length; i++) {
                nodes[i] = new FileNode(files[i], i);
            }
        } else {
            for (int i = 0; i < files.length; i++) {
                nodes[i] = new FileNode(files[files.length - 1 - i], i);
            }
        }

        return nodes;
    }

    /**
     * Returns the the string to be used to display this leaf in the JTree.
     */
    public String toString() {
        String name = file.getName();
        if (name.isEmpty()) {
            name = "/";
        }
        return name;
    }

    /**
     * @return the file associated with this node
     */
    public File getFile() {
        return file;
    }

    /**
     * @return true if this represents the user-home directory
     */
    public boolean isUserHome() {
        return userHome;
    }

    /**
     * @return true if this represents the SCI directory
     */
    public boolean isSCI() {
        return sci;
    }

    /**
     * @return the last modified time for this file
     */
    public long getLastModified() {
        return lastModified;
    }

    /**
     * @return the icon associated with this file
     */
    public Icon getIcon() {
        if (icon == null) {
            icon = FileUtils.getIconForFile(file);
        }

        return icon;
    }

    /**
     * @return true if the file is not a directory or if it is an empty one
     */
    public boolean isLeaf() {
        return isEmpty;
    }

    /**
     * @return the number of files in the directory representated by this file
     */
    public int getChildrenCount() {
        if (childrenFiles == null && !isEmpty) {
            synchronized(file) {
                if (childrenFiles == null) {
                    childrenFiles = file.listFiles(FILTER);
                }
            }
        }

        if (childrenFiles != null) {
            return childrenFiles.length;
        }

        return 0;
    }

    /**
     * @return the children FileNode of this FileNode
     */
    protected Object[] getChildren() {
        if (children == null && !isEmpty) {
            try {
                children = orderFiles(order, childrenFiles);
            } catch (SecurityException se) { }
        }

        return children;
    }

    /**
     * Reset this FileNode
     */
    public void reset() {
        children = null;
        icon = null;
        childrenFiles = null;
        init();
    }

    /**
     * Reset children only
     */
    public void resetChildren() {
        children = null;
    }

    /**
     * {@inheritDoc}
     */
    public boolean equals(Object o) {
        return (o instanceof FileNode) && ((FileNode) o).file.equals(file);
    }

    /**
     * {@inheritDoc}
     */
    public int hashCode() {
        return file.hashCode();
    }

    /**
     * Init
     */
    private void init() {
        isFile = file.isFile();
        lastModified = file.lastModified();
        canRead = file.canRead();
        canWrite = file.canWrite();
        userHome = file.equals(userHomeDir);
        sci = file.equals(SCIDIR);
        isEmpty = isFile || CommonFileUtils.isEmptyDirectory(file.getAbsolutePath()) == 1;
    }
}
