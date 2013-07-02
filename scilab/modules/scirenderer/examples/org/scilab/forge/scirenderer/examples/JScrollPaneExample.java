package org.scilab.forge.scirenderer.examples;

import com.jogamp.opengl.util.Animator;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;

import javax.media.opengl.awt.GLCanvas;
import javax.media.opengl.awt.GLJPanel;
import javax.swing.*;
import java.awt.*;

/**
 * @Author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public class JScrollPaneExample extends JFrame {

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new JScrollPaneExample();
    }

    /** The canvas. */
    private final Canvas canvas;

    /** The gl panel. */
    private final GLCanvas glPanel;

    private JScrollPaneExample() {
        glPanel = new GLCanvas();
        canvas = JoGLCanvasFactory.createCanvas(glPanel);
        glPanel.setPreferredSize(new Dimension(1024, 768));

        Animator animator = new Animator();
        animator.add(glPanel);
        animator.start();

        final Geometry cube = CubeFactory.createCube(canvas, 5, true);
        final Appearance appearance = new Appearance();
        appearance.setLineWidth(3);
        appearance.setLineColor(new Color(.7f, .6f, .5f));

        canvas.setMainDrawer(new Drawer() {
            @Override
            public void draw(DrawingTools drawingTools) {
                try {
                    double t = System.currentTimeMillis() / 50.;
                    drawingTools.clear(new Color());
                    Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(drawingTools.getCanvas().getDimension(), 1f);
                    Transformation perspective = TransformationFactory.getPerspectiveTransformation(1, 10, 45);
                    Transformation displacement = TransformationFactory.getTranslateTransformation(0, 0, -5);
                    drawingTools.getTransformationManager().getProjectionStack().push(projection.rightTimes(perspective.rightTimes(displacement)));
                    drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(t, 1, 2, 3));

                    drawingTools.draw(cube, appearance);
                } catch (SciRendererException ignored) {
                }
            }

            @Override
            public boolean is2DView() {
                return false;
            }
        });

        JPanel panel = new JPanel(new BorderLayout());
        JScrollPane scrollPane = new JScrollPane(glPanel);
        JTextArea textArea = new JTextArea();
        textArea.setText("Test\nTest\nTest\nTest\n");

        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(textArea, BorderLayout.NORTH);


        add(panel);
        setLocationRelativeTo(null);
        setTitle("GLCanvas in JScrollPane example");
        setSize(800, 600);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);
    }
}
