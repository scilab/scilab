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

import java.util.List;

import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;

/**
 * Perform label transformation between Xcos and Scicos.
 */
public final class LabelElement extends AbstractElement<TextBlock> {
    protected static final List<String> DATA_FIELD_NAMES = asList("Text", "graphics", "model", "void", "gui");

    /** Mutable field to easily get the data through methods */
    private ScilabMList data;

    /** Element used to decode/encode Scicos model part into a BasicBlock */
    private final BlockModelElement modelElement = new BlockModelElement(null);

    /** Element used to decode/encode Scicos model part into a BasicBlock */
    private final BlockGraphicElement graphicElement = new BlockGraphicElement(null, 1.0);

    /**
     * Default constructor
     */
    public LabelElement() {
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
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    @Override
    public TextBlock decode(ScilabType element, TextBlock into) throws ScicosFormatException {
        TextBlock block = into;
        data = (ScilabMList) element;

        validate();

        if (into == null) {
            block = new TextBlock();
        }

        block = beforeDecode(element, block);

        /*
         * Fill block with the data structure
         */
        int field = 1;
        graphicElement.decode(data.get(field), block);

        field++;
        modelElement.decode(data.get(field), block);

        // interfunction, keep the default one
        field++;

        // doc, do nothing
        field++;

        /*
         * Fill Jgraphx properties
         */
        if (isEmptyField(block.getRealParameters())) {
            block.setValue("");
        } else {
            final String text = ((ScilabString) block.getRealParameters()).getData()[0][0];
            block.setValue(text);
        }

        block = afterDecode(element, block);

        return block;
    }

    /**
     * Validate the current data.
     *
     * This method doesn't pass the metrics because it perform many test.
     * Therefore all these tests are trivial and the conditioned action only
     * throw an exception.
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

        // the fourth field must contains an empty string
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the last field must contain a gui name
        field++;
        if (!(data.get(field) instanceof ScilabString) && isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity

    /**
     * Test if the current implementation can be used to decode the element.
     *
     * @param element
     *            the element to test
     * @return true when the implementation is the right one, false otherwise.
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabMList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }

    /**
     * Not implemented yet
     *
     * @param from
     *            not used
     * @param element
     *            not used
     * @return always null
     * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object,
     *      org.scilab.modules.types.ScilabType)
     */
    @Override
    public ScilabType encode(TextBlock from, ScilabType element) {
        return null;
    }

}
