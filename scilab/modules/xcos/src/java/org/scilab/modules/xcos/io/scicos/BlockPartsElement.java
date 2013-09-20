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

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;

/**
 * Abstract class which provide commons method for any class that works with
 * block element.
 *
 * This class is intentionally package protected.
 */
abstract class BlockPartsElement extends AbstractElement<BasicBlock> {
    /** input port size */
    private int inSize;
    /** output port size */
    private int outSize;
    /** event input (control) port size */
    private int einSize;
    /** event output (command) port size */
    private int eoutSize;

    /**
     * Default constructor
     */
    protected BlockPartsElement() {
    }

    /**
     * Set the current port size.
     *
     * @param in
     *            the input port vector size
     * @param out
     *            the output port vector size
     * @param ein
     *            the event input (control) port vector size
     * @param eout
     *            the event output (command) port vector size
     */
    protected void setPortsSize(int in, int out, int ein, int eout) {
        this.inSize = in;
        this.outSize = out;
        this.einSize = ein;
        this.eoutSize = eout;
    }

    /**
     * @return the input port vector size
     */
    protected final int getInSize() {
        return inSize;
    }

    /**
     * @return the output port vector size
     */
    protected final int getOutSize() {
        return outSize;
    }

    /**
     * @return the event input (control) port vector size
     */
    protected final int getEinSize() {
        return einSize;
    }

    /**
     * @return the event output (command) port vector size
     */
    protected final int getEoutSize() {
        return eoutSize;
    }

    /**
     * Allocate the port with the right size.
     *
     * This method doesn't use introspection and thus only support
     * {@link ScilabString} and {@link ScilabDouble} class type. With an
     * unsupported class argument, this method return a ScilabDouble instance.
     *
     * @param element
     *            the current working element
     * @param klass
     *            the data class
     * @param size
     *            the size use for the allocation
     */
    protected void addSizedPortVector(ScilabMList element,
                                      Class <? extends ScilabType > klass, int size) {
        ScilabType data;

        if (klass == ScilabString.class) {
            data = getScilabStringInstance(size);
        } else {
            data = getScilabDoubleInstance(size);
        }

        element.add(data);
    }

    /**
     * Return a new {@link ScilabType} instance.
     *
     * If size == 0, then an empty ScilabDouble instance is returned.
     *
     * @param size
     *            the size of the instance
     * @return the new instance
     */
    private ScilabType getScilabStringInstance(int size) {
        if (size != 0) {
            return new ScilabString(new String[size][1]);
        } else {
            return new ScilabDouble();
        }
    }

    /**
     * Return a new {@link ScilabDouble} instance.
     *
     * @param size
     *            the size of the instance
     * @return the new instance
     */
    private ScilabDouble getScilabDoubleInstance(int size) {
        if (size != 0) {
            return new ScilabDouble(new double[size][1]);
        } else {
            return new ScilabDouble();
        }
    }
}
