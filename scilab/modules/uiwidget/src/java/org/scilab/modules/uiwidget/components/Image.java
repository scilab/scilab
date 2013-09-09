/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

import javax.swing.ImageIcon;
import javax.swing.JLabel;

import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Image extends ScilabWidget {

    private double[] scale;
    private double[] shear;
    private double angle;
    private ImageIcon baseIcon;

    public Image(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        JLabel label = new JLabel();
        label.setOpaque(true);
        label.setBackground(Color.WHITE);

        return label;
    }

    public void setString(String[][] strs) {
        JLabel label = (JLabel) getModifiableComponent();
        String s = getSingleString(strs);
        if (s != null) {
            StringConverters.StringConverter converter = StringConverters.getConverter(ImageIcon.class);
            ImageIcon icon = (ImageIcon) converter.convert(s);
            label.setIcon(icon);
            baseIcon = icon;
            super.setString(strs);
        }
    }

    public void initialize() {
        super.initialize();
        setValue(new double[] {1, 1, 0, 0, 0});
        try {
            setRelief(ScilabRelief.RAISED);
        } catch (UIWidgetException e) { }
    }

    public void setValue(double[] value) {
        super.setValue(value);
        if (value != null && value.length == 5) {
            JLabel label = (JLabel) getModifiableComponent();
            if (value[0] == 1 && value[1] == 1 && value[1] == 0 && value[1] == 0 && value[1] == 0 && label.getIcon() != baseIcon) {
                label.setIcon(baseIcon);
            } else if (baseIcon != null) {
                Rectangle r = new Rectangle(0, 0, baseIcon.getIconWidth(), baseIcon.getIconHeight());
                AffineTransform transf = AffineTransform.getScaleInstance(value[0], value[1]);
                transf.shear(value[2], value[3]);
                transf.rotate(Math.toRadians(value[4]), r.width * value[0] / 2, r.height * value[1] / 2);
                r = transf.createTransformedShape(r).getBounds();

                BufferedImage newimg = new BufferedImage(r.width, r.height, BufferedImage.TRANSLUCENT);
                Graphics2D g2d = newimg.createGraphics();
                g2d.drawImage(baseIcon.getImage(), transf, label);
                g2d.dispose();

                label.setIcon(new ImageIcon(newimg));
            }
        }
    }
}
