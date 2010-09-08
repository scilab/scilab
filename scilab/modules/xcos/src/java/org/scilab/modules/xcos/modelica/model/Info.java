/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
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
 * Specify the size of all the parameters, state, inputs or outputs.
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
public final class Info {

    @XmlElement(name = "number_of_integer_parameters")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfIntegerParameters;
    @XmlElement(name = "number_of_real_parameters")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfRealParameters;
    @XmlElement(name = "number_of_string_parameters")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfStringParameters;
    @XmlElement(name = "number_of_discrete_variables")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfDiscreteVariables;
    @XmlElement(name = "number_of_continuous_variables")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfContinuousVariables;
    @XmlElement(name = "number_of_continuous_unknowns")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfContinuousUnknowns;
    @XmlElement(name = "number_of_continuous_states")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfContinuousStates;
    @XmlElement(name = "number_of_inputs")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfInputs;
    @XmlElement(name = "number_of_outputs")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfOutputs;
    @XmlElement(name = "number_of_modes")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfModes;
    @XmlElement(name = "number_of_zero_crossings")
    @XmlSchemaType(name = "nonNegativeInteger")
    private BigInteger numberOfZeroCrossings;

    /**
	 * Default constructor
	 */
	public Info() { }
    
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
        numberOfIntegerParameters = value;
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
        numberOfRealParameters = value;
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
        numberOfStringParameters = value;
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
        numberOfDiscreteVariables = value;
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
        numberOfContinuousVariables = value;
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
        numberOfContinuousUnknowns = value;
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
        numberOfContinuousStates = value;
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
        numberOfInputs = value;
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
        numberOfOutputs = value;
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
        numberOfModes = value;
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
        numberOfZeroCrossings = value;
    }

}
