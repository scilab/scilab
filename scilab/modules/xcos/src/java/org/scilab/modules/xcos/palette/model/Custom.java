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
