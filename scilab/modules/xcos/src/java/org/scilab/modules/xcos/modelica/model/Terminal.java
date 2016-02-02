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

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * Terminal node of a {@link Struct} tree. This node is always a leaf.
 *
 * <p>Java class for Terminal complex type.
 *
 * <p>The following schema fragment specifies the expected content contained within this class.
 *
 * <pre>
 * &lt;complexType name="Terminal">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="name" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="kind" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="fixed" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="initial_value" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="weight" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="max" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="min" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="nominal_value" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="comment" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="selected" type="{}ModelicaValue" minOccurs="0"/>
 *         &lt;element name="output" minOccurs="0">
 *           &lt;complexType>
 *             &lt;complexContent>
 *               &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *               &lt;/restriction>
 *             &lt;/complexContent>
 *           &lt;/complexType>
 *         &lt;/element>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Terminal", propOrder = {
    "name",
    "kind",
    "id",
    "fixed",
    "initialValue",
    "weight",
    "max",
    "min",
    "nominalValue",
    "comment",
    "selected",
    "output"
})
public class Terminal {

    @XmlElement(required = true)
    protected String name;
    @XmlElement(required = true)
    protected String kind;
    @XmlElement(required = true)
    protected String id;
    protected ModelicaValue fixed;
    @XmlElement(name = "initial_value")
    protected ModelicaValue initialValue;
    protected ModelicaValue weight;
    protected ModelicaValue max;
    protected ModelicaValue min;
    @XmlElement(name = "nominal_value")
    protected ModelicaValue nominalValue;
    protected ModelicaValue comment;
    protected ModelicaValue selected;
    protected Terminal.Output output;

    /**
     * Gets the value of the name property.
     *
     * @return
     *     possible object is
     *     {@link String }
     *
     */
    public String getName() {
        return name;
    }

    /**
     * Sets the value of the name property.
     *
     * @param value
     *     allowed object is
     *     {@link String }
     *
     */
    public void setName(String value) {
        this.name = value;
    }

    /**
     * Gets the value of the kind property.
     *
     * @return
     *     possible object is
     *     {@link String }
     *
     */
    public String getKind() {
        return kind;
    }

    /**
     * Sets the value of the kind property.
     *
     * @param value
     *     allowed object is
     *     {@link String }
     *
     */
    public void setKind(String value) {
        this.kind = value;
    }

    /**
     * Gets the value of the id property.
     *
     * @return
     *     possible object is
     *     {@link String }
     *
     */
    public String getId() {
        return id;
    }

    /**
     * Sets the value of the id property.
     *
     * @param value
     *     allowed object is
     *     {@link String }
     *
     */
    public void setId(String value) {
        this.id = value;
    }

    /**
     * Gets the value of the fixed property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getFixed() {
        return fixed;
    }

    /**
     * Sets the value of the fixed property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setFixed(ModelicaValue value) {
        this.fixed = value;
    }

    /**
     * Gets the value of the initialValue property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getInitialValue() {
        return initialValue;
    }

    /**
     * Sets the value of the initialValue property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setInitialValue(ModelicaValue value) {
        this.initialValue = value;
    }

    /**
     * Gets the value of the weight property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getWeight() {
        return weight;
    }

    /**
     * Sets the value of the weight property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setWeight(ModelicaValue value) {
        this.weight = value;
    }

    /**
     * Gets the value of the max property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getMax() {
        return max;
    }

    /**
     * Sets the value of the max property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setMax(ModelicaValue value) {
        this.max = value;
    }

    /**
     * Gets the value of the min property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getMin() {
        return min;
    }

    /**
     * Sets the value of the min property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setMin(ModelicaValue value) {
        this.min = value;
    }

    /**
     * Gets the value of the nominalValue property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getNominalValue() {
        return nominalValue;
    }

    /**
     * Sets the value of the nominalValue property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setNominalValue(ModelicaValue value) {
        this.nominalValue = value;
    }

    /**
     * Gets the value of the comment property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getComment() {
        return comment;
    }

    /**
     * Sets the value of the comment property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setComment(ModelicaValue value) {
        this.comment = value;
    }

    /**
     * Gets the value of the selected property.
     *
     * @return
     *     possible object is
     *     {@link ModelicaValue }
     *
     */
    public ModelicaValue getSelected() {
        return selected;
    }

    /**
     * Sets the value of the selected property.
     *
     * @param value
     *     allowed object is
     *     {@link ModelicaValue }
     *
     */
    public void setSelected(ModelicaValue value) {
        this.selected = value;
    }

    /**
     * Gets the value of the output property.
     *
     * @return
     *     possible object is
     *     {@link Terminal.Output }
     *
     */
    public Terminal.Output getOutput() {
        return output;
    }

    /**
     * Sets the value of the output property.
     *
     * @param value
     *     allowed object is
     *     {@link Terminal.Output }
     *
     */
    public void setOutput(Terminal.Output value) {
        this.output = value;
    }


    /**
     * <p>Java class for anonymous complex type.
     *
     * <p>The following schema fragment specifies the expected content contained within this class.
     *
     * <pre>
     * &lt;complexType>
     *   &lt;complexContent>
     *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
     *     &lt;/restriction>
     *   &lt;/complexContent>
     * &lt;/complexType>
     * </pre>
     *
     *
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "")
    public static class Output {


    }

}
