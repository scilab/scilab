/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2013 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica.model;

import java.math.BigInteger;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * Specify the size of all the parameters, state, inputs or outputs.
 *
 * <p>Java class for Info complex type.
 *
 * <p>The following schema fragment specifies the expected content contained within this class.
 *
 * <pre>
 * &lt;complexType name="Info">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="number_of_integer_parameters" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_real_parameters" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_string_parameters" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_discrete_variables" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_continuous_variables" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_continuous_unknowns" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_continuous_states" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_inputs" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_outputs" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_modes" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *         &lt;element name="number_of_zero_crossings" type="{http://www.w3.org/2001/XMLSchema}nonNegativeInteger" minOccurs="0"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Info", propOrder = {
    "numberOfIntegerParameters",
    "numberOfRealParameters",
    "numberOfStringParameters",
    "numberOfDiscreteVariables",
    "numberOfContinuousVariables",
    "numberOfContinuousUnknowns",
    "numberOfContinuousStates",
    "numberOfInputs",
    "numberOfOutputs",
    "numberOfModes",
    "numberOfZeroCrossings"
})
public class Info {

    @XmlElement(name = "number_of_integer_parameters")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfIntegerParameters;
    @XmlElement(name = "number_of_real_parameters")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfRealParameters;
    @XmlElement(name = "number_of_string_parameters")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfStringParameters;
    @XmlElement(name = "number_of_discrete_variables")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfDiscreteVariables;
    @XmlElement(name = "number_of_continuous_variables")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfContinuousVariables;
    @XmlElement(name = "number_of_continuous_unknowns")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfContinuousUnknowns;
    @XmlElement(name = "number_of_continuous_states")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfContinuousStates;
    @XmlElement(name = "number_of_inputs")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfInputs;
    @XmlElement(name = "number_of_outputs")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfOutputs;
    @XmlElement(name = "number_of_modes")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfModes;
    @XmlElement(name = "number_of_zero_crossings")
    @XmlSchemaType(name = "nonNegativeInteger")
    protected BigInteger numberOfZeroCrossings;

    /**
     * Gets the value of the numberOfIntegerParameters property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfIntegerParameters() {
        return numberOfIntegerParameters;
    }

    /**
     * Sets the value of the numberOfIntegerParameters property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfIntegerParameters(BigInteger value) {
        this.numberOfIntegerParameters = value;
    }

    /**
     * Gets the value of the numberOfRealParameters property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfRealParameters() {
        return numberOfRealParameters;
    }

    /**
     * Sets the value of the numberOfRealParameters property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfRealParameters(BigInteger value) {
        this.numberOfRealParameters = value;
    }

    /**
     * Gets the value of the numberOfStringParameters property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfStringParameters() {
        return numberOfStringParameters;
    }

    /**
     * Sets the value of the numberOfStringParameters property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfStringParameters(BigInteger value) {
        this.numberOfStringParameters = value;
    }

    /**
     * Gets the value of the numberOfDiscreteVariables property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfDiscreteVariables() {
        return numberOfDiscreteVariables;
    }

    /**
     * Sets the value of the numberOfDiscreteVariables property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfDiscreteVariables(BigInteger value) {
        this.numberOfDiscreteVariables = value;
    }

    /**
     * Gets the value of the numberOfContinuousVariables property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfContinuousVariables() {
        return numberOfContinuousVariables;
    }

    /**
     * Sets the value of the numberOfContinuousVariables property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfContinuousVariables(BigInteger value) {
        this.numberOfContinuousVariables = value;
    }

    /**
     * Gets the value of the numberOfContinuousUnknowns property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfContinuousUnknowns() {
        return numberOfContinuousUnknowns;
    }

    /**
     * Sets the value of the numberOfContinuousUnknowns property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfContinuousUnknowns(BigInteger value) {
        this.numberOfContinuousUnknowns = value;
    }

    /**
     * Gets the value of the numberOfContinuousStates property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfContinuousStates() {
        return numberOfContinuousStates;
    }

    /**
     * Sets the value of the numberOfContinuousStates property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfContinuousStates(BigInteger value) {
        this.numberOfContinuousStates = value;
    }

    /**
     * Gets the value of the numberOfInputs property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfInputs() {
        return numberOfInputs;
    }

    /**
     * Sets the value of the numberOfInputs property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfInputs(BigInteger value) {
        this.numberOfInputs = value;
    }

    /**
     * Gets the value of the numberOfOutputs property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfOutputs() {
        return numberOfOutputs;
    }

    /**
     * Sets the value of the numberOfOutputs property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfOutputs(BigInteger value) {
        this.numberOfOutputs = value;
    }

    /**
     * Gets the value of the numberOfModes property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfModes() {
        return numberOfModes;
    }

    /**
     * Sets the value of the numberOfModes property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfModes(BigInteger value) {
        this.numberOfModes = value;
    }

    /**
     * Gets the value of the numberOfZeroCrossings property.
     *
     * @return
     *     possible object is
     *     {@link BigInteger }
     *
     */
    public BigInteger getNumberOfZeroCrossings() {
        return numberOfZeroCrossings;
    }

    /**
     * Sets the value of the numberOfZeroCrossings property.
     *
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *
     */
    public void setNumberOfZeroCrossings(BigInteger value) {
        this.numberOfZeroCrossings = value;
    }

}
