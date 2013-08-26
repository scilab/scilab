/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.model;

import javax.xml.bind.annotation.XmlRegistry;

/**
 * This object contains factory methods for each Java content interface and Java
 * element interface generated in the org.scilab.modules.xcos.palette.model
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

    /**
     * Create a new ObjectFactory that can be used to create new instances of
     * schema derived classes for package: org.scilab.modules.xcos.palette.model
     *
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link PaletteBlock }
     *
     * @return a new instance
     */
    public PaletteBlock createPaletteBlock() {
        return new PaletteBlock();
    }

    /**
     * Create an instance of {@link Category }
     *
     * @return a new instance
     */
    public Category createCategory() {
        return new Category();
    }

    /**
     * Create an instance of {@link Custom }
     *
     * @return a new instance
     */
    public Custom createCustom() {
        return new Custom();
    }

    /**
     * Create an instance of {@link PreLoaded }
     *
     * @return a new instance
     */
    public PreLoaded createPreLoaded() {
        return new PreLoaded();
    }

    /**
     * Create an instance of {@link VariablePath }
     *
     * @return a new instance
     */
    public VariablePath createVariablePath() {
        return new VariablePath();
    }

}
