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
