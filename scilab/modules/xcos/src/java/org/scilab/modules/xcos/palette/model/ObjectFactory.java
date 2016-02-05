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
