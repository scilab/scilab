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

package org.scilab.modules.renderer.JoGLView;

import com.sun.opengl.util.BufferUtil;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.modules.graphic_objects.NativeGL;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

import java.nio.FloatBuffer;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Pierre Lando
 */
public class DataManager implements GraphicView {

    private final Map<String, ElementsBuffer> vertexBuffers = new HashMap<String, ElementsBuffer>();
    private final Canvas canvas;


    public DataManager(Canvas canvas) {
        this.canvas = canvas;
        GraphicController.getController().register(this);
    }

    public ElementsBuffer getVertexBuffer(String id) {
        if (vertexBuffers.containsKey(id)) {
            return vertexBuffers.get(id);
        } else {
            ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();

            int length = NativeGL.getGLDataLength(id);
            FloatBuffer dataBuffer = BufferUtil.newFloatBuffer(length);
            NativeGL.loadGLData(dataBuffer, id);

            vertexBuffer.setData(dataBuffer, 4);

            vertexBuffers.put(id, vertexBuffer);
            return vertexBuffer;
        }
    }

    @Override
    public void updateObject(String id, String property) {
        if (property.equals(GraphicObjectProperties.__GO_DATA_MODEL__) && vertexBuffers.containsKey(id)) {
            ElementsBuffer vertexBuffer = vertexBuffers.get(id);

            int length = NativeGL.getGLDataLength(id);
            FloatBuffer dataBuffer = BufferUtil.newFloatBuffer(length);
            NativeGL.loadGLData(dataBuffer, id);
            
            vertexBuffer.setData(dataBuffer, 4);
        }
    }

    @Override
    public void createObject(String s) {}

    @Override
    public void deleteObject(String id) {
        if (vertexBuffers.containsKey(id)) {
            canvas.getBuffersManager().dispose(vertexBuffers.get(id));
            vertexBuffers.remove(id);
        }
    }
}
