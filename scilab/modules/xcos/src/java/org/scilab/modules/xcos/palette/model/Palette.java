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

package org.scilab.modules.xcos.palette.model;

import java.util.Enumeration;

import javax.swing.tree.TreeNode;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>
 * A Palette has a representation on the main view
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="Palette">
 *   &lt;complexContent>
 *     &lt;extension base="{}PaletteNode">
 *       &lt;attribute name="enable" use="required" type="{http://www.w3.org/2001/XMLSchema}boolean" />
 *     &lt;/extension>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Palette")
@XmlSeeAlso( { Custom.class, PreLoaded.class, PreLoaded.Dynamic.class })
public abstract class Palette extends PaletteNode {

    /*
     * TreeNode method implementation
     */

    /**
     * @return null
     * @see javax.swing.tree.TreeNode#children()
     */
    @Override
    public Enumeration<Object> children() {
        return null;
    }

    /**
     * @return false
     * @see javax.swing.tree.TreeNode#getAllowsChildren()
     */
    @Override
    public boolean getAllowsChildren() {
        return false;
    }

    /**
     * @param childIndex
     *            not used
     * @return null
     * @see javax.swing.tree.TreeNode#getChildAt(int)
     */
    @Override
    public TreeNode getChildAt(int childIndex) {
        return null;
    }

    /**
     * @return 0
     * @see javax.swing.tree.TreeNode#getChildCount()
     */
    @Override
    public int getChildCount() {
        return 0;
    }

    /**
     * @param node
     *            not used
     * @return 0
     * @see javax.swing.tree.TreeNode#getIndex(javax.swing.tree.TreeNode)
     */
    @Override
    public int getIndex(TreeNode node) {
        return 0;
    }

    /**
     * @return true
     * @see javax.swing.tree.TreeNode#isLeaf()
     */
    @Override
    public boolean isLeaf() {
        return true;
    }
}
