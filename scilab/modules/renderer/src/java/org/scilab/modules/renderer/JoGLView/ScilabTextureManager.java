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

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.Canvas;
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

    private final Map<Integer, Texture> hashMap = new HashMap<Integer, Texture>();
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
    public Texture getTexture(Integer identifier) {
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

    public void dispose(Integer identifier) {
        Texture texture = hashMap.get(identifier);
        if (texture != null) {
            hashMap.remove(identifier);
            canvas.getTextureManager().dispose(texture);
        }
    }

    public void disposeAll() {
        for (Map.Entry<Integer, Texture> entry : hashMap.entrySet()) {
            Texture t = entry.getValue();
            if (t != null) {
                canvas.getTextureManager().dispose(t);
            }
        }

        hashMap.clear();
    }

    private class ScilabTextureDataProvider extends AbstractTextureDataProvider implements GraphicView {

        private final Integer identifier;
        private Dimension dimension;
        private int datatype = -1;
        private int iType = -1;
        private boolean isValid;
        private boolean isRowOrder;
        private ByteBuffer buffer;

        public ScilabTextureDataProvider(Integer identifier) {
            this.identifier = identifier;
            this.isValid = (identifier != null);

            if (isValid()) {
                updateData();
            }
            GraphicController.getController().register(this);
        }

        @Override
        public boolean isRowMajorOrder() {
            return isRowOrder;
        }

        private void updateData() {
            int width = MainDataLoader.getTextureWidth(identifier);
            int height = MainDataLoader.getTextureHeight(identifier);
            int gltype = MainDataLoader.getTextureGLType(identifier);
            int itype = MainDataLoader.getTextureImageType(identifier);
            int datatype = MainDataLoader.getTextureDataType(identifier);
            boolean isRowOrder = MainDataLoader.isTextureRowOrder(identifier);
            // todo gerer le cas itype == -1;
            imageType = ImageType.fromInt(gltype);
            if (dimension == null  || dimension.width != width || dimension.height != height || itype != iType || this.isRowOrder != isRowOrder || this.datatype != datatype) {
                dimension = new Dimension(width, height);
                this.isRowOrder = isRowOrder;
                this.datatype = datatype;

                // 3 is MATPLOT_INDEX
                dispose(iType == 3);
                iType = itype;
            }

            fireUpdate();
        }

        public void dispose(boolean isIndex) {
            if (buffer != null) {
                if (isIndex) {
                    MainDataLoader.disposeTextureData(identifier, buffer);
                }
                buffer = null;
            }
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
            if (buffer == null) {
                buffer = MainDataLoader.getTextureData(identifier);
                updateData();
            }

            if (iType == 3) {
                // Indexed colors, so we need to recalculate the color (if needed)
                MainDataLoader.fillTextureData(identifier, buffer, buffer.capacity());
            }

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
        public void updateObject(Integer id, int property) {
            if (isValid() && identifier.equals(id)) {
                // TODO check Property.
                updateData();
            }
        }

        @Override
        public void deleteObject(Integer id) {
            if (isValid() && identifier.equals(id)) {
                isValid = false;
                GraphicController.getController().unregister(this);
                // 3 is MATPLOT_INDEX
                dispose(iType == 3);
            }
        }

        @Override
        public void createObject(Integer id) {
        }
    }
}
