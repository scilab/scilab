/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
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

package org.scilab.modules.xcos.palette.view;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Insets;

// CHECKSTYLE:OFF
/**
 * A modified version of FlowLayout that allows containers using this Layout to
 * behave in a reasonable manner when placed inside a JScrollPane
 *
 * @author Babu Kalakrishnan
 * @see http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=5082531
 * @see http ://www.javakb.com/Uwe/Forum.aspx/java-gui/1904/Flowlayout-JPanel-
 *      and-JScrollPane-Scrolling-vertically-impossible
 */
@SuppressWarnings(value = { "serial" })
public class ModifiedFlowLayout extends FlowLayout {
    public ModifiedFlowLayout() {
        super();
    }

    public ModifiedFlowLayout(int align) {
        super(align);
    }

    public ModifiedFlowLayout(int align, int hgap, int vgap) {
        super(align, hgap, vgap);
    }

    @Override
    public Dimension minimumLayoutSize(Container target) {
        return computeSize(target, false);
    }

    @Override
    public Dimension preferredLayoutSize(Container target) {
        return computeSize(target, true);
    }

    private Dimension computeSize(Container target, boolean minimum) {
        synchronized (target.getTreeLock()) {
            int hgap = getHgap();
            int vgap = getVgap();
            int w = target.getWidth();

            // Let this behave like a regular FlowLayout (single row)
            // if the container hasn't been assigned any size yet
            if (w == 0) {
                w = Integer.MAX_VALUE;
            }

            Insets insets = target.getInsets();
            if (insets == null) {
                insets = new Insets(0, 0, 0, 0);
            }
            int reqdWidth = 0;

            int maxwidth = w - (insets.left + insets.right + hgap * 2);
            int n = target.getComponentCount();
            int x = 0;
            int y = insets.top;
            int rowHeight = 0;

            for (int i = 0; i < n; i++) {
                Component c = target.getComponent(i);
                if (c.isVisible()) {
                    Dimension d = minimum ? c.getMinimumSize()
                                  : c.getPreferredSize();
                    if ((x == 0) || ((x + d.width) <= maxwidth)) {
                        if (x > 0) {
                            x += hgap;
                        }
                        x += d.width;
                        rowHeight = Math.max(rowHeight, d.height);
                    } else {
                        x = d.width;
                        y += vgap + rowHeight;
                        rowHeight = d.height;
                    }
                    reqdWidth = Math.max(reqdWidth, x);
                }
            }
            y += rowHeight;

            // fill up the empty space
            int emptySpace = target.getParent().getParent().getHeight() - y;
            emptySpace = emptySpace < 0 ? 0 : emptySpace;
            y += emptySpace - 2;

            return new Dimension(reqdWidth + insets.left + insets.right, y);
        }
    }
}
// CHECKSTYLE:ON
