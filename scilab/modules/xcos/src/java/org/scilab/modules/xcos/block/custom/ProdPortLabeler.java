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

package org.scilab.modules.xcos.block.custom;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.input.InputPort;

import com.mxgraph.model.mxICell;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;

/**
 * Change the port label accordingly to the ipar values.
 */
public class ProdPortLabeler {

    private static final String NOT_PRINTED_LABEL = "\u00d7";
    private static ProdPortLabeler instance;

    /**
     * Default constructor
     */
    public ProdPortLabeler() {
    }

    /**
     * @return the shared instance
     */
    public static ProdPortLabeler getInstance() {
        if (instance == null) {
            instance = new ProdPortLabeler();
        }
        return instance;
    }

    /**
     * Change the label of the port according to the integer parameters
     * property.
     *
     * @param source the source of the block
     */
    public void updateLabel(final BasicBlock source) {
        /**
         * Get the input port children
         */
        final List<InputPort> ports = new ArrayList<InputPort>();
        for (int i = 0; i < source.getChildCount(); i++) {
            final mxICell port = source.getChildAt(i);

            if (port instanceof InputPort) {
                ports.add((InputPort) port);
            }
        }

        /**
         * Set the ports labels
         */
        JavaController controller = new JavaController();
        VectorOfInt ipar = new VectorOfInt();
        controller.getObjectProperty(source.getUID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);

        for (int i = 0; i < ports.size(); i++) {
            final int gain;

            if (i < ipar.size()) {
                gain = ipar.get(i);
            } else {
                gain = 1;
            }

            ports.get(i).setValue(getLabel(gain));
        }

        /**
         * Check if all the values are equal to the default one.
         */
        if (!hasDefaultValue(ports)) {
            return;
        }

        /**
         * When all values are equal to the default one, set it to the block
         * and hide the children.
         */
        source.setValue(NOT_PRINTED_LABEL);
        for (InputPort port : ports) {
            port.setValue("");
        }
    }

    /**
     * Has all the ports have the default value ?
     *
     * @param ports
     *            the ports list
     * @return true if they all have the default values
     */
    private boolean hasDefaultValue(final List<InputPort> ports) {
        boolean allPortIsDefaultLabel = true;
        for (InputPort port : ports) {
            if (port.getValue() instanceof String) {
                String current = port.getValue().toString();
                if (!NOT_PRINTED_LABEL.equals(current)) {
                    allPortIsDefaultLabel = false;
                    break;
                }
            }
        }
        return allPortIsDefaultLabel;
    }

    /**
     * Return the symbol for the gain value
     *
     * @param gain
     *            the current gain
     * @return A label representing the gain
     */
    private String getLabel(double gain) {
        if (gain > 0) {
            return NOT_PRINTED_LABEL;
        } else {
            return "\u00f7";
        }
    }
}
