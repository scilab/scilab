/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2013 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.modelica.model;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;

/**
 * This object contains factory methods for each Java content interface and Java
 * element interface generated in the org.scilab.modules.xcos.modelica.model
 * package.
 * <p>
 * An ObjectFactory allows you to programatically construct new instances of the
 * Java representation for XML content. The Java representation of XML content
 * can consist of schema derived interfaces and classes representing the binding
 * of schema type definitions, element declarations and model groups. Factory
 * methods for each of these are provided in this class.
 *
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _Model_QNAME = new QName("", "model");
    private final static QName _TerminalOutput_QNAME = new QName("", "output");

    /**
     * Create a new ObjectFactory that can be used to create new instances of
     * schema derived classes for package:
     * org.scilab.modules.xcos.modelica.model
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
     * Create an instance of {@link Output.Dependencies }
     *
     */
    public Output.Dependencies createOutputDependencies() {
        return new Output.Dependencies();
    }

    /**
     * Create an instance of {@link Info }
     *
     */
    public Info createInfo() {
        return new Info();
    }

    /**
     * Create an instance of {@link Output }
     *
     */
    public Output createOutput() {
        return new Output();
    }

    /**
     * Create an instance of {@link Model.Outputs }
     *
     */
    public Model.Outputs createModelOutputs() {
        return new Model.Outputs();
    }

    /**
     * Create an instance of {@link Model.ExplicitRelations.ExplicitRelation }
     *
     */
    public Model.ExplicitRelations.ExplicitRelation createModelExplicitRelationsExplicitRelation() {
        return new Model.ExplicitRelations.ExplicitRelation();
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
     * Create an instance of {@link Model.ImplicitRelations.ImplicitRelation }
     *
     */
    public Model.ImplicitRelations.ImplicitRelation createModelImplicitRelationsImplicitRelation() {
        return new Model.ImplicitRelations.ImplicitRelation();
    }

    /**
     * Create an instance of {@link Struct }
     *
     */
    public Struct createStruct() {
        return new Struct();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link Model }{@code >}
     *
     */
    @XmlElementDecl(namespace = "", name = "model")
    public JAXBElement<Model> createModel(Model value) {
        return new JAXBElement<Model>(_Model_QNAME, Model.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}
     *
     */
    @XmlElementDecl(namespace = "", name = "output", scope = Terminal.class)
    public JAXBElement<String> createTerminalOutput(String value) {
        return new JAXBElement<String>(_TerminalOutput_QNAME, String.class, Terminal.class, value);
    }

}
