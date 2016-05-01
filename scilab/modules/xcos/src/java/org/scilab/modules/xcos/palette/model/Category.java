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

package org.scilab.modules.xcos.palette.model;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.swing.tree.TreeNode;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>
 * A category can contain any number of child ({@link Palette} or
 * {@link Category}).
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="Category">
 *   &lt;complexContent>
 *     &lt;extension base="{}PaletteNode">
 *       &lt;sequence>
 *         &lt;element name="node" type="{}PaletteNode" maxOccurs="unbounded" minOccurs="0"/>
 *       &lt;/sequence>
 *     &lt;/extension>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlRootElement
@XmlType(name = "Category", propOrder = { "node" })
public class Category extends PaletteNode {
    private static final transient Map<String, List<PaletteNode>> SAVED_NODELIST = new HashMap<String, List<PaletteNode>>();

    @XmlElement(nillable = true)
    private List<PaletteNode> node;

    /**
     * Default constructor
     */
    public Category() {
    }

    /**
     * Gets the value of the node property.
     *
     * <p>
     * This accessor method returns a reference to the live list, not a
     * snapshot. Therefore any modification you make to the returned list will
     * be present inside the JAXB object. This is why there is not a
     * <CODE>set</CODE> method for the node property.
     *
     * <p>
     * For example, to add a new item, do as follows:
     *
     * <pre>
     * getNode().add(newItem);
     * </pre>
     *
     *
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link PaletteNode }
     *
     * @return the non-null children list.
     */
    public List<PaletteNode> getNode() {
        if (node == null) {
            node = new ArrayList<PaletteNode>();
        }
        return node;
    }

    /*
     * TreeNode method implementation
     */

    /**
     * @return the children
     * @see javax.swing.tree.TreeNode#children()
     */
    @Override
    public Enumeration<PaletteNode> children() {
        return Collections.enumeration(getNode());
    }

    /**
     * @return true
     * @see javax.swing.tree.TreeNode#getAllowsChildren()
     */
    @Override
    public boolean getAllowsChildren() {
        return true;
    }

    /**
     * @param childIndex
     *            the selected child index
     * @return the child at the specified index
     * @see javax.swing.tree.TreeNode#getChildAt(int)
     */
    @Override
    public TreeNode getChildAt(int childIndex) {
        return getNode().get(childIndex);
    }

    /**
     * @return the number of child
     * @see javax.swing.tree.TreeNode#getChildCount()
     */
    @Override
    public int getChildCount() {
        return getNode().size();
    }

    /**
     * @param node
     *            the node
     * @return the index of the node
     * @see javax.swing.tree.TreeNode#getIndex(javax.swing.tree.TreeNode)
     */
    @Override
    public int getIndex(TreeNode node) {
        return getNode().indexOf(node);
    }

    /**
     * @return false
     * @see javax.swing.tree.TreeNode#isLeaf()
     */
    @Override
    public boolean isLeaf() {
        return false;
    }

    /*
     * Customize the marshalling operation
     */

    /**
     * Invoked by Marshaller after it has created an instance of this object.
     *
     * @param m
     *            the marshaller
     */
    void beforeMarshal(Marshaller m) {
        SAVED_NODELIST.put(getName(), new ArrayList<PaletteNode>(node));

        for (final Iterator<PaletteNode> it = node.iterator(); it.hasNext();) {
            if (it.next() instanceof PreLoaded.Dynamic) {
                it.remove();
            }
        }
    }

    /**
     * Invoked by Marshaller after it has marshalled all properties of this
     * object.
     *
     * @param m
     *            the marshaller
     */
    void afterMarshal(Marshaller m) {
        node = SAVED_NODELIST.get(getName());
    }

    /**
     * Invoked by Unmarshaller after unmarshalling
     *
     * @param u
     *            the unmarshaller
     * @param parent
     *            the parent object
     */
    @Override
    void afterUnmarshal(Unmarshaller u, Object parent) {
        if (node == null) {
            return;
        }

        for (final Iterator<PaletteNode> it = node.iterator(); it.hasNext();) {
            final PaletteNode p = it.next();
            if (p instanceof Category && p.getChildCount() == 0) {
                it.remove();
            }
        }
    }
}
