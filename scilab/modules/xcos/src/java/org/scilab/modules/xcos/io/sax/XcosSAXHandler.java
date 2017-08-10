/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io.sax;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.io.HandledElement;
import org.scilab.modules.xcos.io.HandledElementsCategory;
import org.scilab.modules.xcos.utils.Stack;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;

/**
 * Implement a diagram SAX handler to decode the document as a stream.
 */
public class XcosSAXHandler extends DefaultHandler {
    protected static final Logger LOG = Logger.getLogger("org.scilab.modules.xcos.io.sax");

    /*
     * Utilities classes and methods
     */
    protected static class UnresolvedReference {
        final private ScicosObjectOwner owner;
        final private ObjectProperties property;
        final private ObjectProperties associatedProperty;
        final private int associatedPropertyIndex;

        public UnresolvedReference(ScicosObjectOwner owner, ObjectProperties property, ObjectProperties associatedProperty, int associatedPropertyIndex) {
            this.owner = owner;
            this.property = property;
            this.associatedProperty = associatedProperty;
            this.associatedPropertyIndex = associatedPropertyIndex;
        }

        public void resolve(JavaController controller, long v, Kind kind) {
            controller.setObjectProperty(owner.getUID(), owner.getKind(), property, v);

            if (associatedProperty != null) {
                VectorOfScicosID associated = new VectorOfScicosID();
                controller.getObjectProperty(v, kind, associatedProperty, associated);

                associated.resize(Math.max(associated.size(), associatedPropertyIndex + 1));
                associated.set(associatedPropertyIndex, owner.getUID());

                controller.setObjectProperty(v, kind, associatedProperty, associated);
            }
        }
    }

    XcosCell lookupForParentXcosCellElement() {
        Optional<XcosCell> parentBlock = parents.stream()
                                         .filter(o -> o instanceof XcosCell)
                                         .map(o -> (XcosCell) o)
                                         .findFirst();

        return parentBlock.orElse(null);
    }

    /*
     * Instance data, raw shared with the sub-handlers
     */
    protected final XcosDiagram root;
    protected final ScilabList dictionary;
    protected final JavaController controller;
    protected final Map<String, HandledElement> elementMap;
    protected final Pattern validCIdentifier;

    private final Map<HandledElementsCategory, ScilabHandler> handlers;

    /*
     * Current state of the parser, also raw shared with the sub-handlers
     */

    /** Contains the decoded parent' node (as an in-depth view of decoded elements) */
    Stack<Object> parents = new Stack<>();
    /** Mapping of UID JGraphX strings to an MVC decoded object */
    Stack<HashMap<String, Long>> allChildren = new Stack<>();
    /** List of unresolved references that will be resolved at {@link HandledElement#XcosDiagram} or {@link HandledElement#SuperBlockDiagram} ending */
    HashMap<String, ArrayList<UnresolvedReference>> unresolvedReferences = new HashMap<>();

    public XcosSAXHandler(final XcosDiagram content, final ScilabList dictionary) {
        this.root = content;
        this.dictionary = dictionary;

        this.controller = new JavaController();
        this.elementMap = HandledElement.getMap();

        this.validCIdentifier = Pattern.compile("[a-zA-Z][a-zA-Z0-9_]+");

        // add all the known handler to the map
        EnumMap<HandledElementsCategory, ScilabHandler> localHandlers = new EnumMap<>(HandledElementsCategory.class);
        localHandlers.put(HandledElementsCategory.JGRAPHX, new JGraphXHandler(this));
        localHandlers.put(HandledElementsCategory.BLOCK, new BlockHandler(this));
        localHandlers.put(HandledElementsCategory.LINK, new LinkHandler(this));
        localHandlers.put(HandledElementsCategory.PORT, new PortHandler(this));
        localHandlers.put(HandledElementsCategory.RAW_DATA, new RawDataHandler(this));
        localHandlers.put(HandledElementsCategory.CUSTOM, new CustomHandler(this));

        this.handlers = Collections.unmodifiableMap(localHandlers);
    }

    /*
     * Implemented methods for the reader
     */

    @Override
    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
        // FOR DEBUG only : printout an XML tree
        if (LOG.isLoggable(Level.FINEST)) {
            char[] indent = new char[parents.size()];
            Arrays.fill(indent, ' ');
            StringBuilder args = new StringBuilder();
            if (atts.getValue("id") != null) {
                args.append(" id=\"").append(atts.getValue("id")).append("\"");
            } else if (atts.getValue("as") != null) {
                args.append(" as=\"").append(atts.getValue("as")).append("\"");
            }
            // System.err.println(new StringBuilder().append(indent).append(localName).append(args).toString());
            LOG.finest(new StringBuilder().append(indent).append(localName).append(args).toString());
        }

        HandledElement found = elementMap.get(localName);
        Object localParent = null;

        if (found != null) {
            localParent = handlers.get(found.getCategory()).startElement(found, atts);
        }

        parents.push(localParent);
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        HandledElement found = elementMap.get(localName);

        if (found != null) {
            handlers.get(found.getCategory()).endElement(found);
        }

        parents.pop();
    }

    /*
     * Utilities
     */

    /**
     * Insert a child into the current sub-diagram
     *
     * Manage hierarchy association :
     * <UL>
     *   <LI>{@link ObjectProperties#PARENT_BLOCK}
     *   <LI>{@link ObjectProperties#PARENT_DIAGRAM}
     *   <LI>{@link ObjectProperties#CHILDREN}
     * </UL>
     * @param cell to insert
     */
    protected void insertChild(final XcosCell cell) {
        long parentUID;
        Kind parentKind;

        final XcosCell parentCell = lookupForParentXcosCellElement();
        if (Kind.BLOCK.equals(parentCell.getKind())) {
            parentUID = parentCell.getUID();
            parentKind = parentCell.getKind();

            controller.setObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.PARENT_BLOCK, parentUID);
        } else {
            parentUID = ((XcosCell) root.getDefaultParent()).getUID();
            parentKind = Kind.DIAGRAM;
        }
        controller.setObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.PARENT_DIAGRAM, ((XcosCell) root.getDefaultParent()).getUID());

        VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(parentUID, parentKind, ObjectProperties.CHILDREN, children);

        children.add(cell.getUID());

        controller.setObjectProperty(parentUID, parentKind, ObjectProperties.CHILDREN, children);
    }

    /*
     * Implement ErrorHandler methods
     */

    @Override
    public void warning(SAXParseException e) throws SAXException {
        System.err.println("XcosSAXHandler warning: " + e.getSystemId() + " at line " + e.getLineNumber() + " column " + e.getColumnNumber());
        System.err.println(e.getMessage());
    }

    @Override
    public void error(SAXParseException e) throws SAXException {
        System.err.println("XcosSAXHandler error: " + e.getSystemId() + " at line " + e.getLineNumber() + " column " + e.getColumnNumber());
        System.err.println(e.getMessage());
    }

    @Override
    public void fatalError(SAXParseException e) throws SAXException {
        System.err.println("XcosSAXHandler fatalError: " + e.getSystemId() + " at line " + e.getLineNumber() + " column " + e.getColumnNumber());
        System.err.println(e.getMessage());
    }
}
