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

package org.scilab.modules.xcos.io.scicos;

import static java.util.Arrays.asList;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
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
    private static final ScilabTList DIAGRAM_OPTIONS = new ScilabTList(OPTS_FIELDS,
            Arrays.asList(
                new ScilabList(// 3D
                    Arrays.asList(new ScilabBoolean(true), new ScilabDouble(33))),
    new ScilabDouble(new double[][] { { 8, 1 } }), // Background
    new ScilabDouble(new double[][] { { 1, 5 } }), // Link
    new ScilabList(// ID
    Arrays.asList(new ScilabDouble(new double[][] { { 5, 1 } }), new ScilabDouble(new double[][] { { 4, 1 } }))),
    new ScilabDouble(new double[][] { { 0.8, 0.8, 0.8 } }) // Cmap
            ));
    // CSON: MagicNumber

    private ScilabMList base;

    private double minimalYaxisValue = Double.POSITIVE_INFINITY;
    private double minimalXaxisValue = Double.POSITIVE_INFINITY;

    /**
     * Default constructor
     */
    public DiagramElement(final JavaController controller) {
        super(controller);
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
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
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
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
     */
    public XcosDiagram decode(ScilabType element, XcosDiagram into, boolean validate) throws ScicosFormatException {
        base = (ScilabMList) element;

        XcosDiagram diag = into;
        if (diag == null) {
            throw new IllegalArgumentException();
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
     * Decode the diagram
     *
     * @param diag
     *            the current diagram
     * @param validate
     *            if true, enable graphic updates ; false disable them
     * @throws ScicosFormatException
     *             on decoding error
     */
    private void decodeDiagram(XcosDiagram diag, boolean validate) throws ScicosFormatException {
        // Fill the local parameters
        // NOTE: the title field is checked on the ScicosParametersElement
        final String title = ((ScilabString) ((ScilabTList) base.get(1)).get(2)).getData()[0][0];
        diag.setTitle(title);

        // Fill the diagram attributes
        ScicosParametersElement params = new ScicosParametersElement(controller);
        params.decode(base.get(1), new ScicosParameters(diag.getUID(), diag.getKind()));

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

        final BlockElement blockElement = new BlockElement(controller, diag);
        final LinkElement linkElement = new LinkElement(controller, blocks);
        final LabelElement labelElement = new LabelElement(controller);

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

                BlockPositioning.updateBlockView(diag, block);

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
     * @param validate
     *            perform graphic updates, or not
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
        final Map<IOBlocks, List<mxICell>> ports = IOBlocks.getAllPorts(parent);
        // FIXME is it really needed
        // final Map<IOBlocks, List<BasicBlock>> blocks = IOBlocks.getAllBlocks(parent);
        //
        // for (final IOBlocks io : IOBlocks.values()) {
        // final List<mxICell> port = ports.get(io);
        // final List<BasicBlock> block = blocks.get(io);
        //
        // final int len = Math.min(port.size(), block.size());
        // for (int i = 0; i < len; i++) {
        // final mxICell p = port.get(i);
        // final mxICell b = block.get(i);
        //
        // // if the I/O block has a port child and a label child,
        // // update
        // if (b.getChildCount() > 1) {
        // final Object value = b.getChildAt(b.getChildCount() - 1).getValue();
        // p.setValue(value);
        // }
        // }
        // }
    }

    /**
     * Check that the current ScilabType is a valid Diagram.
     *
     * This method doesn't pass the metrics because it perform many test. Therefore all these tests are trivial and the conditioned action only throw an exception.
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
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
