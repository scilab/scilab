/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
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
package org.scilab.modules.gui.bridge.label;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.html.HTMLDocument;
import javax.swing.text.html.StyleSheet;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel.IconLabel.IconType;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.gui.widget.Widget;

/**
 * Swing implementation for Scilab Labels in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabLabel extends JScrollPane implements SwingViewObject, Widget {

    private enum LabelStyle {
        TEXT, LATEX, MATHML, HTML
    };

    private static final long serialVersionUID = 7177323379068859441L;

    private Integer uid;

    private JComponent label = new JLabel(" ");

    private Border defaultBorder = null;

    private LabelStyle labelStyle = null;

    private String horizontalAlignment = "left"; //Horizontal alignment property

    private String verticalAlignment = "middle"; // Vertical alignment property

    private final JPanel alignmentPanel = new JPanel(); // Used for alignment

    private final GridBagLayout alignmentLayout = new GridBagLayout();

    private String labelText = ""; // Used to save user given text

    private static HyperlinkListener urlOpener = new HyperlinkListener() {
        public void hyperlinkUpdate(HyperlinkEvent event) {
            if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                WebBrowser.openUrl(event.getURL(), event.getDescription());
            }
        }
    };

    /**
     * Constructor
     */
    public SwingScilabLabel() {
        super();
        alignmentPanel.setLayout(alignmentLayout);
        setViewportView(label);
        setBorder(BorderFactory.createEmptyBorder());
        setViewportBorder(BorderFactory.createEmptyBorder());
        setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
        // Initialize display
        setAlignment();
    }

    /**
     * Apply a new font for the label.
     * @param font new font to use.
     */
    public void setFont(Font font) {
        super.setFont(font);
        if (label != null) {
            label.setFont(font);
            setMinimumSize(label.getMinimumSize());

            if (labelStyle == LabelStyle.HTML) {
                StyleSheet styleSheet = ((HTMLDocument) ((JTextPane) label).getDocument()).getStyleSheet();
                styleSheet.addRule("body {font-family:" + font.getName() + ";}");
                styleSheet.addRule("body {font-size:" + font.getSize() + "pt;}");
                if (font.isBold()) {
                    styleSheet.addRule("body {font-weight:bold;}");
                } else {
                    styleSheet.addRule("body {font-weight:normal;}");
                }
                if (font.isItalic()) {
                    styleSheet.addRule("body {font-style:italic;}");
                } else {
                    styleSheet.addRule("body {font-style:normal;}");
                }
            }
        }
    }

    /**
     * To set the Foreground color of the element.
     * @param color the Color
     */
    public void setForeground(Color color) {
        super.setForeground(color);
        if (label != null) {
            label.setForeground(color);
        }
    }

    public void setEnabled(boolean enabled) {
        super.setEnabled(enabled);
        if (label != null) {
            label.setEnabled(enabled);
        }
    }

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    public void setBackground(Color color) {
        super.setBackground(color);
        if (alignmentPanel != null) {
            alignmentPanel.setBackground(color);
        }
        if (label != null) {
            if (label instanceof JLabel) {
                ((JLabel) label).setOpaque(true);
            }
            label.setBackground(color);
        }
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
        // Need validate to force alignement to be applyed
        //setMinimumSize(new Dimension(Math.max((int) label.getMinimumSize().getWidth(), newSize.getWidth()), (int) label.getMinimumSize().getHeight()));
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
     * Add a callback to the Label
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        // Nothing to do...
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Label.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Label.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Label.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Label.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the Label text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        horizontalAlignment = alignment;
        setAlignment();
    }

    /**
     * Set the vertical alignment for the Label text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        verticalAlignment = alignment;
        setAlignment();

    }

    /**
     * Set the Relief of the Label
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the Label
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the Label.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Label.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * get the text displayed in the label
     * @return the label
     * @see org.scilab.modules.gui.text.SimpleText#getText()
     */
    public String getText() {
        return labelText;
    }

    /**
     * Set the text displayed in the label
     * @param newText the text
     * @see org.scilab.modules.gui.text.SimpleText#setText(java.lang.String)
     */
    public void setText(String newText) {
        // Save the data given by the user so that it can be retrieved
        // (Java adds HTML tags in the getlabel().getText() returned value)

        // <math></math>    : LateXLabel ( MathML )
        // <html></html>    : JTextPane for clickable links
        // $...$            : LateXLabel ( LateX )
        // else             : JLabel

        labelText = newText;

        if (labelText != null) {
            if (labelText.startsWith("<math>") && labelText.endsWith("</math>")) {
                boolean mathML = ScilabSpecialTextUtilities.setText(new JLabel(), labelText);

                //if MAthML rendering failed use normal renderer ( JLabel)
                if (mathML) {
                    changeLabelType(LabelStyle.MATHML);
                    ((IconLabel) label).setText(labelText);
                } else {
                    changeLabelType(LabelStyle.TEXT);
                    ((JLabel) label).setText(labelText);
                }
                return;
            }

            if (labelText.startsWith("<html>") && labelText.endsWith("</html>")) {
                changeLabelType(LabelStyle.HTML);
                ((JEditorPane) label).setText(labelText);
                return;
            }

            if (labelText.startsWith("<a href") && labelText.endsWith("</a>")) {
                changeLabelType(LabelStyle.HTML);
                ((JEditorPane) label).setText(labelText);
                return;
            }

            if (labelText.startsWith("$") && labelText.endsWith("$")) {
                boolean latex = ScilabSpecialTextUtilities.setText(new JLabel(), labelText);

                //if MAthML rendering failed use normal renderer ( JLabel)
                if (latex) {
                    changeLabelType(LabelStyle.LATEX);
                    ((IconLabel) label).setText(labelText);
                } else {
                    changeLabelType(LabelStyle.TEXT);
                    ((JLabel) label).setText(labelText);
                }
                return;
            }
        }

        //force window to redraw all component
        JFrame win = (JFrame)SwingUtilities.getAncestorOfClass(JFrame.class, this);
        if (win != null) {
            win.invalidate();
        }

        changeLabelType(LabelStyle.TEXT);
        ((JLabel) label).setText(labelText);
        setMinimumSize(label.getMinimumSize());


        //force window to redraw all component
        if (win != null) {
            win.validate();
        }

    }

    public void setEmptyText() {
        setText(null);
    }

    /**
     * Change Label type to switch between JLabel abd JEditorPane JLabel is
     * quicker on simple text JEditorPane can enable HyperLinks
     * @param isHtmlLabel
     */
    private void changeLabelType(LabelStyle style) {

        if (labelStyle == style) {
            return;
        }

        labelStyle = style;
        Color bgColor = label.getBackground();
        Color fgColor = label.getForeground();
        Font font = label.getFont();
        Dimension dims = label.getSize();
        label.setVisible(false);

        switch (labelStyle) {
            case LATEX:
                alignmentPanel.remove(label);
                label = new IconLabel(IconType.LATEX);
                setViewportView(label);
                setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
                break;
            case MATHML:
                alignmentPanel.remove(label);
                label = new IconLabel(IconType.MATHML);
                setViewportView(label);
                setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
                break;
            case HTML: {
                JTextPane newLabel = new JTextPane();
                newLabel.addHyperlinkListener(urlOpener);
                newLabel.setContentType("text/html");
                newLabel.setEditable(false);
                StyleSheet styleSheet = ((HTMLDocument) newLabel.getDocument()).getStyleSheet();
                styleSheet.addRule("body {font-family:" + font.getName() + ";}");
                styleSheet.addRule("body {font-size:" + font.getSize() + "pt;}");
                if (font.isBold()) {
                    styleSheet.addRule("body {font-weight:bold;}");
                } else {
                    styleSheet.addRule("body {font-weight:normal;}");
                }
                if (font.isItalic()) {
                    styleSheet.addRule("body {font-style:italic;}");
                } else {
                    styleSheet.addRule("body {font-style:normal;}");
                }

                label = newLabel;
                alignmentPanel.add(label);
                setViewportView(alignmentPanel);
                setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
                break;
            }
            case TEXT: {
                alignmentPanel.remove(label);
                label = new JLabel();
                setViewportView(label);
                //refresh icon
                update(__GO_UI_ICON__, GraphicController.getController().getProperty(getId(), __GO_UI_ICON__));
                ((JLabel) label).setOpaque(false);
                setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
                break;
            }
        }

        setBackground(bgColor);
        label.setForeground(fgColor);
        label.setFont(font);
        label.setSize(dims);
        label.setVisible(true);
        setAlignment();
    }

    /**
     * Set alignment of the text component
     */
    private void setAlignment() {

        if (labelStyle != LabelStyle.HTML) {
            ((JLabel) label).setVerticalAlignment(ScilabAlignment.toSwingAlignment(verticalAlignment));
            ((JLabel) label).setHorizontalAlignment(ScilabAlignment.toSwingAlignment(horizontalAlignment));
            return;
        }

        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.weighty = 1;
        gbc.gridwidth = 1;
        gbc.gridheight = 1;

        switch (ScilabAlignment.toSwingAlignment(horizontalAlignment)) {
            case SwingConstants.LEFT:
                switch (ScilabAlignment.toSwingAlignment(verticalAlignment)) {
                    case SwingConstants.TOP:
                        gbc.anchor = GridBagConstraints.NORTHWEST;
                        break;
                    case SwingConstants.CENTER:
                        gbc.anchor = GridBagConstraints.WEST;
                        break;
                    default: // SwingConstants.BOTTOM
                        gbc.anchor = GridBagConstraints.SOUTHWEST;
                        break;
                }
                break;
            case SwingConstants.CENTER:
                switch (ScilabAlignment.toSwingAlignment(verticalAlignment)) {
                    case SwingConstants.TOP:
                        gbc.anchor = GridBagConstraints.NORTH;
                        break;
                    case SwingConstants.CENTER:
                        gbc.anchor = GridBagConstraints.CENTER;
                        break;
                    default: // SwingConstants.BOTTOM
                        gbc.anchor = GridBagConstraints.SOUTH;
                        break;
                }
                break;
            default: // SwingConstants.RIGHT
                switch (ScilabAlignment.toSwingAlignment(verticalAlignment)) {
                    case SwingConstants.TOP:
                        gbc.anchor = GridBagConstraints.NORTHEAST;
                        break;
                    case SwingConstants.CENTER:
                        gbc.anchor = GridBagConstraints.EAST;
                        break;
                    default: // SwingConstants.BOTTOM
                        gbc.anchor = GridBagConstraints.SOUTHEAST;
                        break;
                }
                break;
        }

        alignmentLayout.setConstraints(label, gbc);
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
            case __GO_UI_ICON__: {
                if (labelStyle == LabelStyle.TEXT) {
                    String icon = (String) value;
                    if (icon != null && icon.equals("") == false) {
                        File file = new File((String) value);
                        if (file.exists() == false) {
                            String filename = FindIconHelper.findImage((String) value);
                            file = new File(filename);
                        }

                        try {
                            ((JLabel) label).setIcon(new ImageIcon(ImageIO.read(file)));
                        } catch (IOException e) {
                        }
                    } else {
                        ((JLabel) label).setIcon(null);
                    }
                }
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("Label.background");
        if (color != null) {
            //bypass setBackground
            if (label instanceof JLabel) {
                ((JLabel) label).setOpaque(true);
            }
            label.setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("Label.foreground");
        if (color != null) {
            label.setForeground(color);
        }
    }

    /**
     * Component to display LaTeX Thank you Calixte
     */
    public static class IconLabel extends JLabel {
        public enum IconType {
            LATEX, MATHML
        };

        private static final long serialVersionUID = 3885565301114930030L;
        private final static Font DEFAULTFONT = new Font("serif", Font.PLAIN, 12);
        private final static boolean isWindows = System.getProperty("os.name").toLowerCase().indexOf("win") != -1;

        private Icon icon;
        private Icon iconDisable;
        private float fontSize;
        private String text;
        private Dimension preferred;
        IconType type = IconType.MATHML;

        public IconLabel(IconType type) {
            super();
            fontSize = 12f;
            icon = null;
            iconDisable = null;
            preferred = new Dimension(0, 0);
            this.type = type;
        }

        /**
         * Set the LaTeX content
         * @param text the LaTeX
         */
        public void setText(String text) {
            this.text = text;
            update();
        }

        /**
         * Get the LaTeX content
         * @return the LaTeX string
         */
        public String getText() {
            return text;
        }

        public Dimension getPreferredSize() {
            return preferred;
        }

        public Dimension getMinimumSize() {
            return getPreferredSize();
        }

        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (isOpaque() && getBackground() != null) {
                final Color old = g.getColor();
                g.setColor(getBackground());
                g.fillRect(0, 0, getWidth(), getHeight());
                g.setColor(old);
            }

            int h = getHorizontalAlignment();
            int v = getVerticalAlignment();

            int x = 0;
            int y = 0;

            if (h == JLabel.CENTER) {
                x = (getSize().width - icon.getIconWidth()) / 2;
            } else if (h == JLabel.RIGHT) {
                x = getSize().width - icon.getIconWidth();
            }

            if (v == JLabel.CENTER) {
                y = (getSize().height - icon.getIconHeight()) / 2;
            } else if (v == JLabel.BOTTOM) {
                y = getSize().height - icon.getIconHeight();
            }

            if (type == IconType.LATEX) {
                TeXIcon latex = (TeXIcon) icon;
                if (isEnabled()) {
                    latex.setForeground(getForeground());
                    latex.paintIcon(this, g, x, y);
                } else {
                    if (isWindows && (UIManager.getColor("Label.disabledForeground") instanceof Color) && (UIManager.getColor("Label.disabledShadow") instanceof Color)) {
                        latex.setForeground(UIManager.getColor("Label.disabledShadow"));
                        latex.paintIcon(this, g, x + 1, y + 1);
                        latex.setForeground(UIManager.getColor("Label.disabledForeground"));
                        latex.paintIcon(this, g, x, y);
                    } else {
                        latex.setForeground(getBackground().brighter());
                        latex.paintIcon(this, g, x + 1, y + 1);
                        latex.setForeground(getBackground().darker());
                        latex.paintIcon(this, g, x, y);
                    }
                }
            } else { //MathML
                //enable/disable is made @ icon generation in ScilabSpecialTextUtilities
                if (isEnabled()) {
                    icon.paintIcon(this, g, x, y);
                } else {
                    icon.paintIcon(this, g, x + 1, y + 1);
                    iconDisable.paintIcon(this, g, x, y);
                }
            }
        }

        public void setFont(Font f) {
            this.fontSize = f.getSize2D();
            update();
        }

        public Font getFont() {
            return DEFAULTFONT;
        }

        /**
         * Update the component
         */
        private void update() {
            if (text.equals("") == false) {
                if (type == IconType.LATEX) {
                    icon = new TeXFormula(text).new TeXIconBuilder().setStyle(TeXConstants.STYLE_DISPLAY).setSize(fontSize).build();
                } else {
                    if (isEnabled()) {
                        icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, (int) fontSize);
                    } else {
                        if (isWindows && (UIManager.getColor("Label.disabledForeground") instanceof Color) && (UIManager.getColor("Label.disabledShadow") instanceof Color)) {
                            icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, (int) fontSize, UIManager.getColor("Label.disabledShadow"));
                            iconDisable = ScilabSpecialTextUtilities.compileMathMLExpression(text, (int) fontSize, UIManager.getColor("Label.disabledForeground"));
                        } else {
                            icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, (int) fontSize, getBackground().brighter());
                            iconDisable = ScilabSpecialTextUtilities.compileMathMLExpression(text, (int) fontSize, getBackground().darker());
                        }
                    }
                }
                preferred = new Dimension(icon.getIconWidth(), icon.getIconHeight());
                revalidate();
            }
        }

        public void setEnabled(boolean enabled) {
            super.setEnabled(enabled);
            update();
        }
    }
}
