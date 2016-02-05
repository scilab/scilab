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
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>
 * A variable path is a path dependent of an environment variable. The real path
 * is computed at runtime by concatenate the environment variable result and the
 * path.
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="VariablePath">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="path" use="required" type="{http://www.w3.org/2001/XMLSchema}anyURI" />
 *       &lt;attribute name="variable" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "VariablePath")
public class VariablePath {

    @XmlAttribute(required = true)
    @XmlSchemaType(name = "anyURI")
    private String path;
    @XmlAttribute
    private String variable;

    /**
     * Default constructor
     */
    public VariablePath() {
    }

    /**
     * Gets the value of the path property.
     *
     * @return possible object is {@link String }
     *
     */
    public String getPath() {
        return path;
    }

    /**
     * Sets the value of the path property.
     *
     * @param value
     *            allowed object is {@link String }
     *
     */
    public void setPath(String value) {
        this.path = value;
    }

    /**
     * Gets the value of the variable property.
     *
     * @return possible object is {@link String }
     *
     */
    public String getVariable() {
        return variable;
    }

    /**
     * Sets the value of the variable property.
     *
     * @param value
     *            allowed object is {@link String }
     *
     */
    public void setVariable(String value) {
        this.variable = value;
    }

    /**
     * Evaluate the environment variable and construct a real absolut path.
     *
     * @return the real path
     */
    public String getEvaluatedPath() {
        StringBuilder str = new StringBuilder();

        if (getVariable() != null) {
            str.append(System.getenv(getVariable()));
        }
        str.append(getPath());

        return str.toString();
    }

    /**
     * @return a representative string
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return getEvaluatedPath();
    }
}
