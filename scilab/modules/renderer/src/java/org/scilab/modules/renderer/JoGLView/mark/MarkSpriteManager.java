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
import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.util.HashMap;
import java.util.Map;

/**
 *
 * This class maintains a mark sprite for graphic entities.
 *
 * @author Pierre Lando
 */
public class MarkSpriteManager {

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
    }


    /**
     * Return the mark sprite for the given contoured object.
     *
     * @param contouredObject the given contoured object.
     * @param colorMap the current color map.
     * @return the mark sprite for the given contoured object.
     */
    public Sprite getMarkSprite(ContouredObject contouredObject, ColorMap colorMap) {
        String id = contouredObject.getIdentifier();
        Sprite sprite = spriteMap.get(id);
        if (sprite == null) {
            sprite = MarkSpriteFactory.getMarkSprite(spriteManager, contouredObject.getMark(), colorMap);
            spriteMap.put(id, sprite);
        }
        return sprite;
    }

    /**
     * Return the mark sprite for the given contoured object.
     * @param id the given contoured object's identifier.
     * @param mark the given contoured object's Mark.
     * @param colorMap the current color map.
     * @return the mark sprite for the given contoured object.
     */
    public Sprite getMarkSprite(String id, Mark mark, ColorMap colorMap) {
        Sprite sprite = spriteMap.get(id);
        if (sprite == null) {
            sprite = MarkSpriteFactory.getMarkSprite(spriteManager, mark, colorMap);
            spriteMap.put(id, sprite);
        }
        return sprite;
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {

        /**
         * If the Mark properties have changed.
         * Dispose the involved mark sprite.
         */
        if (property.startsWith(GraphicObjectProperties.__GO_MARK__)) {
            dispose(id);
        }
    }

    /**
     * Dispose the sprite corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(String id) {
        Sprite sprite = spriteMap.get(id);
        spriteManager.dispose(sprite);
        spriteMap.remove(id);
    }

    /**
     * Dispose all the mark sprite.
     */
    public void disposeAll() {
        spriteManager.dispose(spriteMap.values());
        spriteMap.clear();
    }
}
