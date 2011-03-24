package org.scilab.modules.graphic_objects.graphicObject;

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
public interface IVisitor {

    void visit(Axes axes);

    void visit(Arc arc);

    void visit(Axis axis);

    void visit(Compound compound);

    void visit(Fec fec);

    void visit(Figure figure);

    void visit(Grayplot grayplot);

    void visit(Matplot matplot);

    void visit(Label label);

    void visit(Legend legend);

    void visit(Polyline polyline);

    void visit(Rectangle rectangle);

    void visit(Fac3d fac3d);

    void visit(Plot3d plot3d);

    void visit(Text text);

    void visit(Arrow arrow);

    void visit(Champ champ);

    void visit(Segs segs);
}
