/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.graph;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;

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
        Collections.sort(diagrams, new DiagramComparator());
    }

    public static void reverse(List<XcosDiagram> diagrams) {
        Collections.sort(diagrams, Collections.reverseOrder(new DiagramComparator()));
    }

    /*
     * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
     */

    @Override
    public int compare(final XcosDiagram o1, final XcosDiagram o2) {
        long[] o1Parent = new long[] { o1.getUID() };
        long[] o2Parent = new long[] { o2.getUID() };
        JavaController controller = new JavaController();

        int o1Relative = 0;
        while (o1Parent[0] != 0l) {
            o1Relative++;
            controller.getObjectProperty(o1Parent[0], Kind.DIAGRAM, ObjectProperties.PARENT_BLOCK, o1Parent);
        }

        int o2Relative = 0;
        while (o2Parent[0] != 0l) {
            o2Relative++;
            controller.getObjectProperty(o2Parent[0], Kind.DIAGRAM, ObjectProperties.PARENT_BLOCK, o2Parent);
        }

        return o1Relative - o2Relative;
    }

}
