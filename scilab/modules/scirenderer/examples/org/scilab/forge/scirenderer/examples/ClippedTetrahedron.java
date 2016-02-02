/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.examples;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Rotation;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;
import org.scilab.forge.scirenderer.utils.shapes.geometry.TetrahedronFactory;

import javax.swing.BoundedRangeModel;
import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.BorderLayout;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class ClippedTetrahedron extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Clipping Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private ClippedTetrahedron() {
        setTitle(TITLE);

        BoundedRangeModel brm = new DefaultBoundedRangeModel(0, 0, -100, 100);
        add(new JSlider(brm), BorderLayout.SOUTH);

        /**
         * Add a mouse rotation adapter.
         */
        final MouseRotationAdapter mra = new MouseRotationAdapter(
            new Rotation(Math.toRadians(90), new Vector3d(1, 0, 0)),
            getCanvas()
        );
        getPanel().addMouseListener(mra);

        /**
         * Set main drawer.
         */
        getCanvas().setMainDrawer(new ClippingDrawer(getCanvas(), mra, brm));
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new ClippedTetrahedron();
    }

    /**
     * The clipped tetrahedron drawer.
     * @author Pierre Lando
     */
    private final class ClippingDrawer implements Drawer {
        private final MouseRotationAdapter mra;
        private final Geometry tetrahedron;
        private final BoundedRangeModel brm;

        /**
         * Clipping example drawer constructor.
         * @param canvas the canvas.
         * @param mra the mouse rotation adapter used for interaction.
         * @param brm the {@link javax.swing.BoundedRangeModel} used for the clipping plane coordinate.
         */
        public ClippingDrawer(final Canvas canvas, MouseRotationAdapter mra, BoundedRangeModel brm) {
            this.mra = mra;
            this.tetrahedron = TetrahedronFactory.createTetrahedron(canvas);
            this.brm = brm;

            brm.addChangeListener(new ChangeListener() {
                @Override
                public void stateChanged(ChangeEvent e) {
                    canvas.redraw();
                }
            });
        }

        @Override
        public void draw(DrawingTools dt) {
            dt.clear(new Color(1f, 1f, 1f));

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                dt.getTransformationManager().getProjectionStack().push(projection);
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getScaleTransformation(.5, .5, .5));
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(mra.getRotation()));
                ClippingPlane cp = dt.getClippingManager().getClippingPlane(0);
                cp.setEnable(true);
                cp.setEquation(new Vector4d(-1, 0, 0, -getValue()));

                Appearance appearance = new Appearance();
                appearance.setFillColor(new Color(1f, 0f, 0f));
                appearance.setLineColor(new Color(.2f, .2f, .2f));
                appearance.setLineWidth(3);
                dt.draw(tetrahedron, appearance);

                cp.setEquation(new Vector4d(1, 0, 0, getValue()));
                appearance.setFillColor(new Color(0f, 1f, 0f));
                dt.draw(tetrahedron, appearance);

                cp.setEnable(false);
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }

        /**
         * Current {@link javax.swing.BoundedRangeModel} value getter.
         * @return the current {@link javax.swing.BoundedRangeModel}.
         */
        private double getValue() {
            return -(double) brm.getValue() / 100.0;
        }
    }
}
