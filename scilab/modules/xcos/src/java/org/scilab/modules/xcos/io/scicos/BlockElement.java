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

import java.util.HashMap;
import java.util.List;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.port.BasicPort;

/**
 * Perform a block transformation between Scicos and Xcos.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
public final class BlockElement extends AbstractElement<BasicBlock> {
    protected static final List<String> DATA_FIELD_NAMES = asList("Block", "graphics", "model", "gui", "doc");

    private static final int INTERFUNCTION_INDEX = DATA_FIELD_NAMES.indexOf("gui");

    /** Mutable field to easily get the data through methods */
    private ScilabMList data;

    /** Element used to decode/encode Scicos model part into a BasicBlock */
    private final BlockModelElement modelElement;

    /** Element used to decode/encode Scicos model part into a BasicBlock */
    private final BlockGraphicElement graphicElement;

    /*
     * Decoder state
     */

    /**
     * Current block ordering, the ordering change on each {@link BlockElement} instance so be careful when allocated a new {@link BlockElement}.
     */
    private int ordering;
    private HashMap<Long, Integer> orderingMap;

    /**
     * Default constructor.
     *
     * The state change on each {@link BlockElement} instance so be careful when allocated a new {@link BlockElement}.
     */
    public BlockElement(final JavaController controller, final XcosDiagram diag) {
        super(controller);

        modelElement = new BlockModelElement(controller, diag);
        graphicElement = new BlockGraphicElement(controller, diag);

        ordering = 0;
        orderingMap = new HashMap<>();
    }

    /**
     * Decode the element into the block.
     *
     * @param element
     *            The current Scilab data
     * @param into
     *            the target, if null a new instance is allocated and returned.
     * @return the decoded block.
     * @throws ScicosFormatException
     *             when e decoding error occurred.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
     */
    @Override
    public BasicBlock decode(ScilabType element, BasicBlock into) throws ScicosFormatException {
        data = (ScilabMList) element;
        BasicBlock block = into;

        validate();

        /*
         * Instantiate the block if it doesn't exist. Do not invoke scilab here on purpose to avoid threading issues
         */
        final String interfunction = ((ScilabString) data.get(INTERFUNCTION_INDEX)).getData()[0][0];
        if (block == null) {
            BlockInterFunction func = XcosCellFactory.lookForInterfunction(interfunction);
            block = XcosCellFactory.createBlock(controller, func, interfunction, controller.createObject(Kind.BLOCK), Kind.BLOCK);
        }

        block = beforeDecode(element, block);

        /*
         * Allocate and setup ports
         */
        InputPortElement inElement = new InputPortElement(controller, data);
        final int numberOfInputPorts = inElement.getNumberOfInputPort();
        for (int i = 0; i < numberOfInputPorts; i++) {
            final BasicPort port = inElement.decode(data, null);

            // do not use BasicPort#addPort() to avoid the view update
            block.insert(port, i);
        }

        OutputPortElement outElement = new OutputPortElement(controller, data);
        final int numberOfOutputPorts = outElement.getNumberOfOutputPort();
        for (int i = 0; i < numberOfOutputPorts; i++) {
            final BasicPort port = outElement.decode(data, null);

            // do not use BasicPort#addPort() to avoid the view update
            block.insert(port, numberOfInputPorts + i);
        }

        /*
         * Fill block with the data structure
         */
        int field = 1;
        graphicElement.decode(data.get(field), block);

        field++;
        modelElement.decode(data.get(field), block);

        field++;
        controller.setObjectProperty(block.getUID(), block.getKind(), ObjectProperties.INTERFACE_FUNCTION, interfunction);

        field++;
        fillDocStructure(data.get(field), block);

        /*
         * Set state dependent information.
         */
        orderingMap.put(block.getUID(), ordering++);

        block = afterDecode(element, block);

        return block;
    }

    /**
     * Use the Scicos documentation structure to get the previous Xcos IDs.
     *
     * @param scilabType
     *            the scicos documentation field.
     * @param into
     *            the target instance.
     */
    private void fillDocStructure(ScilabType scilabType, BasicBlock into) {
        /*
         * The double type is used as the default one, generate on empty field.
         */
        if (scilabType instanceof ScilabDouble) {
            return;
        }

        /*
         * Classical behavior
         */
        ScilabList list = (ScilabList) scilabType;

        if (list.size() > 0 && list.get(0) instanceof ScilabString) {
            String uid = ((ScilabString) list.get(0)).getData()[0][0];
            if (isValidUid(uid)) {
                into.setId(uid);
                return;
            }
        }
    }

    /**
     * @param uid
     *            The uid to check
     * @return true if the uid is valid, false otherwise
     */
    private boolean isValidUid(String uid) {
        final String[] components = uid.split(":");

        boolean valid = components.length == 3;
        if (valid) {
            try {
                Integer.parseInt(components[0], 16);
                Long.parseLong(components[1], 16);
                Integer.parseInt(components[2], 16);
            } catch (IllegalArgumentException e) {
                valid = false;
            }
        }
        return valid;
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
    private void validate() throws ScicosFormatException {
        if (!canDecode(data)) {
            throw new WrongElementException();
        }

        int field = 0;

        // we test if the structure as enough field
        if (data.size() != DATA_FIELD_NAMES.size()) {
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
        if (header.length != DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }
        for (int i = 0; i < header.length; i++) {
            if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
                throw new WrongStructureException(DATA_FIELD_NAMES);
            }
        }

        /*
         * Checking the data
         */

        // the second field must contain list of all graphic property (how the
        // block will be displayed )
        field++;
        if (!(data.get(field) instanceof ScilabMList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the third field must contains all the information needed to compile
        // the block
        field++;
        if (!(data.get(field) instanceof ScilabMList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the fourth field must contains all the information needed to
        // represent
        // the block
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the last field must contain a list of nothing aka scicos doc
        field++;
        if (!(data.get(field) instanceof ScilabList) && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity

    /**
     * Test if the current instance can be used to decode the element
     *
     * @param element
     *            the current element
     * @return true, if the element can be decoded, false otherwise
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabMList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }
}
// CSON: ClassFanOutComplexity
// CSON: ClassDataAbstractionCoupling
