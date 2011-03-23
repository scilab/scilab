/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console.utils;

import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.jvm.LoadClassPath;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.jlatexmath.ParseException;

import net.sourceforge.jeuclid.MathMLParserSupport;
import net.sourceforge.jeuclid.MutableLayoutContext;
import net.sourceforge.jeuclid.layout.JEuclidView;
import net.sourceforge.jeuclid.context.LayoutContextImpl;
import net.sourceforge.jeuclid.context.Parameter;

import org.w3c.dom.Node;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

/**
 * Utilities with JLaTeXMath.
 * @author Calixte DENIZET
 */
public final class ScilabSpecialTextUtilities {

    private static boolean loadedLaTeX;
    private static boolean loadedMathML;
    private static Thread loadJLM;

    /**
     * @param component where to set a LaTeX icon
     * @param text to use for the menu, if it's enclosed between '$' then it's interpreted as
     * a LaTeX string and if enclosed between '&lt;' and '&gt;', then as MathML one.
     */
    public static boolean setText(JComponent component, String text) {
        Icon icon = null;
        if (text != null && text.length() != 0) {
            if (text.startsWith("$") && text.endsWith("$")) {
                icon = compileLaTeXExpression(text.substring(1, text.length() - 1), component.getFont().getSize());
            } else if (text.startsWith("<") && text.endsWith(">")) {
                icon = compileMathMLExpression(text, component.getFont().getSize());
            }
        }

        if (icon != null) {
            try {
                setIcon(component, icon);
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        }

        return icon != null;
    }

    /**
     * Load, if necessary the jlatexmath package, and compile a LaTeX expression.
     * @param exp the expression to compile
     * @param fontSize the size of the font
     * @return the Icon
     */
    public static Icon compileLaTeXExpression(String exp, int fontSize) {
        if (!loadedLaTeX) {
            LoadClassPath.loadOnUse("graphics_latex_textrendering");
            loadedLaTeX = true;
        }

        return LaTeXCompiler.compile(exp, fontSize);
    }

    /**
     * Load, if necessary the jlatexmath package, and compile a LaTeX expression or a valid subexpression.
     * @param exp the expression to compile
     * @param fontSize the size of the font
     * @return the Icon
     */
    public static Icon compilePartialLaTeXExpression(String exp, int fontSize) {
        if (!loadedLaTeX) {
            if (loadJLM == null) {
                loadJLM = new Thread(new Runnable() {
/* Create a thread in the background to avoid a lag in the loading of jar */
                        public void run() {
                            LoadClassPath.loadOnUse("graphics_latex_textrendering");
                            LaTeXCompiler.compilePartial("", 0);
                            loadedLaTeX = true;
                            loadJLM = null;
                        }
                    });
                loadJLM.setPriority(Thread.MIN_PRIORITY);
                loadJLM.start();
            }
            return null;
        } else {
            return LaTeXCompiler.compilePartial(exp, fontSize);
        }
    }

    /**
     * Load, if necessary the jeuclid package, and compile a LaTeX expression.
     * @param exp the expression to compile
     * @param fontSize the size of the font
     * @return the Icon
     */
    public static Icon compileMathMLExpression(String exp, int fontSize) {
        if (!loadedMathML) {
            LoadClassPath.loadOnUse("graphics_mathml_textrendering");
            loadedMathML = true;
        }

        return MathMLCompiler.compile(exp, fontSize);
    }

    /**
     * @param component where to set a LaTeX icon
     * @param icon to set
     */
    private static void setIcon(JComponent component, Icon icon) throws InvocationTargetException {
        try {
            Class clazz = component.getClass();
            Method method = clazz.getMethod("setIcon", new Class[]{Icon.class});
            method.invoke(component, new Object[]{icon});
        } catch (NoSuchMethodException e) {
            throw new InvocationTargetException(e, "No valid method setIcon");
        } catch (IllegalAccessException e) {
            throw new InvocationTargetException(e, "The method setIcon must be public");
        } catch (InvocationTargetException e) {
            throw new InvocationTargetException(e, "The method setIcon threw an exception");
        }
    }

    /**
     * Use an inner class rather than a single method to avoid the load of the jar of jlatex
     */
    private static final class LaTeXCompiler {

        /**
         * Compile the expression
         * @param str the expression to compile
         * @param fontSize the size of the font
         * @return the Icon
         */
        static Icon compile(String str, int fontSize) {
            Icon icon = null;
            try {
                TeXFormula formula = new TeXFormula(str);
                icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize);
            } catch (ParseException e) { }

            return icon;
        }

        /**
         * Compile the expression
         * @param str the expression to compile
         * @param fontSize the size of the font
         * @return the Icon
         */
        static Icon compilePartial(String str, int fontSize) {
            Icon icon = null;
            try {
                TeXFormula formula = TeXFormula.getPartialTeXFormula(str);
                icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize);
            } catch (ParseException e) { }

            return icon;
        }
    }

    /**
     * Use an inner class rather than a single method to avoid the load of the jar of jeuclid
     */
    private static final class MathMLCompiler {

        private static final Graphics2D TEMPGRAPHIC = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB).createGraphics();

        /**
         * Compile the expression
         * @param str the expression to compile
         * @param fontSize the size of the font
         * @return the Icon
         */
        static Icon compile(String str, int fontSize) {
            LayoutContextImpl parameters = new LayoutContextImpl(LayoutContextImpl.getDefaultLayoutContext());
            parameters.setParameter(Parameter.MATHSIZE, fontSize);
            Document doc = null;
            try {
                doc = MathMLParserSupport.parseString(str);
            } catch (final SAXException e) {
                return null;
            } catch (final ParserConfigurationException e) {
                return null;
            } catch (final IOException e) {
                return null;
            }

            JEuclidView jev = new JEuclidView((Node) doc, parameters, TEMPGRAPHIC);

            int width = (int) Math.ceil(jev.getWidth());
            int ascent = (int) Math.ceil(jev.getAscentHeight());
            int height = (int) Math.ceil(jev.getDescentHeight()) + ascent;

            if (width <= 0 || height <=0) {
                return null;
            }

            BufferedImage bimg = new BufferedImage(width + 2, height, BufferedImage.TYPE_INT_ARGB);
            Graphics2D g2d = bimg.createGraphics();
            g2d.setColor(new Color(255, 255, 255, 0));
            g2d.fillRect(0, 0, width, height);

            jev.draw(g2d, 0, ascent);
            g2d.dispose();

            return new ImageIcon(bimg);
        }
    }
}
