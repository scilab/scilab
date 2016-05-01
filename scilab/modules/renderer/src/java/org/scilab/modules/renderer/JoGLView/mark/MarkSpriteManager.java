/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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
    private final Map<Integer, Map<Integer, Texture>> spriteMap = new ConcurrentHashMap<Integer, Map<Integer, Texture>>();

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
        Map<Integer, Texture> sizeMap = spriteMap.get(id);
        Texture sprite = null;
        int MarkSize = contouredObject.getMark().getSize();
        if (sizeMap == null) {
            sizeMap = new  ConcurrentHashMap<Integer, Texture>();
            spriteMap.put(id, sizeMap);
        } else {
            sprite = sizeMap.get(MarkSize);
        }
        if (sprite == null) {
            Integer selectedColor = contouredObject.getSelected() ? contouredObject.getSelectedColor() : null;
            sprite = MarkSpriteFactory.getMarkSprite(textureManager, contouredObject.getMark(), selectedColor, colorMap, appearance);
            sizeMap.put(MarkSize, sprite);
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
        Map<Integer, Texture> sizeMap = spriteMap.get(id);
        Texture sprite = null;
        int markSize = mark.getSize();
        if (sizeMap == null) {
            sizeMap = new  ConcurrentHashMap<Integer, Texture>();
            spriteMap.put(id, sizeMap);
        } else {
            sprite = sizeMap.get(markSize);
        }
        if (sprite == null) {
            sprite = MarkSpriteFactory.getMarkSprite(textureManager, mark, null, colorMap, appearance);
            sizeMap.put(markSize, sprite);
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
                || property == GraphicObjectProperties.__GO_SELECTED__
                || property == GraphicObjectProperties.__GO_COLOR_SET__
                || property == GraphicObjectProperties.__GO_MARK_SIZES__
                || property == GraphicObjectProperties.__GO_MARK_FOREGROUNDS__
                || property == GraphicObjectProperties.__GO_MARK_BACKGROUNDS__) {
            dispose(id);
        }
    }

    /**
     * Dispose the sprite corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(Integer id) {
        Map<Integer, Texture> sizeMap = spriteMap.get(id);
        if (sizeMap != null) {
            textureManager.dispose(sizeMap.values());
            sizeMap.clear();
        }
        spriteMap.remove(id);
    }

    /**
     * Dispose all the mark sprite.
     */
    public void disposeAll() {
        for (Map<Integer, Texture> sizeMap : spriteMap.values()) {
            textureManager.dispose(sizeMap.values());
            sizeMap.clear();
        }
        spriteMap.clear();
    }
}
