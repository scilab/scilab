/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.picking;

import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.picking.PickingManager;
import org.scilab.forge.scirenderer.picking.PickingTask;
import org.scilab.forge.scirenderer.picking.PickingTools;

import java.util.Stack;

/**
 *
 * JoGL implementation of {@link org.scilab.forge.scirenderer.picking.PickingManager}
 *
 * @author Pierre Lando
 */
public class JoGLPickingManager implements PickingManager {

    private final Stack<PickingTask> stack = new Stack<PickingTask>();
    private final JoGLCanvas canvas;

    /**
     * Default constructor.
     * @param canvas the canvas where picking is performed.
     */
    public JoGLPickingManager(JoGLCanvas canvas) {
        this.canvas = canvas;
    }

    @Override
    public void addPickingTask(PickingTask pickingTask) {
        stack.push(pickingTask);
        canvas.redraw();
    }

    /**
     * Ask to consume picking task with the given drawing tools.
     * @param drawingTools the given drawing tools.
     */
    public void glConsume(JoGLDrawingTools drawingTools) {
        PickingTools pickingTools = new JoGLPickingTools(drawingTools);
        while (!stack.isEmpty()) {
            stack.pop().perform(pickingTools);
        }
    }
}
