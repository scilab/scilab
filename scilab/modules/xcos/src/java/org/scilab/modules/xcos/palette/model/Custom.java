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

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>
 * A Custom palette is loaded as a diagram but in the palette tab.
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="Custom">
 *   &lt;complexContent>
 *     &lt;extension base="{}Palette">
 *       &lt;sequence>
 *         &lt;element name="path" type="{}VariablePath"/>
 *       &lt;/sequence>
 *     &lt;/extension>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Custom", propOrder = { "path" })
public class Custom extends Palette {

    @XmlElement(required = true)
    private VariablePath path;

    /**
     * Default constructor
     */
    public Custom() {
    }

    /**
     * Gets the value of the path property.
     *
     * @return possible object is {@link VariablePath }
     *
     */
    public VariablePath getPath() {
        return path;
    }

    /**
     * Sets the value of the path property.
     *
     * @param value
     *            allowed object is {@link VariablePath }
     *
     */
    public void setPath(VariablePath value) {
        this.path = value;
    }

}
