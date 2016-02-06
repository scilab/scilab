/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.renderer.JoGLView.contouredObject;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.DataManager;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;

/**
 * ContouredObjectDrawer class.
 * Utility class used by DrawerVisitor to draw ContouredObjects objects.
 *
 * @author Manuel JULIACHS
 */
public class ContouredObjectDrawer {
    /** The DrawerVisitor used */
    private final DrawerVisitor visitor;

    /** The DataManager used */
    private final DataManager dataManager;

    /** The MarkSpriteManager used */
    private final MarkSpriteManager markManager;

    /**
     * Constructor.
     * @param visitor the DrawerVisitor {@see DrawerVisitor}.
     * @param dataManagerIn the DataManager {@see DataManager}.
     * @param markManagerIn the MarkSpriteManager {@see MarkSpriteManager}.
     */
    public ContouredObjectDrawer(DrawerVisitor visitor, DataManager dataManagerIn, MarkSpriteManager markManagerIn) {
        this.visitor = visitor;
        this.dataManager = dataManagerIn;
        this.markManager = markManagerIn;
    }

    /**
     * Draws the given ContouredObject.
     * @param contouredObject the ContouredObject to draw.
     * @param use2dView a boolean specifying whether the 2D view mode is used or not.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     * @throws ObjectRemovedException
     */
    public void draw(ContouredObject contouredObject, boolean use2dView) throws SciRendererException, ObjectRemovedException, OutOfMemoryException {
        DrawingTools drawingTools = visitor.getDrawingTools();
        ColorMap colorMap = visitor.getColorMap();

        /* Sets the drawn object's identifier as the current one */
        Integer drawnObjectID = contouredObject.getIdentifier();

        DefaultGeometry geometry = new DefaultGeometry();
        geometry.setVertices(dataManager.getVertexBuffer(drawnObjectID));
        geometry.setIndices(dataManager.getIndexBuffer(drawnObjectID));
        geometry.setWireIndices(dataManager.getWireIndexBuffer(drawnObjectID));
        geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

        Appearance appearance = new Appearance();

        if (contouredObject.getFillMode()) {
            geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
            appearance.setFillColor(ColorFactory.createColor(colorMap, contouredObject.getBackground()));
        } else {
            geometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        }

        if (contouredObject.getLineMode()) {
            geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
            appearance.setLineColor(ColorFactory.createColor(colorMap, contouredObject.getLineColor()));
            appearance.setLinePattern(contouredObject.getLineStyleAsEnum().asPattern());
            appearance.setLineWidth(contouredObject.getLineThickness().floatValue());
        } else {
            geometry.setLineDrawingMode(Geometry.LineDrawingMode.NONE);
        }

        if (!use2dView) {
            geometry.setPolygonOffsetMode(true);
        }

        drawingTools.draw(geometry, appearance);

        if (contouredObject.getMarkMode()) {
            Texture texture = markManager.getMarkSprite(contouredObject, colorMap, appearance);
            ElementsBuffer positions = dataManager.getVertexBuffer(contouredObject.getIdentifier());
            drawingTools.draw(texture, AnchorPosition.CENTER, positions);
        }
    }
}
