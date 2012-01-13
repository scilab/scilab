/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.modelica.model;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;
import javax.xml.bind.annotation.XmlValue;

/**
 * Modelica String assignation.
 * 
 * The data is on the value property and the content doesn't contains any data.
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ModelicaValue")
public final class ModelicaValue {
    @XmlAttribute
    private String value;

    /**
     * Default constructor.
     */
    public ModelicaValue() {
        value = "";
    }

    /**
     * @return always empty String.
     * 
     */
    @XmlValue
    @Deprecated
    public String getContent() {
        return null;
    }

    /**
     * Empty implementation as the content is always null.
     * 
     * @param value
     *            allowed object is {@link String }
     * 
     */
    @Deprecated
    public void setContent(String value) {
    }

    /**
     * Gets the value of the value property.
     * 
     * @return possible object is {@link String }
     * 
     */
    public String getValue() {
        return value;
    }

    /**
     * Sets the value of the value property.
     * 
     * @param value
     *            allowed object is {@link String }
     * 
     */
    public void setValue(String value) {
        this.value = value;
    }
}
