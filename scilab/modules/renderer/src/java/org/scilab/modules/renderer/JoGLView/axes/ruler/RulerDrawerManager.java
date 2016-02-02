/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.texture.TextureManager;
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
class RulerDrawerManager {

    /**
     * Set of properties that affect ruler sprites.
     */
    private static final Set<Integer> SPRITE_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_FONT_SIZE__,
                GraphicObjectProperties.__GO_FONT_COLOR__,
                GraphicObjectProperties.__GO_FONT_STYLE__,
                GraphicObjectProperties.__GO_FONT_FRACTIONAL__,
                GraphicObjectProperties.__GO_X_AXIS_AUTO_TICKS__,
                GraphicObjectProperties.__GO_Y_AXIS_AUTO_TICKS__,
                GraphicObjectProperties.__GO_Z_AXIS_AUTO_TICKS__,
                GraphicObjectProperties.__GO_X_AXIS_FORMAT__,
                GraphicObjectProperties.__GO_Y_AXIS_FORMAT__,
                GraphicObjectProperties.__GO_Z_AXIS_FORMAT__,
                GraphicObjectProperties.__GO_X_AXIS_ST_FACTORS__,
                GraphicObjectProperties.__GO_Y_AXIS_ST_FACTORS__,
                GraphicObjectProperties.__GO_Z_AXIS_ST_FACTORS__,
                GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__,
                GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__,
                GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__,
                GraphicObjectProperties.__GO_DATA_BOUNDS__
            ));

    /**
     * Map of up to date {@see RulerSpriteManager}
     * The key are the {@see Axes} id.
     */
    private final Map<Integer, RulerDrawer[]> rulerSpriteManagerMap = new HashMap<Integer, RulerDrawer[]>();

    /** The {@see TextureManager} of the current {@see Canvas}. */
    private final TextureManager textureManager;

    /**
     * Default constructor.
     * @param textureManager the {@see TextureManager} of the current {@see Canvas}.
     */
    public RulerDrawerManager(TextureManager textureManager) {
        this.textureManager = textureManager;
    }

    /**
     * Return the {@see RulerDrawer} for the rulers of the given {@see Axes}.
     * @param axes the given {@see Axes}
     * @return the {@see RulerSpriteManager} for the rulers of the given {@see Axes}.
     */
    public RulerDrawer[] get(Axes axes) {
        RulerDrawer[] rulerSpriteManager = rulerSpriteManagerMap.get(axes.getIdentifier());
        if (rulerSpriteManager == null) {
            rulerSpriteManager = new RulerDrawer[] {new RulerDrawer(textureManager), new RulerDrawer(textureManager), new RulerDrawer(textureManager)};
            rulerSpriteManager[0].setSpriteFactory(new AxesRulerSpriteFactory(axes, 0));
            rulerSpriteManager[1].setSpriteFactory(new AxesRulerSpriteFactory(axes, 1));
            rulerSpriteManager[2].setSpriteFactory(new AxesRulerSpriteFactory(axes, 2));
            rulerSpriteManagerMap.put(axes.getIdentifier(), rulerSpriteManager);
        }
        return rulerSpriteManager;
    }


    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     * @return true if it is really updated
     */
    public boolean update(Integer id, int property) {

        /**
         * If update affect {@see Axes} ruler sprites, we clear the corresponding {@see RulerSpriteManager}.
         * TODO : optimize by an X, Y, Z auto ticks differentiation.
         */
        if (SPRITE_PROPERTIES.contains(property)) {
            dispose(id);
            return true;
        }

        if (property == GraphicObjectProperties.__GO_X_AXIS_TICKS_LABELS__) {
            RulerDrawer[] rulerSpriteManager = rulerSpriteManagerMap.get(id);
            if (rulerSpriteManager != null) {
                rulerSpriteManager[0].disposeResources();
            }
        } else if (property == GraphicObjectProperties.__GO_Y_AXIS_TICKS_LABELS__) {
            RulerDrawer[] rulerSpriteManager = rulerSpriteManagerMap.get(id);
            if (rulerSpriteManager != null) {
                rulerSpriteManager[1].disposeResources();
            }
        } else if (property == GraphicObjectProperties.__GO_Z_AXIS_TICKS_LABELS__) {
            RulerDrawer[] rulerSpriteManager = rulerSpriteManagerMap.get(id);
            if (rulerSpriteManager != null) {
                rulerSpriteManager[2].disposeResources();
            }
        }

        return false;
    }

    /**
     * Dispose the {@see RulerSpriteManager} of the given axes.
     * @param id the {@see Axes} id.
     */
    public void dispose(Integer id) {
        RulerDrawer[] rulerDrawers = rulerSpriteManagerMap.get(id);
        if (rulerDrawers != null) {
            for (RulerDrawer rulerDrawer : rulerDrawers) {
                if (rulerDrawer != null) {
                    rulerDrawer.disposeResources();
                }
            }
        }
        rulerSpriteManagerMap.remove(id);
    }

    /**
     * Dispose all the {@see RulerSpriteManager}.
     */
    public void disposeAll() {
        for (RulerDrawer[] rulerDrawers : rulerSpriteManagerMap.values()) {
            if (rulerDrawers != null) {
                for (RulerDrawer rulerDrawer : rulerDrawers) {
                    if (rulerDrawer != null) {
                        rulerDrawer.disposeResources();
                    }
                }
            }
        }
        rulerSpriteManagerMap.clear();
    }
}
