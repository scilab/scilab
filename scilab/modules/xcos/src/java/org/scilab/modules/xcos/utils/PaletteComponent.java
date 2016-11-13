/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.utils;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;

/**
 * Customize the default component
 *
 * @see java.awt.Component
 */
public class PaletteComponent extends ScilabComponent {

    private static final long serialVersionUID = 1592816691330208090L;

    /**
     * Construct the component with the associated graph
     *
     * @param graph
     *            The associated graph
     */
    public PaletteComponent(XcosDiagram graph) {
        super(graph);

        addComponentListener(new ComponentListener() {

            @Override
            public void componentShown(ComponentEvent arg0) {
                if (getGraph() instanceof PaletteDiagram) {
                    ((PaletteDiagram) getGraph()).updateDiagram(getSize()
                            .getWidth());
                }
            }

            @Override
            public void componentResized(ComponentEvent arg0) {
                if (getGraph() instanceof PaletteDiagram) {
                    ((PaletteDiagram) getGraph()).updateDiagram(getSize()
                            .getWidth());
                }
            }

            @Override
            public void componentMoved(ComponentEvent arg0) {
            }

            @Override
            public void componentHidden(ComponentEvent arg0) {
            }
        });
    }

    /**
     * @return The filename
     */
    @Override
    public String toString() {
        if (getGraph() instanceof PaletteDiagram) {
            String fileName = ((PaletteDiagram) getGraph()).getName();
            return fileName.substring(0, fileName.lastIndexOf('.'));
        } else {
            return super.toString();
        }
    }
}
