package org.scilab.modules.gui.utils;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.Icon;


public class ColorBox implements Icon {

    private Color color = null;
    private int height = 0;
    private int width = 0;

    public static ColorBox createColorBox(int width, int height, Color color) {
        return new ColorBox(width, height, color);
    }

    public ColorBox(int width, int height, Color color) {
        this.width = width;
        this.height = height;
        this.color = color;
    }

    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2d = (Graphics2D) g;

        //draw filled square
        g2d.setColor(color);
        g2d.setStroke(new BasicStroke(1.0f));
        g2d.fillRect(x, y, width, height);

        //draw black border
        g2d.setColor(Color.BLACK);
        g2d.drawRect(x, y, width, height);
    }

    public int getIconWidth() {
        return width;
    }

    public int getIconHeight() {
        return height;
    }

}
