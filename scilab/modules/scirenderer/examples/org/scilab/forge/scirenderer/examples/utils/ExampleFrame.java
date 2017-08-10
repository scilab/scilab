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

package org.scilab.forge.scirenderer.examples.utils;

import com.jogamp.opengl.util.Animator;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;

import javax.media.opengl.awt.GLJPanel;
import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
public abstract class ExampleFrame extends JFrame {

    /**
     * The default frame size.
     */
    private static final Dimension DEFAULT_SIZE = new Dimension(800, 800);

    /**
     * The used animator.
     */
    private Animator animator;

    /**
     * The canvas.
     */
    private final Canvas canvas;

    /**
     * The gl panel.
     */
    private final GLJPanel glPanel;

    /**
     * Default constructor.
     * Initialise the GUI.
     */
    protected ExampleFrame() {
        glPanel = new GLJPanel();
        canvas = JoGLCanvasFactory.createCanvas(glPanel);

        setSize(DEFAULT_SIZE);
        setLayout(new BorderLayout());
        add(glPanel, BorderLayout.CENTER);

        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
    }

    /**
     * Canvas getter.
     * @return the {@link Canvas}.
     */
    protected final Canvas getCanvas() {
        return canvas;
    }

    /**
     * OpenGL panel getter.
     * @return the OpenGL panel.
     */
    protected final JPanel getPanel() {
        return glPanel;
    }

    /**
     * Animated scene setter.
     * @param animated true if the scene is auto-animated.
     */
    protected final void animate(boolean animated) {
        if (animated && (animator == null)) {
            animator = new Animator();
            animator.add(glPanel);
            animator.start();
        }
        if (!animated && (animator != null)) {
            animator.remove(glPanel);
            animator.stop();
            animator = null;
        }
    }
}
