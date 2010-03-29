/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.model;

import javax.swing.tree.TreeNode;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlTransient;
import javax.xml.bind.annotation.XmlType;

import org.scilab.modules.localization.Messages;

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
@XmlSeeAlso({ Category.class, Palette.class })
public abstract class PaletteNode implements TreeNode {

	@XmlAttribute(required = true)
	private String name;
	@XmlAttribute(required = true)
	private boolean enable;
	@XmlTransient
	private Category parent;

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
		this.name = value;
	}

	/**
	 * Gets the value of the enable property.
	 * @return the status
	 */
	public boolean isEnable() {
		return enable;
	}

	/**
	 * Sets the value of the enable property.
	 * @param value the status 
	 */
	public void setEnable(boolean value) {
		this.enable = value;
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
	 * @return the localized name of the Palette.
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return Messages.gettext(getName());
	}

	/**
	 * This method is called after all the properties (except IDREF) are
	 * unmarshalled for this object, but before this object is set to the parent
	 * object.
	 * 
	 * @param unmarshaller the current unmarshaller object
	 * @param parent the parent object
	 */
	void afterUnmarshal(Unmarshaller unmarshaller, Object parent) {
		if (parent != null) {
			setParent((Category) parent);
		}
	}
}
