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
import java.util.logging.Logger;

import javax.swing.JTree;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlTransient;
import javax.xml.bind.annotation.XmlType;

import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;

/**
 * <p>
 * Java class for PaletteNode complex type.
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="PaletteNode">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="name" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PaletteNode")
@XmlSeeAlso( { Category.class, Palette.class })
public abstract class PaletteNode implements TreeNode {
    private static final Logger LOG = Logger.getLogger(PaletteNode.class.getName());

    @XmlAttribute(required = true)
    private String name;
    @XmlAttribute(required = true)
    private boolean enable;
    @XmlTransient
    private Category parent;

    /*
     * Getters and Setters
     */

    /**
     * Gets the value of the name property.
     *
     * @return possible object is {@link String }
     *
     */
    public String getName() {
        return name;
    }

    /**
     * Sets the value of the name property.
     *
     * @param value
     *            allowed object is {@link String }
     *
     */
    public void setName(String value) {
        name = value;
    }

    /**
     * Gets the value of the enable property.
     *
     * @return the status
     */
    public boolean isEnable() {
        return enable;
    }

    /**
     * Sets the value of the enable property.
     *
     * @param value
     *            the status
     */
    public void setEnable(boolean value) {
        enable = value;
    }

    /**
     * @param parent
     *            the parent to set
     */
    public void setParent(Category parent) {
        this.parent = parent;
    }

    /**
     * @return the parent
     */
    @Override
    public Category getParent() {
        return parent;
    }

    /**
     * @return the name of the Palette.
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        if (getName() != null && !getName().isEmpty()) {
            return Messages.gettext(getName());
        } else {
            return getName();
        }
    }

    /*
     * Helpers methods
     */
    /**
     * Check that the node can be removed (throw exceptions).
     *
     * @param node
     *            the node to check
     */
    public static void checkRemoving(final PaletteNode node) {
        if (node == null) {
            throw new RuntimeException(String.format(org.scilab.modules.xcos.palette.Palette.WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH,
                                       org.scilab.modules.xcos.palette.Palette.NAME));
        } else if (node instanceof PreLoaded && !(node instanceof PreLoaded.Dynamic)) {
            throw new RuntimeException(String.format(org.scilab.modules.xcos.palette.Palette.WRONG_INPUT_ARGUMENT_S_INVALID_NODE,
                                       org.scilab.modules.xcos.palette.Palette.NAME));
        } else if (node instanceof Category) {
            // Iterate over all nodes
            for (final PaletteNode n : ((Category) node).getNode()) {
                checkRemoving(n);
            }
        }

        /*
         * others can be removed safely.
         */
    }

    /**
     * Remove the dynamic {@link PaletteNode} palette
     *
     * @param node
     *            the palette
     */
    public static void remove(final PaletteNode node) {
        checkRemoving(node);

        final Category toBeReloaded = node.getParent();
        if (toBeReloaded == null) {
            LOG.severe("parent node is null");
            throw new RuntimeException("Parent node is 'null'");
        }

        // select the right "next" node
        final TreeNode toBeSelected;
        if (toBeReloaded.getChildCount() > 1) {
            final int index = toBeReloaded.getIndex(node);
            if (index > 0) {
                toBeSelected = toBeReloaded.getChildAt(index - 1);
            } else {
                toBeSelected = toBeReloaded.getChildAt(index + 1);
            }
        } else {
            toBeSelected = toBeReloaded;
        }

        // remove the node
        node.setParent(null);
        toBeReloaded.getNode().remove(node);


        refreshView(toBeReloaded, toBeSelected);
    }

    /**
     * Refresh the palette view if visible.
     *
     * @param toBeReloaded
     *            the category to refresh
     */
    public static void refreshView(final TreeNode toBeReloaded, final TreeNode selected) {
        if (PaletteManagerView.get() != null) {
            final JTree tree = PaletteManagerView.get().getTree();
            final DefaultTreeModel model = (DefaultTreeModel) tree.getModel();

            /*
             * Reload the whole model
             */
            model.reload();

            /*
             * Select the better path, if applicable
             */
            ArrayList<Object> path = new ArrayList<Object>();
            for (TreeNode n = selected; n != null; n = n.getParent()) {
                path.add(n);
            }
            path.add(tree.getModel().getRoot());
            Collections.reverse(path);

            tree.setSelectionPath(new TreePath(path.toArray()));
        }
    }

    /*
     * Marshalling/Unmarshalling specific methods
     */

    /**
     * This method is called after all the properties (except IDREF) are
     * unmarshalled for this object, but before this object is set to the parent
     * object.
     *
     * @param unmarshaller
     *            the current unmarshaller object
     * @param parent
     *            the parent object
     */
    void afterUnmarshal(Unmarshaller unmarshaller, Object parent) {
        if (parent != null) {
            setParent((Category) parent);
        }
    }
}
