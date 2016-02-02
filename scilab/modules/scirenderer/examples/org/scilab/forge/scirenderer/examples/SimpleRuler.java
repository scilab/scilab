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
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.ruler.DefaultRulerModel;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerModel;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import javax.swing.BoundedRangeModel;
import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JCheckBox;
import javax.swing.JFormattedTextField;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DecimalFormat;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class SimpleRuler extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Ruler Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private SimpleRuler() {
        setTitle(TITLE);

        final BoundedRangeModel zoomLevel = new DefaultBoundedRangeModel(13, 0, 1, 1000);


        JPanel buttonsPanel = new JPanel(new FlowLayout());

        final JFormattedTextField minValue = new JFormattedTextField(new DecimalFormat("0.0000"));
        minValue.setValue(1);

        final JFormattedTextField maxValue = new JFormattedTextField(new DecimalFormat("0.0000"));
        maxValue.setValue(4);

        final JCheckBox logBox = new JCheckBox("Log");
        logBox.setSelected(true);

        buttonsPanel.add(minValue);
        buttonsPanel.add(maxValue);
        buttonsPanel.add(logBox);
        buttonsPanel.add(new JSlider(zoomLevel));

        add(buttonsPanel, BorderLayout.SOUTH);

        // SciRenderer canvas.
        final DefaultRulerModel rulerModel = new DefaultRulerModel();
        rulerModel.setFirstPoint(new Vector3d(-1, 0, 0));
        rulerModel.setSecondPoint(new Vector3d(1, 0, 0));
        rulerModel.setFirstValue(Double.parseDouble(minValue.getValue().toString()));
        rulerModel.setSecondValue(Double.parseDouble(maxValue.getValue().toString()));
        rulerModel.setTicksDirection(new Vector3d(0, -1, 0));
        rulerModel.setLogarithmic(true);

        Drawer drawer = new MainRulerDrawer(getCanvas(), zoomLevel, rulerModel);
        getCanvas().setMainDrawer(drawer);

        zoomLevel.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                getCanvas().redraw();
            }
        });

        logBox.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                if (logBox.isSelected()) {
                    double a = Math.abs(rulerModel.getFirstValue());
                    double b = Math.abs(rulerModel.getSecondValue());
                    if (b == 0) {
                        b = 10;
                    }
                    if (a == 0) {
                        a = 1;
                    }
                    rulerModel.setFirstValue(Math.min(a, b));
                    rulerModel.setSecondValue(Math.max(a, b));
                    rulerModel.setLogarithmic(true);
                } else {
                    rulerModel.setLogarithmic(false);
                }
                getCanvas().redraw();
            }
        });

        ActionListener valueChangedListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                rulerModel.setFirstValue(Double.parseDouble(minValue.getValue().toString()));
                rulerModel.setSecondValue(Double.parseDouble(maxValue.getValue().toString()));
                getCanvas().redraw();
            }
        };

        maxValue.addActionListener(valueChangedListener);
        minValue.addActionListener(valueChangedListener);
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new SimpleRuler();
    }

    /**
     * The ruler drawer.
     */
    public static class MainRulerDrawer implements Drawer {

        private final RulerDrawer rulerDrawer;
        private final RulerModel rulerModel;
        private final BoundedRangeModel zoomLevel;

        /**
         * Default constructor.
         * @param canvas the canvas where the draw will be performed.
         * @param zoomLevel the current zoom level model.
         * @param rulerModel the current ruler model.
         */
        public MainRulerDrawer(Canvas canvas, BoundedRangeModel zoomLevel, RulerModel rulerModel) {
            this.zoomLevel = zoomLevel;
            this.rulerModel = rulerModel;
            this.rulerDrawer = new RulerDrawer(canvas.getTextureManager());
        }

        @Override
        public void draw(DrawingTools dt) {
            dt.clear(new Color(1f, 1f, 1f));

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                dt.getTransformationManager().getProjectionStack().push(projection);
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getScaleTransformation(zoomLevel.getValue() / 100.0));

            } catch (DegenerateMatrixException ignored) {
                // Should not occur.
            }

            rulerDrawer.draw(dt, rulerModel);
        }

        @Override
        public boolean is2DView() {
            return false;
        }
    }
}
