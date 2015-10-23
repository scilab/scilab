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
        private ScicosObjectOwner owner;
        private ObjectProperties property;
        private ObjectProperties associatedProperty;
        private int associatedPropertyIndex;

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
        // TODO in fact we can use the depth (eg. size) of the stack to retrieve
        // this value
        // is it necessary to improve performance (over safety) there ?
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

    private final Map<HandledElementsCategory, ScilabHandler> handlers;

    /*
     * Current state of the parser, also raw shared with the sub-handlers
     */

    /** Contains the decoded parent' node (as an in-depth view of decoded elements) */
    Stack<Object> parents = new Stack<>();
    /** Mapping of UUID JGraphX strings to an MVC decoded object */
    Stack<HashMap<String, Long>> allChildren = new Stack<>();
    /** List of unresolved references that will be resolved at {@link HandledElement#XcosDiagram} or {@link HandledElement#SuperBlockDiagram} ending */
    HashMap<String, ArrayList<UnresolvedReference>> unresolvedReferences = new HashMap<>();

    public XcosSAXHandler(final XcosDiagram content, final ScilabList dictionary) {
        this.root = content;
        this.dictionary = dictionary;

        this.controller = new JavaController();
        this.elementMap = HandledElement.getMap();

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
     * Implemented method
     */

    @Override
    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
        // FOR DEBUG only : printout an XML tree
        if (LOG.isLoggable(Level.FINEST)) {
            char[] indent = new char[parents.size()];
            Arrays.fill(indent, ' ');
            LOG.finest(new String(indent) + localName + " id=\"" + atts.getValue("id") + "\"");
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

    /**
     * Insert a child into the current sub-diagram
     *
     * Manage hierarchy association :
     * <UL>
     *   <LI>{@link ObjectProperties#PARENT_BLOCK}
     *   <LI>{@link ObjectProperties#PARENT_DIAGRAM}
     *   <LI>{@link ObjectProperties#CHILDREN}
     */
    protected void insertChild(final XcosCell cell) {
        long parentUID;
        Kind parentKind;

        final XcosCell parentBlock = lookupForParentXcosCellElement();
        if (parentBlock != null) {
            parentUID = parentBlock.getUID();
            parentKind = parentBlock.getKind();

            controller.setObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.PARENT_BLOCK, parentUID);
        } else {
            parentUID = ((XcosCell) root.getDefaultParent()).getUID();
            parentKind = Kind.DIAGRAM;
        }
        controller.setObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.PARENT_DIAGRAM, ((XcosCell) root.getDefaultParent()).getUID());

        VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(parentUID, parentKind, ObjectProperties.CHILDREN, children);

        children.add(cell.getUID());
        controller.referenceObject(cell.getUID());

        controller.setObjectProperty(parentUID, parentKind, ObjectProperties.CHILDREN, children);
    }
}
