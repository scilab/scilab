/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.font.FontRenderContext;
import java.awt.font.GlyphVector;
import java.awt.geom.Rectangle2D;

import javax.swing.FocusManager;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UISplashScreen extends UIComponent {

    private final Color defaultBarColor = Color.decode("#E00000");
    private JDialog win;
    private ImageIcon icon;
    private Point position;
    private Color color;
    private double percent;
    private String text;
    private Font font;
    private Color fontColor;
    private boolean centerText;

    public UISplashScreen(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        win = new JDialog();

        return win;
    }

    @UIComponentAnnotation(attributes = {"icon", "parent", "center-parent", "text-position", "color", "bar-height", "font", "center-text"})
    public Object newInstance(ImageIcon icon, UIComponent parent, boolean centerParent, Point position, Color color, int barHeight, Font font, boolean centerText) {
        this.icon = icon;
        this.position = position;
        this.color = color;
        this.centerText = centerText;

        if (font == null) {
            this.font = UITools.getFont(null);
        } else {
            this.font = font;
        }

        this.fontColor = UITools.getFontColor(this.font);

        if (parent == null) {
            win = new JDialog();
        } else {
            Object c = parent.getComponent();
            if (c instanceof Frame) {
                win = new JDialog((Frame) c, true);
            } else if (c instanceof Dialog) {
                win = new JDialog((Dialog) c, true);
            } else {
                win = new JDialog();
            }
        }

        final int bh = barHeight == Integer.MAX_VALUE ? 5 : barHeight;

        win.setUndecorated(true);
        win.setResizable(false);
        win.setAlwaysOnTop(true);
        win.setPreferredSize(new Dimension(icon.getIconWidth(), icon.getIconHeight() + bh));

        Point p = new Point(0, 0);
        Dimension dim;
        if (parent != null && parent.getComponent() instanceof Component) {
            Component c = (Component) parent.getComponent();
            if (centerParent) {
                SwingUtilities.convertPointToScreen(p, c);
                dim = c.getSize(null);
            } else {
                Rectangle r = ScreenInfo.getScreenBounds(ScreenInfo.getScreenID(c));
                dim = new Dimension(r.width, r.height);
                p.x = r.x;
                p.y = r.y;
            }
        } else {
            Component c = FocusManager.getCurrentManager().getFocusOwner();
            if (c == null) {
                dim = ScreenInfo.getScreenDimension(0);
            } else {
                Rectangle r = ScreenInfo.getScreenBounds(ScreenInfo.getScreenID(c));
                dim = new Dimension(r.width, r.height);
                p.x = r.x;
                p.y = r.y;
            }
        }

        p.x += (dim.width - icon.getIconWidth()) / 2;
        p.y += (dim.height - icon.getIconHeight()) / 2;

        win.setLocation(p.x, p.y);

        JPanel image = new JPanel() {
            public void paintComponent(Graphics g) {
                ImageIcon im = UISplashScreen.this.icon;
                if (im != null) {
                    Graphics2D g2d = (Graphics2D) g;
                    int w = im.getIconWidth();
                    int h = im.getIconHeight();
                    g2d.drawImage(im.getImage(), 0, 0, w, h, this);
                    g2d.setColor(Color.WHITE);
                    g2d.fill(new Rectangle(0, h, w, bh));
                    g2d.setColor(UISplashScreen.this.color == null ? defaultBarColor : UISplashScreen.this.color);
                    g2d.fill(new Rectangle(0, h, (int) (w * percent), bh));
                    if (text != null && !text.isEmpty()) {
                        g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                        g2d.setColor(UISplashScreen.this.fontColor);
                        g2d.setFont(UISplashScreen.this.font);
                        if (UISplashScreen.this.centerText) {
                            FontRenderContext frc = g2d.getFontRenderContext();
                            Font f = UISplashScreen.this.font;
                            GlyphVector gv = f.createGlyphVector(frc, text);
                            Rectangle2D r = gv.getVisualBounds();
                            g2d.drawGlyphVector(gv, (float) ((w - r.getWidth()) / 2), (float) (h - UISplashScreen.this.position.y));
                        } else {
                            g2d.drawString(text, UISplashScreen.this.position.x, h - UISplashScreen.this.position.y);
                        }
                    }
                }
            }
        };

        win.getContentPane().setLayout(new BorderLayout());
        win.getContentPane().add(image, BorderLayout.CENTER);

        return win;
    }

    public void add(JComponent c) {
    }

    public void finish() {
        win.pack();
        win.setVisible(true);
    }

    public void remove() {
        win.dispose();
        super.remove();
    }

    public ImageIcon getIcon() {
        return icon;
    }

    public void setText(String text) {
        this.text = text;
        win.repaint();
    }

    public void setPercent(double percent) {
        if (percent > 1) {
            remove();
        } else {
            this.percent = percent;
            win.repaint();
        }
    }
}
