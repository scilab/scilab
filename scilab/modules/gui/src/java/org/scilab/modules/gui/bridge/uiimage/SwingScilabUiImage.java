/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.gui.bridge.uiimage;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.uiimage.SimpleUiImage;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Image in GUIs
 * @author Han DONG
 * @author Vincent COUVERT
 */
public class SwingScilabUiImage extends JLabel implements SwingViewObject, SimpleUiImage {

    private static final long serialVersionUID = -3394912554085956130L;

    private Integer uid;

    private ImageIcon imi;
    private Image img;
    private String imageFile;
    private int imageWidth;
    private int imageHeight;

    private double[] scale;
    private double[] shear;
    private double angle;

    private Border defaultBorder = null;

    /**
     * Constructor
     */
    public SwingScilabUiImage() {
        super();
        setOpaque(true);
        setBackground(Color.WHITE);
        scale = new double[2];
        shear = new double[2];
    }

    /**
     * Draws a swing Scilab Image
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab element
     * @return the dimensions of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(getWidth(), getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * element
     * @return the position of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab element
     * @param newSize the dimensions to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * element
     * @param newPosition the position to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the Image
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        // Nothing to do...
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Image.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for Image */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Image.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for Image */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Image.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for Image */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Image.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for Image */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the Image text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the vertical alignment for the Image text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the Relief of the Image
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the Image
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the Image.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for Image */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Image.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for Image */
        throw new UnsupportedOperationException();
    }

    /**
     * gets directory of image
     * @return the directory string
     * @see org.scilab.modules.gui.text.SimpleText#getText()
     */
    public String getText() {
        return "";
    }

    /**
     * Sets the directory for image
     * @param newText the new directory to image
     */
    public void setText(String newText) {
        imageFile = newText;

        File file = new File(imageFile);
        if (file.exists() == false) {
            String filename = FindIconHelper.findImage(imageFile);
            file = new File(filename);
        }

        try {
            imi = new ImageIcon(ImageIO.read(file));
            img = imi.getImage();
            setIcon(imi);
        } catch (IOException e) {
        }

        //        imi = new ImageIcon(imageFile);
        //        img = imi.getImage();
        //        setIcon(imi);
    }

    public void setEmptyText() {
        imageFile = null;
        imi = null;
        img = null;
        setIcon(null);
    }

    /**
     * Rotates the image
     * @param angle the double value of the angle to rotate
     */
    public void setRotate(double angle) {
        this.angle = angle;
        updateImage();
    }

    /**
     * Shears the image
     * @param indices the double array of x, y values to shear
     */
    public void setShear(double[] indices) {
        shear = indices;
        updateImage();
    }

    /**
     * Scaless the image
     * @param indices the double array of x, y values to scale
     */
    public void setScale(double[] indices) {
        scale = indices;
        updateImage();
    }

    /**
     * Update the image with new values
     */
    private void updateImage() {
        if (img != null && img.getHeight(this) >= 0 && img.getWidth(this) >= 0) {
            imageWidth = (int) Math.ceil(img.getWidth(this) * scale[0]);
            imageHeight = (int) Math.ceil(img.getHeight(this) * scale[1]);
            BufferedImage bim = new BufferedImage(imageWidth, imageHeight, BufferedImage.TRANSLUCENT);
            Graphics2D g2 = bim.createGraphics();
            g2.fillRect(0, 0, imageWidth, imageHeight);
            g2.shear(shear[0], shear[1]);
            g2.rotate(Math.toRadians(angle), imageWidth / 2, imageHeight / 2);
            g2.drawImage(img, 0, 0, imageWidth, imageHeight, this);
            setIcon(new ImageIcon(bim));
        }
    }

    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(Integer id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {

        switch (property) {
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length == 0) {
                    return;
                }

                // Update the image parameters
                double[] imageParams = new double[5];
                if (doubleValue.length < 1) {
                    imageParams[0] = 1.0;
                } else {
                    imageParams[0] = doubleValue[0];
                }
                if (doubleValue.length < 2) {
                    imageParams[1] = 1.0;
                } else {
                    imageParams[1] = doubleValue[1];
                }
                if (doubleValue.length < 3) {
                    imageParams[2] = 0.0;
                } else {
                    imageParams[2] = doubleValue[2];
                }
                if (doubleValue.length < 4) {
                    imageParams[3] = 0.0;
                } else {
                    imageParams[3] = doubleValue[3];
                }
                if (doubleValue.length < 5) {
                    imageParams[4] = 0.0;
                } else {
                    imageParams[4] = doubleValue[4];
                }

                double[] scale = new double[2];
                scale[0] = imageParams[0];
                scale[1] = imageParams[1];
                setScale(scale);

                double[] shear = new double[2];
                shear[0] = imageParams[2];
                shear[1] = imageParams[3];
                setShear(shear);
                setRotate(imageParams[4]);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("Label.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Label.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
