/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.textObject.Text;

import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.text.TextManager;

/**
 * This is a static class to access the renderer module directly
 * from C/C++ code through JNI.
 * See SCI/modules/renderer/src/jni/renderer.giws.xml for other details.
 **/
public final class CallRenderer {
	
    /**
     * Start an interactive zoom.
     * @param id the uid of the figure where the zoom happen.
     */
    public static void startInteractiveZoom(String id) {
        DrawerVisitor visitor = DrawerVisitor.getVisitor(id);
        if (visitor != null) {
            visitor.getInteractionManager().startInteractiveZoom();
        }
    }
    
    public static double[] clickRubberBox(String id, double initialRect[]) {
        DrawerVisitor visitor = DrawerVisitor.getVisitor(id);
        if (visitor != null) {
            return visitor.getInteractionManager().startClickRubberBox(initialRect);
        }
        return new double[] {-1,-1,-1,-1,-1,-1,-1};
    }
    
    public static double[] dragRubberBox(String id) {
        DrawerVisitor visitor = DrawerVisitor.getVisitor(id);
        if (visitor != null) {
            return visitor.getInteractionManager().startDragRubberBox();
        }
        return new double[] {-1,-1,-1,-1,-1,-1,-1};
    }
    /**
     * Updates the coordinate transformation of the Axes object given by the identifier.
     * @param id the Axes' identifier.
     */
    public static void updateSubwinScale(String id) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Axes) {
            AxesDrawer.updateAxesTransformation((Axes) object);
        }
    }

    /**
     * Updates the corners of the text object corresponding to the identifier.
     * @param id the text object's identifier.
     */
    public static void updateTextBounds(String id) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Text) {
            TextManager.updateTextCorners((Text) object);
        }
    }

    /**
     * Computes and returns the coordinates of a point projected onto the default 2d view plane.
     * The obtained coordinates correspond to the point's object coordinates in the default 2d view
     * coordinate frame (the point's position being fixed in window coordinates).
     * The projected point's z coordinate is set to 0, as only x and y are relevant.
     * @param id the identifier of the Axes object.
     * @param coords the input object coordinates (3-element array).
     * @return the 2d view coordinates (3-element array).
     */
    public static double[] get2dViewCoordinates(String id, double[] coords) {
        double[] point2d = new double[]{0.0, 0.0, 0.0};

        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Axes) {
            double[] tmp = AxesDrawer.compute2dViewCoordinates((Axes) object, coords);

            point2d[0] = tmp[0];
            point2d[1] = tmp[1];
        }

        return point2d;
    }

    /**
     * Computes and returns the pixel coordinates from a point's coordinates expressed
     * in the default 2d view coordinate frame, using the given Axes.
     * The returned pixel coordinates are expressed in the AWT's 2d coordinate frame.
     * @param id the Axes' identifier.
     * @param coords the 2d view coordinates (3-element array: x, y, z).
     * @return the pixel coordinates (2-element array: x, y).
     */
    public static double[] getPixelFrom2dViewCoordinates(String id, double[] coords) {
        double[] pointPix = new double[]{0.0, 0.0};

        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Axes) {
            double[] tmp = AxesDrawer.computePixelFrom2dViewCoordinates((Axes) object, coords);

            pointPix[0] = tmp[0];
            pointPix[1] = tmp[1];
        }

        return pointPix;
    }

    /**
     * Computes and returns the coordinates of a point projected onto the default 2d view plane
     * from its pixel coordinates, using the given Axes. The pixel coordinates are expressed in
     * the AWT's 2d coordinate frame.
     * The returned point's z component is set to 0, as we only have x and y as an input.
     * @param id the Axes' identifier.
     * @param coords the pixel coordinates (2-element array: x, y).
     * @return the 2d view coordinates (3-element array: x, y, z).
     */
    public static double[] get2dViewFromPixelCoordinates(String id, double[] coords) {
        double[] point2d = new double[]{0.0, 0.0, 0.0};

        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Axes) {
            double[] tmp = AxesDrawer.compute2dViewFromPixelCoordinates((Axes) object, coords);

            point2d[0] = tmp[0];
            point2d[1] = tmp[1];
        }

        return point2d;
    }

    /**
     * Computes and returns the viewing area of the Axes object given by the identifier.
     * The viewing area is defined by its upper-left corner and its dimensions, all values
     * are in pixels. It is expressed in the AWT's 2d coordinate frame.
     * @param id the Axes' identifier.
     * @return the Axes' viewing area (4-element array: x, y, width, height).
     */
    public static double[] getViewingArea(String id) {
        double[] viewingArea = new double[]{0.0, 0.0, 0.0, 0.0};
        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Axes) {
            double[] tmp = AxesDrawer.getViewingArea((Axes) object);

            viewingArea[0] = tmp[0];
            viewingArea[1] = tmp[1];
            viewingArea[2] = tmp[2];
            viewingArea[3] = tmp[3];
        }

        return viewingArea;
    }

}
