/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

import java.util.List;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.ScilabTypeCoder;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxConstants;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * Protected class which decode graphic fields of a block.
 *
 * This class is intentionally package-protected to prevent external use.
 */
// CSOFF: ClassDataAbstractionCoupling
final class BlockGraphicElement extends BlockPartsElement {
    /*
     * "in_style", "out_style" and style have been added on the 5.3-5.4 dev. cycle they are not checked to be compatible with older versions.
     */
    protected static final List<String> DATA_FIELD_NAMES = asList("graphics", "orig", "sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout", "gr_i",
            "id", "in_implicit", "out_implicit");
    protected static final List<String> DATA_FIELD_NAMES_FULL = asList("graphics", "orig", "sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout",
            "gr_i", "id", "in_implicit", "out_implicit", "in_style", "out_style", "in_label", "out_label", "style");

    private static final int ORIGIN_INDEX = DATA_FIELD_NAMES_FULL.indexOf("orig");
    private static final int DIMS_INDEX = DATA_FIELD_NAMES_FULL.indexOf("sz");
    private static final int FLIP_INDEX = DATA_FIELD_NAMES_FULL.indexOf("flip");
    private static final int EXPRS_INDEX = DATA_FIELD_NAMES_FULL.indexOf("exprs");
    private static final int ID_INDEX = DATA_FIELD_NAMES_FULL.indexOf("id");
    private static final int STYLE_INDEX = DATA_FIELD_NAMES_FULL.indexOf("style");

    private static final int GRAPHICS_INSTRUCTION_SIZE = 8;
    private static final double DEFAULT_SIZE_FACTOR = 20.0;

    /** Mutable field to easily get the data through methods */
    private ScilabMList data;

    /** In-progress decoded diagram */
    private final XcosDiagram diag;

    /** Size factor use to scale Xcos-Scicos dimensions */
    private final double sizeFactor;

    /**
     * Default constructor
     */
    public BlockGraphicElement(final JavaController controller) {
        this(controller, null);
    }

    /**
     * Default constructor with diagram
     *
     * @param diag
     *            the diagram
     */
    public BlockGraphicElement(final JavaController controller, final XcosDiagram diag) {
        super(controller);

        this.diag = diag;

        /*
         * Out of a diagram update, use the DEFAULT_SIZE_FACTOR.
         */
        if (diag == null) {
            sizeFactor = DEFAULT_SIZE_FACTOR;
        } else {
            sizeFactor = 1.0;
        }
    }

    /**
     * Default constructor with diagram
     *
     * @param diag
     *            the diagram
     * @param sizeFactor
     *            the size factor
     */
    public BlockGraphicElement(final JavaController controller, final XcosDiagram diag, final double sizeFactor) {
        super(controller);

        this.diag = diag;
        this.sizeFactor = sizeFactor;
    }

    /**
     * Decode Scicos element into the block.
     *
     * This decode method doesn't coverage Port management because we need model information to handle it.
     *
     * @param element
     *            the scicos element
     * @param into
     *            the previously instantiated block.
     * @return the modified into block.
     * @throws ScicosFormatException
     *             on error.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
     */
    @Override
    public BasicBlock decode(ScilabType element, final BasicBlock into) throws ScicosFormatException {

        if (into == null) {
            throw new IllegalArgumentException();
        }
        BasicBlock block = into;

        data = (ScilabMList) element;

        validate();

        block = beforeDecode(element, block);

        /*
         * fill the data
         */
        decodeDimension(block);
        decodeOrigin(block);
        decodeFlipAndRotation(block);
        decodeIdCell(block);

        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.EXPRS, new ScilabTypeCoder().var2vec(data.get(EXPRS_INDEX)));

        if (data.size() > STYLE_INDEX && !isEmptyField(data.get(STYLE_INDEX))) {
            final ScilabString style = (ScilabString) data.get(STYLE_INDEX);
            final String s = style.getData()[0][0];

            if (s != null && !s.isEmpty()) {
                block.setStyle(s);
            }
        }

        block = afterDecode(element, block);

        return block;
    }

    /**
     * Validate the current data.
     *
     * This method doesn't pass the metrics because it perform many test. Therefore all these tests are trivial and the conditioned action only throw an exception.
     *
     * @throws ScicosFormatException
     *             when there is a validation error.
     */
    // CSOFF: CyclomaticComplexity
    // CSOFF: NPathComplexity
    // CSOFF: JavaNCSS
    private void validate() throws ScicosFormatException {
        if (!canDecode(data)) {
            throw new WrongElementException();
        }

        int field = 0;

        // we test if the structure as enough field
        if (data.size() < DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }

        /*
         * Checking the MList header
         */

        // Check the first field
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
        final String[] header = ((ScilabString) data.get(field)).getData()[0];

        // Checking for the field names
        if (header.length < DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }
        for (int i = 0; i < DATA_FIELD_NAMES.size(); i++) {
            if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
                throw new WrongStructureException(DATA_FIELD_NAMES);
            }
        }

        /*
         * Checking the data
         */

        // orig : must contain the coord of the block
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // sz : must contains the size of the block
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // flip
        field++;
        if (!(data.get(field) instanceof ScilabBoolean)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // theta
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // exprs
        field++;
        if (!(data.get(field) instanceof ScilabString) && !(data.get(field) instanceof ScilabList) && !(data.get(field) instanceof ScilabTList)
                && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // pin
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // pout
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // pein
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // peout
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // gr_i
        // !! WARNING !! we do not care about gr_i because there are only
        // block look related.
        field++;

        // id
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // in_implicit
        field++;
        if (!(data.get(field) instanceof ScilabString) && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // out_implicit
        field++;
        if (!(data.get(field) instanceof ScilabString) && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // field added on the 5.3-5.4 dev. cycle
        // not checked due to compatibility
        // in_style
        // out_style
        // style
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity
    // CSON: JavaNCSS

    /**
     * Fill the block with the origin parameters
     *
     * @param into
     *            the target instance
     */
    private void decodeOrigin(final BasicBlock into) {
        /*
         * Getting the values
         */
        double x;
        double y;

        final double[][] real = ((ScilabDouble) data.get(ORIGIN_INDEX)).getRealPart();
        x = real[0][0];
        final double[] vector = real[real.length - 1];
        y = vector[vector.length - 1];

        /*
         * Apply compatibility patterns
         */
        x *= sizeFactor;
        y *= sizeFactor;

        /*
         * Invert the y-axis value and translate it.
         */
        y = -y - into.getGeometry().getHeight();

        /*
         * fill parameter
         */
        into.getGeometry().setX(x);
        into.getGeometry().setY(y);
    }

    /**
     * Fill the block with the dimension parameters
     *
     * @param into
     *            the target instance
     */
    private void decodeDimension(final BasicBlock into) {
        /*
         * Getting the values
         */
        double w;
        double h;

        final double[][] real = ((ScilabDouble) data.get(DIMS_INDEX)).getRealPart();
        w = real[0][0];
        final double[] vector = real[real.length - 1];
        h = vector[vector.length - 1];

        /*
         * When a block has no parent diagram, the size should be updated. On a diagram decode, size is right.
         */
        h *= sizeFactor;
        w *= sizeFactor;

        /*
         * fill parameter
         */
        into.getGeometry().setWidth(w);
        into.getGeometry().setHeight(h);
    }

    /**
     * Fill the block with the flip and theta parameters
     *
     * @param into
     *            the target instance
     */
    private void decodeFlipAndRotation(final BasicBlock into) {
        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(into.getUID(), into.getKind(), ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        StyleMap styleMap = new StyleMap(interfaceFunction[0]);

        /*
         * Flip management
         */
        if (!((ScilabBoolean) data.get(FLIP_INDEX)).getData()[0][0]) {
            styleMap.put(XcosConstants.STYLE_FLIP, Boolean.TRUE.toString());
        }

        /*
         * Rotation management
         */
        int theta = (int) ((ScilabDouble) data.get(FLIP_INDEX + 1)).getRealPart()[0][0];
        if (theta != 0) {
            // convert to a valid value
            theta = BlockPositioning.roundAngle(-theta);
            styleMap.put(mxConstants.STYLE_ROTATION, Integer.toString(theta));
        }

        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.STYLE, styleMap.toString());
    }

    /**
     * Preserve the id if applicable
     *
     * @param into
     *            the target instance
     */
    private void decodeIdCell(final BasicBlock into) {
        if (diag == null) {
            return;
        }

        final String[][] id = ((ScilabString) data.get(ID_INDEX)).getData();
        if (id.length == 0 || id[0].length == 0 || id[0][0].isEmpty()) {
            return;
        }

        /*
         * Create the local identifier
         */
        final mxCell identifier = diag.createCellIdentifier(into);
        identifier.setValue(id[0][0]);

        into.insert(identifier);
    }

    /**
     * Check if the element can be decoded.
     *
     * @param element
     *            the Scicos element
     * @return true, if the Scicos types match.
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabMList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }
}
// CSON: ClassDataAbstractionCoupling
