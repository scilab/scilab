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
package org.scilab.forge.scirenderer.examples;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AbstractTextureDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.awt.Dimension;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.nio.ByteBuffer;
import java.util.Date;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class TextureExample extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Texture Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private TextureExample() {

        setTitle(TITLE);

        final ClickCounter clickCounter = new ClickCounter();
        getPanel().addMouseListener(clickCounter);
        getCanvas().setMainDrawer(new TextureDrawer(getCanvas(), clickCounter));

        animate(true);
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new TextureExample();
    }

    /**
     * Texture drawer.
     */
    private final class TextureDrawer implements Drawer {
        private final ClickCounter clickCounter;

        private final Appearance appearance1;
        private final Appearance appearance2;

        private final Geometry geometry1;
        private final Geometry geometry2;

        /**
         * Constructor.
         * @param canvas the canvas where the textures will be drawn.
         * @param clickCounter the click counter used to determine witch texture is drawn.
         */
        public TextureDrawer(Canvas canvas, ClickCounter clickCounter) {
            this.clickCounter = clickCounter;

            geometry1 = new SimpleGeometry(canvas);
            geometry2 = new LessSimpleGeometry(canvas);

            Texture texture1 = canvas.getTextureManager().createTexture();
            Texture texture2 = canvas.getTextureManager().createTexture();

            appearance1 = new Appearance();
            texture1.setDataProvider(new Simple2DTextureDataProvider());
            appearance1.setTexture(texture1);

            appearance2 = new Appearance();
            texture2.setDataProvider(new Simple1DTextureDataProvider());
            appearance2.setTexture(texture2);
        }

        @Override
        public void draw(DrawingTools dt) {
            dt.clear(new Color());

            try {
                double t = System.currentTimeMillis() / 30.;

                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                dt.getTransformationManager().getProjectionStack().push(projection);
                dt.getTransformationManager().getModelViewStack().push(TransformationFactory.getScaleTransformation(Math.sqrt(2) / 2));
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(t, 1, 2, 3));

                if (clickCounter.getCount() % 2 == 0) {
                    dt.draw(geometry1, appearance1);
                } else {
                    dt.draw(geometry2, appearance2);
                }
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }
    }

    /**
     * This mouse adapter count the number of click.
     */
    private final class ClickCounter extends MouseAdapter {
        private int count;

        /**
         * Default constructor.
         */
        public ClickCounter() {
            count = 0;
        }

        @Override
        public void mousePressed(MouseEvent e) {
            count++;
        }

        /**
         * Number of click getter.
         * @return the number of click until the application start.
         */
        public int getCount() {
            return count;
        }
    }

    /**
     * @author Pierre Lando
     */
    private final class SimpleGeometry extends DefaultGeometry implements Geometry {

        /**
         * Default constructor.
         * @param canvas the canvas where the geometry will be created.
         */
        public SimpleGeometry(Canvas canvas) {
            ElementsBuffer vertices = canvas.getBuffersManager().createElementsBuffer();
            vertices.setData(new float[] {
                                 -1, -1, 0, 1,
                                 -1, +1, 0, 1,
                                 +1, +1, 0, 1,
                                 +1, -1, 0, 1,
                             }, 4);

            IndicesBuffer indices = canvas.getBuffersManager().createIndicesBuffer();
            indices.setData(new int[] {
                                0, 1, 2, 0, 2, 3
                            });

            ElementsBuffer textureCoordinates =  canvas.getBuffersManager().createElementsBuffer();
            textureCoordinates.setData(new float[] {
                                           0, 0,
                                           0, 1,
                                           1, 1,
                                           1, 0
                                       }, 2);

            setFillDrawingMode(FillDrawingMode.TRIANGLES);
            setVertices(vertices);
            setIndices(indices);
            setTextureCoordinates(textureCoordinates);
        }
    }

    /**
     * @author Pierre Lando
     */
    private final class LessSimpleGeometry extends DefaultGeometry implements Geometry {

        private static final int SIZE = 65;
        private static final double HALF_SIZE = SIZE / 2.;

        /**
         * Default constructor.
         * @param canvas the {@link org.scilab.forge.scirenderer.Canvas} where the drawn will be performed.
         */
        public LessSimpleGeometry(Canvas canvas) {
            int k;
            ElementsBuffer vertices = canvas.getBuffersManager().createElementsBuffer();
            float[] verticesData = new float [SIZE * SIZE * 4];
            k = 0;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    double x = 2 * (i / (SIZE - 1f) - .5);
                    double y = 2 * (j / (SIZE - 1f) - .5);
                    double d = Math.max(Math.abs(x), Math.abs(y));
                    Vector3d v = new Vector3d(x, y, 0).getNormalized().times(d);
                    verticesData[k++] = (float) v.getX();
                    verticesData[k++] = (float) v.getY();
                    verticesData[k++] = 0;
                    verticesData[k++] = 1;
                }
            }

            vertices.setData(verticesData, 4);

            ElementsBuffer textureCoordinates =  canvas.getBuffersManager().createElementsBuffer();
            float[] textureCoordinatesData = new float[SIZE * SIZE];
            k = 0;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    double v = Math.sqrt(Math.pow(i - HALF_SIZE, 2) + Math.pow(j - HALF_SIZE, 2));
                    v = (Math.sin(4 * Math.PI * v / SIZE) + 1) / 2;
                    textureCoordinatesData[k++] = (float) v;
                }
            }
            textureCoordinates.setData(textureCoordinatesData, 1);

            IndicesBuffer indices = canvas.getBuffersManager().createIndicesBuffer();
            int[] indicesData = new int [6 * (SIZE - 1) * (SIZE - 1)];
            k = 0;
            int u = 0;
            for (int i = 0; i < SIZE - 1; i++) {
                for (int j = 0; j < SIZE - 1; j++) {
                    indicesData[k++] = u;
                    indicesData[k++] = u + 1;
                    indicesData[k++] = u + SIZE + 1;
                    indicesData[k++] = u;
                    indicesData[k++] = u + SIZE + 1;
                    indicesData[k++] = u + SIZE;
                    u++;
                }
                u++;
            }
            indices.setData(indicesData);

            setFillDrawingMode(FillDrawingMode.TRIANGLES);
            setVertices(vertices);
            setIndices(indices);
            setTextureCoordinates(textureCoordinates);
        }
    }

    /**
     * @author Pierre Lando
     */
    private final class Simple1DTextureDataProvider extends AbstractTextureDataProvider {
        private final int size = 16;
        private ByteBuffer buffer;

        /**
         * Default constructor.
         */
        public Simple1DTextureDataProvider() {
            imageType = ImageType.RGBA_BYTE;
            this.buffer = ByteBuffer.allocate(4 * size);  // 4 for RGBA.
            buffer.rewind();
            new SimpleThread().start();
        }

        /**
         * Fill the data buffer.
         */
        private void fillBuffer() {
            double phi = Math.PI * 2 / 1024;
            long time = new Date().getTime();
            long base = (int) ((time / 4) % 1024);
            ByteBuffer tempBuffer = ByteBuffer.allocate(4 * size);
            tempBuffer.rewind();
            for (int i = 0; i < size; i++) {
                tempBuffer.put(toByte((Math.sin(base * phi) + 1) / 2));
                tempBuffer.put(toByte((Math.sin(2 * base * phi + i) + 1) / 2));
                tempBuffer.put(toByte((Math.sin(base * phi + i) + 1) / 2));
                tempBuffer.put(toByte(255));
            }
            tempBuffer.rewind();
            this.buffer = tempBuffer;
        }

        @Override
        public Dimension getTextureSize() {
            return new Dimension(size, 1);
        }

        @Override
        public ByteBuffer getData() {
            buffer.rewind();
            return buffer;
        }

        @Override
        public ByteBuffer getSubData(int x, int y, final int width, final int height) {
            ByteBuffer tempBuffer = ByteBuffer.allocate(4 * width * height);
            this.buffer.position(x + y * size);
            byte[] data = new byte[4];
            for (int i = x; i < x + width; i++) {
                for (int j = y; j < y + height; j++) {
                    this.buffer.get(data);
                    tempBuffer.put(data);
                }
            }
            tempBuffer.rewind();
            this.buffer.rewind();
            return tempBuffer;
        }

        @Override
        public boolean isValid() {
            return true;
        }

        /**
         * This thread update the texture regularly.
         */
        private class SimpleThread extends Thread {

            /**
             * Default constructor.
             */
            public SimpleThread() {
            }

            @Override
            public void run() {
                for (;;) {
                    fillBuffer();
                    fireUpdate();
                    try {
                        synchronized (this) {
                            wait(32);
                        }
                    } catch (InterruptedException e) {
                        break;
                    }
                }
            }
        }
    }

    /**
     * @author Pierre Lando
     */
    private final class Simple2DTextureDataProvider extends AbstractTextureDataProvider {
        private final int size = 16;
        private ByteBuffer buffer;

        /**
         * Default constructor.
         */
        public Simple2DTextureDataProvider() {
            imageType = ImageType.RGBA_BYTE;
            this.buffer = ByteBuffer.allocate(4 * size * size);  // 4 for RGBA.
            buffer.rewind();
            new SimpleThread().start();
        }

        /**
         * Fill the data buffer.
         */
        private void fillBuffer() {
            double phi = Math.PI * 2 / 1024;
            long time = new Date().getTime();
            long base = (int) ((time / 4) % 1024);
            ByteBuffer tempBuffer = ByteBuffer.allocate(4 * size * size);
            tempBuffer.rewind();
            int k = 0;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    tempBuffer.put(toByte((Math.sin((5 * base + 16 * i) * phi) + 1) / 2));
                    tempBuffer.put(toByte((Math.sin((2 * base + 8 * j) * phi) + 1) / 2));
                    tempBuffer.put(toByte((Math.sin((7 * base + 4 * k) * phi) + 1) / 2));
                    tempBuffer.put(toByte(255));
                    k++;
                }
            }
            tempBuffer.rewind();
            this.buffer = tempBuffer;
        }

        @Override
        public Dimension getTextureSize() {
            return new Dimension(size, size);
        }

        @Override
        public ByteBuffer getData() {
            buffer.rewind();
            return buffer;
        }

        @Override
        public ByteBuffer getSubData(int x, int y, final int width, final int height) {
            ByteBuffer tempBuffer = ByteBuffer.allocate(4 * width * height);
            this.buffer.position(x + y * size);
            byte[] data = new byte[4];
            for (int i = x; i < x + width; i++) {
                for (int j = y; j < y + height; j++) {
                    this.buffer.get(data);
                    tempBuffer.put(data);
                }
            }
            tempBuffer.rewind();
            this.buffer.rewind();
            return tempBuffer;
        }

        @Override
        public boolean isValid() {
            return true;
        }

        /**
         * This thread update the texture regularly.
         */
        private class SimpleThread extends Thread {

            /**
             * Default constructor.
             */
            public SimpleThread() {
            }

            @Override
            public void run() {
                for (;;) {
                    fillBuffer();
                    fireUpdate();
                    try {
                        synchronized (this) {
                            wait(32);
                        }
                    } catch (InterruptedException e) {
                        break;
                    }
                }
            }
        }
    }
}
