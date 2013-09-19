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

import java.io.File;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>
 * Representation of a function. This can be associated with an icon.
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="PaletteBlock">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="data" type="{}VariablePath"/>
 *         &lt;element name="icon" type="{}VariablePath"/>
 *       &lt;/sequence>
 *       &lt;attribute name="name" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PaletteBlock", propOrder = { "data", "icon" })
public class PaletteBlock {

    @XmlElement(required = true)
    private VariablePath data;
    @XmlElement(required = true)
    private VariablePath icon;
    @XmlAttribute(required = true)
    private String name;

    /**
     * Default constructor
     */
    public PaletteBlock() {
    }

    /**
     * Gets the value of the data property.
     *
     * @return possible object is {@link VariablePath }
     *
     */
    public VariablePath getData() {
        return data;
    }

    /**
     * Sets the value of the data property.
     *
     * @param value
     *            allowed object is {@link VariablePath }
     *
     */
    public void setData(VariablePath value) {
        this.data = value;
    }

    /**
     * Gets the value of the icon property.
     *
     * @return possible object is {@link VariablePath }
     *
     */
    public VariablePath getIcon() {
        return icon;
    }

    /**
     * Sets the value of the icon property.
     *
     * @param value
     *            allowed object is {@link VariablePath }
     *
     */
    public void setIcon(VariablePath value) {
        this.icon = value;
    }

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
     * Load the icon and return it.
     *
     * @return the loaded icon
     * @see PaletteBlock#getIcon()
     */
    public Icon getLoadedIcon() {
        String path = getIcon().getEvaluatedPath();

        /*
         * Return an image icon only if the file exists to avoid caching an
         * erroneous image status on the Toolkit.
         */
        if (new File(path).exists()) {
            return new ImageIcon(path);
        } else {
            return new ImageIcon();
        }
    }

}
