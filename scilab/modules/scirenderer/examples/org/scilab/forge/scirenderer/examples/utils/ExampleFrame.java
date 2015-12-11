/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
