/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.*;
import java.awt.Color;
import java.awt.Graphics2D;
import java.util.ArrayList;
import java.util.List;
import org.scilab.forge.scirenderer.implementation.g2d.*;

import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Calixte DENIZET
 */
public class PolyLine {
    /* extends AbstractDrawable3DObject {

    private boolean monochromatic;
    private Stroke stroke;
    private Shape clip;

    public PolyLine(Vector3d[] vertices, Color[] colors, Stroke stroke, Shape clip) throws InvalidPolygonException {
    super(vertices, colors);
    this.monochromatic = isMonochromatic(colors);
    this.stroke = stroke;
    this.clip = clip;
    }

    @Override
    protected Path2D getProjectedPolyLine() {
    int i;
    Path2D.Double path = new Path2D.Double();

    // Trim on the left
    for (i = 0; i < vertices.length; i++) {
    if (!AbstractDrawable3DObject.isNanOrInf(vertices[i])) {
    break;
    }
    }

    if (i < vertices.length) {
    path.moveTo(vertices[i].getX(), vertices[i].getY());
    i++;
    boolean broken = false;
    for (; i < vertices.length; i++) {
    if (AbstractDrawable3DObject.isNanOrInf(vertices[i])) {
    if (!broken) {
    broken = true;
    }
    } else {
    if (broken) {
    broken = false;
    path.moveTo(vertices[i].getX(), vertices[i].getY());
    } else {
    path.lineTo(vertices[i].getX(), vertices[i].getY());
    }
    }
    }
    }

    return path;
    }

    public void draw(Graphics2D g2d) {
    Shape oldClip = g2d.getClip();
    Stroke oldStroke = g2d.getStroke();

    g2d.clip(clip);

    if (monochromatic) {
    g2d.setColor(colors[0]);
    g2d.setStroke(stroke);
    g2d.draw(getProjectedPolyLine());
    } else {
    Vector3D start = null;
    Color color = null;
    double cumLen = 0;
    float[] dashArray = stroke.getDashArray();
    float lwidth = stroke.getLineWidth();
    for (int i = 0; i < vertices.length; i++) {
    if (AbstractDrawable3DObject.isNanOrInf(vertices[i])) {
    start = null;
    } else {
    if (start == null) {
    start = vertices[i];
    color = colors[i];
    } else {
    Stroke nstroke = new G2DStroke(lwidth, dashArray, cumLen);
    g2d.setStroke(nstroke);
    g2d.setColor(color);
    g2d.draw(new Line2D.Double(start.getX(), start.getY(), vertices[i].getX(), vertices[i].getY()));
    cumLen += Math.hypot(start.getX() - vertices[i].getX(), start.getY() - vertices[i].getY());
    }
    }
    }
    }

    g2d.setClip(oldClip);
    g2d.setStroke(oldStroke);
    }*/
}
