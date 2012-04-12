/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Pierre LANDO
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
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

    void visit(Grayplot grayplot) throws ObjectRemovedException;

    void visit(Matplot matplot) throws ObjectRemovedException;

    void visit(Label label);

    void visit(Legend legend);

    void visit(Polyline polyline) throws ObjectRemovedException;

    void visit(Rectangle rectangle);

    void visit(Fac3d fac3d) throws ObjectRemovedException;

    void visit(Plot3d plot3d) throws ObjectRemovedException;

    void visit(Text text);

    void visit(Arrow arrow);

    void visit(Champ champ) throws ObjectRemovedException;

    void visit(Segs segs) throws ObjectRemovedException;
}
