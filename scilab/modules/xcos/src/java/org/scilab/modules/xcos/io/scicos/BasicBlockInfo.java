/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.ArrayList;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxICell;

/**
 * Convert BasicBlock pure objects to a mixed BasicBlock objects (update the
 * scicos information)
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
    protected static ScilabDouble getAllLinkId(List <? extends BasicPort > ports) {
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
     * Get all the port data lines.
     *
     * @param ports
     *            the ports
     * @return array of ports data lines
     */
    protected static ScilabDouble getAllPortsDataLines(List <? extends BasicPort > ports) {
        if (ports.isEmpty()) {
            return new ScilabDouble();
        }
        double[][] data = new double[ports.size()][1];
        for (int i = 0; i < ports.size(); ++i) {
            data[i][0] = ((BasicPort) ports.get(i)).getDataLines();
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
                if (revert) {
                    data.add(0, (T) cell);
                } else {
                    data.add((T) cell);
                }
            }
        }

        return data;
    }

    /**
     * Get the n-th port for a n position.
     *
     * This method assume that the port are sorted and that the
     * {@link BasicPort#getOrdering()} is filled with the right value.
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

        final int childrenCount = block.getChildCount();
        for (int i = 0; i < childrenCount; ++i) {
            final mxICell cell = block.getChildAt(i);
            if (cell instanceof BasicPort) {
                final BasicPort p = ((BasicPort) cell);
                if (p.getOrdering() == position) {
                    data.add(p);
                }
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
