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
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;

/**
 * Root of the modelica tree.
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Model", propOrder = { "name", "modelInfo", "identifiers",
        "implicitRelations", "explicitRelations", "outputs", "elements",
        "equations", "whenClauses" })
@XmlRootElement(name = "model")
public final class Model {

    private String name;
    @XmlElement(name = "model_info")
    private Info modelInfo;
    private Model.Identifiers identifiers;
    @XmlElement(name = "implicit_relations")
    private Model.ImplicitRelations implicitRelations;
    @XmlElement(name = "explicit_relations")
    private Model.ExplicitRelations explicitRelations;
    private Model.Outputs outputs;
    private Model.Elements elements;
    private Model.Equations equations;
    @XmlElement(name = "when_clauses")
    private Model.WhenClauses whenClauses;

    /**
     * Default constructor.
     */
    public Model() {
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
     * Gets the value of the modelInfo property.
     * 
     * @return possible object is {@link Info }
     * 
     */
    public Info getModelInfo() {
        return modelInfo;
    }

    /**
     * Sets the value of the modelInfo property.
     * 
     * @param value
     *            allowed object is {@link Info }
     * 
     */
    public void setModelInfo(Info value) {
        modelInfo = value;
    }

    /**
     * Gets the value of the identifiers property.
     * 
     * @return possible object is {@link Model.Identifiers }
     * 
     */
    public Model.Identifiers getIdentifiers() {
        return identifiers;
    }

    /**
     * Sets the value of the identifiers property.
     * 
     * @param value
     *            allowed object is {@link Model.Identifiers }
     * 
     */
    public void setIdentifiers(Model.Identifiers value) {
        identifiers = value;
    }

    /**
     * Gets the value of the implicitRelations property.
     * 
     * @return possible object is {@link Model.ImplicitRelations }
     * 
     */
    public Model.ImplicitRelations getImplicitRelations() {
        return implicitRelations;
    }

    /**
     * Sets the value of the implicitRelations property.
     * 
     * @param value
     *            allowed object is {@link Model.ImplicitRelations }
     * 
     */
    public void setImplicitRelations(Model.ImplicitRelations value) {
        implicitRelations = value;
    }

    /**
     * Gets the value of the explicitRelations property.
     * 
     * @return possible object is {@link Model.ExplicitRelations }
     * 
     */
    public Model.ExplicitRelations getExplicitRelations() {
        return explicitRelations;
    }

    /**
     * Sets the value of the explicitRelations property.
     * 
     * @param value
     *            allowed object is {@link Model.ExplicitRelations }
     * 
     */
    public void setExplicitRelations(Model.ExplicitRelations value) {
        explicitRelations = value;
    }

    /**
     * Gets the value of the outputs property.
     * 
     * @return possible object is {@link Model.Outputs }
     * 
     */
    public Model.Outputs getOutputs() {
        return outputs;
    }

    /**
     * Sets the value of the outputs property.
     * 
     * @param value
     *            allowed object is {@link Model.Outputs }
     * 
     */
    public void setOutputs(Model.Outputs value) {
        outputs = value;
    }

    /**
     * Gets the value of the elements property.
     * 
     * @return possible object is {@link Model.Elements }
     * 
     */
    public Model.Elements getElements() {
        return elements;
    }

    /**
     * Sets the value of the elements property.
     * 
     * @param value
     *            allowed object is {@link Model.Elements }
     * 
     */
    public void setElements(Model.Elements value) {
        elements = value;
    }

    /**
     * Gets the value of the equations property.
     * 
     * @return possible object is {@link Model.Equations }
     * 
     */
    public Model.Equations getEquations() {
        return equations;
    }

    /**
     * Sets the value of the equations property.
     * 
     * @param value
     *            allowed object is {@link Model.Equations }
     * 
     */
    public void setEquations(Model.Equations value) {
        equations = value;
    }

    /**
     * Gets the value of the whenClauses property.
     * 
     * @return possible object is {@link Model.WhenClauses }
     * 
     */
    public Model.WhenClauses getWhenClauses() {
        return whenClauses;
    }

    /**
     * Sets the value of the whenClauses property.
     * 
     * @param value
     *            allowed object is {@link Model.WhenClauses }
     * 
     */
    public void setWhenClauses(Model.WhenClauses value) {
        whenClauses = value;
    }

    /**
     * Wrapper around the {@link Struct} list.
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = { "struct" })
    public static class Elements {

        private List<Struct> struct;

        /**
         * Default constructor.
         */
        public Elements() {
        }

        /**
         * Gets the value of the struct property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the struct property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getStruct().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link Struct }
         * 
         * @return the value of the struct property.
         */
        public List<Struct> getStruct() {
            if (struct == null) {
                struct = new ArrayList<Struct>();
            }
            return struct;
        }

    }

    /**
     * Wrapper around the equations list.
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = { "equation" })
    public static class Equations {

        private List<ModelicaValue> equation;

        /**
         * Default constructor.
         */
        public Equations() {
        }

        /**
         * Gets the value of the equation property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the equation property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getEquation().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link ModelicaValue }
         * 
         * @return the value of the equation property.
         */
        public List<ModelicaValue> getEquation() {
            if (equation == null) {
                equation = new ArrayList<ModelicaValue>();
            }
            return equation;
        }

    }

    /**
     * Wrapper around the {@link Model.ExplicitRelations.ExplicitRelation}.
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = { "explicitRelation" })
    public static class ExplicitRelations {

        @XmlElement(name = "explicit_relation")
        private List<Model.ExplicitRelations.ExplicitRelation> explicitRelation;

        /**
         * Default constructor.
         */
        public ExplicitRelations() {
        }

        /**
         * Gets the value of the explicitRelation property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the explicitRelation property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getExplicitRelation().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link Model.ExplicitRelations.ExplicitRelation }
         * 
         * @return the value of the explicitRelation property.
         */
        public List<Model.ExplicitRelations.ExplicitRelation> getExplicitRelation() {
            if (explicitRelation == null) {
                explicitRelation = new ArrayList<Model.ExplicitRelations.ExplicitRelation>();
            }
            return explicitRelation;
        }

        /**
         * Wrapper around the explicit variable list.
         */
        @XmlAccessorType(XmlAccessType.FIELD)
        @XmlType(name = "", propOrder = { "explicitVariable" })
        public static class ExplicitRelation {

            @XmlElement(name = "explicit_variable")
            private List<String> explicitVariable;

            /**
             * Default constructor.
             */
            public ExplicitRelation() {
            }

            /**
             * Gets the value of the explicitVariable property.
             * 
             * <p>
             * This accessor method returns a reference to the live list, not a
             * snapshot. Therefore any modification you make to the returned
             * list will be present inside the JAXB object. This is why there is
             * not a <CODE>set</CODE> method for the explicitVariable property.
             * 
             * <p>
             * For example, to add a new item, do as follows:
             * 
             * <pre>
             * getExplicitVariable().add(newItem);
             * </pre>
             * 
             * 
             * <p>
             * Objects of the following type(s) are allowed in the list
             * {@link String }
             * 
             * @return the value of the explicitVariable property.
             */
            public List<String> getExplicitVariable() {
                if (explicitVariable == null) {
                    explicitVariable = new ArrayList<String>();
                }
                return explicitVariable;
            }

        }

    }

    /**
     * Wrapper around the identifiers types.
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = { "parameter", "explicitVariable",
            "implicitVariable" })
    public static class Identifiers {

        private List<String> parameter;
        @XmlElement(name = "explicit_variable")
        private List<String> explicitVariable;
        @XmlElement(name = "implicit_variable")
        private List<String> implicitVariable;

        /**
         * Default constructor.
         */
        public Identifiers() {
        }

        /**
         * Gets the value of the parameter property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the parameter property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getParameter().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link String }
         * 
         * @return the value of the parameter property.
         */
        public List<String> getParameter() {
            if (parameter == null) {
                parameter = new ArrayList<String>();
            }
            return parameter;
        }

        /**
         * Gets the value of the explicitVariable property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the explicitVariable property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getExplicitVariable().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link String }
         * 
         * @return the value of the explicitVariable property.
         */
        public List<String> getExplicitVariable() {
            if (explicitVariable == null) {
                explicitVariable = new ArrayList<String>();
            }
            return explicitVariable;
        }

        /**
         * Gets the value of the implicitVariable property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the implicitVariable property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getImplicitVariable().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link String }
         * 
         * @return the value of the implicitVariable property.
         */
        public List<String> getImplicitVariable() {
            if (implicitVariable == null) {
                implicitVariable = new ArrayList<String>();
            }
            return implicitVariable;
        }

    }

    /**
     * Wrapper around the {@link Model.ImplicitRelations.ImplicitRelation} list
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = { "implicitRelation" })
    public static class ImplicitRelations {

        @XmlElement(name = "implicit_relation")
        private List<Model.ImplicitRelations.ImplicitRelation> implicitRelation;

        /**
         * Default constructor.
         */
        public ImplicitRelations() {
        }

        /**
         * Gets the value of the implicitRelation property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the implicitRelation property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getImplicitRelation().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link Model.ImplicitRelations.ImplicitRelation }
         * 
         * @return the value of the implicitRelation property.
         */
        public List<Model.ImplicitRelations.ImplicitRelation> getImplicitRelation() {
            if (implicitRelation == null) {
                implicitRelation = new ArrayList<Model.ImplicitRelations.ImplicitRelation>();
            }
            return implicitRelation;
        }

        /**
         * Wrapper around the implicit variable list
         */
        @XmlAccessorType(XmlAccessType.FIELD)
        @XmlType(name = "", propOrder = { "implicitVariable" })
        public static class ImplicitRelation {

            @XmlElement(name = "implicit_variable")
            private List<String> implicitVariable;

            /**
             * Default constructor.
             */
            public ImplicitRelation() {
            }

            /**
             * Gets the value of the implicitVariable property.
             * 
             * <p>
             * This accessor method returns a reference to the live list, not a
             * snapshot. Therefore any modification you make to the returned
             * list will be present inside the JAXB object. This is why there is
             * not a <CODE>set</CODE> method for the implicitVariable property.
             * 
             * <p>
             * For example, to add a new item, do as follows:
             * 
             * <pre>
             * getImplicitVariable().add(newItem);
             * </pre>
             * 
             * 
             * <p>
             * Objects of the following type(s) are allowed in the list
             * {@link String }
             * 
             * @return the value of the implicitVariable property.
             */
            public List<String> getImplicitVariable() {
                if (implicitVariable == null) {
                    implicitVariable = new ArrayList<String>();
                }
                return implicitVariable;
            }

        }

    }

    /**
     * Wrapper around the {@link Output} list.
     * 
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "")
    public static class Outputs {
        @XmlElement
        private List<Output> output;

        /**
         * Default constructor.
         */
        public Outputs() {
        }

        /**
         * Gets the value of the output property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
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
         * Objects of the following type(s) are allowed in the list
         * {@link Output }
         * 
         * @return the value of the output property.
         */
        public List<Output> getOutput() {
            if (output == null) {
                output = new ArrayList<Output>();
            }
            return output;
        }
    }

    /**
     * Wrapper around the when clauses.
     * 
     * This class is only partially defined as we need more information on the
     * internals.
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType
    public static class WhenClauses {
        @XmlElement(name = "when_clause")
        private List<String> whenClause;

        /**
         * Default constructor.
         */
        public WhenClauses() {
        }

        /**
         * Gets the value of the whenClause property.
         * 
         * <p>
         * This accessor method returns a reference to the live list, not a
         * snapshot. Therefore any modification you make to the returned list
         * will be present inside the JAXB object. This is why there is not a
         * <CODE>set</CODE> method for the output property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * 
         * <pre>
         * getWhenClause().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link String }
         * 
         * @return the value of the whenClause property.
         */
        public List<String> getWhenClause() {
            if (whenClause == null) {
                whenClause = new ArrayList<String>();
            }

            return whenClause;
        }

    }

    /*
     * Overriden methods
     */

    /**
     * @return the name
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return getName();
    }
}
