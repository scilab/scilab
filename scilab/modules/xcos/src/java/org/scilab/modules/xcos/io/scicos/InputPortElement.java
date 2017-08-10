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

import java.rmi.server.UID;
import java.util.List;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;

/**
 * Perform an input port transformation between Scicos and Xcos.
 *
 * On this element we doesn't validate the Scicos values has they have been already checked on the {@link BlockElement}.
 */
public final class InputPortElement extends AbstractElement<InputPort> {
    protected static final List<String> DATA_FIELD_NAMES = BlockElement.DATA_FIELD_NAMES;

    protected static final List<String> GRAPHICS_DATA_FIELD_NAMES_FULL = BlockGraphicElement.DATA_FIELD_NAMES_FULL;
    protected static final List<String> MODEL_DATA_FIELD_NAMES = BlockModelElement.DATA_FIELD_NAMES;

    private static final int GRAPHICS_INDEX = DATA_FIELD_NAMES.indexOf("graphics");
    private static final int MODEL_INDEX = DATA_FIELD_NAMES.indexOf("model");

    private static final int GRAPHICS_PIN_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("pin");
    private static final int GRAPHICS_INIMPL_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("in_implicit");
    private static final int GRAPHICS_INSTYLE_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("in_style");
    private static final int GRAPHICS_INLABEL_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("in_label");

    private static final int MODEL_IN_DATALINE_INDEX = MODEL_DATA_FIELD_NAMES.indexOf("in");
    private static final int MODEL_IN_DATACOL_INDEX = MODEL_DATA_FIELD_NAMES.indexOf("in2");
    private static final int MODEL_IN_DATATYPE_INDEX = MODEL_DATA_FIELD_NAMES.indexOf("intyp");

    private static final String EXPLICIT = "E";
    private static final String IMPLICIT = "I";

    /** Mutable fields to easily get the data through methods */
    private ScilabMList data;
    private final ScilabMList graphics;
    private final ScilabMList model;

    private int alreadyDecodedCount;
    private boolean allColumnsAreZeros = true;

    /**
     * Default constructor
     *
     * @param element
     *            the Scicos block parameters used by this element.
     */
    public InputPortElement(final JavaController controller, ScilabType element) {
        super(controller);

        data = (ScilabMList) element;
        graphics = (ScilabMList) data.get(GRAPHICS_INDEX);
        model = (ScilabMList) data.get(MODEL_INDEX);
    }

    /**
     * @return the number of input port for the element.
     */
    public int getNumberOfInputPort() {
        return model.get(MODEL_IN_DATALINE_INDEX).getHeight();
    }

    /**
     * Decode Scicos element into the block.
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
    public InputPort decode(ScilabType element, InputPort into) throws ScicosFormatException {

        InputPort port;
        data = (ScilabMList) element;

        port = allocatePort();
        port.setId(new UID().toString());

        port = beforeDecode(element, port);

        decodeModel(port);
        decodeGraphics(port);

        // Update the index counter
        alreadyDecodedCount++;

        port = afterDecode(element, port);

        return port;
    }

    /**
     * Allocate a port according to the explicit/implicit values.
     *
     * @return a new typed port
     */
    private InputPort allocatePort() {
        InputPort ret;

        /*
         * backward compatibility, use explicit as default.
         */
        if (graphics.size() <= GRAPHICS_INIMPL_INDEX) {
            return new ExplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null);
        }
        ScilabType inImpl = graphics.get(GRAPHICS_INIMPL_INDEX);

        /*
         * backward compatibility, use explicit as default.
         */
        if (isEmptyField(inImpl)) {
            return new ExplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null);
        }

        final ScilabString inImplicit = (ScilabString) inImpl;

        /*
         * backward compatibility, use explicit as default.
         */
        if (isEmptyField(inImplicit)) {
            return new ExplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null);
        }

        final boolean isColumnDominant = inImplicit.getHeight() >= inImplicit.getWidth();
        final int[] indexes = getIndexes(alreadyDecodedCount, isColumnDominant);
        final String[][] inimpl = inImplicit.getData();

        // can we safely access the indexed data ?
        final boolean isSet = canGet(inImplicit, indexes);

        /*
         * when the type is set, create a new port instance; create an explicit typed port otherwise.
         */
        if (isSet && inimpl[indexes[0]][indexes[1]].equals(EXPLICIT)) {
            ret = new ExplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null);
        } else if (isSet && inimpl[indexes[0]][indexes[1]].equals(IMPLICIT)) {
            ret = new ImplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null);
        } else {
            // when not specified, use explicit
            ret = new ExplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null);
        }

        return ret;
    }

    /**
     * Fill the port with the parameters from the model structure.
     *
     * @param port
     *            the target instance
     */
    private void decodeModel(InputPort port) {
        ScilabDouble dataLines = (ScilabDouble) model.get(MODEL_IN_DATALINE_INDEX);
        ScilabDouble dataColumns = (ScilabDouble) model.get(MODEL_IN_DATACOL_INDEX);
        ScilabDouble dataType = (ScilabDouble) model.get(MODEL_IN_DATATYPE_INDEX);

        // The number of row of the port
        int nbLines;
        if (dataLines.getRealPart() != null) {
            nbLines = (int) dataLines.getRealPart()[alreadyDecodedCount][0];
        } else {
            nbLines = 1;
        }

        // The number of column of the port
        int nbColumns;
        if (dataColumns.getRealPart() != null) {
            try {
                nbColumns = (int) dataColumns.getRealPart()[alreadyDecodedCount][0];
            } catch (ArrayIndexOutOfBoundsException e) {
                nbColumns = 1;
            }
        } else {
            nbColumns = 1;
        }

        // port scilab type
        int type;
        if (dataType.getRealPart() != null) {
            try {
                type = (int) dataType.getRealPart()[alreadyDecodedCount][0];
            } catch (ArrayIndexOutOfBoundsException e) {
                type = 1;
            }
        } else {
            type = 1;
        }

        VectorOfInt v = new VectorOfInt(3);
        v.set(0, nbLines);
        v.set(1, nbColumns);
        v.set(2, type);
        controller.setObjectProperty(port.getUID(), port.getKind(), ObjectProperties.DATATYPE, v);
    }

    /**
     * Fill the port with the parameters from the graphics structure.
     *
     * @param port
     *            the target instance
     */
    private void decodeGraphics(InputPort port) {
        // protection against previously stored blocks
        if (graphics.size() > GRAPHICS_INSTYLE_INDEX && !isEmptyField(graphics.get(GRAPHICS_INSTYLE_INDEX))) {
            final ScilabString styles = (ScilabString) graphics.get(GRAPHICS_INSTYLE_INDEX);

            boolean isColumnDominant = styles.getHeight() >= styles.getWidth();
            int[] indexes = getIndexes(alreadyDecodedCount, isColumnDominant);

            if (canGet(styles, indexes)) {
                final String style;

                style = styles.getData()[indexes[0]][indexes[1]];
                port.setStyle(new StyleMap(port.getStyle()).putAll(style).toString());
            }
        }

        // protection against previously stored blocks
        if (graphics.size() > GRAPHICS_INLABEL_INDEX && !isEmptyField(graphics.get(GRAPHICS_INLABEL_INDEX))) {
            final ScilabString labels = (ScilabString) graphics.get(GRAPHICS_INLABEL_INDEX);

            boolean isColumnDominant = labels.getHeight() >= labels.getWidth();
            int[] indexes = getIndexes(alreadyDecodedCount, isColumnDominant);

            if (canGet(labels, indexes)) {
                final String label = labels.getData()[indexes[0]][indexes[1]];

                if (label != null) {
                    port.setValue(label);
                } else {
                    port.setValue("");
                }
            }
        }
    }

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
        return type.equals(DATA_FIELD_NAMES.get(0)) && getNumberOfInputPort() > alreadyDecodedCount;
    }
}
