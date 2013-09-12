/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Compare two diagrams per kind.
 *
 * Is used to sort diagram list per distance to the root diagram.
 */
public final class DiagramComparator implements Comparator<XcosDiagram> {

    /*
     * Static API
     */

    public static void sort(List<XcosDiagram> diagrams) {
        Collections.sort(diagrams, getInstance());
    }

    public static void reverse(List<XcosDiagram> diagrams) {
        Collections.sort(diagrams, Collections.reverseOrder(getInstance()));
    }

    /*
     * Singleton
     */
    private static final DiagramComparator instance = new DiagramComparator();

    public static final DiagramComparator getInstance() {
        return instance;
    }

    private DiagramComparator() {
    }

    @Override
    public int compare(final XcosDiagram o1, final XcosDiagram o2) {
        int o1Relative = 0;
        for (XcosDiagram graph1 = o1; graph1 instanceof SuperBlockDiagram && ((SuperBlockDiagram) graph1).getContainer() != null; graph1 = ((SuperBlockDiagram) graph1)
                .getContainer().getParentDiagram()) {
            o1Relative++;
        }

        int o2Relative = 0;
        for (XcosDiagram graph2 = o2; graph2 instanceof SuperBlockDiagram && ((SuperBlockDiagram) graph2).getContainer() != null; graph2 = ((SuperBlockDiagram) graph2)
                .getContainer().getParentDiagram()) {
            o2Relative++;
        }

        return o1Relative - o2Relative;
    }

}
