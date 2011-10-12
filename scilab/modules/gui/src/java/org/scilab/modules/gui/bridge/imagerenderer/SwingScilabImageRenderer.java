/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.imagerenderer;

import java.awt.Color;
import java.awt.Font;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.Dimension;
import java.awt.Graphics2D;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JScrollPane;

import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.imagerenderer.SimpleImageRenderer;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab ImageRenderer in GUIs
 * @author Han DONG
 * @author Vincent COUVERT
 */
public class SwingScilabImageRenderer extends JScrollPane implements SwingViewObject, SimpleImageRenderer {

	private static final long serialVersionUID = -3394912554085956130L;

	private String uid;

	private JLabel imageRenderer;
	private ImageIcon imi;
	private Image img;
	private String directory;

	/**
	 * Constructor
	 */
	public SwingScilabImageRenderer() {
		super();
		getViewport().add(getLabel());
		setBorder(BorderFactory.createEmptyBorder());
		setViewportBorder(BorderFactory.createEmptyBorder());
		setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
	}

	/**
	 * Apply a new font for the imageRendereri.
	 * @param font new font to use.
	 */
	public void setFont(Font font) {
		getLabel().setFont(font);
	}

	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		return getLabel().getFont();
	}

	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		return getLabel().getForeground();
	}

	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		getLabel().setForeground(color);
	}

	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		getLabel().setBackground(color);
	}

	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		return getLabel().getBackground();
	}

	/**
	 * Draws a swing Scilab PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Sets the visibility status of an UIElement
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 *                      (true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
		getLabel().setVisible(newVisibleState);
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
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab element
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
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab element
	 * @param newPosition the position to set to the element
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}

	/**
	 * Add a callback to the ImageRenderer
	 * @param callback the callback to set.
	 */
	public void setCallback(CommonCallBack callback) {
		// Nothing to do...
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the ImageRenderer.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for ImageRenderers */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the ImageRenderer.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for ImageRenderers */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the ImageRenderer.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for ImageRenderers */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the ImageRenderer.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for ImageRenderers */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the ImageRenderer text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		getLabel().setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the ImageRenderer text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		getLabel().setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the Relief of the ImageRenderer
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the ImageRenderer
	 */
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}

	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the ImageRenderer.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for ImageRenderers */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the ImageRenderer.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for ImageRenderers */
		throw new UnsupportedOperationException();
	}

	/**
	 * Create/Return the imageRenderer Java object
	 * @return the imageRenderer
	 */
	private JLabel getLabel() {
		if (imageRenderer == null) {
			imageRenderer = new JLabel();
			directory = "";
			imageRenderer.setOpaque(true);
			imi = new ImageIcon();
			img = imi.getImage();
			imageRenderer.setIcon(imi);
		}
		return imageRenderer;
	}

	/**
	 * gets directory of image in image render
	 * @return the directory string
	 * @see org.scilab.modules.gui.text.SimpleText#getText()
	 */
	public String getText() {
		return directory;
	}

	/**
	 * Sets the directory for image to render
	 * @param newText the new directory to image
	 */
	public void setText(String newText) {
		directory = newText;
		imi = new ImageIcon(directory);
		img = imi.getImage();
		imageRenderer.setIcon(imi);	
		setPreferredSize(new Dimension(imi.getIconWidth(), imi.getIconHeight()));
	}

	/**
	 * Rotates the image
	 * @param indices the double value of the angle to rotate
	 */
	public void setRotate(double[] indices) {
		if (img != null && img.getHeight(this) >= 0 && img.getWidth(this) >= 0) {
			int h = img.getHeight(this);
			int w = img.getWidth(this);
			BufferedImage bim = new BufferedImage(h, w, BufferedImage.TYPE_INT_RGB);
			Graphics2D g2 = bim.createGraphics();
			g2.rotate(Math.toRadians(indices[0]), w / 2, h / 2);
			g2.drawImage(img, 0, 0, this);
			imageRenderer.setIcon(new ImageIcon(bim));
		}
	}

	/**
	 * Shears the image
	 * @param indices the double array of x, y values to shear
	 */
	public void setShear(double[] indices) {
		if (img != null && img.getHeight(this) >= 0 && img.getWidth(this) >= 0) {
			int h = img.getHeight(this);
			int w = img.getWidth(this);
			BufferedImage bim = new BufferedImage(h, w, BufferedImage.TYPE_INT_RGB);
			Graphics2D g2 = bim.createGraphics();
			g2.shear(indices[0], indices[1]);
			g2.drawImage(img, 0, 0, this);
			imageRenderer.setIcon(new ImageIcon(bim));
		}
	}

	/**
	 * Scaless the image
	 * @param indices the double array of x, y values to scale
	 */
	public void setScale(double[] indices) {
		if (img != null && img.getHeight(this) >= 0 && img.getWidth(this) >= 0) {
			int h = img.getHeight(this) * (int) Math.ceil(indices[0]);
			int w = img.getWidth(this) * (int) Math.ceil(indices[1]);
			BufferedImage bim = new BufferedImage(h, w, BufferedImage.TYPE_INT_RGB);
			Graphics2D g2 = bim.createGraphics();
			g2.scale(indices[0], indices[1]);
			g2.drawImage(img, 0, 0, this);
			imageRenderer.setIcon(new ImageIcon(bim));
		}
	}

	/**
	 * Set the UID
	 * @param id the UID
	 */
	public void setId(String id) {
		uid = id;
	}

	/**
	 * Get the UID
	 * @return the UID
	 */
	public String getId() {
		return uid;
	}

	/**
	 * Generic update method
	 * @param property property name
	 * @param value property value
	 */
	public void update(String property, Object value) {
		SwingViewWidget.update(this, property, value);
	}
}
