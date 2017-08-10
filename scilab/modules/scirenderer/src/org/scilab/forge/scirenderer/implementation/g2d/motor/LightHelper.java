/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
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

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.Color;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;


import org.scilab.forge.scirenderer.tranformations.Vector3f;
import org.scilab.forge.scirenderer.implementation.g2d.lighting.G2DLight;
import org.scilab.forge.scirenderer.shapes.appearance.Material;

/**
 * @author Pedro SOUZA
 */
public class LightHelper {

    /**
     * @param buffer the float buffer.
     * @param stride the stride between elements.
     * @return an array of Vector3f from the given float buffer.
     */
    public static Vector3f[] getVector3f(FloatBuffer buffer, int stride) {
        if (buffer == null) {
            return null;
        }
        if (stride < 3) {
            return null;
        }

        float[] floats;
        buffer.rewind();
        if (buffer.hasArray()) {
            floats = buffer.array();
        } else {
            floats = new float[buffer.limit()];
            buffer.get(floats);
        }

        Vector3f[] ret = new Vector3f[floats.length / stride];
        for (int i = 0; i < floats.length; i += stride) {
            ret[i] = new Vector3f(floats[i], floats[i + 1], floats[i + 2]);
        }
        return ret;
    }

    /**
     * @param buffer the float buffer.
     * @param index the indices  buffer.
     * @param stride the stride between elements.
     * @param transf matrix to transform the vector, if null no transformation is applied.
     * @return an array of Vector3f from the given float buffer.
     */
    public static Vector3f[] getIndexedVector3f(FloatBuffer buffer, IntBuffer index, int stride, float[] transf) {
        if (buffer == null || index == null) {
            return null;
        }
        if (stride < 3) {
            return null;
        }

        float[] floats;
        buffer.rewind();
        if (buffer.hasArray()) {
            floats = buffer.array();
        } else {
            floats = new float[buffer.limit()];
            buffer.get(floats);
        }

        int[] idx;
        index.rewind();
        if (index.hasArray()) {
            idx = index.array();
        } else {
            idx = new int[index.limit()];
            index.get(idx);
        }

        Vector3f[] ret = new Vector3f[idx.length];
        float x, y, z;
        if (transf != null && transf.length == 16) {
            for (int i = 0; i < idx.length; ++i) {
                ret[i] = transform(floats[stride * idx[i]], floats[stride * idx[i] + 1], floats[stride * idx[i] + 2], transf);
            }
        } else {
            for (int i = 0; i < idx.length; ++i) {
                ret[i] = new Vector3f(floats[stride * idx[i]], floats[stride * idx[i] + 1], floats[stride * idx[i] + 2]);
            }
        }
        return ret;
    }

    static Vector3f transform(float x, float y, float z, float[] transf) {
        float xx = transf[0] * x + transf[4] * y + transf[8] * z + transf[12];
        float yy = transf[1] * x + transf[5] * y + transf[9] * z + transf[13];
        float zz = transf[2] * x + transf[6] * y + transf[10] * z + transf[14];
        return new Vector3f(xx, yy, zz);
    }

    static Vector3f transformDirection(float x, float y, float z, float[] transf) {
        float xx = transf[0] * x + transf[4] * y + transf[8] * z;
        float yy = transf[1] * x + transf[5] * y + transf[9] * z;
        float zz = transf[2] * x + transf[6] * y + transf[10] * z;
        return new Vector3f(xx, yy, zz);
    }

    /**
     * Apply the given ambient color to the output.
     * @param ambient the ambient color.
     * @param output the color vector to apply the ambient color.
     * @param additive if true the ambient color is added to output.
     * @return the resulting color vector.
     */
    public static Color[] applyAmbient(Color ambient, Color[] output, boolean additive) {
        for (int i = 0; i < output.length; ++i) {
            if (additive) {
                output[i] = getColorSum(ambient, output[i]);
            } else {
                output[i] = ambient;
            }
        }
        return output;
    }

    /**
     * Apply the given ambient color to the output.
     * @param ambient the ambient color.
     * @param input the input color.
     * @param output the color vector to apply the ambient color.
     * @param additive if true the ambient color is added to output.
     * @return the resulting color vector.
     */
    public static Color[] applyAmbient(Color ambient, Color[] input, Color[] output, boolean additive) {
        for (int i = 0; i < output.length; ++i) {
            if (additive) {
                output[i] = getColorSum(getColorProduct(ambient, input[i]), output[i]);
            } else {
                output[i] = getColorProduct(ambient, input[i]);
            }
        }
        return output;
    }

    /**
     * Apply diffuse light to the output colors
     * @param light the light position or direction.
     * @param directional if true the vector light is considered a direction otherwise a position.
     * @param vertices the surface vertices.
     * @param normals the surface normals.
     * @param colors the surface per-vertex colors.
     * @param dffuse the light diffuse color.
     * @param output the output color vector.
     * @param additive if true the calculated diffuse color is added to the output.
     * @return the resulting color vector.
     */
    public static Color[] applyDiffuse(Vector3f light, boolean directional, Vector3f[] vertices, Vector3f[] normals, Color[] colors, Color diffuse, Color[] output, boolean additive) {
        float ndotl;
        for (int i = 0; i < colors.length; ++i) {

            if (directional) {
                ndotl = normals[i].scalar(light);
            } else {
                Vector3f ray = light.minus(vertices[i]).getNormalized();
                ndotl = normals[i].scalar(ray);
            }
            ndotl = clamp(ndotl);
            Color c = getColorProduct(colors[i], diffuse);
            if (additive) {
                output[i] = getColorSum(getColorProduct(c, ndotl), output[i]);
            } else {
                output[i] = getColorProduct(c, ndotl);
            }
        }
        return output;
    }

    /**
     * Apply diffuse light to the output colors
     * @param light the light position or direction.
     * @param directional if true the vector light is considered a direction otherwise a position.
     * @param vertices the surface vertices.
     * @param normals the surface normals.
     * @param color the surface color.
     * @param output the output color vector.
     * @param additive if true the calculated diffuse color is added to the output.
     * @return the resulting color vector.
     */
    public static Color[] applyDiffuse(Vector3f light, boolean directional, Vector3f[] vertices, Vector3f[] normals, Color color, Color[] output, boolean additive) {
        float ndotl;
        for (int i = 0; i < output.length; ++i) {

            if (directional) {
                ndotl = normals[i].scalar(light);
            } else {
                Vector3f ray = light.minus(vertices[i]).getNormalized();
                ndotl = normals[i].scalar(ray);
            }
            ndotl = clamp(ndotl);
            if (additive) {
                output[i] = getColorSum(getColorProduct(color, ndotl), output[i]);
            } else {
                output[i] = getColorProduct(color, ndotl);
            }
        }
        return output;
    }

    public static Color[] applySpecular(Vector3f camera, Vector3f light, float shininess, boolean directional, Vector3f[] vertices, Vector3f[] normals, Color specular, Color[] output, boolean additive) {

        for (int i = 0; i < output.length; ++i) {

            Vector3f view = camera.minus(vertices[i]).getNormalized();
            Vector3f half;
            float ndotl;
            if (directional) {
                half = view.plus(light);
                ndotl = normals[i].scalar(light);
            } else {
                Vector3f ray = light.minus(vertices[i]).getNormalized();
                half = view.plus(ray);
                ndotl = normals[i].scalar(ray);
            }
            half = half.getNormalized();

            float s = 0.0f;
            if (ndotl > 0.0f) {
                s = normals[i].scalar(half);
                s = clamp(s);
                s = (float)Math.pow((double)s, (double)shininess);
            }

            if (additive) {
                output[i] = getColorSum(getColorProduct(specular, s), output[i]);
            } else {
                output[i] = getColorProduct(specular, s);
            }
        }
        return output;
    }

    /**
     * Apply a per-vertex lighting to the given colors
     * @param light the light.
     * @param mat the material properties.
     * @param camera the camera position.
     * @param vertices the surface vertices.
     * @param normals the surface normals.
     * @param colors the surface per-vertex colors.
     * @param output the output color vector.
     * @param transf the light transformation matrix. If null no transformation is applyed.
     * @param additive if true the calculated color is added to the output.
     * @return the resulting color vector.
     */
    public static Color[] applyLight(G2DLight light, Material mat, Vector3f camera, Vector3f[] vertices, Vector3f[] normals, Color[] colors, Color[] output, float[] transf, boolean additive) {
        Color ambient = getColorProduct(mat.getAmbientColor(), light.getAmbientColor());
        Color diffuse = getColorProduct(mat.getDiffuseColor(), light.getDiffuseColor());
        Color specular = getColorProduct(mat.getSpecularColor(), light.getSpecularColor());

        Color[] finalColor;
        if (mat.isColorMaterialEnable()) {
            finalColor = applyAmbient(light.getAmbientColor(), colors, output, additive);
        } else {
            finalColor = applyAmbient(ambient, output, additive);
        }

        float[] v;
        if (light.isPoint()) {
            v = light.getPosition().getDataAsFloatArray();
        } else {
            v = light.getDirection().getDataAsFloatArray();
        }

        Vector3f vec;
        if (transf != null && transf.length == 16) {
            if (light.isPoint()) {
                vec = transform(v[0], v[1], v[2], transf);
            } else {
                vec = transformDirection(v[0], v[1], v[2], transf).getNormalized();
            }
        } else {
            vec = new Vector3f(v[0], v[1], v[2]);
        }

        if (mat.isColorMaterialEnable()) {
            finalColor = applyDiffuse(vec, !light.isPoint(), vertices, normals, colors, light.getDiffuseColor(), finalColor, true);
        } else {
            finalColor = applyDiffuse(vec, !light.isPoint(), vertices, normals, diffuse, finalColor, true);
        }

        finalColor = applySpecular(camera, vec, mat.getShininess(), !light.isPoint(), vertices, normals, specular, finalColor, true);

        return finalColor;
    }

    /**
     * return the product of the given colors
     */
    private static Color getColorProduct(Color a, Color b) {
        float[] ca = a.getComponents(null);
        float[] cb = b.getComponents(null);
        return new Color(ca[0] * cb[0], ca[1] * cb[1], ca[2] * cb[2]);
    }

    /**
     * return the clamped product of the color
     */
    private static Color getColorProduct(Color a, float f) {
        float[] ca = a.getComponents(null);
        return new Color(clamp(ca[0] * f), clamp(ca[1] * f), clamp(ca[2] * f));
    }

    /**
     * return the clamped sum of the given colors
     */
    private static Color getColorSum(Color a, Color b) {
        float[] ca = a.getComponents(null);
        float[] cb = b.getComponents(null);
        return new Color(clamp(ca[0] + cb[0]), clamp(ca[1] + cb[1]), clamp(ca[2] + cb[2]));
    }

    /**
     * Clamp the given value to [0, 1]
     */
    private static float clamp(float f) {
        f = f < 0.0f ? 0.0f : f;
        f = f > 1.0f ? 1.0f : f;
        return f;
    }

    static Vector3f reflect(Vector3f I, Vector3f N) {
        return I.minus(N.times(2 * I.scalar(N)));
    }
}
