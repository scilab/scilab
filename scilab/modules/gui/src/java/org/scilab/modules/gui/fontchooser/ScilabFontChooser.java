/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.gui.fontchooser;

import java.awt.Font;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Interface for FontChooser for Scilab
 * @author Vincent COUVERT
 */
public class ScilabFontChooser extends ScilabUIElement implements FontChooser {

    private SimpleFontChooser component;

    /**
     * Constructor
     * @param font the default font
     */
    protected ScilabFontChooser(Font font) {
        component = ScilabBridge.createFontChooser(font);
        component.setElementId(UIElementMapper.add(this));
    }

    /**
     * Creates a Scilab FontChooser
     * @return the created FontChooser
     */
    public static FontChooser createFontChooser() {
        return createFontChooser(null);
    }

    /**
     * Creates a Scilab FontChooser
     * @param font the default font
     * @return the created FontChooser
     */
    public static FontChooser createFontChooser(Font font) {
        return new ScilabFontChooser(font);
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleFontChooser getAsSimpleFontChooser() {
        return component;
    }

    /**
     * Retrieve the selected font, or null
     * @return the selected font
     */
    public Font getSelectedFont() {
        return ScilabBridge.getSelectedFont(this);
    }

    /**
     * Set the default font name
     * @param fontName the default font name
     */
    public void setDefaultFontName(String fontName) {
        ScilabBridge.setDefaultFontName(this, fontName);
    }

    /**
     * Set the default font size
     * @param fontSize the default font size
     */
    public void setDefaultFontSize(int fontSize) {
        ScilabBridge.setDefaultFontSize(this, fontSize);
    }

    /**
     * Set the default font bold attribute
     * @param bold the default font bold attribute
     */
    public void setDefaultBold(boolean bold) {
        ScilabBridge.setDefaultBold(this, bold);
    }

    /**
     * Set the default font italic attribute
     * @param italic the default font italic attribute
     */
    public void setDefaultItalic(boolean italic) {
        ScilabBridge.setDefaultItalic(this, italic);
    }

    /**
     * Display the font chooser and wait for a user input
     */
    public void displayAndWait() {
        ScilabBridge.displayAndWait(this);
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#getDims()
     * @return nothing
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public boolean isVisible() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newSize is never used
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newPosition is never used
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newVisibleState is never used
     */
    public void setVisible(boolean newVisibleState) {
        throw new UnsupportedOperationException();
    }

}
