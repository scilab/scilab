/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.data.AbstractDataProvider;
import org.scilab.forge.scirenderer.texture.AbstractTextureDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.modules.graphic_objects.MainDataLoader;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.renderer.JoGLView.util.BufferAllocation;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;

import java.awt.Dimension;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Pierre Lando
 */
public class ScilabTextureManager {

    private final Map<String, Texture> hashMap = new HashMap<String, Texture>();
    private final DrawerVisitor drawerVisitor;
    private final Canvas canvas;


    public ScilabTextureManager(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;
        this.canvas = drawerVisitor.getCanvas();
    }

    /**
     * {@link Texture} getter.
     * @param identifier object identifier.
     * @return the {@link Texture} used to drawn the object who have the given identifier.
     */
    public Texture getTexture(String identifier) {
        Texture texture = hashMap.get(identifier);
        if (texture == null) {
            texture = canvas.getTextureManager().createTexture();
            texture.setDataProvider(new ScilabTextureDataProvider(identifier));
            texture.setMagnificationFilter(Texture.Filter.NEAREST);
            texture.setMinifyingFilter(Texture.Filter.NEAREST);
            hashMap.put(identifier, texture);
        }
        return texture;
    }

    public void dispose(String identifier) {
        Texture texture = hashMap.get(identifier);
        if (texture != null) {
            hashMap.remove(identifier);
            canvas.getTextureManager().dispose(texture);
        }
    }

    public void disposeAll() {
        for (Map.Entry<String, Texture> entry : hashMap.entrySet()) {
            Texture t = entry.getValue();
            if (t != null) {
                canvas.getTextureManager().dispose(t);
            }
        }

        hashMap.clear();
    }

    private class ScilabTextureDataProvider extends AbstractTextureDataProvider implements GraphicView {
        private final String identifier;
        private Dimension dimension;
        private boolean isValid;

        public ScilabTextureDataProvider(String identifier) {
            this.identifier = identifier;
            this.isValid = (identifier != null);

            if (isValid()) {
                updateData();
            }
            GraphicController.getController().register(this);
        }

        private void updateData() {
            int width = MainDataLoader.getTextureWidth(identifier);
            int height = MainDataLoader.getTextureHeight(identifier);
            dimension = new Dimension(width, height);
            fireUpdate();
        }

        @Override
        public Dimension getTextureSize() {
            if (isValid()) {
                return new Dimension(dimension);
            } else {
                return null;
            }
        }

        @Override
        public ByteBuffer getData() {
            int bufferLength = dimension.width * dimension.height * 4;
            ByteBuffer buffer;
            try {
                buffer = BufferAllocation.newByteBuffer(bufferLength);
            } catch (OutOfMemoryException exception) {
                drawerVisitor.invalidate(GraphicController.getController().getObjectFromId(identifier), exception);
                return null;
            }
            MainDataLoader.fillTextureData(identifier, buffer, bufferLength);
            buffer.rewind();
            return buffer;
        }

        @Override
        public ByteBuffer getSubData(int x, int y, int width, int height) {
            int bufferLength = width * height * 4;
            ByteBuffer buffer;
            try {
                buffer = BufferAllocation.newByteBuffer(bufferLength);
            } catch (OutOfMemoryException exception) {
                drawerVisitor.invalidate(GraphicController.getController().getObjectFromId(identifier), exception);
                return null;
            }
            MainDataLoader.fillTextureData(identifier, buffer, bufferLength, x, y, width, height);
            buffer.rewind();
            return buffer;
        }

        @Override
        public boolean isValid() {
            return isValid;
        }

        @Override
        public void updateObject(String id, int property) {
            if (isValid() && identifier.equals(id)) {
                // TODO check Property.
                updateData();
            }
        }

        @Override
        public void deleteObject(String id) {
            if (isValid() && identifier.equals(id)) {
                isValid = false;
                GraphicController.getController().unregister(this);
            }
        }

        @Override
        public void createObject(String id) {
        }
    }
}
