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

package org.scilab.modules.xcos.io.scicos;

import org.scilab.modules.types.ScilabType;

/**
 * Define the methods used by any *Element to read/write Scicos elements.
 *
 * @param <T>
 *            Type of the Xcos associated data.
 */
public interface Element<T> {

    /**
     * Perform element decoding initialization.
     *
     * @param element
     *            the element
     * @param into
     *            the target instance
     * @return the modified target instance
     */
    T beforeDecode(ScilabType element, T into);

    /**
     * Read the element and write the result into the object.
     *
     * @param element
     *            the element to read
     * @param into
     *            where to write data, if null a new instance is created.
     * @throws ScicosFormatException
     *             when an error has occurred.
     * @return the resulting instance
     */
    T decode(ScilabType element, T into) throws ScicosFormatException;

    /**
     * Perform element decoding cleanup.
     *
     * @param element
     *            the element
     * @param into
     *            the target instance
     * @return the modified target instance
     */
    T afterDecode(ScilabType element, T into);

    /**
     * Test if the element instance can be decoded.
     *
     * Note that of this method return false the call to
     * {@link #decode(ScilabType, Object)} will throw a
     * {@link org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException}
     * .
     *
     * @param element
     *            the element instance to test.
     * @return true if the element can be decoded, true if the element cannot be
     *         decoded.
     */
    boolean canDecode(ScilabType element);

    /**
     * Perform element encoding initialization.
     *
     * @param from
     *            the data
     * @param element
     *            the target element
     * @return the modified target element
     */
    ScilabType beforeEncode(T from, ScilabType element);

    /**
     * Write an element from data to a Scilab element.
     *
     * @param from
     *            the source data
     * @param element
     *            the previously encoded element or null.
     * @return the encoded element
     */
    ScilabType encode(T from, ScilabType element);

    /**
     * Write an element from data to a Scilab element.
     *
     * @param from
     *            the source data
     * @return the encoded element
     */
    ScilabType encode(T from);

    /**
     * Perform element encoding cleanup.
     *
     * @param from
     *            the data
     * @param element
     *            the target element
     * @return the modified target element
     */
    ScilabType afterEncode(T from, ScilabType element);
}
