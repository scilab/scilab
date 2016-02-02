/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
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

package org.scilab.modules.gui.tree;

import java.util.Vector;

import javax.swing.Icon;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.widget.Widget;


/**
 * Interface for tree used in Scilab
 * @author Sylvestre KOUMAR
 */
public interface Tree extends Widget {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleTree getAsSimpleTree();

    /**
     * Get the label of a tree node
     * @return label of a node
     */
    String getLabel();

    /**
     * Get the icon image of a tree node
     * @return icon of a node
     */
    Icon getIcon();

    /**
     * Get the callback of a tree node
     * @return callback of a node
     */
    CommonCallBack getCallback();

    /**
     * Add child to a tree
     * @param firstChild to add
     */
    void addChild(Tree firstChild);

    /**
     * Get children of a tree
     * @return vector of children
     */
    Vector<Tree> getChildren();

    /**
     * Display a tree
     */
    void showTree();

}
