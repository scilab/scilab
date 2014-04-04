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

import static java.util.Arrays.asList;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxGraphModel.Filter;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.util.mxPoint;

/**
 * Perform a diagram transformation between Scicos and Xcos.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
public final class DiagramElement extends AbstractElement<XcosDiagram> {
    protected static final List<String> DATA_FIELD_NAMES = asList("diagram", "props", "objs");
    protected static final List<String> DATA_FIELD_NAMES_FULL = asList("diagram", "props", "objs", "version", "contrib");
    private static final List<String> VERSIONS = asList("", "scicos4.2", "scicos4.3", "scicos4.4");

    private static final int OBJS_INDEX = DATA_FIELD_NAMES_FULL.indexOf("objs");
    private static final int VERSION_INDEX = DATA_FIELD_NAMES_FULL.indexOf("version");

    private static final double H_MARGIN = 40.0;
    private static final double V_MARGIN = 40.0;

    /** Diagram properties MList header (scs_m.props) */
    private static final String[] PROPS_FIELDS = { "params", "wpar", "title", "tol", "tf", "context", "void1", "options", "void2", "void3", "doc" };

    /** Index of the title in the props field */
    private static final int TITLE_INDEX = 2;

    /** Diagram options MList header (scs_m.props.options) */
    private static final String[] OPTS_FIELDS = { "scsopt", "3D", "Background", "Link", "ID", "Cmap" };
    /**
     * Window properties (scs_m.props.wpar).
     *
     * This property has no impact among simulation
     */
    private static final double[][] WPAR = { { 600, 450, 0, 0, 600, 450 } };

    // The window parameters and diagram options are not used in the simulation
    // thus we set it to default values.
    // As the values are scicos dependent we avoid using constant references.
    // CSOFF: MagicNumber
    private static final ScilabTList DIAGRAM_OPTIONS = new ScilabTList(OPTS_FIELDS, Arrays.asList(new ScilabList(// 3D
    Arrays.asList(new ScilabBoolean(true), new ScilabDouble(33))), new ScilabDouble(new double[][] { { 8, 1 } }), // Background
    new ScilabDouble(new double[][] { { 1, 5 } }), // Link
    new ScilabList(// ID
    Arrays.asList(new ScilabDouble(new double[][] { { 5, 1 } }), new ScilabDouble(new double[][] { { 4, 1 } }))), new ScilabDouble(
    new double[][] { { 0.8, 0.8, 0.8 } }) // Cmap
                                                                                                 ));
    // CSON: MagicNumber

    private ScilabMList base;

    private double minimalYaxisValue = Double.POSITIVE_INFINITY;
    private double minimalXaxisValue = Double.POSITIVE_INFINITY;

    /**
     * Default constructor
     */
    public DiagramElement() {
    }

    /**
     * Decode the diagram with version validation.
     *
     * @param element
     *            the diagram Scicos element
     * @param into
     *            the Xcos instance, if null, a new instance is returned.
     * @return the modified into parameters
     * @throws ScicosFormatException
     *             when a decoding error occurs
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    @Override
    public XcosDiagram decode(ScilabType element, XcosDiagram into) throws ScicosFormatException {
        return decode(element, into, true);
    }

    /**
     * Decode the diagram
     *
     * @param element
     *            the diagram Scicos element
     * @param into
     *            the Xcos instance, if null, a new instance is returned.
     * @param validate
     *            true, if the diagram version will be checked. false otherwise.
     * @return the modified into parameters
     * @throws ScicosFormatException
     *             when a decoding error occurs
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    public XcosDiagram decode(ScilabType element, XcosDiagram into, boolean validate) throws ScicosFormatException {
        base = (ScilabMList) element;

        XcosDiagram diag = into;
        if (diag == null) {
            diag = new XcosDiagram();
        }

        diag = beforeDecode(element, diag);
        diag.getModel().beginUpdate();

        // Validate the base field
        String wrongVersion = null;
        try {
            validate(validate);
        } catch (VersionMismatchException e) {
            wrongVersion = e.getWrongVersion();
        }

        // Fill the diag
        decodeDiagram(diag, validate);

        diag.getModel().endUpdate();
        diag = afterDecode(element, diag);

        // Rethrow the version exception after a decode.
        if (wrongVersion != null) {
            throw new VersionMismatchException(wrongVersion);
        }

        return diag;
    }

    /**
     * Reassociate the children with the current diagram.
     *
     * @param element
     *            the encoded element
     * @param into
     *            the target instance
     * @return the modified target instance
     * @see org.scilab.modules.xcos.io.scicos.AbstractElement#afterDecode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    @Override
    public XcosDiagram afterDecode(ScilabType element, XcosDiagram into) {
        into.setChildrenParentDiagram();

        return super.afterDecode(element, into);
    }

    /**
     * Decode the diagram
     *
     * @param diag
     *            the current diagram
     * @param validate if true, enable graphic updates ; false disable them
     * @throws ScicosFormatException
     *             on decoding error
     */
    private void decodeDiagram(XcosDiagram diag, boolean validate) throws ScicosFormatException {
        // Fill the local parameters
        // NOTE: the title field is checked on the ScicosParametersElement
        final String title = ((ScilabString) ((ScilabTList) base.get(1)).get(2)).getData()[0][0];
        diag.setTitle(title);

        // Fill the diagram attributes
        ScicosParametersElement params = new ScicosParametersElement();
        params.decode(base.get(1), diag.getScicosParameters());

        // Decode the objs attributes
        decodeObjs(diag);
        // Update the objs properties if applicable
        updateObjs(diag, validate);
    }

    /**
     * Decode the objs list into cells
     *
     * @param diag
     *            the target instance
     * @throws ScicosFormatException
     *             on error
     */
    private void decodeObjs(final XcosDiagram diag) throws ScicosFormatException {
        final int nbOfObjs = ((ScilabList) base.get(OBJS_INDEX)).size();
        final HashMap<Integer, BasicBlock> blocks = new HashMap<Integer, BasicBlock>(nbOfObjs, 1.0f);

        final BlockElement blockElement = new BlockElement(diag);
        final LinkElement linkElement = new LinkElement(blocks);
        final LabelElement labelElement = new LabelElement();

        /*
         * Decode blocks
         */
        for (int i = 0; i < nbOfObjs; i++) {
            final ScilabMList data = (ScilabMList) ((ScilabList) base.get(OBJS_INDEX)).get(i);
            Object cell = null;

            if (blockElement.canDecode(data)) {
                BasicBlock block = blockElement.decode(data, null);
                blocks.put(i, block);
                cell = block;

                BlockPositioning.updateBlockView(block);

                minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
                minimalXaxisValue = Math.min(minimalXaxisValue, ((mxCell) cell).getGeometry().getX());
            } else if (labelElement.canDecode(data)) {
                cell = labelElement.decode(data, null);

                minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
                minimalXaxisValue = Math.min(minimalXaxisValue, ((mxCell) cell).getGeometry().getX());
            }

            if (cell != null) {
                diag.addCell(cell);
            }
        }

        /*
         * Decode links
         */
        for (int i = 0; i < nbOfObjs; i++) {
            final ScilabMList data = (ScilabMList) ((ScilabList) base.get(OBJS_INDEX)).get(i);
            Object cell = null;

            if (linkElement.canDecode(data)) {
                BasicLink link = linkElement.decode(data, null);
                cell = link;

                final List<mxPoint> points = ((mxCell) cell).getGeometry().getPoints();
                for (final mxPoint p : points) {
                    minimalYaxisValue = Math.min(minimalYaxisValue, p.getY());
                    minimalXaxisValue = Math.min(minimalXaxisValue, p.getX());
                }
            }

            if (cell != null) {
                diag.addCell(cell);
            }
        }
    }

    /**
     * Update the diagram object after decode
     *
     * @param diag
     *            the diagram to update
     * @param validate perform graphic updates, or not
     */
    private void updateObjs(final XcosDiagram diag, boolean validate) {
        final mxGraphModel model = (mxGraphModel) diag.getModel();

        final double minY = -minimalYaxisValue + V_MARGIN;
        final double minX = -minimalXaxisValue + H_MARGIN;
        for (final Object cell : model.getCells().values()) {
            updateMinimalSize(cell, model);
            translate(cell, model, minX, minY);
            if (validate) {
                updateLabels(cell, model);
            }
        }
    }

    // update the cell size to be at least selectable
    private static final void updateMinimalSize(final Object cell, final mxIGraphModel model) {
        if (!(cell instanceof BasicBlock)) {
            return;
        }

        final double min = 7.0;

        final mxGeometry geom = model.getGeometry(cell);
        if (geom == null) {
            return;
        }

        final double dx;
        if (geom.getWidth() < min) {
            dx = (geom.getWidth() - min) / 2;
            geom.setWidth(min);
        } else {
            dx = 0.0;
        }
        final double dy;
        if (geom.getHeight() < min) {
            dy = (geom.getHeight() - min) / 2;
            geom.setHeight(min);
        } else {
            dy = 0.0;
        }

        geom.translate(dx, dy);
    }

    // Translate the y axis for blocks and links
    private static final void translate(final Object cell, final mxIGraphModel model, final double minX, final double minY) {
        if (cell instanceof BasicPort) {
            return;
        }

        final mxGeometry geom = model.getGeometry(cell);
        if (geom != null) {
            geom.translate(minX, minY);
        }
    }

    // update the labels of ports for SuperBlock
    private static final void updateLabels(final Object cell, final mxIGraphModel model) {
        if (!(cell instanceof SuperBlock)) {
            return;
        }
        final SuperBlock parent = (SuperBlock) cell;

        // Assume that the children are sorted after decode
        // blk.sortChildren();
        final Map<IOBlocks, List<mxICell>> ports = IOBlocks.getAllPorts(parent);
        final Map<IOBlocks, List<BasicBlock>> blocks = IOBlocks.getAllBlocks(parent);

        for (final IOBlocks io : IOBlocks.values()) {
            final List<mxICell> port = ports.get(io);
            final List<BasicBlock> block = blocks.get(io);

            final int len = Math.min(port.size(), block.size());
            for (int i = 0; i < len; i++) {
                final mxICell p = port.get(i);
                final mxICell b = block.get(i);

                // if the I/O block has a port child and a label child,
                // update
                if (b.getChildCount() > 1) {
                    final Object value = b.getChildAt(b.getChildCount() - 1).getValue();
                    p.setValue(value);
                }
            }
        }
    }

    /**
     * Check that the current ScilabType is a valid Diagram.
     *
     * This method doesn't pass the metrics because it perform many test.
     * Therefore all these tests are trivial and the conditioned action only
     * throw an exception.
     *
     * @param checkVersion
     *            true, when the check validate the version
     * @throws ScicosFormatException
     *             When the diagram is not valid
     */
    // CSOFF: CyclomaticComplexity
    // CSOFF: NPathComplexity
    private void validate(boolean checkVersion) throws ScicosFormatException {

        // Have we enough fields ?
        if (base.size() < DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }

        int field = 0;

        /*
         * Checking the MList header
         */

        // Check the first field
        if (!(base.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
        String[] header = ((ScilabString) base.get(field)).getData()[0];

        // Check the number of fields
        if (header.length < DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }

        // Check the first fields values
        for (int i = 0; i < DATA_FIELD_NAMES.size(); i++) {
            if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
                throw new WrongStructureException(DATA_FIELD_NAMES);
            }
        }

        /*
         * Checking the data types
         */

        // the second field must contain list of props
        field++;
        if (!(base.get(field) instanceof ScilabTList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the third field must contains lists of blocks and links
        field++;
        if (!(base.get(field) instanceof ScilabList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the last field must contain the scicos version used
        field++;

        // doesn't check version if not present (optional field)
        if (base.size() <= field) {
            return;
        }

        if (!(base.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        /*
         * Check the version if applicable
         */
        if (checkVersion) {
            String scicosVersion = ((ScilabString) base.get(field)).getData()[0][0];
            if (!VERSIONS.contains(scicosVersion)) {
                throw new VersionMismatchException(scicosVersion);
            }
        }
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity

    /**
     * @param element
     *            the base element
     * @return true if the header is valid, false otherwise
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        if (!(element instanceof ScilabMList)) {
            return false;
        }

        base = (ScilabMList) element;

        /*
         * Checking header
         */
        final String type = ((ScilabString) base.get(0)).getData()[0][0];
        final boolean typeIsValid = type.equals(DATA_FIELD_NAMES.get(0));

        /*
         * Check the version if applicable
         */
        final String scicosVersion;
        if (base.size() > VERSION_INDEX) {
            scicosVersion = ((ScilabString) base.get(VERSION_INDEX)).getData()[0][0];
        } else {
            scicosVersion = "";
        }
        final boolean versionIsValid = VERSIONS.contains(scicosVersion);
        return typeIsValid && versionIsValid;
    }

    /**
     * {@inheritDoc}
     *
     * Clear cell warnings before encoding
     */
    @Override
    public ScilabType beforeEncode(XcosDiagram from, ScilabType element) {
        if (from.getAsComponent() != null) {
            from.getAsComponent().clearCellOverlays();
        }
        return super.beforeEncode(from, element);
    }

    /**
     * Encode the instance into the element
     *
     * @param from
     *            the source instance
     * @param element
     *            the previously allocated element.
     * @return the element parameter
     * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object,
     *      org.scilab.modules.types.ScilabType)
     */
    @Override
    public ScilabType encode(XcosDiagram from, ScilabType element) {
        base = (ScilabMList) element;

        int field = 0;

        if (base == null) {
            base = allocateElement();
        }

        base = (ScilabMList) beforeEncode(from, base);

        field++;
        fillParams(from, field);

        field++;
        fillObjs(from, field);

        base = (ScilabMList) afterEncode(from, base);

        return base;
    }

    /**
     * Allocate a new element
     *
     * @return the new element
     */
    private ScilabMList allocateElement() {
        ScilabMList data = new ScilabMList(DATA_FIELD_NAMES_FULL.toArray(new String[0]));
        data.add(allocatePropsField()); // props
        data.add(new ScilabList()); // objs
        data.add(new ScilabString(VERSIONS.get(0))); // official version
        data.add(new ScilabList()); // contrib
        return data;
    }

    /**
     * Allocate the props field
     *
     * @return the new props field
     */
    private ScilabTList allocatePropsField() {
        ScilabTList data = new ScilabTList(PROPS_FIELDS);
        data.add(new ScilabDouble(WPAR)); // wpar
        data.add(new ScilabString("")); // title
        data.add(new ScilabDouble()); // tol
        data.add(new ScilabDouble()); // tf
        data.add(new ScilabDouble()); // context
        data.add(new ScilabDouble()); // void1
        data.add(DIAGRAM_OPTIONS); // options
        data.add(new ScilabDouble()); // void2
        data.add(new ScilabDouble()); // void3
        data.add(new ScilabList()); // doc

        return data;
    }

    /**
     * Fill the params field
     *
     * @param from
     *            the source instance
     * @param field
     *            the params field number
     */
    private void fillParams(XcosDiagram from, int field) {
        ScilabType data;
        final ScicosParametersElement paramsElement = new ScicosParametersElement();
        data = base.get(field);
        data = paramsElement.encode(from.getScicosParameters(), data);

        // set the title as it is need for generating files
        ((ScilabTList) data).set(TITLE_INDEX, new ScilabString(FileUtils.toValidCIdentifier(from.getTitle())));

        base.set(field, data);
    }

    /**
     * Fill the objs field
     *
     * @param from
     *            the source instance
     * @param field
     *            the objs field number
     */
    private void fillObjs(XcosDiagram from, int field) {
        final BlockElement blockElement = new BlockElement(from);
        final LinkElement linkElement = new LinkElement(null);
        final ScilabList data = (ScilabList) base.get(field);

        final List<BasicBlock> blockList = new ArrayList<BasicBlock>();
        final List<BasicLink> linkList = new ArrayList<BasicLink>();

        /*
         * Fill the block and link lists
         */
        final Filter filter = new Filter() {
            @Override
            public boolean filter(Object current) {
                if (current instanceof BasicBlock && !(current instanceof TextBlock)) {
                    filterBlocks(blockList, linkList, (BasicBlock) current);
                } else if (current instanceof BasicLink) {
                    filterLink(linkList, (BasicLink) current);
                }

                return false;
            }

            /**
             * Filter blocks
             *
             * @param blockList
             *            the current block list
             * @param linkList
             *            the current link list
             * @param block
             *            the block to filter
             */
            private void filterBlocks(final List<BasicBlock> blockList, final List<BasicLink> linkList, final BasicBlock block) {
                blockList.add(block);

                //
                // Look inside a Block to see if there is no "AutoLink"
                // Jgraphx will store this link as block's child
                //
                for (int j = 0; j < block.getChildCount(); ++j) {
                    if (block.getChildAt(j) instanceof BasicLink) {
                        final BasicLink link = (BasicLink) block.getChildAt(j);

                        // do not add the link if not connected
                        if (link.getSource() != null && link.getTarget() != null) {
                            linkList.add(link);
                        }
                    }
                }

            }

            /**
             * Filter links
             *
             * @param linkList
             *            the current link list
             * @param link
             *            the link to filter
             */
            private void filterLink(final List<BasicLink> linkList, final BasicLink link) {
                // Only add connected links
                final mxICell source = link.getSource();
                final mxICell target = link.getTarget();
                if (source != null && target != null) {
                    linkList.add(link);
                }
            }
        };
        mxGraphModel.filterDescendants(from.getModel(), filter);

        /*
         * Use a predictable block and links order when debug is enable
         */
        if (Logger.getLogger(BlockElement.class.getName()).isLoggable(Level.FINE)) {
            Collections.sort(blockList);
            Collections.sort(linkList, new Comparator<BasicLink>() {
                @Override
                public int compare(BasicLink o1, BasicLink o2) {
                    return ((ScilabGraphUniqueObject) o1.getSource()).compareTo((ScilabGraphUniqueObject) o2.getSource());
                }
            });
        }

        /*
         * Reorder links
         */
        for (int i = 0; i < linkList.size(); ++i) {
            linkList.get(i).setOrdering(i + blockList.size() + 1);
        }

        /*
         * Reorder and encode blocks
         */
        for (int i = 0; i < blockList.size(); ++i) {
            blockList.get(i).setOrdering(i + 1);

            data.add(blockElement.encode(blockList.get(i), null));
        }

        /*
         * Encode links
         */
        for (int i = 0; i < linkList.size(); ++i) {
            final ScilabType link = linkElement.encode(linkList.get(i), null);
            if (link != null) {
                data.add(link);
            } else {
                data.add(new ScilabMList(new String[] { "Deleted" }));
                from.warnCellByUID(linkList.get(i).getId(), XcosMessages.LINK_NOT_CONNECTED);
            }
        }
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
