/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

import java.util.ArrayList;
import java.util.Collections;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxICell;
import java.util.HashMap;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Convert BasicBlock pure objects to a mixed BasicBlock objects (update the scicos information)
 */
public final class BasicBlockInfo {
    /**
     * This class is a static singleton.
     */
    private BasicBlockInfo() {
    }

    /**
     * Get all the ids.
     *
     * @param ports
     *            the ports
     * @return array of links id
     */
    protected static ScilabDouble getAllLinkId(List<? extends BasicPort> ports) {
        if (ports.isEmpty()) {
            return new ScilabDouble();
        }

        double[][] data = new double[ports.size()][1];
        for (int i = 0; i < ports.size(); ++i) {
            final BasicPort p = ports.get(i);

            if (p.getEdgeCount() == 1) {
                data[i][0] = ((BasicLink) p.getEdgeAt(0)).getOrdering();
            } else {
                data[i][0] = 0;
            }
        }

        return new ScilabDouble(data);
    }

    /**
     * Get the block' children list of the specified type.
     *
     * @param block
     *            the block we are working on
     * @param revert
     *            True if the returned list have to be in a reversed order or
     * @param type
     *            the class instance to search for
     * @param <T>
     *            The type to search for.
     * @return control ports of given block
     */
    @SuppressWarnings("unchecked")
    public static <T extends BasicPort> List<T> getAllTypedPorts(BasicBlock block, boolean revert, Class<T> type) {
        final List<T> data = new ArrayList<T>();
        if (block == null) {
            return data;
        }
        final int childrenCount = block.getChildCount();

        /*
         * type must be first child of BasicPort
         */
        Class<T> realType = type;
        while (!realType.getSuperclass().equals(BasicPort.class)) {
            realType = (Class<T>) realType.getSuperclass();
        }

        for (int i = 0; i < childrenCount; ++i) {
            final mxICell cell = block.getChildAt(i);
            if (realType.isInstance(cell)) {
                // There we are sure that the cell is an instance of the type
                // class. Thus we can safely cast it and add it to the result
                // vector.
                data.add((T) cell);
            }
        }

        if (revert) {
            Collections.reverse(data);
        }

        return data;
    }

    /**
     * Get the n-th port for a n position.
     *
     * This method assume that the port are sorted and that the {@link BasicPort#getOrdering()} is filled with the right value.
     *
     * @param block
     *            the block
     * @param position
     *            the position to look for
     * @return a list of applicable ports
     */
    public static List<BasicPort> getAllPortsAtPosition(final BasicBlock block, final int position) {
        final List<BasicPort> data = new ArrayList<BasicPort>();
        if (block == null) {
            return data;
        }

        HashMap<Class< ? extends BasicPort>, Integer> counterMap = new HashMap<>();

        final int childrenCount = block.getChildCount();
        for (int i = 0; i < childrenCount; ++i) {
            final mxICell cell = block.getChildAt(i);

            // avoid generic class comparaison because inputs might be explicit or implicit
            Class< ? extends BasicPort> klass;
            if (cell instanceof InputPort) {
                klass = InputPort.class;
            } else if (cell instanceof OutputPort) {
                klass = OutputPort.class;
            } else if (cell instanceof ControlPort) {
                klass = ControlPort.class;
            } else if (cell instanceof CommandPort) {
                klass = CommandPort.class;
            } else {
                klass = null;
            }

            if (klass != null) {
                final BasicPort p = ((BasicPort) cell);

                // order the ports per kind using a locally allocated map (do not call Controller nor use an ordering field)
                Integer counter = counterMap.getOrDefault(klass, 1);
                if (counter == position) {
                    data.add(p);
                }

                counterMap.put(p.getClass(), counter + 1);
            }
        }

        return data;
    }

    /**
     * Get the associated port ordered by orientation.
     *
     * @param block
     *            The block we are working on
     * @return Lists of ports where key are BasicPort.Orientation
     */
    public static Map<Orientation, List<BasicPort>> getAllOrientedPorts(BasicBlock block) {
        EnumMap<Orientation, List<BasicPort>> map = new EnumMap<Orientation, List<BasicPort>>(Orientation.class);
        List<BasicPort> northPorts = new ArrayList<BasicPort>();
        List<BasicPort> southPorts = new ArrayList<BasicPort>();
        List<BasicPort> eastPorts = new ArrayList<BasicPort>();
        List<BasicPort> westPorts = new ArrayList<BasicPort>();

        final int childrenCount = block.getChildCount();
        for (int i = 0; i < childrenCount; ++i) {
            if (block.getChildAt(i) instanceof BasicPort) {
                BasicPort port = (BasicPort) block.getChildAt(i);
                switch (port.getOrientation()) {
                    case NORTH:
                        northPorts.add(port);
                        break;
                    case SOUTH:
                        southPorts.add(port);
                        break;
                    case EAST:
                        eastPorts.add(port);
                        break;
                    case WEST:
                        westPorts.add(port);
                        break;
                    default:
                        break;
                }
            }
        }

        map.put(Orientation.NORTH, northPorts);
        map.put(Orientation.SOUTH, southPorts);
        map.put(Orientation.EAST, eastPorts);
        map.put(Orientation.WEST, westPorts);

        return map;
    }
}
