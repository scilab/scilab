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

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each
 * Java content interface and Java element interface
 * generated in the org.scilab.modules.xcos.modelica.model package.
 * <p>An ObjectFactory allows you to programatically
 * construct new instances of the Java representation
 * for XML content. The Java representation of XML
 * content can consist of schema derived interfaces
 * and classes representing the binding of schema
 * type definitions, element declarations and model
 * groups.  Factory methods for each of these are
 * provided in this class.
 *
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _Model_QNAME = new QName("", "model");
    private final static QName _ModelIdentifiersInput_QNAME = new QName("", "input");
    private final static QName _ModelIdentifiersExplicitVariable_QNAME = new QName("", "explicit_variable");
    private final static QName _ModelIdentifiersParameter_QNAME = new QName("", "parameter");
    private final static QName _ModelIdentifiersImplicitVariable_QNAME = new QName("", "implicit_variable");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: org.scilab.modules.xcos.modelica.model
     *
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link Model.Elements }
     *
     */
    public Model.Elements createModelElements() {
        return new Model.Elements();
    }

    /**
     * Create an instance of {@link Model.WhenClauses }
     *
     */
    public Model.WhenClauses createModelWhenClauses() {
        return new Model.WhenClauses();
    }

    /**
     * Create an instance of {@link ModelicaValue }
     *
     */
    public ModelicaValue createModelicaValue() {
        return new ModelicaValue();
    }

    /**
     * Create an instance of {@link Struct.Subnodes }
     *
     */
    public Struct.Subnodes createStructSubnodes() {
        return new Struct.Subnodes();
    }

    /**
     * Create an instance of {@link Terminal }
     *
     */
    public Terminal createTerminal() {
        return new Terminal();
    }

    /**
     * Create an instance of {@link org.scilab.modules.xcos.modelica.model.Output.Dependencies }
     *
     */
    public org.scilab.modules.xcos.modelica.model.Output.Dependencies createOutputDependencies() {
        return new org.scilab.modules.xcos.modelica.model.Output.Dependencies();
    }

    /**
     * Create an instance of {@link Info }
     *
     */
    public Info createInfo() {
        return new Info();
    }

    /**
     * Create an instance of {@link Terminal.Output }
     *
     */
    public Terminal.Output createTerminalOutput() {
        return new Terminal.Output();
    }

    /**
     * Create an instance of {@link org.scilab.modules.xcos.modelica.model.Output }
     *
     */
    public org.scilab.modules.xcos.modelica.model.Output createOutput() {
        return new org.scilab.modules.xcos.modelica.model.Output();
    }

    /**
     * Create an instance of {@link Model.Outputs }
     *
     */
    public Model.Outputs createModelOutputs() {
        return new Model.Outputs();
    }

    /**
     * Create an instance of {@link Model.ExplicitRelations }
     *
     */
    public Model.ExplicitRelations createModelExplicitRelations() {
        return new Model.ExplicitRelations();
    }

    /**
     * Create an instance of {@link Model.Equations }
     *
     */
    public Model.Equations createModelEquations() {
        return new Model.Equations();
    }

    /**
     * Create an instance of {@link Model }
     *
     */
    public Model createModel() {
        return new Model();
    }

    /**
     * Create an instance of {@link Model.ImplicitRelations }
     *
     */
    public Model.ImplicitRelations createModelImplicitRelations() {
        return new Model.ImplicitRelations();
    }

    /**
     * Create an instance of {@link Model.Identifiers }
     *
     */
    public Model.Identifiers createModelIdentifiers() {
        return new Model.Identifiers();
    }

    /**
     * Create an instance of {@link Model.ExplicitRelations.ExplicitRelation }
     *
     */
    public Model.ExplicitRelations.ExplicitRelation createModelExplicitRelationsExplicitRelation() {
        return new Model.ExplicitRelations.ExplicitRelation();
    }

    /**
     * Create an instance of {@link Struct }
     *
     */
    public Struct createStruct() {
        return new Struct();
    }

    /**
     * Create an instance of {@link Model.ImplicitRelations.ImplicitRelation }
     *
     */
    public Model.ImplicitRelations.ImplicitRelation createModelImplicitRelationsImplicitRelation() {
        return new Model.ImplicitRelations.ImplicitRelation();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link Model }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "model")
    public JAXBElement<Model> createModel(Model value) {
        return new JAXBElement<Model>(_Model_QNAME, Model.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "input", scope = Model.Identifiers.class)
    public JAXBElement<String> createModelIdentifiersInput(String value) {
        return new JAXBElement<String>(_ModelIdentifiersInput_QNAME, String.class, Model.Identifiers.class, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "explicit_variable", scope = Model.Identifiers.class)
    public JAXBElement<String> createModelIdentifiersExplicitVariable(String value) {
        return new JAXBElement<String>(_ModelIdentifiersExplicitVariable_QNAME, String.class, Model.Identifiers.class, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "parameter", scope = Model.Identifiers.class)
    public JAXBElement<String> createModelIdentifiersParameter(String value) {
        return new JAXBElement<String>(_ModelIdentifiersParameter_QNAME, String.class, Model.Identifiers.class, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "implicit_variable", scope = Model.Identifiers.class)
    public JAXBElement<String> createModelIdentifiersImplicitVariable(String value) {
        return new JAXBElement<String>(_ModelIdentifiersImplicitVariable_QNAME, String.class, Model.Identifiers.class, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "input", scope = Model.ImplicitRelations.ImplicitRelation.class)
    public JAXBElement<String> createModelImplicitRelationsImplicitRelationInput(String value) {
        return new JAXBElement<String>(_ModelIdentifiersInput_QNAME, String.class, Model.ImplicitRelations.ImplicitRelation.class, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     *
     */
    @XmlElementDecl(namespace = "", name = "implicit_variable", scope = Model.ImplicitRelations.ImplicitRelation.class)
    public JAXBElement<String> createModelImplicitRelationsImplicitRelationImplicitVariable(String value) {
        return new JAXBElement<String>(_ModelIdentifiersImplicitVariable_QNAME, String.class, Model.ImplicitRelations.ImplicitRelation.class, value);
    }

}
