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

package org.scilab.modules.renderer.JoGLView.text;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Text;

import java.util.HashMap;
import java.util.Map;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;

/**
 *
 * This class manage scilab text entity drawing.
 *
 *
 * TODO, Manage: {text_box, text_box_mode, auto_dimensionning, clip_state, clip_box}
 *
 *
 * @author Pierre Lando
 */
public class TextManager {

    /**
     * The {@see Map} off existing {@see TextEntity}.
     */
    private final Map<String, Sprite> spriteMap = new HashMap<String, Sprite>();

    /**
     * The used sprite manager.
     */
    private final SpriteManager spriteManager;


    /**
     * Default constructor.
     * @param spriteManager the sprite manager.
     */
    public TextManager(SpriteManager spriteManager) {
        this.spriteManager = spriteManager;
    }

    /**
     * Draw the given Scilab {@see Text} with the given {@see DrawingTools}.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}
     * @param text the given Scilab {@see Text}
     */
    public final void draw(final DrawingTools drawingTools, final ColorMap colorMap, final Text text) {
        Sprite sprite = getSprite(colorMap, text);

        /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in radians. */
        drawingTools.draw(sprite, SpriteAnchorPosition.LOWER_LEFT, new Vector3d(text.getPosition()), -180.0*text.getFontAngle()/Math.PI);
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {
        if (!__GO_POSITION__.equals(property) && !__GO_FONT_ANGLE__.equals(property)) {
            dispose(id);
        }
    }

    /**
     * Return the SciRender {@see Sprite} corresponding to the given Scilab {@see Text}.
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the SciRender {@see Sprite} corresponding to the given Scilab {@see Text}.
     */
    private Sprite getSprite(final ColorMap colorMap, final Text text) {
        Sprite sprite = spriteMap.get(text.getIdentifier());
        if (sprite == null) {
            sprite = createSprite(colorMap, text);
            spriteMap.put(text.getIdentifier(), sprite);
        }
        return sprite;
    }

    /**
     * Create a sprite for the given text object.
     * @param colorMap the current colormap.
     * @param textObject the given text object.
     * @return a new sprite for the given text object.
     */
    private Sprite createSprite(final ColorMap colorMap, final Text textObject) {
        TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(spriteManager, colorMap, textObject);
        Sprite sprite = spriteManager.createRotatableSprite(spriteDrawer.getWidth(), spriteDrawer.getHeight());
        sprite.setDrawer(spriteDrawer);
        return sprite;
    }

    /**
     * Dispose the sprite corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(String id) {
        Sprite sprite = spriteMap.get(id);
        if (sprite != null) {
            spriteManager.dispose(sprite);
            spriteMap.remove(id);
        }
    }

    /**
     * Dispose all the text sprites.
     */
    public void disposeAll() {
        spriteManager.dispose(spriteMap.values());
        spriteMap.clear();
    }
}
