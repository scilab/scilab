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

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * Terminal node of a {@link Struct} tree. This node is always a leaf.
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Terminal", propOrder = { "name", "kind", "id", "fixed",
        "initialValue", "weight", "max", "min", "nominalValue", "comment",
        "selected", "output" })
public final class Terminal {

    @XmlElement(required = true)
    private String name;
    @XmlElement(required = true)
    private String kind;
    @XmlElement(required = true)
    private String id;
    private ModelicaValue fixed;
    @XmlElement(name = "initial_value")
    private ModelicaValue initialValue;
    private ModelicaValue weight;
    private ModelicaValue max;
    private ModelicaValue min;
    @XmlElement(name = "nominal_value")
    private ModelicaValue nominalValue;
    private ModelicaValue comment;
    private ModelicaValue selected;
    private List<Output> output;

    /**
     * Default constructor.
     */
    public Terminal() {
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
     * Gets the value of the kind property.
     * 
     * @return possible object is {@link String }
     * 
     */
    public String getKind() {
        return kind;
    }

    /**
     * Sets the value of the kind property.
     * 
     * @param value
     *            allowed object is {@link String }
     * 
     */
    public void setKind(String value) {
        kind = value;
    }

    /**
     * Gets the value of the id property.
     * 
     * @return possible object is {@link String }
     * 
     */
    public String getId() {
        return id;
    }

    /**
     * Sets the value of the id property.
     * 
     * @param value
     *            allowed object is {@link String }
     * 
     */
    public void setId(String value) {
        id = value;
    }

    /**
     * Gets the value of the fixed property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getFixed() {
        return fixed;
    }

    /**
     * Sets the value of the fixed property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setFixed(ModelicaValue value) {
        fixed = value;
    }

    /**
     * Gets the value of the initialValue property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getInitialValue() {
        return initialValue;
    }

    /**
     * Sets the value of the initialValue property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setInitialValue(ModelicaValue value) {
        initialValue = value;
    }

    /**
     * Gets the value of the weight property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getWeight() {
        return weight;
    }

    /**
     * Sets the value of the weight property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setWeight(ModelicaValue value) {
        weight = value;
    }

    /**
     * Gets the value of the max property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getMax() {
        return max;
    }

    /**
     * Sets the value of the max property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setMax(ModelicaValue value) {
        max = value;
    }

    /**
     * Gets the value of the min property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getMin() {
        return min;
    }

    /**
     * Sets the value of the min property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setMin(ModelicaValue value) {
        min = value;
    }

    /**
     * Gets the value of the nominalValue property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getNominalValue() {
        return nominalValue;
    }

    /**
     * Sets the value of the nominalValue property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setNominalValue(ModelicaValue value) {
        nominalValue = value;
    }

    /**
     * Gets the value of the comment property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getComment() {
        return comment;
    }

    /**
     * Sets the value of the comment property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setComment(ModelicaValue value) {
        comment = value;
    }

    /**
     * Gets the value of the selected property.
     * 
     * @return possible object is {@link ModelicaValue }
     * 
     */
    public ModelicaValue getSelected() {
        return selected;
    }

    /**
     * Sets the value of the selected property.
     * 
     * @param value
     *            allowed object is {@link ModelicaValue }
     * 
     */
    public void setSelected(ModelicaValue value) {
        selected = value;
    }

    /**
     * Gets the value of the output property.
     * 
     * <p>
     * This accessor method returns a reference to the live list, not a
     * snapshot. Therefore any modification you make to the returned list will
     * be present inside the JAXB object. This is why there is not a
     * <CODE>set</CODE> method for the output property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * 
     * <pre>
     * getOutput().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list {@link Output }
     * 
     * @return the value of the output property.
     */
    public List<Output> getOutput() {
        if (output == null) {
            output = new ArrayList<Output>();
        }

        return output;
    }

    /**
     * @return the id
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return getId();
    }
}
