/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.utils.shapes.geometry;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * @author Pierre Lando
 */
public final class SphereFactory {
    private static SphereFactory singleton;

    private SphereFactory() {
    }

    public static SphereFactory getSingleton() {
        if (singleton == null) {
            singleton = new SphereFactory();
        }
        return singleton;
    }

    /**
     * Create a sphere geometry with classic longitude / latitude division.
     * @param canvas the canvas where the geometry will be created.
     * @param radius the sphere radius.
     * @param givenLatitude number of division in latitude.
     * @param givenLongitude number of division in longitude.
     * @return a sphere geometry with classic longitude / latitude division.
     */
    public Geometry create(Canvas canvas, Float radius, int givenLatitude, int givenLongitude) {
        int latitude;
        int longitude;
        if (givenLatitude < 3) {
            latitude = 3;
        } else {
            latitude = givenLatitude;
        }

        if (givenLongitude < 4) {
            longitude = 4;
        } else {
            longitude = givenLongitude;
        }

        ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();
        vertexBuffer.setData(createVertexData(radius, latitude, longitude), 4);

        IndicesBuffer indicesBuffer = canvas.getBuffersManager().createIndicesBuffer();
        indicesBuffer.setData(createIndicesData(latitude, longitude));

        DefaultGeometry geometry = new DefaultGeometry();
        geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
        geometry.setVertices(vertexBuffer);
        geometry.setIndices(indicesBuffer);

        return geometry;
    }

    private IntBuffer createIndicesData(int latitudeResolution, int longitudeResolution) {
        int indexNumber = 6 * (latitudeResolution - 3) * longitudeResolution + 6 * longitudeResolution;
        IntBuffer indexBuffer = IntBuffer.allocate(indexNumber);
        indexBuffer.rewind();

        for (int i = 0; i < longitudeResolution; i++) {
            indexBuffer.put(0);
            indexBuffer.put(1 + i);
            indexBuffer.put(1 + (i + 1) % longitudeResolution);
        }

        for (int latitude = 1; latitude < latitudeResolution - 2; latitude++) {
            for (int longitude = 0; longitude < longitudeResolution; longitude++) {
                int aIndex = 1 + longitude + (latitude - 1) * longitudeResolution;
                int bIndex = 1 + ((longitude + 1) % longitudeResolution) + (latitude - 1) * longitudeResolution;
                int cIndex = 1 + ((longitude + 1) % longitudeResolution) + latitude * longitudeResolution;
                int dIndex = 1 + longitude + latitude * longitudeResolution;

                indexBuffer.put(aIndex);
                indexBuffer.put(bIndex);
                indexBuffer.put(cIndex);

                indexBuffer.put(aIndex);
                indexBuffer.put(cIndex);
                indexBuffer.put(dIndex);
            }
        }


        int lastVertexIndex = (latitudeResolution - 2) * longitudeResolution + 2 - 1;
        for (int i = 0; i < longitudeResolution; i++) {
            indexBuffer.put(lastVertexIndex);
            indexBuffer.put(lastVertexIndex - longitudeResolution + i);
            indexBuffer.put(lastVertexIndex - longitudeResolution + (i + 1) % longitudeResolution);
        }

        indexBuffer.rewind();
        return indexBuffer;
    }

    private FloatBuffer createVertexData(Float radius, int latitudeResolution, int longitudeResolution) {
        int vertexNumber = (latitudeResolution - 2) * longitudeResolution + 2;
        FloatBuffer dataBuffer = FloatBuffer.allocate(vertexNumber * 4);
        dataBuffer.rewind();

        dataBuffer.put(new float[] {0, 0, radius, 1});

        for (int latitude = 1; latitude < latitudeResolution - 1; latitude++) {
            double latitudeAngle = latitude * Math.PI / (latitudeResolution - 1);

            float z = (float) (radius * Math.cos(latitudeAngle));
            float discRadius = (float) (radius * Math.sin(latitudeAngle));

            for (int longitude = 0; longitude < longitudeResolution; longitude++) {
                double longitudeAngle = longitude * 2 * Math.PI / longitudeResolution;
                float x = (float) (discRadius * Math.cos(longitudeAngle));
                float y = (float) (discRadius * Math.sin(longitudeAngle));

                dataBuffer.put(x);
                dataBuffer.put(y);
                dataBuffer.put(z);
                dataBuffer.put(1);
            }
        }

        dataBuffer.put(new float[] {0, 0, -radius, 1});

        dataBuffer.rewind();
        return dataBuffer;
    }
}
