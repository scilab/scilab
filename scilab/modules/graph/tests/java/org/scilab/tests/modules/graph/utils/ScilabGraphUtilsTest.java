/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.tests.modules.graph.utils;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import javax.swing.Icon;
import java.awt.GraphicsEnvironment;

import org.apache.batik.gvt.GraphicsNode;
import org.scilab.forge.jlatexmath.ParseException;
import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.junit.*;
import org.junit.Assume;

/**
 * Test the {@link ScilabGraphUtils} class.
 */
public class ScilabGraphUtilsTest {
    private static final String TEXT = "Sample text without any symbols";
    private static final String TEXT_WITH_BLANKS = "        Sample text without any symbols";
    private static final String SOME_HTML_SYMBOLS = "<>&\\àÀâÂäÄ";

    private static final String SAMPLE_LATEX = "\frac{a+b}{c-d}";

    private static final String SAMPLE_SVG = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>"
            + "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"300\" height=\"200\">"
            + "<title>Exemple simple de figure SVG</title>"
            + "<rect width=\"100\" height=\"80\" x=\"0\" y=\"70\" fill=\"green\" />"
            + "</svg>";

    /**
     * Check the equality with an escaped non-formatted text and itself.
     */
    @Test
    public void escapingUnformattedText() {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());

        StringBuilder str = new StringBuilder(TEXT);
        ScilabGraphUtils.unescape(str, 0);

        assert TEXT.equals(str.toString());
    }

    /**
     * Check the equality with an escaped-unescaped text and itself.
     */
    @Test
    public void escapingUnescapingText() {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());

        StringBuilder str = new StringBuilder(ScilabGraphUtils.getBodyMarkup(SOME_HTML_SYMBOLS, true));
        ScilabGraphUtils.unescape(str, 0);

        assert SOME_HTML_SYMBOLS.equals(str.toString());

        str = new StringBuilder(ScilabGraphUtils.getBodyMarkup(TEXT, true));
        ScilabGraphUtils.unescape(str, 0);

        assert TEXT.equals(str.toString());

        str = new StringBuilder(ScilabGraphUtils.getBodyMarkup(SAMPLE_LATEX, true));
        ScilabGraphUtils.unescape(str, 0);

        assert SAMPLE_LATEX.equals(str.toString());

        str = new StringBuilder(ScilabGraphUtils.getBodyMarkup(SAMPLE_SVG, true));
        ScilabGraphUtils.unescape(str, 0);

        assert SAMPLE_SVG.equals(str.toString());
    }

    /**
     * Check the right behavior of the remove blanks method
     * @see ScilabGraphUtils#removeBlanks(StringBuilder)
     */
    @Test
    public void removeBlanks() {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());

        StringBuilder str = new StringBuilder(TEXT_WITH_BLANKS);
        ScilabGraphUtils.removeBlanks(str);
        assert str.toString().equals(TEXT);
    }

    /**
     * Check the return status of a valid and invalid latex expressions.
     */
    @Test
    public void checkSampleTEX() throws ParseException {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());

        Icon valid = ScilabGraphUtils.getTexIcon(SAMPLE_LATEX, 16);

        assert valid != null;

        Icon invalid = null;
        try {
            invalid = ScilabGraphUtils.getTexIcon(SAMPLE_LATEX + "$", 16);
        } catch (ParseException e) {
            assert invalid == null;
            return;
        }

        // Unreachable valid statement
        assert false;
    }

    /**
     * Check the return status of a valid and invalid SVG expressions.
     * @throws IOException when an I/O errors occurs
     */
    @Test
    public void checkSampleSVG() throws IOException {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());

        File validSvgFile;
        File invalidSvgFile;

        validSvgFile = File.createTempFile("sampleValid", ".svg");
        validSvgFile.deleteOnExit();

        FileWriter out = new FileWriter(validSvgFile);
        out.append(SAMPLE_SVG);
        out.close();

        invalidSvgFile = File.createTempFile("sampleInvalid", ".svg");
        invalidSvgFile.deleteOnExit();

        out = new FileWriter(invalidSvgFile);
        out.append(SAMPLE_SVG + "invalid text");
        out.close();

        // check that for a valid content the reference is valid
        GraphicsNode svg = ScilabGraphUtils.getSVGComponent(validSvgFile.toURI().toURL());
        assert svg != null;

        // check that for the same content, the reference is the same
        GraphicsNode svg2 = ScilabGraphUtils.getSVGComponent(validSvgFile.toURI().toURL());
        assert svg2 == svg;

        // check that for a valid content the reference is invalid
        GraphicsNode invalid = ScilabGraphUtils.getSVGComponent(invalidSvgFile.toURI().toURL());
        assert invalid == null;
    }

    /**
     * Call all public methods through introspection
     * @param args not used
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws IllegalArgumentException
     */
    public static void main(String[] args) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
        Object obj = new ScilabGraphUtilsTest();
        Method[] tests = ScilabGraphUtilsTest.class.getDeclaredMethods();
        for (Method method : tests) {
            int modifiers = method.getModifiers();
            if ((modifiers | Modifier.STATIC) != modifiers) {
                method.invoke(obj, (Object[]) null);
            }
        }
    }
}
