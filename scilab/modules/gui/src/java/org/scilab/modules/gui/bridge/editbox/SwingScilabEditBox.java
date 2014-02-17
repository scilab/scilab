/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.editbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;

import javax.swing.AbstractAction;
import javax.swing.InputMap;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.ScrollPaneConstants;
import javax.swing.UIManager;
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
import org.scilab.modules.gui.editbox.SimpleEditBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab EditBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabEditBox extends JScrollPane implements SwingViewObject, SimpleEditBox {

    private static final long serialVersionUID = 2048261239598753717L;

    private Integer uid;

    private CommonCallBack callback;

    private FocusListener focusListener;

    private StyledDocument doc;
    private SimpleAttributeSet docAttributes = new SimpleAttributeSet();

    private JTextPane textPane = new JTextPane();

    private Object enterKeyAction;
    private Object tabKeyAction;

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
        super(new JTextPane());
        textPane = (JTextPane) getViewport().getView();
        setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);

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
        InputMap map = textPane.getInputMap();
        enterKeyAction = map.get(enterKey);
        tabKeyAction = map.get(tabKey);

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

    public void setText(String text) {
        try {
            textPane.setText(text);
            doc.setParagraphAttributes(0, doc.getLength() - 1, docAttributes, true);
        } catch (Exception e) {
            // Do nothing
        }
    }

    /**
     * Set if the EditBox is enabled or not
     * @param status true if the EditBox is enabled
     */
    public void setEnabled(boolean status) {
        super.setEnabled(status);
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
        if (reliefType.equals("") == false) {
            textPane.setBorder(ScilabRelief.getBorderFromRelief(reliefType));
        }
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
            // Force rendering
            //setText(getText());
        }
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        switch (property) {
            case __GO_UI_MAX__ : {
                //                Double columns = (Double)value;
                //                Graphics g = textPane.getGraphics();
                //                Integer width = 50;
                //                if (g != null) {
                //                    width = textPane.getGraphics().getFontMetrics(textPane.getFont()).charWidth('m');
                //                }
                //                Integer totalWidth = columns.intValue() * width;
                //                Dimension current = textPane.getPreferredSize();
                //                System.out.println("current : " + current.toString());
                //                current.width = totalWidth;
                //                textPane.setPreferredSize(current);
                //                current = textPane.getPreferredSize();
                //                System.out.println("new : " + current.toString());
                break;
            }

            default : {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public String getText() {
        return textPane.getText();
    }

    public void setMultiLineText(boolean enable) {
        KeyStroke enterKey = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0);
        KeyStroke tabKey = KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0);
        if (enable) {
            setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
            textPane.getInputMap().remove(enterKey);
            textPane.getInputMap().remove(tabKey);
            textPane.getInputMap().put(enterKey, enterKeyAction);
            textPane.getInputMap().put(tabKey, tabKeyAction);
        } else {
            setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
            AbstractAction validateUserInput = new AbstractAction() {
                private static final long serialVersionUID = -5286137769378297783L;

                public void actionPerformed(ActionEvent e) {
                    validateUserInput();
                }
            };
            textPane.getInputMap().remove(enterKey);
            textPane.getInputMap().remove(tabKey);
            textPane.getInputMap().put(enterKey, validateUserInput);
            textPane.getInputMap().put(tabKey, validateUserInput);
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("TextField.background");
        if (color != null) {
            setBackground(color);
        }
    }
}
