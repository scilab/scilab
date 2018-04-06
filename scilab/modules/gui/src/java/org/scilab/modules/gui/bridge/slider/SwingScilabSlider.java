/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 * Copyright (C) 2018 - Stéphane Mottelet
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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SNAPTOTICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
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
    private static final int MAX_MAXIMUM_VALUE = 10000;
    private int MAXIMUM_VALUE = MAX_MAXIMUM_VALUE;

    private Integer uid;

    private CommonCallBack callback;

    private ChangeListener changeListener;

    private Border defaultBorder = null;

    private double previousUserValueCallback = Double.POSITIVE_INFINITY;

    static {
        if (OS.get() == OS.UNIX) {
            // Force Slider on Unix not to display value upon
            UIManager.put("Slider.paintValue", false);
        }
    }

    class CtrlLeftAction extends AbstractAction {
        private static final long serialVersionUID = -3289281207742516486L;

        public void actionPerformed(ActionEvent arg0) {
            setValue(SwingScilabSlider.this.getValue() - SwingScilabSlider.this.getMajorTickSpacing());
        }
    }

    class LeftAction extends AbstractAction {
        private static final long serialVersionUID = 2099826485447918397L;

        public void actionPerformed(ActionEvent arg0) {
            setValue(SwingScilabSlider.this.getValue() - SwingScilabSlider.this.getMinorTickSpacing());
        }
    }

    class RightAction extends AbstractAction {
        private static final long serialVersionUID = 8666161246122371904L;

        public void actionPerformed(ActionEvent arg0) {
          setValue(SwingScilabSlider.this.getValue() + SwingScilabSlider.this.getMinorTickSpacing());
        }
    }

    class CtrlRightAction extends AbstractAction {
        private static final long serialVersionUID = -1364255463511656338L;

        public void actionPerformed(ActionEvent arg0) {
          setValue(SwingScilabSlider.this.getValue() + SwingScilabSlider.this.getMajorTickSpacing());
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
        getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_UP, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()), "CtrlRightAction");
        getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()), "CtrlRightAction");
        getActionMap().put("CtrlRightAction", new CtrlRightAction());
        getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()), "CtrlLeftAction");
        getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()), "CtrlLeftAction");
        getActionMap().put("CtrlLeftAction", new CtrlLeftAction());


        changeListener = new ChangeListener() {
            public void stateChanged(ChangeEvent changeEvent) {
                JSlider source = (JSlider) changeEvent.getSource();
                double offset = Math.abs(getUserValue() - previousUserValueCallback);
                previousUserValueCallback = getUserValue();
                if (offset != 0) {
                    updateModel();
                    if (callback != null) {
                        callback.actionPerformed(null);
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
     * get the current value of the Slider
     */
    private double getUserValue() {
        double value;
        double userMin = (double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
        double userMax = (double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
        Double[] userStep = (Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__);
        value = userMin + ((getValue() - MINIMUM_VALUE) * (userMax - userMin) / (MAXIMUM_VALUE - MINIMUM_VALUE));
        if (getSnapToTicks()) {
            value = Math.min(userMin+Math.round((value-userMin)/userStep[0])*userStep[0],userMax);
        }
        return value;
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
        setValue(MINIMUM_VALUE + (int)Math.round((value - userMin) * (MAXIMUM_VALUE - MINIMUM_VALUE) / (userMax - userMin)));

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
                Double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                // Update the slider properties
                updateSwingSlider(minValue,maxValue,sliderStep);
                break;
            }
            case __GO_UI_MIN__ : {
                Double minValue = (Double)value;
                Double maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                // Update the slider properties
                updateSwingSlider(minValue,maxValue,sliderStep);
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
                Double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                Double maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                // Update the slider properties
                updateSwingSlider(minValue,maxValue,sliderStep);
                break;
            }
            case __GO_UI_SNAPTOTICKS__ : {
                Boolean state=(Boolean) value;
                setSnapToTicks(state);
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
     * Update values in SwingSlider widget when needed
     */
    private void updateSwingSlider(Double userMin, Double userMax, Double[] sliderStep) {
      double minorSliderStep = sliderStep[0].doubleValue();
      double majorSliderStep = sliderStep[1].doubleValue();
      MAXIMUM_VALUE=MAX_MAXIMUM_VALUE-(MAX_MAXIMUM_VALUE % Math.min(Math.max(1,(int)Math.abs((userMax-userMin)/minorSliderStep)),MAX_MAXIMUM_VALUE)); 

      /* Remove the listener to avoid the callback to be executed */
      removeChangeListener(changeListener);
      setMaximum(MAXIMUM_VALUE);
      setMinorTickSpacing(minorSliderStep);
      setMajorTickSpacing(majorSliderStep);

      /* Put back the listener */
      addChangeListener(changeListener);
      updateModel();
    }

    /**
     * Update values in the model when needed
     */
    private void updateModel() {
      Double[] value = new Double[1];
      value[0] = getUserValue();
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
