/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */
package org.scilab.modules.renderer.JoGLView.mark;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

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
    private final TextureManager textureManager;

    /**
     * The sprite map.
     */
    private final Map<Integer, Texture> spriteMap = new ConcurrentHashMap<Integer, Texture>();


    /**
     * Default constructor.
     * @param textureManager the texture manager to use.
     */
    public MarkSpriteManager(TextureManager textureManager) {
        this.textureManager = textureManager;
    }


    /**
     * Return the mark sprite for the given contoured object.
     *
     * @param contouredObject the given contoured object.
     * @param colorMap the current color map.
     * @return the mark sprite for the given contoured object.
     */
    public Texture getMarkSprite(ContouredObject contouredObject, ColorMap colorMap, Appearance appearance) {
        Integer id = contouredObject.getIdentifier();
        Texture sprite = spriteMap.get(id);
        if (sprite == null) {
            Integer selectedColor = contouredObject.getSelected() ? contouredObject.getSelectedColor() : null;
            sprite = MarkSpriteFactory.getMarkSprite(textureManager, contouredObject.getMark(), selectedColor, colorMap, appearance);
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
    public Texture getMarkSprite(Integer id, Mark mark, ColorMap colorMap, Appearance appearance) {
        Texture sprite = spriteMap.get(id);
        if (sprite == null) {
            sprite = MarkSpriteFactory.getMarkSprite(textureManager, mark, null, colorMap, appearance);
            spriteMap.put(id, sprite);
        }
        return sprite;
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(Integer id, int property) {

        /**
         * If the Mark properties have changed.
         * Dispose the involved mark sprite.
         */
        if (property == GraphicObjectProperties.__GO_MARK_MODE__
                || property == GraphicObjectProperties.__GO_MARK_STYLE__
                || property == GraphicObjectProperties.__GO_MARK_SIZE_UNIT__
                || property == GraphicObjectProperties.__GO_MARK_SIZE__
                || property == GraphicObjectProperties.__GO_MARK_FOREGROUND__
                || property == GraphicObjectProperties.__GO_MARK_BACKGROUND__
                || property == GraphicObjectProperties.__GO_LINE_THICKNESS__
                || property == GraphicObjectProperties.__GO_SELECTED__) {
            dispose(id);
        }
    }

    /**
     * Dispose the sprite corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(Integer id) {
        Texture sprite = spriteMap.get(id);
        textureManager.dispose(sprite);
        spriteMap.remove(id);
    }

    /**
     * Dispose all the mark sprite.
     */
    public void disposeAll() {
        textureManager.dispose(spriteMap.values());
        spriteMap.clear();
    }
}
