package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AbstractTextureDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;

import java.awt.Dimension;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * @author Pierre Lando
 */
class FecDrawer {

    /** Set of properties that affect the texture. */
    private static final Set<Integer> TEXTURE_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_Z_BOUNDS__,
                GraphicObjectProperties.__GO_DATA_MODEL__,
                GraphicObjectProperties.__GO_COLOR_RANGE__,
                GraphicObjectProperties.__GO_OUTSIDE_COLOR__
            ));

    /** The parent drawer visitor. */
    private final DrawerVisitor drawerVisitor;

    /** Map of texture sorted by fec identifier */
    private final Map<Integer, Texture> textureMap;

    /**
     * Default constructor.
     * @param drawerVisitor the parent drawer visitor.
     */
    FecDrawer(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;
        textureMap = new HashMap<Integer, Texture>();
    }

    /**
     * Draw the given fec.
     * @param fec given fec object.
     * @throws ObjectRemovedException
     */
    public void draw(Fec fec) throws ObjectRemovedException, OutOfMemoryException {
        if (fec.getVisible()) {

            DrawingTools drawingTools = drawerVisitor.getDrawingTools();
            DataManager dataManager = drawerVisitor.getDataManager();
            ColorMap colorMap = drawerVisitor.getColorMap();

            try {
                DefaultGeometry geometry = new DefaultGeometry();
                Appearance appearance = new Appearance();

                geometry.setVertices(dataManager.getVertexBuffer(fec.getIdentifier()));
                geometry.setTextureCoordinates(dataManager.getTextureCoordinatesBuffer(fec.getIdentifier()));
                geometry.setIndices(dataManager.getIndexBuffer(fec.getIdentifier()));

                geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                geometry.setPolygonOffsetMode(true);

                if (fec.getLineMode()) {
                    geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                    geometry.setWireIndices(dataManager.getWireIndexBuffer(fec.getIdentifier()));
                    appearance.setLineColor(ColorFactory.createColor(colorMap, fec.getLineColor()));
                }
                appearance.setTexture(getTexture(fec));

                drawingTools.draw(geometry, appearance);
            } catch (SciRendererException e) {
                System.err.println("A '" + fec.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    /**
     * Texture getter.
     *
     * This method return the texture associated with the given fec object.
     * If no texture is associated, a new one will be created.
     *
     * If the texture to return have no data provider a new one is created.
     *
     * @param fec given fec object.
     * @return the texture associated with the given fec.
     */
    private Texture getTexture(Fec fec) {
        Texture texture = textureMap.get(fec.getIdentifier());
        if (texture == null) {
            texture = drawerVisitor.getCanvas().getTextureManager().createTexture();
            texture.setSWrappingMode(Texture.Wrap.CLAMP);
            texture.setTWrappingMode(Texture.Wrap.CLAMP);
            texture.setMagnificationFilter(Texture.Filter.NEAREST);
            texture.setMinifyingFilter(Texture.Filter.NEAREST);
            textureMap.put(fec.getIdentifier(), texture);
        }
        if (texture.getDataProvider() == null) {
            texture.setDataProvider(new FecColorTexture(fec));
        }
        return texture;
    }

    /**
     * Manage changes on the given object.
     *
     * If the given property affect the texture of a Fec object, the data provider will be reset.
     *
     * @param id the given object id.
     * @param property the changed property.
     */
    public void update(Integer id, int property) {
        if (TEXTURE_PROPERTIES.contains(property)) {
            Texture texture = textureMap.get(id);
            if (texture != null) {
                texture.setDataProvider(null);
            }
        }
    }

    /**
     * Update all texture.
     * Reset all texture data provider.
     * @throws ObjectRemovedException
     */
    void updateAll() throws ObjectRemovedException, OutOfMemoryException {
        for (Map.Entry<Integer, Texture> entry : textureMap.entrySet()) {
            drawerVisitor.getDataManager().updateTextureCoordinatesBuffer(entry.getKey());
            entry.getValue().setDataProvider(null);
        }
    }

    /**
     * Dispose the given object.
     * @param id given object identifier.
     */
    public void dispose(Integer id) {
        /** TODO
        Texture texture = textureMap.get(id);
        if (texture != null) {
            drawerVisitor.getCanvas().getTextureManager().release(texture);
        }
         **/
        textureMap.remove(id);
    }

    /**
     * This class is an implementation of {@link TextureDataProvider} that provide texture data for a given fec object.
     *
     * The texture is a 1D texture with:
     * - at first the minimal outside color.
     * - main colors:
     *  . If colorRange is enable, all color in the color range, in order.
     *  . If colorRange is disable, all color from the color map, in order.
     * - at last, the maximal outside color.
     *
     * The outside colors are always presents.
     *  .As user defined outside colors if any.
     *  .As minimal and maximal color from the main colors otherwise.
     */
    private class FecColorTexture extends AbstractTextureDataProvider {

        /** The fec object for which this class provide texture data */
        private final Fec fec;

        /**
         * Default constructor.
         * @param fec The fec object for which this class will provide texture data.
         */
        public FecColorTexture(Fec fec) {
            this.fec = fec;
            this.imageType = ImageType.RGBA_BYTE;
        }

        @Override
        public Dimension getTextureSize() {
            return new Dimension(getTextureLength(), 1);
        }

        @Override
        public ByteBuffer getData() {
            ColorMap colorMap = drawerVisitor.getColorMap();
            Integer[] outsideColor = fec.getOutsideColor();

            ByteBuffer buffer = ByteBuffer.allocate(4 * getTextureLength());

            int min;
            int max;
            if (useColorRange()) {
                Integer[] colorRange = fec.getColorRange();
                min = Math.max(1, colorRange[0]);
                max = Math.min(colorMap.getSize(), colorRange[1]);
            } else {
                min = 1;
                max = colorMap.getSize();
            }


            if (outsideColor[0] == 0) {
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, min)));
            } else if (outsideColor[0] > 0) {
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, outsideColor[0])));
            } else {
                // TODO: transparency.
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, -2)));
            }

            for (int i = min; i <= max; i++) {
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, i)));
            }

            if (outsideColor[1] == 0) {
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, max)));
            } else if (outsideColor[1] > 0) {
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, outsideColor[1])));
            } else {
                // TODO: transparency.
                buffer.put(toByte(ColorFactory.createRGBAColor(colorMap, -2)));
            }

            buffer.rewind();
            return buffer;
        }

        @Override
        public ByteBuffer getSubData(int x, int y, int width, int height) {
            ByteBuffer buffer = getData();
            ByteBuffer tempBuffer = ByteBuffer.allocate(4 * width * height);
            buffer.position(x + y * getTextureLength());
            byte[] data = new byte[4];
            for (int i = x; i < x + width; i++) {
                for (int j = y; j < y + height; j++) {
                    buffer.get(data);
                    tempBuffer.put(data);
                }
            }
            tempBuffer.rewind();
            buffer.rewind();
            return tempBuffer;
        }

        @Override
        public boolean isValid() {
            return true;
        }

        /**
         * Check if color range option is enable.
         * @return true if the color range is valid and not [0, 0].
         */
        private boolean useColorRange() {
            Integer[] colorRange = fec.getColorRange();
            return (colorRange != null) && (colorRange.length == 2) && ((colorRange[0] != 0) || (colorRange[1] != 0));
        }

        /**
         * Compute the texture length.
         * @return the texture length.
         */
        private int getTextureLength() {
            ColorMap colorMap = drawerVisitor.getColorMap();
            int length;
            if (useColorRange()) {
                Integer[] colorRange = fec.getColorRange();
                int min = Math.max(1, colorRange[0]);
                int max = Math.min(colorMap.getSize(), colorRange[1]);
                length = 1 + max - min;
            } else {
                length = colorMap.getSize();
            }

            // For outside colors.
            length += 2;
            return length;
        }
    }
}
