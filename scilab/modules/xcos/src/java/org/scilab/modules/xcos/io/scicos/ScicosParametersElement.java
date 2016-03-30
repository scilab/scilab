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

import java.beans.PropertyVetoException;
import java.util.List;
import java.util.logging.Logger;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;

/**
 * Default element used to handle Scicos simulation parameters
 */
public final class ScicosParametersElement extends AbstractElement<ScicosParameters> {
    protected static final List<String> DATA_FIELD_NAMES = asList("params", "wpar", "title", "tol", "tf", "context", "void1", "options", "void2", "void3",
            "doc");

    private static final int TOL_INDEX = DATA_FIELD_NAMES.indexOf("tol");
    private static final int TF_INDEX = DATA_FIELD_NAMES.indexOf("tf");
    private static final int CONTEXT_INDEX = DATA_FIELD_NAMES.indexOf("context");

    private static final int TOL_SIZE = 7;

    /**
     * Mutable field which contains the current working data.
     *
     * This field must be modified on each read/write call.
     */
    private ScilabTList data;

    /**
     * Default constructor
     *
     * @param controller
     *            the shared controller
     */
    public ScicosParametersElement(final JavaController controller) {
        super(controller);
    }

    /**
     * Decode the element into the instance
     *
     * @param element
     *            the element to be decoded
     * @param into
     *            the Xcos associated instance
     * @return the modified into parameters
     * @throws ScicosFormatException
     *             on decode error
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
     */
    @Override
    public ScicosParameters decode(ScilabType element, ScicosParameters into) throws ScicosFormatException {
        data = (ScilabTList) element;
        ScicosParameters local = into;

        // Validate the fields
        validate();

        local = beforeDecode(element, local);

        /*
         * fill data, if accessible
         */
        if (local.getKind() == Kind.DIAGRAM) {
            fillWithThirdFields(local);

            try {

                double val = ((ScilabDouble) data.get(TF_INDEX)).getRealPart()[0][0];

                VectorOfDouble v = local.getProperties(controller);
                v.set(ScicosParameters.FINAL_INTEGRATION_TIME, val);
                local.setProperties(controller, v);
            } catch (PropertyVetoException e) {
                Logger.getLogger(ScicosParametersElement.class.getName()).severe(e.toString());
            }
        }

        fillContext(local);

        local = afterDecode(element, local);

        return local;
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

        // wpar
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // title
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // tol
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // tf
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // context
        field++;
        if (!(data.get(field) instanceof ScilabString) && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // void1
        field++;
        if (!isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // options
        field++;
        if (!(data.get(field) instanceof ScilabTList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // void2
        field++;
        if (!isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // void3
        field++;
        if (!isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // doc
        field++;
        if (!(data.get(field) instanceof ScilabList) && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity

    /**
     * Fill the object with the data third field
     *
     * @param into
     *            the current object to put data into.
     */
    private void fillWithThirdFields(ScicosParameters into) {
        final boolean isColumnDominant = data.get(TOL_INDEX).getHeight() >= data.get(TOL_INDEX).getWidth();
        final double[][] realPart = ((ScilabDouble) data.get(TOL_INDEX)).getRealPart();

        // global index used to get the data
        int[] indexes = { 0, 0 };

        VectorOfDouble v = into.getProperties(controller);

        v.set(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE, realPart[indexes[0]][indexes[1]]);
        incrementIndexes(indexes, isColumnDominant);

        v.set(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE, realPart[indexes[0]][indexes[1]]);
        incrementIndexes(indexes, isColumnDominant);

        v.set(ScicosParameters.TOLERANCE_ON_TIME, realPart[indexes[0]][indexes[1]]);
        incrementIndexes(indexes, isColumnDominant);

        v.set(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL, realPart[indexes[0]][indexes[1]]);
        incrementIndexes(indexes, isColumnDominant);

        v.set(ScicosParameters.REAL_TIME_SCALING, realPart[indexes[0]][indexes[1]]);
        incrementIndexes(indexes, isColumnDominant);

        v.set(ScicosParameters.SOLVER, realPart[indexes[0]][indexes[1]]);
        incrementIndexes(indexes, isColumnDominant);

        // Some times the maximum step size may not exist. Catch it.
        try {
            v.set(ScicosParameters.MAXIMUM_STEP_SIZE, realPart[indexes[0]][indexes[1]]);
        } catch (ArrayIndexOutOfBoundsException e) {
            // do nothing as the maximum step size will keep its default value.
        }

        try {
            into.setProperties(controller, v);
        } catch (PropertyVetoException e) {
            Logger.getLogger(ScicosParametersElement.class.getName()).severe(e.toString());
        }
    }

    /**
     * Fill the object with the context (5th) field
     *
     * @param into
     *            the current object to put data into.
     */
    private void fillContext(ScicosParameters into) {
        final ScilabType contextType = data.get(CONTEXT_INDEX);

        /*
         * On an empty context the type is ScilabDouble.
         */
        if (contextType instanceof ScilabDouble) {
            // preserve the default value
            return;
        }

        /*
         * Normal case
         */
        final boolean isColumnDominant = contextType.getHeight() >= contextType.getWidth();
        final String[][] str = ((ScilabString) contextType).getData();
        final int length = contextType.getHeight() + contextType.getWidth() - 1;

        VectorOfString context = new VectorOfString(length);

        int[] indexes = { 0, 0 };
        for (int i = 0; i < length; i++) {
            context.set(i, str[indexes[0]][indexes[1]]);
            incrementIndexes(indexes, isColumnDominant);
        }

        try {
            into.setContext(controller, context);
        } catch (PropertyVetoException e) {
            Logger.getLogger(ScicosParametersElement.class.getName()).severe(e.toString());
        }
    }

    /**
     * Test if the element can be decoded with this instance implementation.
     *
     * @param element
     *            the element
     * @return true when the implementation can be used to decode the element.
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabTList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }
}
