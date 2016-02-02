/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.bridge.slider;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

import javax.swing.JScrollBar;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;

/**
 * Swing implementation for Scilab Slider in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabScroll extends JScrollBar implements SwingViewObject, Widget {

    private static final long serialVersionUID = -4262320156090829309L;

    private static final int MIN_KNOB_SIZE = 40;

    private static final int MINIMUM_VALUE = 0;
    private static final int MAXIMUM_VALUE = 10000;

    private Integer uid;

    private CommonCallBack callback;

    private AdjustmentListener adjustmentListener;

    private Border defaultBorder = null;

    /**
     * Constructor
     */
    public SwingScilabScroll() {
        super();
        // needed to have slider working with GLCanvas
        setOpaque(true);
        /* Enable to manage the slider using keyboard (See bug #10840) */
        setFocusable(true);
        setMinimum(MINIMUM_VALUE);
        setMaximum(MAXIMUM_VALUE + getVisibleAmount());
        adjustmentListener = new AdjustmentListener() {
            public void adjustmentValueChanged(AdjustmentEvent arg0) {
                updateModel();
                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }
        };
        addAdjustmentListener(adjustmentListener);
    }

    /**
     * Draws a swing Scilab Slider
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab Slider
     * @return the dimensions of the Slider
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(super.getSize().width, super.getSize().height);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * Slider
     * @return the position of the Slider
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab Slider
     * @param newSize the dimensions to set to the Slider
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        super.setSize(newSize.getWidth(), newSize.getHeight());
        super.doLayout(); // Usefull in case of resize
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * Slider
     * @param newPosition the position to set to the Slider
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the Slider
     * @param cb the callback to set.
     */
    public void setCallback(CommonCallBack cb) {
        /* Create a callback */
        callback = cb;
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Get the text of the Slider
     * @return the text
     * @see org.scilab.modules.gui.widget.Widget#getText()
     */
    public String getText() {
        return this.getName();
    }

    /**
     * Set the text of the Slider
     * @param text the text to set to the Slider
     * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
     */
    public void setText(String text) {
        this.setName(text);
    }

    public void setEmptyText() {
        this.setName(null);
    }

    /**
     * Set the horizontal alignment for the Slider text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the vertical alignment for the Slider text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the minimum value of a Slider
     * @param value the minimum value
     */
    public void setMinimumValue(double value) {
        updateModel(); /* Update the model according to the knob position */
    }

    /**
     * Set the maximum value of a Slider
     * @param value the maximum value
     */
    public void setMaximumValue(double value) {
        updateModel(); /* Update the model according to the knob position */
    }

    /**
     * Set the Relief of the Slider
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Set the major tick spacing for a Slider
     * @param space the increment value
     */
    public void setMajorTickSpacing(double space) {
        /* Remove the listener to avoid the callback to be executed */
        removeAdjustmentListener(adjustmentListener);

        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
        setBlockIncrement((int) (space * (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin)));
        int oldMax = getMaximum() - getVisibleAmount();
        setVisibleAmount(Math.max((int) ((MAXIMUM_VALUE - MINIMUM_VALUE) / space), MIN_KNOB_SIZE));
        setMaximum(oldMax + getVisibleAmount());

        /* Put back the listener */
        addAdjustmentListener(adjustmentListener);
    }

    /**
     * Set the minor tick spacing for a Slider
     * @param space the increment value
     */
    public void setMinorTickSpacing(double space) {
        /* Remove the listener to avoid the callback to be executed */
        removeAdjustmentListener(adjustmentListener);

        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
        setUnitIncrement((int) (space * (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin)));

        /* Put back the listener */
        addAdjustmentListener(adjustmentListener);
    }

    /**
     * Set the slider orientation to vertical
     */
    public void setVertical() {
        setOrientation(JScrollBar.VERTICAL);
    }

    /**
     * Set the slider orientation to horizontal
     */
    public void setHorizontal() {
        setOrientation(JScrollBar.HORIZONTAL);
    }

    /**
     * Destroy the Slider
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the Slider.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for Sliders */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Slider.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for Sliders */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the current value of the Slider
     * @param value the new value
     */
    public void setUserValue(double value) {
        /* Remove the listener to avoid the callback to be executed */
        removeAdjustmentListener(adjustmentListener);

        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
        super.setValue(MINIMUM_VALUE + (int) ((value - userMin) * (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin)));

        /* Put back the listener */
        addAdjustmentListener(adjustmentListener);
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
        GraphicController controller = GraphicController.getController();

        switch (property) {
            case __GO_UI_MAX__: {
                Double maxValue = (Double) value;
                // Update the slider properties
                Double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                setMaximumValue(maxValue);
                Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                if (minValue <= maxValue) {
                    setMinorTickSpacing(minorSliderStep);
                    setMajorTickSpacing(majorSliderStep);
                }
                break;
            }
            case __GO_UI_MIN__: {
                Double minValue = (Double) value;
                // Update the slider properties
                Double maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                setMinimumValue(minValue);
                Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                if (minValue <= maxValue) {
                    setMinorTickSpacing(minorSliderStep);
                    setMajorTickSpacing(majorSliderStep);
                }
                break;
            }
            case __GO_POSITION__: {
                Double[] dblValues = SwingViewWidget.updatePosition(this, uid, value);
                if (dblValues[2].intValue() > dblValues[3].intValue()) {
                    setHorizontal();
                } else {
                    setVertical();
                }
                break;
            }
            case __GO_UI_SLIDERSTEP__: {
                Double[] sliderStep = ((Double[]) value);
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                setMinorTickSpacing(minorSliderStep);
                setMajorTickSpacing(majorSliderStep);
                break;
            }
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);
                setUserValue(doubleValue[0]);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    /**
     * Update values in the model when needed
     */
    private void updateModel() {
        Double[] value = new Double[1];
        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
        value[0] = userMin + ((getValue() - MINIMUM_VALUE) * (userMax - userMin) / (MAXIMUM_VALUE - MINIMUM_VALUE));
        GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, value);
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("ScrollBar.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("ScrollBar.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
