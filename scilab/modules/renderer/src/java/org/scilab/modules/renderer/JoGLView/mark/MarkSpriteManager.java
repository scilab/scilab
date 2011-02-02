/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.mark;

import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.polyline.Polyline;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 *
 * This class maintain a mark sprite for graphic entities.
 *
 * @author Pierre Lando
 */
public class MarkSpriteManager implements GraphicView {

    /**
     * The SciRender sprite manager.
     */
    private final SpriteManager spriteManager;

    /**
     * The sprite map.
     */
    private final Map<String, Sprite> spriteMap = new HashMap<String, Sprite>();


    /**
     * Default constructor.
     * @param spriteManager the sprite manager to use.
     */
    public MarkSpriteManager(SpriteManager spriteManager) {
        this.spriteManager = spriteManager;
        GraphicController.getController().register(this);
    }


    /**
     * Return the mark sprite for the given polyline.
     *
     * @param polyline the given polyline.
     * @param colorMap the current color map.
     * @return the mark sprite for the given polyline.
     */
    public Sprite getMarkSprite(Polyline polyline, ColorMap colorMap) {
        String id = polyline.getIdentifier();
        Sprite sprite = spriteMap.get(id);
        if (sprite == null) {
            sprite = MarkSpriteFactory.getMarkSprite(spriteManager, polyline.getMark(), colorMap);
            spriteMap.put(id, sprite);
        }
        return sprite;
    }

    @Override
    public void updateObject(String id, String property) {

        /**
         * If the Mark properties have changed.
         * Dispose the involved mark sprite.
         */
        if (property.startsWith(GraphicObjectProperties.__GO_MARK__)) {
            dispose(id);
        }

        /**
         * If a colormap have changed.
         * Dispose all involved mark sprite.
         */
        if (property.equals(GraphicObjectProperties.__GO_COLORMAP__)) {

            List<String> toRemove = new ArrayList<String>();

            /**
             * Make list of involved mark sprite.
             */
            for (String  childId : spriteMap.keySet()) {
                String parentFigureID = String.valueOf(GraphicController.getController().getProperty(childId, GraphicObjectProperties.__GO_PARENT_FIGURE__));
                if (id.equals(parentFigureID)) {
                    toRemove.add(childId);
                }
            }

            /**
             * Dispose involved mark sprite.
             */
            for (String childId : toRemove) {
                dispose(childId);
            }
        }
    }

    @Override
    public void deleteObject(String id) {
        dispose(id);
    }

    /**
     * Dispose the sprite corresponding to the given id.
     * @param id the given id.
     */
    private void dispose(String id) {
        Sprite sprite = spriteMap.get(id);
        spriteManager.dispose(sprite);
        spriteMap.remove(id);
    }

    /**
     * Nothing to do.
     */
    @Override
    public void createObject(String id) {
    }
}
