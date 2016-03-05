/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Pierre LANDO
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
package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.datatip.Datatip;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.uicontrol.frame.Frame;
import org.scilab.modules.graphic_objects.vectfield.Arrow;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;

/**
 * @author Pierre Lando
 */
public interface Visitor {

    void visit(Axes axes);

    void visit(Arc arc);

    void visit(Axis axis);

    void visit(Compound compound);

    void visit(Fec fec) throws ObjectRemovedException;

    void visit(Figure figure);

    void visit(Frame frame);

    void visit(Grayplot grayplot) throws ObjectRemovedException;

    void visit(Matplot matplot) throws ObjectRemovedException;

    void visit(Label label);

    void visit(Legend legend);

    void visit(Polyline polyline) throws ObjectRemovedException;

    void visit(Rectangle rectangle);

    void visit(Fac3d fac3d) throws ObjectRemovedException;

    void visit(Plot3d plot3d) throws ObjectRemovedException;

    void visit(Text text);

    void visit(Datatip datatip);

    void visit(Arrow arrow);

    void visit(Champ champ) throws ObjectRemovedException;

    void visit(Segs segs) throws ObjectRemovedException;
}
