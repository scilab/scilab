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

package org.scilab.modules.xcos.block.listener;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.input.InputPort;

import com.mxgraph.model.mxICell;

/**
 * Change the port label on ipar change.
 *
 * This listener must be installed on "integerParameters" property.
 */
@SuppressWarnings(value = { "serial" })
public class ProdPortLabelingListener implements PropertyChangeListener,
    Serializable {

    private static final String NOT_PRINTED_LABEL = "\u00d7";
    private static ProdPortLabelingListener instance;

    /**
     * Default constructor
     */
    public ProdPortLabelingListener() {
    }

    /**
     * @return the shared instance
     */
    public static ProdPortLabelingListener getInstance() {
        if (instance == null) {
            instance = new ProdPortLabelingListener();
        }
        return instance;
    }

    /**
     * Change the label of the port according to the integer parameters
     * property.
     *
     * @param evt
     *            the event
     * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
     */
    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        final BasicBlock source = (BasicBlock) evt.getSource();
        final ScilabDouble data = (ScilabDouble) evt.getNewValue();

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
        // FIXME
        //        for (InputPort port : ports) {
        //            final double gain;
        //
        //            if (data.isEmpty()
        //                    || data.getRealPart().length < port.getOrdering()) {
        //                gain = 1;
        //            } else {
        //                gain = data.getRealPart()[port.getOrdering() - 1][0];
        //            }
        //
        //            port.setValue(getLabel(gain));
        //        }

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
