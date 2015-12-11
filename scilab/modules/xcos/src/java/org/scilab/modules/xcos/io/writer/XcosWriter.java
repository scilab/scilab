/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.xcos.io.writer;

import java.util.HashSet;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.utils.Stack;

/**
 * Implement an XML writer for any kind of model object
 */
public class XcosWriter {
    protected static final Logger LOG = Logger.getLogger("org.scilab.modules.xcos.io.writer");

    /*
     * Shared data
     */
    protected final JavaController controller;
    protected final ScilabList dictionary;
    protected final RawDataWriter rawDataWriter;
    protected final XMLStreamWriter stream;
    protected final Stack<String> layers;
    protected final HashSet<String> uniqueUIDs;

    public XcosWriter(ScilabList dictionary, XMLStreamWriter writer) {
        this.controller = new JavaController();
        this.dictionary = dictionary;
        this.rawDataWriter = new RawDataWriter(this);
        this.stream = writer;

        this.layers = new Stack<>();
        this.uniqueUIDs = new HashSet<>();
    }

    public void write(long uid, Kind kind) throws XMLStreamException {

        if (LOG.isLoggable(Level.FINER)) {
            LOG.entering(XcosWriter.class.getName(), "write");
        }

        switch (kind) {
            case ANNOTATION:
            case BLOCK:
                new BlockWriter(this).write(uid, kind);
                break;
            case DIAGRAM:
                new CustomWriter(this).write(uid, kind);
                break;
            case LINK:
                new LinkWriter(this).write(uid, kind);
                break;
            case PORT:
                // this will be managed by the BlockWriter
                break;
        }

        if (LOG.isLoggable(Level.FINER)) {
            LOG.exiting(XcosWriter.class.getName(), "write");
        }
    }
}
