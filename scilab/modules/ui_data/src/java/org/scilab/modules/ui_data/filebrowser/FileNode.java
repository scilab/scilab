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

import java.io.File;
import java.util.Comparator;
import java.util.TreeSet;
import java.util.regex.Pattern;

import javax.swing.Icon;
import javax.swing.SortOrder;

import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.commons.ScilabConstants;
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

    private static final File userHomeDir = new File(ScilabConstants.USERHOME);
    private static final File SCIDIR = new File(System.getenv("SCI"));

    private static Comparator<FileNode> comparator = FileComparators.ASCENDING_ALPHA;

    protected File file;
    protected Icon icon;
    protected FileNode[] children;
    protected long lastModified;
    protected boolean isFile;
    protected boolean canRead;
    protected boolean canWrite;
    protected boolean isEmpty;
    protected boolean userHome;
    protected boolean sci;
    protected long length;
    protected String name;
    protected String extension;
    private int order = 1;
    private Pattern pat;
    private int position;

    /**
     * Default constructor
     * @param file the file in this node
     */
    public FileNode(File file, int position) {
        this(file, file.getName(), file.canRead(), file.isFile());
        this.position = position;
    }

    public FileNode(File file, String name, boolean canRead, boolean isFile) {
        this.file = file;
        this.name = name;
        this.isFile = isFile;
        this.canRead = canRead;
        this.canWrite = file.canWrite();
        this.lastModified = file.lastModified();
        this.userHome = file.equals(userHomeDir);
        if (isFile) {
            this.length = file.length();
        }
        this.extension = FileUtils.getFileExtension(file);
        this.sci = file.equals(SCIDIR);
        this.isEmpty = isFile || CommonFileUtils.isEmptyDirectory(file.getAbsolutePath()) == 1;
    }

    public int getPosition() {
        return position;
    }

    /**
     * @param pat the pattern to filter the files
     */
    public void setFilter(Pattern pat) {
        this.pat = pat;
        resetChildren();
    }

    /**
     * @param order the order to use to sort the file. If order is positive, then ascending order is used.
     */
    public void setOrder(int order) {
        this.order = order;
        switch (order) {
            case NAMEORDER:
                comparator = FileComparators.ASCENDING_ALPHA;
                break;
            case -NAMEORDER:
                comparator = FileComparators.DESCENDING_ALPHA;
                break;
            case TYPEORDER:
                comparator = FileComparators.ASCENDING_TYPE;
                break;
            case -TYPEORDER:
                comparator = FileComparators.DESCENDING_TYPE;
                break;
            case DATEORDER:
                comparator = FileComparators.ASCENDING_DATE;
                break;
            case -DATEORDER:
                comparator = FileComparators.DESCENDING_DATE;
                break;
            case SIZEORDER:
                comparator = FileComparators.ASCENDING_SIZE;
                break;
            case -SIZEORDER:
                comparator = FileComparators.DESCENDING_SIZE;
                break;
        }
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
    protected void orderFiles() {
        if (children != null) {
            TreeSet<FileNode> set = new TreeSet<FileNode>(comparator);
            for (FileNode fn : children) {
                set.add(fn);
            }

            children = set.toArray(children);
            for (int i = 0; i < children.length; i++) {
                children[i].position = i;
            }
        }
    }

    /**
     * Returns the string to be used to display this leaf in the JTree.
     */
    public String toString() {
        String name = file.getName();
        if (name.isEmpty()) {
            name = file.toString();
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
        if (children == null && !isEmpty) {
            synchronized (file) {
                if (children == null) {
                    children = listFiles();
                }
            }
        }

        if (children != null) {
            return children.length;
        }

        return 0;
    }

    /**
     * @return the children FileNode of this FileNode
     */
    protected Object[] getChildren() {
        if (children == null && !isEmpty) {
            children = listFiles();
        }

        return children;
    }

    public FileNode[] listFiles() {
        String[] filesName = file.list();
        if (filesName != null) {
            TreeSet<FileNode> nodes = new TreeSet<FileNode>(comparator);
            for (String fileName : filesName) {
                File f = new File(file, fileName);
                if (pat != null && !pat.matcher(fileName).matches()) {
                    continue;
                }
                if (f.isHidden()) {
                    continue;
                }
                boolean canRead = f.canRead();
                if (!canRead) {
                    continue;
                }
                boolean isFile = f.isFile();
                if (!isFile && !f.isDirectory()) {
                    continue;
                }
                nodes.add(new FileNode(f, fileName, canRead, isFile));
            }

            FileNode[] fnodes = new FileNode[nodes.size()];
            fnodes = nodes.toArray(fnodes);

            for (int i = 0; i < fnodes.length; i++) {
                fnodes[i].position = i;
            }

            return fnodes;
        }

        return null;
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
}
