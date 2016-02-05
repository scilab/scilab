/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.forge.scirenderer.ruler;

import java.awt.Dimension;
import java.awt.geom.Rectangle2D;
import java.nio.FloatBuffer;
import java.text.DecimalFormat;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.ruler.graduations.Graduations;
import org.scilab.forge.scirenderer.ruler.graduations.UserDefinedFormat;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Pierre Lando
 */
public class RulerDrawer {

    /**
     * Sprite map.
     */
    private final Map<Double, Texture> spriteMap = new ConcurrentHashMap<Double, Texture>();

    /**
     * The current {@link TextureManager}.
     */
    private final TextureManager textureManager;

    /**
     * The used {@link RulerSpriteFactory}.
     */
    private RulerSpriteFactory spriteFactory;

    private OneShotRulerDrawer oneShotRulerDrawer;

    /**
     * Ruler drawer constructor.
     * @param textureManager the {@link TextureManager} of the canvas where the ruler will be drawn.
     */
    public RulerDrawer(TextureManager textureManager) {
        this.textureManager = textureManager;
        this.spriteFactory = new DefaultRulerSpriteFactory();
        this.oneShotRulerDrawer = new OneShotRulerDrawer();
    }

    /**
     * Ruler drawing method.
     * @param drawingTools the {@link DrawingTools} of the canvas where the ruler will be drawn.
     * @param model the {@link RulerModel} of the drawn ruler.
     * @return the {@link RulerDrawingResult} give information about how the ruler have been drawn.
     */
    public RulerDrawingResult draw(DrawingTools drawingTools, RulerModel model) {
        return oneShotRulerDrawer.drawWithResults(drawingTools, model);
    }

    /**
     * Get the sprite factory
     * @return the sprite factory
     */
    public RulerSpriteFactory getSpriteFactory() {
        return this.spriteFactory;
    }

    /**
     * Draw the ruler
     * @param drawingTools the {@link DrawingTools} of the canvas where the ruler will be drawn.
     */
    public void draw(DrawingTools drawingTools) {
        oneShotRulerDrawer.draw(drawingTools);
    }

    /**
     * Get the model
     * @return the ruler model.
     */
    public RulerModel getModel() {
        return oneShotRulerDrawer.rulerModel;
    }

    /**
     * Get the subticks values
     * @return the values.
     */
    public List<Double> getSubTicksValue() {
        return oneShotRulerDrawer.subTicksValue;
    }

    /**
     * Get the ticks values
     * @return the values.
     */
    public List<Double> getTicksValue() {
        return oneShotRulerDrawer.ticksValue;
    }

    /**
     * Compute different parameters on a ruler
     * @param rulerModel the {@link RulerModel} of the drawn ruler.
     * @param canvasProjection the canvas projection.
     */
    public RulerDrawingResult computeRuler(RulerModel model, Transformation canvasProjection) {
        return oneShotRulerDrawer.computeRuler(model, canvasProjection);
    }

    /**
     * Set the current {@link RulerSpriteFactory}.
     * All existing sprite will be cleared.
     * This ruler drawer will use the new {@link RulerSpriteFactory}.
     * @param spriteFactory the new {@link RulerSpriteFactory}.
     */
    public void setSpriteFactory(RulerSpriteFactory spriteFactory) {
        disposeResources();
        this.spriteFactory = spriteFactory;
    }

    /**
     * Dispose all used resources.
     */
    public void disposeResources() {
        textureManager.dispose(spriteMap.values());
        spriteMap.clear();
        oneShotRulerDrawer.dispose();
    }

    public double getDistanceRatio() {
        return oneShotRulerDrawer.getDistanceRatio();
    }

    /**
     * This class actually perform all the rendering of one ruler.
     */
    private class OneShotRulerDrawer {

        private Transformation canvasProjection;
        private RulerModel rulerModel;
        private Vector3d windowSubTicksDelta;
        private Vector3d windowTicksDelta;

        /**
         * Sprite Dimension map. Used as cached values in order not to charge DataProvider.
         */
        private Map<Double, Dimension> spriteDimensionMap = new HashMap<Double, Dimension>();

        private List<PositionedSprite> spritesList = new LinkedList<PositionedSprite>();

        /**
         * The maximum distance corresponding to the actually displayed sprites.
         */
        private double maximalSpritesDistance;

        /**
         * Deepest possible {@see Graduations}
         */
        private Graduations graduations;

        private List<Double> subTicksValue;
        private List<Double> ticksValue;
        private int density;
        private double distRatio;

        public OneShotRulerDrawer() { }

        public synchronized void dispose() {
            spriteDimensionMap.clear();
            spritesList.clear();
            if (subTicksValue != null) {
                subTicksValue.clear();
            }
            if (ticksValue != null) {
                ticksValue.clear();
            }
            rulerModel = null;
        }

        public double getDistanceRatio() {
            return distRatio;
        }

        /**
         * Compute different parameters on a ruler
         * @param drawingTools the {@link DrawingTools} of the canvas where the ruler will be drawn.
         * @param rulerModel the {@link RulerModel} of the drawn ruler.
         * @param canvasProjection the canvas projection.
         */
        public synchronized RulerDrawingResult computeRuler(RulerModel rulerModel, Transformation canvasProjection) {
            // Same code as drawWithResults (without drawing)
            // Historically, computations were made when drawing and they are made before drawing.
            // TODO: remove drawWithResults ??
            this.canvasProjection = canvasProjection;
            this.rulerModel = rulerModel;
            subTicksValue = new LinkedList<Double>();
            ticksValue = new LinkedList<Double>();
            spritesList = new LinkedList<PositionedSprite>();

            Vector3d windowTicksDirection = canvasProjection.projectDirection(rulerModel.getTicksDirection());
            windowTicksDirection = windowTicksDirection.setZ(0);

            Vector3d normalizedProjectedTicksDirection = windowTicksDirection.getNormalized();
            windowSubTicksDelta = normalizedProjectedTicksDirection.times(rulerModel.getSubTicksLength());
            windowTicksDelta = normalizedProjectedTicksDirection.times(rulerModel.getTicksLength());

            DecimalFormat format;
            if (rulerModel.isAutoTicks()) {
                format = computeAutoGraduation();
            } else {
                format = computeUserGraduation();
            }
            computeTicksData();

            double distRatio = computeTicksDistanceRatio(windowTicksDirection.getNorm());

            return new RulerDrawingResult(format, ticksValue, subTicksValue, density, distRatio, normalizedProjectedTicksDirection);
        }

        /**
         * Constructor.
         * @param drawingTools the {@link DrawingTools} of the canvas where the ruler will be drawn.
         * @param rulerModel the {@link RulerModel} of the drawn ruler.
         */
        public synchronized RulerDrawingResult drawWithResults(DrawingTools drawingTools, RulerModel rulerModel) {
            this.rulerModel = rulerModel;
            subTicksValue = new LinkedList<Double>();
            ticksValue = new LinkedList<Double>();
            spritesList = new LinkedList<PositionedSprite>();
            canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();

            Vector3d windowTicksDirection = canvasProjection.projectDirection(rulerModel.getTicksDirection());
            windowTicksDirection = windowTicksDirection.setZ(0);

            Vector3d normalizedProjectedTicksDirection = windowTicksDirection.getNormalized();
            windowSubTicksDelta = normalizedProjectedTicksDirection.times(rulerModel.getSubTicksLength());
            windowTicksDelta = normalizedProjectedTicksDirection.times(rulerModel.getTicksLength());

            DecimalFormat format;
            if (rulerModel.isAutoTicks()) {
                format = computeAutoGraduation();
            } else {
                format = computeUserGraduation();
            }
            computeTicksData();

            draw(drawingTools);

            double distRatio = computeTicksDistanceRatio(windowTicksDirection.getNorm());

            return new RulerDrawingResult(format, ticksValue, subTicksValue, density, distRatio, normalizedProjectedTicksDirection);
        }

        /**
             * Compute the ratio between windows ticks norm and the sprite distance.
             * @param windowTicksNorm the windows tics norm.
             * @return the ratio between windows ticks norm and the sprite distance.
             */
        private double computeTicksDistanceRatio(double windowTicksNorm) {
            if (windowTicksNorm == 0) {
                distRatio = 1.0;
            } else if (maximalSpritesDistance == 0) {
                distRatio = rulerModel.getSpriteDistance() / windowTicksNorm;
            } else {
                distRatio = maximalSpritesDistance / windowTicksNorm;
            }
            return distRatio;
        }

        /**
         * Actually perform the ruler drawing.
         * @param drawingTools {@link DrawingTools} used to perform the ruler drawing.
         */
        private synchronized void draw(DrawingTools drawingTools) {
            if (rulerModel == null) {
                return;
            }

            BuffersManager bufferManager = drawingTools.getCanvas().getBuffersManager();
            ElementsBuffer vertices = bufferManager.createElementsBuffer();
            fillVertices(vertices, rulerModel, ticksValue, subTicksValue, canvasProjection);
            DefaultGeometry geometry = new DefaultGeometry();
            geometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
            geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
            geometry.setVertices(vertices);

            Appearance appearance = new Appearance();
            appearance.setLineColor(rulerModel.getColor());
            appearance.setLineWidth((float) rulerModel.getLineWidth());

            drawingTools.getTransformationManager().useWindowCoordinate();
            try {
                for (PositionedSprite positionedSprite : spritesList) {
                    drawingTools.draw(positionedSprite.getSprite(), AnchorPosition.CENTER, positionedSprite.getWindowPosition());
                }
                drawingTools.draw(geometry, appearance);
            } catch (SciRendererException ignored) {
            }
            drawingTools.getTransformationManager().useSceneCoordinate();
            bufferManager.dispose(vertices);
        }

        /**
         * Compute the {@link Graduations} used to the ruler drawing in auto-ticks mode..
         * @return the used decimal format.
         */
        private DecimalFormat computeAutoGraduation() {
            /* The maximum distance corresponding to the actually displayed sprites. */
            double maxSpritesDistance = 0.0;

            Graduations currentGraduations = rulerModel.getGraduations();
            Graduations ticksGraduation = currentGraduations;
            DecimalFormat format = currentGraduations.getFormat();
            String f = rulerModel.getFormat();
            if (f != null && !f.isEmpty()) {
                format = new UserDefinedFormat(format, f, rulerModel.getScale(), rulerModel.getTranslate());
            }

            boolean canGetMore = true;
            List<PositionedSprite> newSpritesList = new LinkedList<PositionedSprite>();
            while (currentGraduations != null) {
                /* The maximum distance to any of the sprites' farthest sides at a given iteration. */
                double currentMaximalSpritesDistance = 0;

                newSpritesList.clear();
                List<Double> ticks = currentGraduations.getNewValues();
                for (double value : ticks) {
                    Texture sprite = computeSprite(value, format);
                    Vector3d windowPosition = canvasProjection.project(rulerModel.getPosition(value));

                    // X != X means NaN so we are not able to project coordinates
                    // return basic format
                    if (windowPosition.getX() != windowPosition.getX()) {
                        return format;
                    }

                    Dimension textureSize = computeSpriteDimension(value);

                    Vector3d delta = projectCenterToEdge(textureSize, windowTicksDelta);
                    PositionedSprite newSprite = new PositionedSprite(sprite, textureSize, windowPosition.plus(windowTicksDelta.plus(delta)));
                    newSpritesList.add(newSprite);
                    Vector3d farDelta = windowTicksDelta.plus(delta.times(2.0));
                    currentMaximalSpritesDistance = Math.max(currentMaximalSpritesDistance, farDelta.getNorm());
                }

                if (collide(newSpritesList, rulerModel.getMargin()) || collide(spritesList, newSpritesList, rulerModel.getMargin())) {
                    currentGraduations = currentGraduations.getAlternative();
                    canGetMore = false;
                } else {
                    maxSpritesDistance = Math.max(maxSpritesDistance, currentMaximalSpritesDistance);
                    spritesList.addAll(newSpritesList);
                    ticksGraduation = currentGraduations;
                    if (canGetMore) {
                        currentGraduations = currentGraduations.getMore();
                    } else {
                        currentGraduations = null;
                    }
                }
            }

            this.graduations = ticksGraduation;
            this.maximalSpritesDistance = maxSpritesDistance;

            return format;
        }

        /**
         * Compute the {@link Graduations} used to the ruler drawing in auto-ticks mode..
         * @return the used decimal format.
         */
        private DecimalFormat computeUserGraduation() {
            /* The maximum distance corresponding to the actually displayed sprites. */
            double maxSpritesDistance = 0.0;
            Graduations currentGraduations = rulerModel.getGraduations();

            List<Double> ticks = currentGraduations.getNewValues();
            DecimalFormat format = currentGraduations.getFormat();
            for (double value : ticks) {
                Texture sprite = computeSprite(value, format);
                if (sprite != null) {
                    Vector3d windowPosition = canvasProjection.project(rulerModel.getPosition(value));

                    Vector3d delta = projectCenterToEdge(sprite, windowTicksDelta);
                    spritesList.add(new PositionedSprite(sprite, windowPosition.plus(windowTicksDelta.plus(delta))));

                    Vector3d farDelta = windowTicksDelta.plus(delta.times(2.0));
                    maxSpritesDistance = Math.max(maxSpritesDistance, farDelta.getNorm());
                }
            }

            this.graduations = currentGraduations;
            this.maximalSpritesDistance = maxSpritesDistance;
            return format;
        }

        /**
         * Compute the ticks, sub-ticks data and the sub-ticks density.
         */
        private void computeTicksData() {
            if (graduations != null) {
                ticksValue = graduations.getAllValues();
                final int N = rulerModel.getSubticksNumber();

                if (N < 0) {
                    Graduations subGraduation = graduations.getSubGraduations();
                    while ((subGraduation != null) && (computeTicksDistance(subGraduation) < rulerModel.getMinimalSubTicksDistance())) {
                        subGraduation = subGraduation.getAlternative();
                    }

                    if (subGraduation != null) {
                        subTicksValue = subGraduation.getAllValues();
                    } else {
                        subTicksValue = new LinkedList<Double>();
                    }
                } else {
                    subTicksValue = graduations.getSubGraduations(N);
                }
                density = getDensity();
            } else {
                subTicksValue = new LinkedList<Double>();
                ticksValue = new LinkedList<Double>();
                density = 0;
            }
        }

        private int getDensity() {
            int N = ticksValue == null ? 0 : ticksValue.size();
            int M = subTicksValue == null ? 0 : subTicksValue.size();

            if (M <= N || N == 1) {
                return 0;
            }

            return (M - N) / (N - 1);
        }

        /**
         * Compute and return the minimal screen distance between two successive ticks of the given {@link Graduations}.
         * If the given {@link Graduations} is <code>null</code>, the returned value is {@link Double#MAX_VALUE}.
         * @param graduations the given {@link Graduations}.
         * @return the minimal screen distance between two successive ticks of the given {@link Graduations}.
         */
        private double computeTicksDistance(Graduations graduations) {
            double minimalDistance = Double.MAX_VALUE;
            if (graduations != null) {
                Vector3d previousProjection = null;
                for (double currentValue : graduations.getAllValues()) {
                    Vector3d currentProjection = canvasProjection.project(rulerModel.getPosition(currentValue));
                    if (previousProjection != null) {
                        minimalDistance = Math.min(minimalDistance, currentProjection.minus(previousProjection).getNorm2());
                    }
                    previousProjection = currentProjection;
                }
                minimalDistance = Math.sqrt(minimalDistance);
            }
            return minimalDistance;
        }

        /**
         * Return true if at leas two element of {@see spritesList} collide.
         * @param spritesList the list of sprite to be tested.
         * @param margin the collision margin.
         * @return true if at leas two element of {@see spritesList} collide.
         */
        private boolean collide(List<PositionedSprite> spritesList, double margin) {
            for (int i = 0; i < spritesList.size(); i++) {
                Rectangle2D bounds = spritesList.get(i).getWindowBounds();
                for (int j = i + 1; j < spritesList.size(); j++) {
                    if (collide(bounds, spritesList.get(j).getWindowBounds(), margin)) {
                        return true;
                    }
                }
            }
            return false;
        }

        /**
         * Return true if the at least one element of <code>newSpritesList</code> collides with one element of <code>spritesList</code>.
         * @param spritesList the list of reference sprites.
         * @param newSpritesList the list of new sprites.
         * @param margin the collision margin.
         * @return true if the at least one element of <code>newSpritesList</code> collides with one element of <code>spritesList</code>.
         */
        private boolean collide(List<PositionedSprite> spritesList, List<PositionedSprite> newSpritesList, double margin) {
            for (PositionedSprite sprite1 : newSpritesList) {
                Rectangle2D bounds = sprite1.getWindowBounds();
                for (PositionedSprite sprite2 : spritesList) {
                    if (collide(bounds, sprite2.getWindowBounds(), margin)) {
                        return true;
                    }
                }
            }
            return false;
        }

        /**
         * Return true if the givens rectangles collide.
         * @param rectangle1 first rectangle.
         * @param rectangle2 second rectangle.
         * @param margin the margin.
         * @return true if the givens rectangles collide.
         */
        private boolean collide(Rectangle2D rectangle1, Rectangle2D rectangle2, double margin) {
            return  ((rectangle2.getMinX() - rectangle1.getMaxX()) < margin)
                    &&  ((rectangle1.getMinX() - rectangle2.getMaxX()) < margin)
                    &&  ((rectangle2.getMinY() - rectangle1.getMaxY()) < margin)
                    &&  ((rectangle1.getMinY() - rectangle2.getMaxY()) < margin);
        }

        /**
         * Compute and return a translation along the given <code>direction</code>.
         * This translation is the vector between the <code>sprite</code> center and is projection along the given
         * {@see direction} to the sprite edges.
         * @param sprite the given {@link Texture}
         * @param direction the given direction.
         * @return the vector between the sprite center and is projection to the sprite edges.
         */
        private Vector3d projectCenterToEdge(Texture sprite, Vector3d direction) {
            Vector3d usedDirection;
            if ((direction == null) || (direction.isZero())) {
                usedDirection = new Vector3d(1, 0, 0);
            } else {
                usedDirection = direction;
            }

            /* +1 is used to have a space between the tick and its label */
            Dimension textureSize = sprite.getDataProvider().getTextureSize();
            double ratioX = textureSize.width / Math.abs(usedDirection.getX());
            double ratioY = textureSize.height / Math.abs(usedDirection.getY());
            double ratio = Math.min(ratioY, ratioX);

            return usedDirection.times((ratio + 1) / 2);
        }

        /**
         * Compute and return a translation along the given <code>direction</code>.
         * This translation is the vector between the <code>sprite</code> center and is projection along the given
         * {@see direction} to the sprite edges.
         * @param textureSize the size of corresponding texture
         * @param direction the given direction.
         * @return the vector between the sprite center and is projection to the sprite edges.
         */
        private Vector3d projectCenterToEdge(Dimension textureSize, Vector3d direction) {
            Vector3d usedDirection;
            if ((direction == null) || (direction.isZero())) {
                usedDirection = new Vector3d(1, 0, 0);
            } else {
                usedDirection = direction;
            }

            double ratioX = textureSize.width / Math.abs(usedDirection.getX());
            double ratioY = textureSize.height / Math.abs(usedDirection.getY());
            double ratio = Math.min(ratioY, ratioX);

            return usedDirection.times((ratio + 1) / 2);
        }

        /**
         * Fill a vertices buffer with the needed data to draw a ruler.
         * @param verticesBuffer the {@link ElementsBuffer} to fill.
         * @param rulerModel the {@link RulerModel} to draw.
         * @param ticksValue the list of ticks.
         * @param subTicksValue the list of sub-ticks.
         * @param canvasProjection the used canvas projection.
         */
        private void fillVertices(ElementsBuffer verticesBuffer, RulerModel rulerModel, List<Double> ticksValue, List<Double> subTicksValue, Transformation canvasProjection) {
            Vector3d a = rulerModel.getFirstPoint();
            Vector3d b = rulerModel.getSecondPoint();

            if ((a != null) && (b != null)) {
                int bufferSize = 2 * ticksValue.size() + 2 * subTicksValue.size();
                if (rulerModel.isLineVisible()) {
                    bufferSize += 2;
                }
                FloatBuffer data = FloatBuffer.allocate(4 * bufferSize);
                data.rewind();

                for (double value : ticksValue) {
                    Vector3d p = canvasProjection.project(rulerModel.getPosition(value));
                    data.put(p.getDataAsFloatArray(4));
                    data.put(p.plus(windowTicksDelta).getDataAsFloatArray(4));
                }

                for (double value : subTicksValue) {
                    Vector3d p = canvasProjection.project(rulerModel.getPosition(value));
                    data.put(p.getDataAsFloatArray(4));
                    data.put(p.plus(windowSubTicksDelta).getDataAsFloatArray(4));
                }

                if (rulerModel.isLineVisible()) {
                    data.put(canvasProjection.project(a).getDataAsFloatArray(4));
                    data.put(canvasProjection.project(b).getDataAsFloatArray(4));
                }

                data.rewind();
                verticesBuffer.setData(data, 4);
            } else {
                verticesBuffer.setData(new float[0], 4);
            }
        }

        /**
         * Compute the {@link Texture} for a given value.
         * The {@link Texture} is made once using the current {@link RulerSpriteFactory}.
         * @param value the given value.
         * @param format the format to use.
         * @return A {@link Texture} for the label at the given value.
         */
        private Texture computeSprite(double value, DecimalFormat format) {
            Texture sprite = spriteMap.get(value);
            if (sprite == null) {
                sprite = spriteFactory.create(value, format, textureManager);
                if (sprite != null) {
                    spriteMap.put(value, sprite);
                }
            }
            return sprite;
        }

        private Dimension computeSpriteDimension(double value) {
            Dimension spriteDimension = spriteDimensionMap.get(value);
            if (spriteDimension == null) {
                Texture sprite = spriteMap.get(value);
                if (sprite != null) {
                    spriteDimension = sprite.getDataProvider().getTextureSize();
                    spriteDimensionMap.put(value, spriteDimension);
                }
            }

            return spriteDimension;
        }

        /**
         * This class is a basic container for a {@link Texture} and an associated window position.
         *
         * @author Pierre Lando
         */
        private class PositionedSprite {

            private final Texture sprite;
            private final Vector3d windowPosition;
            private final Rectangle2D windowsBounds;

            /**
             * Default constructor.
             * @param sprite the {@link Texture}.
             * @param windowPosition the window position.
             */
            public PositionedSprite(Texture sprite,  Vector3d windowPosition) {
                //long tic = Calendar.getInstance().getTimeInMillis();
                this.windowPosition = windowPosition;
                this.sprite = sprite;

                Dimension textureSize = sprite.getDataProvider().getTextureSize();
                windowsBounds = new Rectangle2D.Double(
                    windowPosition.getX(),
                    windowPosition.getY(),
                    textureSize.width,
                    textureSize.height
                );

                //System.err.println("====[new PositionedSprite] time = "+(Calendar.getInstance().getTimeInMillis() - tic));
            }

            public PositionedSprite(Texture sprite, Dimension textureSize, Vector3d windowPosition) {
                //long tic = Calendar.getInstance().getTimeInMillis();
                this.windowPosition = windowPosition;
                this.sprite = sprite;

                windowsBounds = new Rectangle2D.Double(
                    windowPosition.getX(),
                    windowPosition.getY(),
                    textureSize.width,
                    textureSize.height
                );

                //System.err.println("====[new PositionedSprite] time = "+(Calendar.getInstance().getTimeInMillis() - tic));
            }
            /**
             * Return the {@link Texture}
             * @return the {@link Texture}
             */
            public Texture getSprite() {
                return sprite;
            }

            /**
             * Return the window position of the {@link Texture}.
             * @return the window position of the {@link Texture}.
             */
            public Vector3d getWindowPosition() {
                return windowPosition;
            }

            /**
             * Return the window bounds of the {@link Texture}
             * @return the window bounds of the {@link Texture}
             */
            public Rectangle2D getWindowBounds() {
                return windowsBounds;
            }
        }
    }
}
