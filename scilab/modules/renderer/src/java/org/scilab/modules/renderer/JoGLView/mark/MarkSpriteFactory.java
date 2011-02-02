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

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

/**
 * @author Pierre Lando
 */
public class MarkSpriteFactory {

    /**
     * Return a mark sprite corresponding to the given scilab mark.
     *
     *
     * @param spriteManager the current sprite manager.
     * @param mark the scilab mark.
     * @param colorMap the scilab color map.
     * @return a mark sprite corresponding to the given scilab mark.
     */
    public static Sprite getMarkSprite(SpriteManager spriteManager, Mark mark, ColorMap colorMap) {
        int finalSize;

        /**
         * Compute the sprite pixel size.
         */
        if (mark.getMarkSizeUnit() == Mark.MarkSizeUnitType.TABULATED) {
            finalSize = (8 + 2 * mark.getSize());
        } else {
            finalSize = mark.getSize();
        }

        /**
         * Add a margin such that (0, 0) was pixel aligned.
         */
        int margin;
        if (finalSize % 2 == 0) {
            margin = 3;
        } else {
            margin = 2;
        }

        Sprite sprite = spriteManager.createSprite(finalSize + margin, finalSize + margin);
        sprite.setDrawer(getSpriteDrawer(mark, finalSize, colorMap));

        return sprite;
    }

    /**
     * Return the sprite drawer corresponding to the given mark.
     * @param mark the mark to draw.
     * @param finalSize the final mark size.
     * @param colorMap the scilab colormap to use.
     * @return the sprite drawer corresponding to the given mark.
     */
    private static SpriteDrawer getSpriteDrawer(Mark mark, int finalSize, ColorMap colorMap) {

        final Appearance appearance = new Appearance();
        Color backgroundColor = ColorFactory.createColor(colorMap, mark.getBackground());
        Color foregroundColor = ColorFactory.createColor(colorMap, mark.getForeground());

        appearance.setFillColor(backgroundColor);
        appearance.setLineColor(foregroundColor);

        switch (mark.getStyle()) {
            case  0: return new DotSpriteDrawer(foregroundColor, finalSize);
            case  1: return new PlusSpriteDrawer(appearance, finalSize);
            case  2: return new CrossSpriteDrawer(appearance, finalSize);
            case  3: return new StarSpriteDrawer(appearance, finalSize);
            case  4: return new FilledDiamondSpriteDrawer(foregroundColor, finalSize);

            case 10: return new AsteriskSpriteDrawer(appearance, finalSize);
            case 11: return new SquareSpriteDrawer(appearance, finalSize);

            default:
                return new PlusSpriteDrawer(appearance, finalSize);
        }
    }

    /**
     * Abstract class for all scilab mark sprites.
     */
    private static abstract class ScilabSpriteDrawer implements SpriteDrawer {
        protected final Appearance appearance;
        protected final int size;

        public ScilabSpriteDrawer(Appearance appearance, int size) {
            this.appearance = appearance;
            this.size = size;
        }

        @Override
        public OriginPosition getOriginPosition() {
            return OriginPosition.CENTER;
        }
    }

    /**
     * Dot sprite
     * Scilab ID = 0
     */
    private static class DotSpriteDrawer implements SpriteDrawer {
        private final Color color;
        private final int size;


        public DotSpriteDrawer(Color color, int size) {
            this.color = color;
            this.size = size;
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillDisc(0, 0, size, color);
        }

        @Override
        public OriginPosition getOriginPosition() {
            return OriginPosition.CENTER;
        }
    }

    /**
     * Plus sprite
     * Scilab ID = 1
     */
    private static class PlusSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public PlusSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Cross sprite
     * Scilab ID = 2
     */
    private static class CrossSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public CrossSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = (int) (size * Math.sqrt(2.0) / 2.0);
            coordinate1 = new int[] {-r, -r, r,  r};
            coordinate2 = new int[] {-r,  r, r, -r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Star sprite
     * Scilab ID = 3
     */
    private static class StarSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public StarSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillDisc(0, 0, size, appearance.getFillColor());
            spriteDrawingTools.drawCircle(0, 0, size, appearance);
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Filled diamond sprite
     * Scilab ID = 4
     */
    private static class FilledDiamondSpriteDrawer implements SpriteDrawer {
        private final Appearance appearance;
        private final int[] coordinates;
        private final int size;

        public FilledDiamondSpriteDrawer(Color color, int size) {
            this.size = size;
            int r = size / 2;

            appearance = new Appearance();
            appearance.setFillColor(color);
            appearance.setLineColor(color);

            coordinates = new int[] {-r,0, 0,-r, +r,0, 0,+r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinates, appearance);
        }

        @Override
        public OriginPosition getOriginPosition() {
            return OriginPosition.CENTER;
        }
    }

    /**
     * Asterisk sprite
     * Scilab ID = 10
     */
    private static class AsteriskSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;
        private final int[] coordinate3;
        private final int[] coordinate4;

        public AsteriskSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
            r = (int) (r*Math.sqrt(2.0) / 2.0);
            coordinate3 = new int[] {-r,-r, r, r};
            coordinate4 = new int[] {-r, r,+r,-r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
            spriteDrawingTools.drawPolyline(coordinate3, appearance);
            spriteDrawingTools.drawPolyline(coordinate4, appearance);
        }
    }

    /**
     * Square sprite
     * Scilab ID = 11
     */
    private static class SquareSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate;

        public SquareSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate = new int[] {-r,-r, +r,-r, +r,+r, -r,+r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinate, appearance);
        }
    }
}
