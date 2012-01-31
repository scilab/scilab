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

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * This class maintain a set of {@see RulerSpriteManager} for all the {@see Axes} create in the {@see GraphicController}
 * This {@see RulerSpriteManager} are used to draw the rulers of all {@see Axes}.
 *
 * @author Pierre Lando
 */
public class RulerDrawerManager {

    /**
     * Set of properties that affect ruler sprites.
     */
    private static final Set<String> SPRITE_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_FONT_SIZE__,
            GraphicObjectProperties.__GO_FONT_COLOR__,
            GraphicObjectProperties.__GO_FONT_STYLE__,
            GraphicObjectProperties.__GO_FONT_FRACTIONAL__
    ));

    /**
     * Map of up to date {@see RulerSpriteManager}
     * The key are the {@see Axes} id.
     */
    private final Map<String, RulerDrawer> rulerSpriteManagerMap = new HashMap<String, RulerDrawer>();

    /**
     * The {@see SpriteManager} of the current {@see Canvas}.
     */
    private final SpriteManager spriteManager;

    /**
     * Default constructor.
     * @param spriteManager the {@see SpriteManager} of the current {@see Canvas}.
     */
    public RulerDrawerManager(SpriteManager spriteManager) {
        this.spriteManager = spriteManager;
    }

    /**
     * Return the {@see RulerDrawer} for the rulers of the given {@see Axes}.
     * @param axes the given {@see Axes}
     * @return the {@see RulerSpriteManager} for the rulers of the given {@see Axes}.
     */
    public RulerDrawer get(Axes axes) {
        RulerDrawer rulerSpriteManager = rulerSpriteManagerMap.get(axes.getIdentifier());
        if (rulerSpriteManager == null) {
            rulerSpriteManager = new RulerDrawer(spriteManager);
            rulerSpriteManager.setSpriteFactory(new AxesRulerSpriteFactory(axes));
            rulerSpriteManagerMap.put(axes.getIdentifier(), rulerSpriteManager);
        }
        return rulerSpriteManager;
    }


    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {

        /**
         * If update affect {@see Axes} ruler sprites, we clear the corresponding {@see RulerSpriteManager}.
         */
        if (SPRITE_PROPERTIES.contains(property)) {
            dispose(id);
        }
    }

    /**
     * Dispose the {@see RulerSpriteManager} of the given axes.
     * @param id the {@see Axes} id.
     */
    public void dispose(String id) {
        RulerDrawer spriteManager = rulerSpriteManagerMap.get(id);
        if (spriteManager != null) {
            spriteManager.disposeResources();
        }
        rulerSpriteManagerMap.remove(id);
    }

    /**
     * Dispose all the {@see RulerSpriteManager}.
     */
    public void disposeAll() {
        for (RulerDrawer spriteManager : rulerSpriteManagerMap.values()) {
            spriteManager.disposeResources();
        }
        rulerSpriteManagerMap.clear();
    }
}
