/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
import java.awt.event.ActionEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.AbstractAction;
import javax.swing.JScrollBar;
import javax.swing.JSlider;
import javax.swing.KeyStroke;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.commons.OS;
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
 * @author Antoine ELIAS
 */
public class SwingScilabSlider extends JSlider implements SwingViewObject, Widget {

    private static final long serialVersionUID = -4262320156090829309L;

    private static final int MINIMUM_VALUE = 0;
    private static final int MAXIMUM_VALUE = 10000;

    private Integer uid;

    private CommonCallBack callback;

    private ChangeListener changeListener;

    private Border defaultBorder = null;

    private int previousValueCallback = 0;

    static {
        if (OS.get() == OS.UNIX) {
            // Force Slider on Unix not to display value upon
            UIManager.put("Slider.paintValue", false);
        }
    }

    class CtrlLeftAction extends AbstractAction {
        private static final long serialVersionUID = -3289281207742516486L;

        public void actionPerformed(ActionEvent arg0) {
            double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
            double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
            Double[] step = (Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__);
            int value = SwingScilabSlider.this.getValue();

            double ratio = (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin);
            int newValue = Math.max(MINIMUM_VALUE, value - (int)(step[1] * ratio));
            setValue(newValue);
        }
    }

    class LeftAction extends AbstractAction {
        private static final long serialVersionUID = 2099826485447918397L;

        public void actionPerformed(ActionEvent arg0) {
            double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
            double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
            Double[] step = (Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__);
            int value = SwingScilabSlider.this.getValue();

            double ratio = (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin);
            int newValue = Math.max(MINIMUM_VALUE, value - (int)(step[0] * ratio));
            setValue(newValue);
        }
    }

    class RightAction extends AbstractAction {
        private static final long serialVersionUID = 8666161246122371904L;

        public void actionPerformed(ActionEvent arg0) {
            double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
            double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
            Double[] step = (Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__);
            int value = SwingScilabSlider.this.getValue();

            double ratio = (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin);
            int newValue = Math.min(MAXIMUM_VALUE, value + (int)(step[0] * ratio));
            setValue(newValue);
        }
    }

    class CtrlRightAction extends AbstractAction {
        private static final long serialVersionUID = -1364255463511656338L;

        public void actionPerformed(ActionEvent arg0) {
            double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
            double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
            Double[] step = (Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__);
            int value = SwingScilabSlider.this.getValue();

            double ratio = (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin);
            int newValue = Math.min(MAXIMUM_VALUE, value + (int)(step[1] * ratio));
            setValue(newValue);
        }
    }

    /**
     * Constructor
     */
    public SwingScilabSlider() {
        super();
        // needed to have slider working with GLCanvas
        setOpaque(true);
        setMinimum(MINIMUM_VALUE);
        setMaximum(MAXIMUM_VALUE);
        setValue(0);

        /* some keys binding */
        getInputMap().put(KeyStroke.getKeyStroke("DOWN"), "LeftAction");
        getInputMap().put(KeyStroke.getKeyStroke("LEFT"), "LeftAction");
        getActionMap().put("LeftAction", new LeftAction());
        getInputMap().put(KeyStroke.getKeyStroke("UP"), "RightAction");
        getInputMap().put(KeyStroke.getKeyStroke("RIGHT"), "RightAction");
        getActionMap().put("RightAction", new RightAction());
        getInputMap().put(KeyStroke.getKeyStroke("control UP"), "CtrlRightAction");
        getInputMap().put(KeyStroke.getKeyStroke("control RIGHT"), "CtrlRightAction");
        getActionMap().put("CtrlRightAction", new CtrlRightAction());
        getInputMap().put(KeyStroke.getKeyStroke("control DOWN"), "CtrlLeftAction");
        getInputMap().put(KeyStroke.getKeyStroke("control LEFT"), "CtrlLeftAction");
        getActionMap().put("CtrlLeftAction", new CtrlLeftAction());


        changeListener = new ChangeListener() {
            public void stateChanged(ChangeEvent changeEvent) {
                JSlider source = (JSlider) changeEvent.getSource();
                if (!source.getValueIsAdjusting()) {
                    previousValueCallback = getValue();
                    updateModel();
                    if (callback != null) {
                        callback.actionPerformed(null);
                    }
                } else {
                    int offset = Math.abs(getValue() - previousValueCallback);
                    previousValueCallback = getValue();
                    // When the user has clicked on the slider itself (not the knob)
                    // and the knob makes a step of getMajorTickSpacing() value
                    // ==> We do not call the callback (Bug #13549)
                    if (offset != getMajorTickSpacing() && offset != 0) {
                        updateModel();
                        if (callback != null) {
                            callback.actionPerformed(null);
                        }
                    }
                }
            }
        };
        addChangeListener(changeListener);

        // Fix for bug #13543
        this.addMouseWheelListener(new MouseWheelListener() {
            public void mouseWheelMoved(MouseWheelEvent e) {
                int notches = e.getWheelRotation();
                int step = (e.isControlDown() ? getMajorTickSpacing() : getMinorTickSpacing());
                if (notches < 0) {
                    setValue(getValue() + step);
                } else {
                    setValue(getValue() - step);
                }
            }
        });
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
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab Slider
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
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab Slider
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
        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);

        double ratio = (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin);
        int newspace = (int)(space * ratio);
        super.setMajorTickSpacing(newspace);
    }

    /**
     * Set the minor tick spacing for a Slider
     * @param space the increment value
     */
    public void setMinorTickSpacing(double space) {
        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);

        double ratio = (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin);
        int newspace = (int)(space * ratio);
        super.setMinorTickSpacing(newspace);
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
        removeChangeListener(changeListener);

        double userMin = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
        setValue(MINIMUM_VALUE + (int) ((value - userMin) * (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin)));

        /* Put back the listener */
        addChangeListener(changeListener);
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
            case __GO_UI_MIN__ : {
                Double minValue = (Double)value;
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
            case __GO_POSITION__ : {
                Double[] dblValues = SwingViewWidget.updatePosition(this, uid, value);
                if (dblValues[2].intValue() > dblValues[3].intValue()) {
                    setHorizontal();
                } else {
                    setVertical();
                }
                break;
            }
            case __GO_UI_SLIDERSTEP__ : {
                Double[] sliderStep = ((Double[]) value);
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                setMinorTickSpacing(minorSliderStep);
                setMajorTickSpacing(majorSliderStep);
                break;
            }
            case __GO_UI_VALUE__ : {
                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length != 0) {
                    setUserValue(doubleValue[0]);
                } else {
                    Double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                    setUserValue(minValue);
                }
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
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Slider.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Slider.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
