/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.Map;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;

public interface Handler {

    /**
     * Decode an Xcos block
     *
     * @return the decoded block
     * @throws ScicosFormatException
     *             on decoding error
     */
    public abstract BasicBlock readBlock() throws ScicosFormatException;

    /**
     * Decode an Xcos block into an instance
     *
     * @param into
     *            the instance to update
     * @return the updated instance.
     * @throws ScicosFormatException
     *             on decoding error
     */
    public abstract BasicBlock readBlock(BasicBlock into)
    throws ScicosFormatException;

    /**
     * Decode an evaluated Xcos context
     *
     * @return the decoded context
     */
    public abstract Map<String, String> readContext();

    /**
     * Decode an Xcos diagram from an H5 file
     *
     * @return the decoded diagram
     * @throws VersionMismatchException
     *             when the diagram version mismatch
     */
    public abstract XcosDiagram readDiagram() throws VersionMismatchException;

    /**
     * Decode an Xcos diagram from an H5 file in place.
     *
     * @param instance
     *            the previously allocated diagram where to decode.
     * @return the decoded diagram
     */
    public abstract XcosDiagram readDiagram(XcosDiagram instance);

    /**
     * Encode an Xcos block
     *
     * @param block
     *            the block
     */
    public abstract void writeBlock(BasicBlock block);

    /**
     * Encode an Xcos context
     *
     * @param context
     *            the context
     */
    public abstract void writeContext(String[] context);

    /**
     * Encode an Xcos diagram
     *
     * @param diagram
     *            the block
     */
    public abstract void writeDiagram(XcosDiagram diagram);

}