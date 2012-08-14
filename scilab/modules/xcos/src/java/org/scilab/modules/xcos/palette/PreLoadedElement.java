/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import static java.util.Arrays.asList;

import java.io.File;
import java.io.IOException;
import java.util.List;

import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.io.scicos.AbstractElement;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.palette.model.VariablePath;

/**
 * Decode a palette into a {@link PreLoaded} palette.
 */
public class PreLoadedElement extends AbstractElement<PreLoaded> {
    private static final List<String> DATA_FIELD_NAMES = asList("palette", "name", "blockNames", "icons", "style");

    /** Mutable field to easily get the data through methods */
    private ScilabTList data;

    /**
     * Default constructor
     */
    public PreLoadedElement() {
    }

    /**
     * Check the type name compatibility.
     *
     * @param element
     *            the current element
     * @return <code>true</code> if the data can be decoded by this instance,
     *         <code>false</code> otherwise.
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabTList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }

    /**
     * Decode the current element on the into argument.
     *
     * @param element
     *            the Scilab data
     * @param into
     *            the target of the decoding (may be null)
     * @return the filled instance
     * @throws ScicosFormatException
     *             on decoding error
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    @Override
    public PreLoaded decode(ScilabType element, PreLoaded into) throws ScicosFormatException {
        data = (ScilabTList) element;
        PreLoaded palette = into;

        validate();

        if (into == null) {
            palette = new PreLoaded();
        }

        palette.setEnable(true);

        /*
         * get the data
         */

        int field = 1;
        String[][] name = ((ScilabString) data.get(field)).getData();

        field++;
        String[][] blockNames = ((ScilabString) data.get(field)).getData();

        field++;
        String[][] icons = ((ScilabString) data.get(field)).getData();

        /*
         * Configure the current palette instance
         */

        palette.setName(name[0][0]);

        final List<PaletteBlock> paletteBlocks = palette.getBlock();
        for (int i = 0; i < blockNames.length; i++) {
            for (int j = 0; j < blockNames[i].length; j++) {
                PaletteBlock current = new PaletteBlock();
                current.setName(blockNames[i][j]);

                VariablePath iconPath = new VariablePath();
                iconPath.setVariable(null);
                File icon = new File(icons[i][j]);
                if (!icon.exists()) {
                    try {
                        Palette.generatePaletteIcon(blockNames[i][j], icons[i][j]);
                    } catch (IOException e) {
                        throw new WrongTypeException(e);
                    }
                }
                iconPath.setPath(icons[i][j]);
                current.setIcon(iconPath);

                paletteBlocks.add(current);
            }
        }

        return palette;
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
         * Checking the TList header
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

        // the second field must contain the palette name.
        field++;
        if (!(data.get(field) instanceof ScilabString) || data.get(field).getWidth() != 1 || data.get(field).getHeight() != 1) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the third field must contains the block name (row column)
        field++;
        if (!(data.get(field) instanceof ScilabString) || data.get(field).getWidth() != 1) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the fourth field must contains the block icon path (row column)
        field++;
        if (!(data.get(field) instanceof ScilabString) || data.get(field).getWidth() != 1) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the fifth field is handled by the StyleElement decoder.
    }

    /**
     * Not implemented yet, always return null.
     *
     * @param from
     *            the source
     * @param element
     *            the destination
     * @return null
     * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object,
     *      org.scilab.modules.types.ScilabType)
     */
    @Override
    @Deprecated
    public ScilabType encode(PreLoaded from, ScilabType element) {
        return null;
    }

}
