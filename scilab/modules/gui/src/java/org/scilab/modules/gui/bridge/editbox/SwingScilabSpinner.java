package org.scilab.modules.gui.bridge.editbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

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
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;

public class SwingScilabSpinner extends JSpinner implements SwingViewObject, Widget {
    private static final long serialVersionUID = -8036645127098703686L;

    private Integer uid;
    private CommonCallBack callback;
    private Border defaultBorder = null;

    public SwingScilabSpinner() {
        setModel(new SpinnerNumberModel(0.0, 0.0, 1.0, 0.1));
        JSpinner.NumberEditor num = (NumberEditor) getEditor();
        num.getTextField().addKeyListener(new KeyListener() {

            public void keyTyped(KeyEvent e) {
                System.out.println("keyTyped : " + e.getKeyChar());
            }

            public void keyReleased(KeyEvent e) {
            }

            public void keyPressed(KeyEvent e) {
            }
        });

        addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                Double val = (Double)getValue();
                GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, new Double[] {val});
                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }
        });
    }

    public void setText(String newText) {
    }

    public void setEmptyText() {
    }

    public String getText() {
        return null;
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("Spinner.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void setCallback(CommonCallBack callback) {
        this.callback = callback;
    }

    public void setHorizontalAlignment(String alignment) {
    }

    public void setVerticalAlignment(String alignment) {
    }

    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    public void destroy() {
        getParent().remove(this);
        this.setVisible(false);
    }

    public void addToolBar(ToolBar toolBarToAdd) {
        throw new UnsupportedOperationException();
    }

    public void addMenuBar(MenuBar menuBarToAdd) {
        throw new UnsupportedOperationException();
    }

    public void addInfoBar(TextBox infoBarToAdd) {
        throw new UnsupportedOperationException();
    }

    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    public MenuBar getMenuBar() {
        throw new UnsupportedOperationException();
    }

    public ToolBar getToolBar() {
        throw new UnsupportedOperationException();
    }

    public TextBox getInfoBar() {
        throw new UnsupportedOperationException();
    }

    public void setId(Integer id) {
        uid = id;
    }

    public Integer getId() {
        return uid;
    }

    public void update(int property, Object value) {
        GraphicController controller = GraphicController.getController();
        switch (property) {
            case __GO_UI_MAX__:
            case __GO_UI_MIN__:
            case __GO_UI_SLIDERSTEP__: {
                Double max = (Double) controller.getProperty(getId(), __GO_UI_MAX__);
                Double min = (Double) controller.getProperty(getId(), __GO_UI_MIN__);
                //wait update of max before update component
                if (min > max) {
                    return;
                }
                Double[] step = (Double[]) controller.getProperty(getId(), __GO_UI_SLIDERSTEP__);
                Double[] val = (Double[]) controller.getProperty(getId(), __GO_UI_VALUE__);
                Double v = (val != null && val.length != 0) ? val[0] : 0.0;
                if (v > max) {
                    v = max;
                }

                if (v < min) {
                    v = min;
                }

                //adjust value to current step
                if (step[0] != 0) {
                    v = Math.floor((v / step[0])) * step[0];
                } else {
                    v = min;
                }

                controller.setProperty(uid, __GO_UI_VALUE__, new Double[] {v});
                setModel(new SpinnerNumberModel(v, min, max, step[0]));
                break;
            }
            case __GO_UI_VALUE__: {
                setValue(((Double[])value)[0]);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }


    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Spinner.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
