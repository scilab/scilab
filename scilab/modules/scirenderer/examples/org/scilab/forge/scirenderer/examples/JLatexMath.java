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

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import javax.swing.JComponent;
import javax.swing.JTextArea;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.util.EventListener;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class JLatexMath extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "JLatexMath Drawing Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private JLatexMath() {
        setTitle(TITLE);
        LatexCompositor latexCompositor = new LatexCompositor();


        add(latexCompositor, BorderLayout.SOUTH);
        LatexDrawer latexDrawer = new LatexDrawer(getCanvas());
        latexCompositor.addListener(latexDrawer);
        getCanvas().setMainDrawer(latexDrawer);
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new JLatexMath();
    }

    /**
     * @author Pierre Lando
     */
    private final class LatexCompositor extends JComponent {

        private static final String GAUSS = "\\int_{-\\infty}^{+\\infty}\\!e^{-x^2}\\, \\mathrm{d}x = \\sqrt \\pi";
        private final PlainDocument document;
        private final JTextArea textArea;

        /**
         * Default constructor.
         */
        public LatexCompositor() {
            setLayout(new BorderLayout());
            document = new PlainDocument();
            try {
                document.insertString(0, GAUSS, null);
            } catch (BadLocationException ignored) {
            }

            document.addDocumentListener(new DocumentListener() {
                @Override
                public void insertUpdate(DocumentEvent e) {
                    fireDataChanged();
                }

                @Override
                public void removeUpdate(DocumentEvent e) {
                    fireDataChanged();
                }

                @Override
                public void changedUpdate(DocumentEvent e) {
                    fireDataChanged();
                }
            });

            textArea = new JTextArea(document);
            add(textArea, BorderLayout.CENTER);
        }

        /**
         * Compute the {@see TeXIcon} corresponding to the current formula.
         * @return the {@see TeXIcon} corresponding to the current formula.
         */
        public TeXIcon getTextIcon() {
            try {
                TeXFormula formula = new TeXFormula(document.getText(0, document.getLength()));
                return formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, 32);
            } catch (org.scilab.forge.jlatexmath.ParseException e) {
                return null;
            } catch (BadLocationException e) {
                return null;
            }
        }

        /**
         * Add a listener on the latex composition.
         * @param latexDrawer the new listener.
         */
        public void addListener(LatexDrawer latexDrawer) {
            listenerList.add(LatexCompositorListener.class, latexDrawer);
            latexDrawer.dataChanged(this);
        }

        /**
         * Notify listeners for a data change.
         */
        private void fireDataChanged() {
            for (LatexCompositorListener listener : listenerList.getListeners(LatexCompositorListener.class)) {
                listener.dataChanged(this);
            }
        }
    }

    /**
     * @author Pierre Lando
     */
    private interface LatexCompositorListener extends EventListener {

        /**
         * Inform listeners from data changes.
         * @param compositor compositor where data have changed.
         */
        void dataChanged(LatexCompositor compositor);
    }

    /**
     * @author Pierre Lando
     */
    private final class LatexDrawer implements Drawer, LatexCompositorListener {


        private final Canvas canvas;
        private Texture sprite;

        /**
         * Default constructor.
         * @param canvas the {@link org.scilab.forge.scirenderer.Canvas} where the drawn will be performed.
         */
        public LatexDrawer(Canvas canvas) {
            this.canvas = canvas;
            sprite =  null;
        }

        @Override
        public void draw(DrawingTools drawingTools) {
            drawingTools.clear(new Color());

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(drawingTools.getCanvas().getDimension(), 1f);
                drawingTools.getTransformationManager().getProjectionStack().push(projection);
                drawingTools.getTransformationManager().getModelViewStack().push(TransformationFactory.getScaleTransformation(.1, .1, .1));
                if (sprite != null) {
                    drawingTools.draw(sprite, AnchorPosition.CENTER, new Vector3d(0, 0, 0));
                }
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }

        @Override
        public void dataChanged(final LatexCompositor compositor) {
            compositor.textArea.setBackground(Color.RED);
            final TeXIcon ti = compositor.getTextIcon();
            canvas.getTextureManager().dispose(sprite);
            sprite = null;
            if (ti != null) {
                ti.setForeground(java.awt.Color.WHITE);

                sprite = canvas.getTextureManager().createTexture();
                sprite.setDrawer(new TextureDrawer() {

                    @Override
                    public void draw(TextureDrawingTools drawingTools) {
                        drawingTools.clear(new Color(0, 0, 0));
                        drawingTools.draw(ti, -(ti.getIconWidth() / 2), -(ti.getIconHeight() / 2));
                    }

                    @Override
                    public Dimension getTextureSize() {
                        return new Dimension(ti.getIconWidth(), ti.getIconHeight());
                    }

                    @Override
                    public TextureDrawer.OriginPosition getOriginPosition() {
                        return TextureDrawer.OriginPosition.CENTER;
                    }
                });
            }

            /** Wait image to set text area background to green */
            new Thread(new Runnable() {
                @Override
                public void run() {
                    canvas.waitImage();
                    compositor.textArea.setBackground(Color.GREEN);
                }
            }).start();

            canvas.redraw();
        }
    }
}
