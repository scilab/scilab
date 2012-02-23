/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.contouredObject;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.DataManager;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

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
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     */
    public void draw(ContouredObject contouredObject) throws SciRendererException {
        DrawingTools drawingTools = visitor.getDrawingTools();
        ColorMap colorMap = visitor.getColorMap();

        /* Sets the drawn object's identifier as the current one */
        String drawnObjectID = contouredObject.getIdentifier();

        DefaultGeometry triangles = new DefaultGeometry();
        triangles.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
        triangles.setVertices(dataManager.getVertexBuffer(drawnObjectID));
        triangles.setIndices(dataManager.getIndexBuffer(drawnObjectID));
        triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

        /**
         * TODO: use "triangle.edgesIndices".
         */
        DefaultGeometry segments = new DefaultGeometry();
        segments.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        segments.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        segments.setVertices(dataManager.getVertexBuffer(drawnObjectID));
        segments.setWireIndices(dataManager.getWireIndexBuffer(drawnObjectID));
        segments.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

        if (contouredObject.getFillMode()) {
                Appearance trianglesAppearance = new Appearance();
                trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, contouredObject.getBackground()));
                drawingTools.draw(triangles, trianglesAppearance);
        }

        if (contouredObject.getLineMode()) {
                Appearance segmentAppearance = new Appearance();
                segmentAppearance.setLineColor(ColorFactory.createColor(colorMap, contouredObject.getLineColor()));
                segmentAppearance.setLinePattern(contouredObject.getLineStyleAsEnum().asPattern());
                segmentAppearance.setLineWidth(contouredObject.getLineThickness().floatValue());
                drawingTools.draw(segments, segmentAppearance);
        }

        if (contouredObject.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(contouredObject, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(contouredObject.getIdentifier());
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
        }
    }
}
