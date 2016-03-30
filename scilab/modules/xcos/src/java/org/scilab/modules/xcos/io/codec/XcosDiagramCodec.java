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

package org.scilab.modules.xcos.io.codec;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.logging.Logger;

import org.scilab.modules.graph.io.ScilabGraphCodec;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;

/**
 * Codec for an {@link org.scilab.modules.xcos.graph.XcosDiagram} instance.
 */
// CSOFF: ClassFanOutComplexity
public class XcosDiagramCodec extends ScilabGraphCodec {
    private static final String SCICOS_PARAMETERS = "scicosParameters";
    private static final String AS_ATTRIBUTE = "as";
    private static final String SEP = " - ";

    private static final String INCOMPATIBILITY_DETECTED = Messages.gettext("Incompatibility detected");
    private static final String PLEASE_CHECK_THE_DIAGRAM = Messages.gettext("Please check the diagram, before trying to simulate it.");
    private static final String SOME_BLOCKS_HAVE_BEEN_REMOVED = Messages.gettext("Some blocks have been removed to ensure compatibility.");

    // The non saved fields are hardcoded and can have the same name.
    // CSOFF: MultipleStringLiterals
    private static final String[] DIAGRAM_IGNORED_FIELDS = { "stylesheet", "parentTab", "viewPort", "viewPortMenu", "view", "selectionModel", "savedFile",
                                                             "multiplicities", "opened", "modified", "undoManager", "background"
                                                           };
    private static final String[] SUPERBLOCKDIAGRAM_IGNORED_FIELDS = { "stylesheet", "parentTab", "viewPort", "viewPortMenu", "view", "selectionModel",
                                                                       "multiplicities", "opened", "modified", "undoManager", "savedFile", "container", "integratorAbsoluteTolerance", "integratorRelativeTolerance",
                                                                       "maxIntegrationTimeInterval", "toleranceOnTime", "background"
                                                                     };

    // CSON: MultipleStringLiterals

    /**
     * Default constructor
     *
     * @param template
     *            the instance template
     */
    public XcosDiagramCodec(Object template) {
        super(template);
    }

    /**
     * The constructor used for configuration
     *
     * @param template
     *            Prototypical instance of the object to be encoded/decoded.
     * @param exclude
     *            Optional array of fieldnames to be ignored.
     * @param idrefs
     *            Optional array of fieldnames to be converted to/from references.
     * @param mapping
     *            Optional mapping from field- to attributenames.
     */
    public XcosDiagramCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
    }

    /**
     * Register this codec into the {@link mxCodecRegistry}.
     */
    public static void register() {
        JavaController controller = new JavaController();

        ScilabGraphCodec diagramCodec = new XcosDiagramCodec(new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, ""), DIAGRAM_IGNORED_FIELDS, null,
                null);
        mxCodecRegistry.register(diagramCodec);
        ScilabGraphCodec superBlockDiagramCodec = new XcosDiagramCodec(new XcosDiagram(controller, controller.createObject(Kind.BLOCK), Kind.BLOCK, ""),
                SUPERBLOCKDIAGRAM_IGNORED_FIELDS, null, null);
        mxCodecRegistry.register(superBlockDiagramCodec);
    }

    /**
     * Encode the fieldname value.
     *
     * This method encode the 'scicosParameters' variable to the parent node.
     *
     * @param enc
     *            Codec that controls the encoding process.
     * @param obj
     *            Object whose field is going to be encoded.
     * @param fieldname
     *            Name if the field to be encoded.
     * @param value
     *            Value of the property to be encoded.
     * @param node
     *            XML node that contains the encoded object.
     * @see com.mxgraph.io.mxObjectCodec#encodeValue(com.mxgraph.io.mxCodec, java.lang.Object, java.lang.String, java.lang.Object, org.w3c.dom.Node)
     */
    @Override
    protected void encodeValue(mxCodec enc, Object obj, String fieldname, Object value, Node node) {
        super.encodeValue(enc, obj, fieldname, value, node);

        /*
         * Put child parameters on the parent when child is 'scicosParameters'.
         */
        if (fieldname.equals(SCICOS_PARAMETERS)) {
            Node params = node.getLastChild();

            /*
             * Remove the "as" attribute
             */
            NamedNodeMap childAttributes = params.getAttributes();
            childAttributes.removeNamedItem(AS_ATTRIBUTE);

            /*
             * Move each attribute from child to parent
             */
            NamedNodeMap parentAttributes = node.getAttributes();
            for (int length = childAttributes.getLength() - 1; length >= 0; length--) {
                Node element = childAttributes.item(length);

                childAttributes.removeNamedItem(element.getNodeName());
                parentAttributes.setNamedItem(element);
            }

            /*
             * Move each childNode from child to parent
             */
            NodeList children = params.getChildNodes();
            for (int length = children.getLength() - 1; length >= 0; length--) {
                Node element = children.item(length);

                params.removeChild(element);
                node.appendChild(element);
            }

            /*
             * Remove the ScicosParameter instance
             */
            node.removeChild(params);
        }
    }

    /**
     * Load the ScicosParameters fields from the current object
     *
     * @param obj
     *            the {@link XcosDiagram} instance
     * @param fieldname
     *            the {@link Current} field name
     * @param value
     *            the current field value
     * @see com.mxgraph.io.mxObjectCodec#setFieldValue(java.lang.Object, java.lang.String, java.lang.Object)
     */
    @Override
    protected void setFieldValue(Object obj, String fieldname, Object value) {
        Field field;
        try {
            field = ScicosParameters.class.getDeclaredField(fieldname);
            ScicosParameters params = new ScicosParameters(((XcosDiagram) obj).getUID(), ((XcosDiagram) obj).getKind());
            super.setFieldValue(params, fieldname, value);
        } catch (SecurityException e) {
            field = null;
        } catch (NoSuchFieldException e) {
            field = null;
        }

        // pre 6.0.0 Xcos have an mxCell defaultParent whereas it should be an XcosCell
        if (field == null && "defaultParent".equals(fieldname) && !(value instanceof XcosCell)) {
            XcosDiagram diag = (XcosDiagram) obj;
            XcosCell defaultParent = (XcosCell) diag.getDefaultParent();
            mxICell root = (mxICell) diag.getModel().getRoot();

            /*
             * Restore the initial hierarchy mxCell -> root XcosCell -> default parent with diagram uid / kind BasicBlock BasicLink . . .
             */

            Object[] cells = diag.getChildCells(value);
            root.remove(0);
            diag.addCells(cells, defaultParent);

            return;
        }

        if (field == null) {
            super.setFieldValue(obj, fieldname, value);
        }
    }

    /**
     * {@inheritDoc} <BR>
     * <BR>
     * <B>UPDATED TO COVER</B>
     *
     * <UL>
     * <LI>Strip out any node with an invalid parent id. (5.3.1 diagrams may contains invalid default parents, remove them.)</LI>
     * <LI>Remove cell where id end with "#identifier#identifier"</LI>
     * </UL>
     */
    @Override
    public Node beforeDecode(mxCodec dec, Node node, Object obj) {
        final Set<String> ids = new HashSet<String>();
        final Collection<Node> trash = new ArrayList<Node>();

        if (node instanceof Element) {
            final Node model = ((Element) node).getElementsByTagName("mxGraphModel").item(0);
            if (model instanceof Element) {
                final Node root = ((Element) model).getElementsByTagName("root").item(0);
                if (root != null) {
                    for (Node cell = root.getFirstChild(); cell != null; cell = cell.getNextSibling()) {

                        if (cell instanceof Element && cell.getLocalName().contentEquals("mxCell")) {
                            cleanUpNode(ids, trash, cell);
                        }
                    }

                    for (Node cell : trash) {
                        root.removeChild(cell);
                    }
                }
            }
        }

        return super.beforeDecode(dec, node, obj);
    }

    /**
     * Add the cell to the ids or trash set
     *
     * @param ids
     *            the valid id set
     * @param trash
     *            the invalid id list
     * @param cell
     *            the cell to clean or not
     */
    private void cleanUpNode(final Set<String> ids, final Collection<Node> trash, Node cell) {
        final Node id = cell.getAttributes().getNamedItem("id");
        final String idValue = id.getNodeValue();
        final Node parent = cell.getAttributes().getNamedItem("parent");

        if (id instanceof Element) {
            ids.add(idValue);
        }
        if (parent instanceof Element && !ids.contains(parent.getNodeValue())) {
            trash.add(parent);
        }

        // remove dual identifier cells
        if (idValue.endsWith(XcosDiagram.HASH_IDENTIFIER + XcosDiagram.HASH_IDENTIFIER)) {
            trash.add(cell);
        }
    }

    /**
     * Put a comment with versions.
     *
     * @param enc
     *            the encoder
     * @param obj
     *            the object to encode
     * @param node
     *            the cureent node
     * @return the updated object
     * @see org.scilab.modules.graph.io.ScilabGraphCodec#beforeEncode(com.mxgraph.io.mxCodec, java.lang.Object, org.w3c.dom.Node)
     */
    @Override
    public Object beforeEncode(mxCodec enc, Object obj, Node node) {
        final Package p = Package.getPackage("org.scilab.modules.xcos");

        trace(enc, node, new StringBuilder().append(Xcos.TRADENAME).append(SEP).append(Xcos.VERSION).append(SEP).append(p.getSpecificationVersion()).append(SEP)
              .append(p.getImplementationVersion()).toString());

        return super.beforeEncode(enc, obj, node);
    }

    /**
     * Apply compatibility pattern to the decoded object
     *
     * @param dec
     *            Codec that controls the decoding process.
     * @param node
     *            XML node to decode the object from.
     * @param obj
     *            Object decoded.
     * @return The Object transformed
     * @see org.scilab.modules.graph.io.ScilabGraphCodec#afterDecode(com.mxgraph.io.mxCodec, org.w3c.dom.Node, java.lang.Object)
     */
    @Override
    public Object afterDecode(mxCodec dec, Node node, Object obj) {
        final XcosDiagram diag = (XcosDiagram) obj;

        final mxGraphModel model = (mxGraphModel) diag.getModel();
        final Object parent = diag.getDefaultParent();

        // pre-5.3 diagram may be saved in a locked state
        // unlock it
        diag.setReadOnly(false);

        // 5.3.1 diagrams may contains invalid default parents, remove them.
        final Object root = model.getParent(parent);
        if (root != model.getRoot() && root != null) {
            Logger.getLogger(XcosDiagramCodec.class.getName()).warning("Removing misplaced cells");
            model.setRoot(root);
        }

        return super.afterDecode(dec, node, obj);
    }

    /**
     * Pop up an update dialog to alert the user.
     */
    public void showUpdateDialog() {
        ScilabModalDialog.show(null, new String[] { SOME_BLOCKS_HAVE_BEEN_REMOVED, "", PLEASE_CHECK_THE_DIAGRAM }, INCOMPATIBILITY_DETECTED,
                               IconType.WARNING_ICON);
    }
}
// CSON: ClassFanOutComplexity
