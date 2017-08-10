/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.bridge.editbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.util.ArrayList;

import javax.swing.AbstractAction;
import javax.swing.InputMap;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.ScrollPaneConstants;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.text.AbstractDocument;
import javax.swing.text.BoxView;
import javax.swing.text.ComponentView;
import javax.swing.text.Element;
import javax.swing.text.IconView;
import javax.swing.text.LabelView;
import javax.swing.text.ParagraphView;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;
import javax.swing.text.StyledEditorKit;
import javax.swing.text.View;
import javax.swing.text.ViewFactory;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingView;
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
 * Swing implementation for Scilab EditBox in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabEditBox extends JScrollPane implements SwingViewObject, Widget {

    private static final long serialVersionUID = 2048261239598753717L;

    private Integer uid;

    private CommonCallBack callback;

    private FocusListener focusListener;

    private Border defaultBorder = null;

    private StyledDocument doc;
    private SimpleAttributeSet docAttributes = new SimpleAttributeSet();

    private JTextPane textPane = new JTextPane();
    //use to disable wordwarp
    private JPanel noWrapPanel = new JPanel(new BorderLayout());
    private boolean scrollable = false;
    private boolean isMultiLine = false;

    private Object enterKeyAction;
    private Object tabKeyAction;
    private Object shiftTabKeyAction;


    private class EditBoxView extends BoxView {
        public EditBoxView(Element elem, int axis) {
            super(elem, axis);
        }

        protected void layoutMajorAxis(int targetSpan, int axis, int[] offsets, int[] spans) {
            super.layoutMajorAxis(targetSpan, axis, offsets, spans);
            int textBlockHeight = 0;
            int offset = 0;

            if (textPane.getAlignmentY() == BOTTOM_ALIGNMENT) {
                for (int i = 0; i < spans.length; i++) {
                    textBlockHeight += spans[i];
                }
                offset = (targetSpan - textBlockHeight);
                for (int i = 0; i < offsets.length; i++) {
                    offsets[i] += offset;
                }
            } else if (textPane.getAlignmentY() == CENTER_ALIGNMENT) {
                for (int i = 0; i < spans.length; i++) {
                    textBlockHeight += spans[i];
                }
                offset = (targetSpan - textBlockHeight) / 2;
                for (int i = 0; i < offsets.length; i++) {
                    offsets[i] += offset;
                }
            } else {
                // TOP_ALIGNEMENT or other
                // default behaviour : do nothing special
            }
        }
    }

    private class EditBoxEditorKit extends StyledEditorKit {
        private static final long serialVersionUID = -3293325523458217074L;

        public ViewFactory getViewFactory() {
            return new ViewFactory() {
                public View create(Element elem) {
                    String kind = elem.getName();
                    if (kind != null) {
                        if (kind.equals(AbstractDocument.ContentElementName)) {
                            return new LabelView(elem);
                        } else if (kind.equals(AbstractDocument.ParagraphElementName)) {
                            return new ParagraphView(elem);
                        } else if (kind.equals(AbstractDocument.SectionElementName)) {
                            return new EditBoxView(elem, View.Y_AXIS);
                        } else if (kind.equals(StyleConstants.ComponentElementName)) {
                            return new ComponentView(elem);
                        } else if (kind.equals(StyleConstants.IconElementName)) {
                            return new IconView(elem);
                        }
                    }
                    return new LabelView(elem);
                }
            };
        }
    }

    /**
     * Constructor
     */
    public SwingScilabEditBox() {
        super(new JPanel(new BorderLayout()));
        noWrapPanel = (JPanel) getViewport().getView();
        textPane = new JTextPane();
        noWrapPanel.add(textPane, BorderLayout.CENTER);

        setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
        setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

        textPane.setEditorKit(new EditBoxEditorKit());
        doc = (StyledDocument) textPane.getDocument();

        // Create a focus listener to call the callback action
        focusListener = new FocusListener() {
            public void focusGained(FocusEvent arg0) {
                // Do nothing
            }

            public void focusLost(FocusEvent arg0) {
                validateUserInput();
            }
        };
        textPane.addFocusListener(focusListener);
        KeyStroke enterKey = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0);
        KeyStroke tabKey = KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0);
        KeyStroke shiftTabKey = KeyStroke.getKeyStroke(KeyEvent.VK_TAB, KeyEvent.SHIFT_DOWN_MASK);
        InputMap map = textPane.getInputMap();
        enterKeyAction = map.get(enterKey);
        tabKeyAction = map.get(tabKey);
        shiftTabKeyAction = map.get(shiftTabKey);

        if (Console.getConsole().getUseDeprecatedLF() == false) {
            setEditFont(getFont());
        }
    }

    /**
     * Validate UserInput and call Scilab Callback if needed
     */
    private void validateUserInput() {
        // Validates user input
        if (getParent() != null) { // To avoid to execute the callback when then parent figure is destroyed

            String[] stringProperty = getText().split("\n");
            GraphicController.getController().setProperty(uid, __GO_UI_STRING__, stringProperty);

            if (SwingView.getFromId(uid) != null && callback != null) {
                callback.actionPerformed(null);
            }
        }
    }

    /**
     * Draws a swing Scilab EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab EditBox
     * @return the dimensions of the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * EditBox
     * @return the position of the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab EditBox
     * @param newSize the dimensions we want to set to the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * EditBox
     * @param newPosition the position we want to set to the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the EditBox
     * @param cb the callback to set.
     */
    public void setCallback(CommonCallBack cb) {
        this.callback = cb;
    }

    public void setText(String[] texts) {
        if (texts.length == 0) {
            try {
                textPane.setText(null);
                doc.setParagraphAttributes(0, doc.getLength() - 1, docAttributes, true);
            } catch (Exception e) {
                // Do nothing
            }
        } else {
            StringBuffer newText = new StringBuffer(texts[0]);

            for (int i = 1; i < texts.length; ++i) {
                newText.append("\n" + texts[i]);
            }

            try {
                textPane.setText(newText.toString());
                doc.setParagraphAttributes(0, doc.getLength() - 1, docAttributes, true);
            } catch (Exception e) {
                // Do nothing
            }
        }
    }

    public void setText(String text) {
        try {
            textPane.setText(text);
            doc.setParagraphAttributes(0, doc.getLength() - 1, docAttributes, true);
        } catch (Exception e) {
            // Do nothing
        }
    }

    public void setEmptyText() {
        textPane.setText(null);
        doc.setParagraphAttributes(0, doc.getLength() - 1, docAttributes, true);
    }

    /**
     * Set if the EditBox is enabled or not
     * @param status true if the EditBox is enabled
     */
    public void setEnabled(boolean status) {
        if (status == isEnabled()) {
            return;
        }

        super.setEnabled(status);
        textPane.setEnabled(status);

        //force background to gray
        if (status) {
            SwingViewWidget.update(this, __GO_UI_BACKGROUNDCOLOR__, GraphicController.getController().getProperty(uid, __GO_UI_BACKGROUNDCOLOR__));
        } else {
            Color gray = new Color(0.9f,  0.9f,  0.9f);
            setBackground(gray);
        }


        /* (Des)Activate the callback */
        if (callback != null) {
            if (status) {
                removeFocusListener(focusListener); /*
                                                     * To be sure the callback
                                                     * is not added two times
                                                     */
                //removeActionListener(actionListener); /* To be sure the callback is not added two times */
                addFocusListener(focusListener);
                //addActionListener(actionListener);
            } else {
                removeFocusListener(focusListener);
                //removeActionListener(actionListener);
            }
        }
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the EditBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        if (alignment.equals("") == false) {
            int alignConstant = StyleConstants.ALIGN_LEFT;
            if (alignment.equals("right")) {
                alignConstant = StyleConstants.ALIGN_RIGHT;
            } else if (alignment.equals("center")) {
                alignConstant = StyleConstants.ALIGN_CENTER;
            }

            StyleConstants.setAlignment(docAttributes, alignConstant);
            doc.setParagraphAttributes(0, doc.getLength(), docAttributes, true);
        }
    }

    /**
     * Set the vertical alignment for the EditBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        if (alignment.equals("") == false) {
            if (alignment.equals("bottom")) {
                textPane.setAlignmentY(BOTTOM_ALIGNMENT);
            } else if (alignment.equals("top")) {
                textPane.setAlignmentY(TOP_ALIGNMENT);
            } else if (alignment.equals("middle")) {
                textPane.setAlignmentY(CENTER_ALIGNMENT);
            }
            // Force text update to render
            setText(getText());
        }
    }

    /**
     * Set the Relief of the EditBox
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = textPane.getBorder();
        }
        textPane.setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the EditBox
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the EditBox.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the EditBox.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
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

    public void setBackground(Color bg) {
        super.setBackground(bg);
        if (docAttributes != null && textPane != null) {
            textPane.setBackground(bg);
            StyleConstants.setBackground(docAttributes, bg);
        }
    }

    public void setEditFont(Font font) {
        super.setFont(font);
        if (textPane != null) {
            textPane.setFont(font);
            StyleConstants.setFontFamily(docAttributes, font.getFamily());
            StyleConstants.setFontSize(docAttributes, font.getSize());
            StyleConstants.setBold(docAttributes, font.isBold());
            StyleConstants.setItalic(docAttributes, font.isItalic());
            doc.setParagraphAttributes(0, doc.getLength() - 1, docAttributes, true);
        }
    }

    public void setFont(Font font) {
        setEditFont(font);
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
                double min = ((Double) controller.getProperty(uid, __GO_UI_MIN__));
                double max = ((Double) controller.getProperty(uid, __GO_UI_MAX__));
                if (max - min > 1.0) {
                    setMultiLineText(true);
                } else {
                    setMultiLineText(false);
                }

                //refresh scrollable state
                setScrollable(scrollable);
                // Force String update
                update(__GO_UI_STRING__, GraphicController.getController().getProperty(uid, __GO_UI_STRING__));
                break;
            }
            case __GO_UI_MIN__: {
                Double min = ((Double) value);
                Double max = ((Double) controller.getProperty(uid, __GO_UI_MAX__));
                if (max - min > 1.0) {
                    setMultiLineText(true);
                } else {
                    setMultiLineText(false);
                }

                //refresh scrollable state
                setScrollable(scrollable);
                // Force String update
                update(__GO_UI_STRING__, GraphicController.getController().getProperty(uid, __GO_UI_STRING__));
                break;
            }
            case __GO_UI_STRING__: {
                String[] str = (String[])value;

                double min = ((Double) controller.getProperty(uid, __GO_UI_MIN__));
                double max = ((Double) controller.getProperty(uid, __GO_UI_MAX__));

                //if str.length > 1 automactly switch to multiline
                if (str.length > 1 && max - min <= 1.0 ) {
                    //update max in model that recall setText
                    controller.setProperty(uid, __GO_UI_MAX__, min + 2);
                    return;
                }

                ArrayList<String> lst = new ArrayList<String>();
                for (int i = 0 ; i < str.length ; i++) {
                    String s = str[i];
                    int index = 0;
                    int lastIndex = 0;
                    while ((index = s.indexOf('\n', lastIndex)) != -1) {
                        lst.add(s.substring(lastIndex, index));
                        lastIndex = index + 1;
                    }
                    if (lastIndex <= s.length()) {
                        lst.add(s.substring(lastIndex));
                    }
                }

                if (lst.size() != str.length) {
                    String[] newStr = new String[lst.size()];
                    lst.toArray(newStr);
                    controller.setProperty(uid,  __GO_UI_STRING__, newStr);
                    return;
                }

                if (max - min > 1.0) {
                    setText(str);
                    setMultiLineText(true);
                } else {
                    if (str.length == 0) {
                        setEmptyText();
                    } else {
                        setText(str[0]);
                    }
                    setMultiLineText(false);
                }
                break;
            }
            case __GO_UI_SCROLLABLE__ : {
                setScrollable((Boolean)value);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }

        setMinimumSize(textPane.getMinimumSize());
    }

    public void setScrollable(Boolean scrollable) {
        this.scrollable  = scrollable;
        if (scrollable) {
            //scrollbar -> true
            if (isMultiLine) {
                //wrap -> false
                setWordWrap(false);
            } else {
                //wrap -> false
                setWordWrap(false);
            }

            //scrollbar -> true
            setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        } else {
            if (isMultiLine) {
                //wrap -> true
                setWordWrap(true);
            } else {
                //wrap -> false
                setWordWrap(false);
            }

            //scrollbar -> false
            setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        }
    }

    private void setWordWrap(boolean wordWrap) {
        if (wordWrap) {
            setViewportView(textPane);
        } else {
            setViewportView(noWrapPanel);
            noWrapPanel.add(textPane);
        }
    }

    public String getText() {
        return textPane.getText();
    }

    public void setMultiLineText(boolean enable) {
        isMultiLine = enable;
        KeyStroke enterKey = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0);
        KeyStroke tabKey = KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0);
        KeyStroke shiftTabKey = KeyStroke.getKeyStroke(KeyEvent.VK_TAB, KeyEvent.SHIFT_DOWN_MASK);
        if (enable) {
            setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
            textPane.getInputMap().remove(enterKey);
            textPane.getInputMap().remove(tabKey);
            textPane.getInputMap().remove(shiftTabKey);
            textPane.getInputMap().put(enterKey, enterKeyAction);
            textPane.getInputMap().put(tabKey, tabKeyAction);
            textPane.getInputMap().put(tabKey, shiftTabKeyAction);
        } else {
            setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
            AbstractAction focusNextComponent = new AbstractAction() {
                private static final long serialVersionUID = -5286137769378297783L;

                public void actionPerformed(ActionEvent e) {
                    KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
                    manager.focusNextComponent();
                }
            };

            AbstractAction validateUserInput = new AbstractAction() {
                private static final long serialVersionUID = -5286137769378297783L;

                public void actionPerformed(ActionEvent e) {
                    validateUserInput();
                }
            };

            AbstractAction focusPreviousComponent = new AbstractAction() {
                private static final long serialVersionUID = -5286137769378297783L;

                public void actionPerformed(ActionEvent e) {
                    KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
                    manager.focusPreviousComponent();
                }
            };
            textPane.getInputMap().remove(enterKey);
            textPane.getInputMap().remove(tabKey);
            textPane.getInputMap().remove(shiftTabKey);
            textPane.getInputMap().put(enterKey, validateUserInput);
            textPane.getInputMap().put(tabKey, focusNextComponent); // input validation will be performed by focusLost
            textPane.getInputMap().put(shiftTabKey, focusPreviousComponent); // input validation will be performed by focusLost
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("TextField.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("TextField.foreground");
        if (color != null) {
            setForeground(color);
        }
    }

    public void requestFocus() {
        super.requestFocus();
        textPane.requestFocus();
    }
}
