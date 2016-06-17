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

package org.scilab.modules.xcos.palette;

import static java.util.Arrays.asList;

import java.io.File;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.io.scicos.AbstractElement;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
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
        super(null);
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
                    /*
                     * Invoke scilab to create a new palette icon later (eg. not
                     * on this thread)
                     */
                    final StringBuilder str = new StringBuilder();
                    str.append(ScilabInterpreterManagement.buildCall(ScilabDirectHandler.BLK + "=" + blockNames[i][j], "define"));
                    str.append("; ");
                    str.append(ScilabInterpreterManagement.buildCall("xcosPalGenerateIcon", ScilabDirectHandler.BLK.toCharArray(), icons[i][j]));
                    str.append("; ");
                    ScilabInterpreterManagement.putCommandInScilabQueue(str.toString());
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
     * Encode the Preload as a palette
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
    public ScilabType encode(PreLoaded from, ScilabType element) {
        data = (ScilabTList) element;

        int field = 0;

        if (data == null) {
            data = allocateElement();
        }

        field++;
        data.set(field, new ScilabString(from.getName()));

        final List<PaletteBlock> blocks = from.getBlock();
        final String[][] blockNames = new String[blocks.size()][1];
        final String[][] icons = new String[blocks.size()][1];
        final String[][] styles = new String[blocks.size()][1];

        final Map<String, Map<String, Object>> styleMap = Xcos.getInstance().getStyleSheet().getStyles();
        for (int i = 0; i < blocks.size(); i++) {
            final PaletteBlock current = blocks.get(i);
            blockNames[i][0] = current.getName();
            icons[i][0] = current.getIcon().getEvaluatedPath();
            final StringBuilder str = new StringBuilder();
            final Map<String, Object> m = styleMap.get(current.getName());
            if (m != null) {
                for (Entry<String, Object> e : m.entrySet()) {
                    str.append(e.getKey()).append('=').append(e.getValue()).append(';');
                }
            }
            styles[i][0] = str.toString();
        }

        field++;
        data.set(field, new ScilabString(blockNames));

        field++;
        data.set(field, new ScilabString(icons));

        field++;
        data.set(field, new ScilabString(styles));

        return data;
    }

    /**
     * Allocate a new element
     *
     * @return the new element
     */
    private ScilabTList allocateElement() {
        ScilabTList data = new ScilabTList(DATA_FIELD_NAMES.toArray(new String[DATA_FIELD_NAMES.size()]));
        data.add(new ScilabString()); // name
        data.add(new ScilabString()); // blockNames
        data.add(new ScilabString()); // icons
        data.add(new ScilabString()); // style
        return data;
    }
}
