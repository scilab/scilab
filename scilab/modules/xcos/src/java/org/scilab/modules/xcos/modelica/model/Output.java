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

import java.math.BigInteger;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;

/**
 * Describe an output of the modelica world.
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Output", propOrder = { "name", "order", "dependencies" })
public final class Output {

    @XmlElement(required = true)
    private String name;
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger order;
    @XmlElement(required = true)
    private Output.Dependencies dependencies;

    /**
     * Default constructor.
     */
    public Output() {
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
        name = value;
    }

    /**
     * Gets the value of the order property.
     * 
     * @return possible object is {@link BigInteger }
     * 
     */
    public BigInteger getOrder() {
        return order;
    }

    /**
     * Sets the value of the order property.
     * 
     * @param value
     *            allowed object is {@link BigInteger }
     * 
     */
    public void setOrder(BigInteger value) {
        order = value;
    }

    /**
     * Gets the value of the dependencies property.
     * 
     * @return possible object is {@link Output.Dependencies }
     * 
     */
    public Output.Dependencies getDependencies() {
        return dependencies;
    }

    /**
     * Sets the value of the dependencies property.
     * 
     * @param value
     *            allowed object is {@link Output.Dependencies }
     * 
     */
    public void setDependencies(Output.Dependencies value) {
        dependencies = value;
    }

    /**
     * Dependency of an {@link Output}.
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = { "variable" })
    public static class Dependencies {

        private String variable;

        /**
         * Default constructor.
         */
        public Dependencies() {
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
            variable = value;
        }

    }

}
