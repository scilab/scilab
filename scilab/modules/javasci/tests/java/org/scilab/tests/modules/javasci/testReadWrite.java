/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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
package org.scilab.tests.modules.javasci;

import org.junit.*;
import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.Map;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabBooleanSparse;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabPolynomial;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabSparse;
import org.scilab.modules.types.ScilabTypeEnum;

public class testReadWrite {
    private Scilab sci;

    /*
     * This method will be called for each test.
     * with @After, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */
    @Before
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assertTrue(sci.open());
    }

    @Test()
    public void putAndGetEmptyMatrixTest() throws NullPointerException, JavasciException {
        ScilabDouble aOriginal = new ScilabDouble();
        sci.put("a", aOriginal);
        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetDoubleTest() throws NullPointerException, JavasciException {
        double [][]a = {{21.2, 22.0, 42.0, 39.0}, {23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a", aOriginal);
        assertTrue(sci.exec("somme = sum(a);"));

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetComplexDoubleTest() throws NullPointerException, JavasciException {
        double [][]a = {{21.2, 22.0, 42.0, 39.0}, {23.2, 24.0, 44.0, 40.0}};
        double [][]aImg = {{212.2, 221.0, 423.0, 393.0}, {234.2, 244.0, 441.0, 407.0}};

        ScilabDouble aOriginal = new ScilabDouble(a, aImg);
        sci.put("a", aOriginal);

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");
        assertTrue(aFromScilab.equals(aOriginal));
    }


    @Test()
    public void putAndGetBooleanTest() throws NullPointerException, JavasciException {
        boolean [][]a = {{true, true, false, false}, {true, false, true, false}};
        ScilabBoolean aOriginal = new ScilabBoolean(a);
        sci.put("a", aOriginal);

        ScilabBoolean aFromScilab = (ScilabBoolean)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetSparseTest() throws NullPointerException, JavasciException {
        double [][]a = {{0, 22.0, 0, 39.0}, {23.2, 0, 0, 40.0}};
        ScilabSparse aOriginal = new ScilabSparse(a);
        sci.put("a", aOriginal);
        assertTrue(sci.exec("somme = sum(a);"));

        ScilabSparse aFromScilab = (ScilabSparse)sci.get("a");
        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetComplexSparseTest() throws NullPointerException, JavasciException {
        double [][] a = {{0, 22.0, 0, 39.0}, {23.2, 0, 0, 40.0}};
        double [][] aImg = {{0, 11.0, 0, 18.5}, {1.34, 0, 0, 41.0}};
        ScilabSparse aOriginal = new ScilabSparse(a, aImg);
        sci.put("a", aOriginal);
        assertTrue(sci.exec("somme = sum(a);"));

        ScilabSparse aFromScilab = (ScilabSparse)sci.get("a");
        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetBooleanSparseTest() throws NullPointerException, JavasciException {
        boolean [][]a = {{false, true, false, true}, {true, false, false, true}};
        ScilabBooleanSparse aOriginal = new ScilabBooleanSparse(a);
        sci.put("a", aOriginal);
        assertTrue(sci.exec("andA = and(a);"));

        ScilabBooleanSparse aFromScilab = (ScilabBooleanSparse)sci.get("a");
        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetPolynomialTest() throws NullPointerException, JavasciException {
        double [][][] a = {{{1, 2, 3}, {2}, {0, 0, 0, -4}}, {{ -1, 0, 2}, {0, 2, 0, 0, 3}, {0}}};
        ScilabPolynomial aOriginal = new ScilabPolynomial(a, "X");
        sci.put("a", aOriginal);
        assertTrue(sci.exec("somme = sum(a);"));

        ScilabPolynomial aFromScilab = (ScilabPolynomial)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetComplexPolynomialTest() throws NullPointerException, JavasciException {
        double [][][] a = {{{1, 2, 3}, {2}, {0, 0, 0, -4}}, {{ -1, 0, 2}, {0, 2, 0, 0, 3}, {0}}};
        double [][][] aImg = {{{2, -1, 3}, {0}, {2, 0, 1, -3}}, {{3, 0, 1}, {1, -3, 0, 0, -2}, {0}}};
        ScilabPolynomial aOriginal = new ScilabPolynomial(a, aImg, "NAME");
        sci.put("a", aOriginal);
        assertTrue(sci.exec("somme = sum(a);"));

        ScilabPolynomial aFromScilab = (ScilabPolynomial)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetStringTest() throws NullPointerException, JavasciException {
        String [][]a = {{"String1", "String2", "String3", "String4"},
            {"String5", "String6", "My String 7", "String8"}
        };
        ScilabString aOriginal = new ScilabString(a);
        sci.put("a", aOriginal);

        assertTrue(sci.exec("checksize = and(size(a)==[2,4]);"));

        ScilabBoolean checksize = (ScilabBoolean)sci.get("checksize");
        assertTrue(checksize.getData()[0][0]);

        ScilabString aFromScilab = (ScilabString)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetListTest() throws NullPointerException, JavasciException {
        ScilabList aOriginal = new ScilabList();
        aOriginal.add(new ScilabDouble(new double[][] {{1, 2, 3}, {3, 4, 5}}));
        aOriginal.add(new ScilabInteger(new int[][] {{1, 2, 3}, {3, 4, 5}}, false));
        aOriginal.add(new ScilabString(new String[][] {{"1", "22", "333"}, {"333", "4444", "55555"}}));
        aOriginal.add(new ScilabSparse(new double[][] {{0, 22.0, 0, 39.0}, {23.2, 0, 0, 40.0}}));
        aOriginal.add(new ScilabDouble(new double[][] {{1, 2, 3}, {3, 4, 5}}, new double[][] {{3, 4, 5}, {1, 2, 3}}));
        aOriginal.add(new ScilabBooleanSparse(new boolean[][] {{false, true, false, true}, {true, false, false, true}}));
        aOriginal.add(new ScilabPolynomial(new double[][][] {{{1, 2, 3}, {2}, {0, 0, 0, -4}}, {{ -1, 0, 2}, {0, 2, 0, 0, 3}, {0}}}));
        aOriginal.add(new ScilabInteger(new byte[][] {{1, 2, 3}, {3, 4, 5}}, true));
        aOriginal.add(new ScilabSparse(new double[][] {{0, 22.0, 0, 39.0}, {23.2, 0, 0, 40.0}}, new double[][] {{0, 11.0, 0, 18.5}, {1.34, 0, 0, 41.0}}));
        aOriginal.add(new ScilabPolynomial(new double[][][] {{{1, 2, 3}, {2}, {0, 0, 0, -4}}, {{ -1, 0, 2}, {0, 2, 0, 0, 3}, {0}}}, new double[][][] {{{2, -1, 3}, {0}, {2, 0, 1, -3}}, {{3, 0, 1}, {1, -3, 0, 0, -2}, {0}}}));

        ScilabMList ml = new ScilabMList();
        aOriginal.add(ml);
        ml.add(new ScilabDouble(new double[][] {{1, 2, 3}, {3, 4, 5}}));
        ml.add(new ScilabPolynomial(new double[][][] {{{1, 2, 3}, {2}, {0, 0, 0, -4}}, {{ -1, 0, 2}, {0, 2, 0, 0, 3}, {0}}}, new double[][][] {{{2, -1, 3}, {0}, {2, 0, 1, -3}}, {{3, 0, 1}, {1, -3, 0, 0, -2}, {0}}}));

        ScilabList l = new ScilabList();
        ml.add(l);
        l.add(new ScilabInteger(new short[][] {{1, 2, 3}, {3, 4, 5}}, false));
        l.add(new ScilabInteger(new short[][] {{1, 2, 3}, {3, 4, 5}}, true));
        ml.add(new ScilabPolynomial(new double[][][] {{{1, 2, 3}, {2}, {0, 0, 0, -4}}, {{ -1, 0, 2}, {0, 2, 0, 0, 3}, {0}}}));

        ScilabTList tl = new ScilabTList();
        ml.add(tl);
        tl.add(new ScilabInteger(new short[][] {{1, 2, 3}, {3, 4, 5}}, true));

        ScilabList l1 = new ScilabList();
        tl.add(l1);
        l1.add(new ScilabInteger(new byte[][] {{1, 2, 3}, {3, 4, 5}}, false));
        aOriginal.add(new ScilabString(new String[][] {{"1", "22", "333"}, {"333", "4444", "55555"}}));
        sci.put("a", aOriginal);

        ScilabList aFromScilab = (ScilabList)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test()
    public void putAndGetXCOSMListTest() throws NullPointerException, JavasciException {
        String mlistcode = "mlist([\"diagram\", \"props\", \"objs\"], tlist([\"params\", \"wpar\", \"title\", \"tol\", \"tf\", \"context\", \"void1\", \"options\", \"void2\", \"void3\", \"doc\"], [600.0, 450.0, 0.0, 0.0, 600.0, 450.0], [\"CLSS\"], [1.0E-6, 1.0E-6, 1.0E-10, 100001.0, 0.0, 0.0, 0.0], [10.0], [\"\"], [], tlist([\"scsopt\", \"3D\", \"Background\", \"Link\", \"ID\", \"Cmap\"], list([%t], [33.0]), [8.0, 1.0], [1.0, 5.0], list([5.0, 1.0], [4.0, 1.0]), [0.8, 0.8, 0.8]), [], [], list()), list(mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [430.0, -180.0], [40.0, 40.0], [%t], [0.0], [], [], [], [], [7.0], list([\"xstringb(orig(1),orig(2),\"\"CLOCK_c\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [], [], [], [], [\"CLOCK_c;flip=false;mirror=false\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], [\"csuper\"], [], [], [], [], [], [], [], [-1.0], [], [], list(), mlist([\"diagram\", \"props\", \"objs\"], tlist([\"params\", \"wpar\", \"title\", \"tol\", \"tf\", \"context\", \"void1\", \"options\", \"void2\", \"void3\", \"doc\"], [600.0, 450.0, 0.0, 0.0, 600.0, 450.0], [\"Untitled\"], [1.0E-6, 1.0E-6, 1.0E-10, 100001.0, 0.0, 0.0, 0.0], [100000.0], [\"\"], [], tlist([\"scsopt\", \"3D\", \"Background\", \"Link\", \"ID\", \"Cmap\"], list([%t], [33.0]), [8.0, 1.0], [1.0, 5.0], list([5.0, 1.0], [4.0, 1.0]), [0.8, 0.8, 0.8]), [], [], list()), list(mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [440.0, -160.0], [40.0, 40.0], [%t], [0.0], [\"0.1\" ; \"0.1\"], [], [], [5.0], [6.0], list([\"xstringb(orig(1),orig(2),\"\"EVTDLY_c\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [], [], [], [], [\"EVTDLY_c\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], list([\"evtdly4\"], [4.0]), [], [], [], [], [], [], [-1.0], [-1.0], [], [], list(), [0.1 ; 0.1], [], list(), [\"d\"], [0.1], [%f, %f], [\"bedc105:13552780191:-7fc7\"], [0.0], [0.0], list()), [\"EVTDLY_c\"], list([\"bedc105:13552780191:-7fc7\"])), mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [519.0, -230.0], [20.0, 20.0], [%t], [0.0], [\"1\"], [], [], [4.0], [], list([\"xstringb(orig(1),orig(2),\"\"CLKOUT_f\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [], [], [], [], [\"CLKOUT_f\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], [\"output\"], [], [], [], [], [], [], [-1.0], [], [], [], list(), [], [1.0], list(), [\"d\"], [], [%f, %f], [\"bedc105:13552780191:-7fcb\"], [0.0], [0.0], list()), [\"CLKOUT_f\"], list([\"bedc105:13552780191:-7fcb\"])), mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [500.71066, -220.0], [8.0, 8.0], [%t], [0.0], [], [], [], [6.0], [4.0 ; 5.0], list([\"xstringb(orig(1),orig(2),\"\"CLKSPLIT_f\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [], [], [], [], [\"CLKSPLIT_f\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], [\"split\"], [], [], [], [], [], [], [-1.0], [-1.0 ; -1.0], [], [], list(), [], [], list(), [\"d\"], [-1.0 ; -1.0], [%f, %f], [\"bedc105:13552780191:-7fc2\"], [0.0], [0.0], list()), [\"CLKSPLIT_f\"], list([\"bedc105:13552780191:-7fc2\"])), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [500.71066 ; 529.0], [-216.0 ; -186.0], [\"drawlink\"], [0.0, 0.0], [5.0, -1.0], [3.0, 1.0, 1.0], [2.0, 1.0, 0.0]), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [510.71066 ; 533.71 ; 493.0 ; 460.0], [-216.0 ; -114.0 ; -114.0 ; -76.0], [\"drawlink\"], [0.0, 0.0], [5.0, -1.0], [3.0, 2.0, 1.0], [1.0, 1.0, 0.0]), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [460.0 ; 478.0 ; 500.71066], [-124.0 ; -276.0 ; -200.0], [\"drawlink\"], [0.0, 0.0], [5.0, -1.0], [1.0, 1.0, 1.0], [3.0, 1.0, 0.0])), [\"\"]), [], list(), [\"h\"], [-1.0], [%f, %f], [\"3bcfd9d2:12c7e695a9b:-7fc5\"], [0.0], [0.0], list()), [\"CLOCK_c\"], list([\"3bcfd9d2:12c7e695a9b:-7fc5\"])), mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [270.0, -300.0], [100.0, 70.0], [%t], [0.0], [\"[-2 -1/5;10 0]\" ; \"[1/5;0]\" ; \"[0 1]\" ; \"0\" ; \"[0 0.5]\"], [5.0], [6.0], [], [], list([\"xstringb(orig(1),orig(2),\"\"CLSS\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [\"E\"], [\"E\"], [\"ExplicitInputPort;align=center;labelPosition=right;verticalLabelPosition=middle;rotation=0;spacingLeft=5;flip=false;mirror=false\"], [\"ExplicitOutputPort;align=center;labelPosition=left;verticalLabelPosition=middle;rotation=0;spacingRight=5;flip=false;mirror=false\"], [\"CLSS;flip=false;mirror=false\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], list([\"csslti4\"], [4.0]), [1.0], [1.0], [1.0], [1.0], [1.0], [1.0], [], [], [0.0 ; 0.5], [], list(), [-2.0 ; 10.0 ; -0.2 ; 0.0 ; 0.2 ; 0.0 ; 0.0 ; 1.0 ; 0.0], [], list(), [\"c\"], [], [%f, %t], [\"3bcfd9d2:12c7e695a9b:-7fde\"], [0.0], [0.0], list()), [\"CLSS\"], list([\"3bcfd9d2:12c7e695a9b:-7fde\"])), mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [160.0, -290.0], [48.0, 40.0], [%t], [0.0], [], [], [5.0], [], [], list([\"xstringb(orig(1),orig(2),\"\"STEP_FUNCTION\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [], [\"E\"], [], [\"ExplicitOutputPort;align=center;labelPosition=left;verticalLabelPosition=middle;rotation=0;spacingRight=5;flip=false;mirror=false\"], [\"STEP_FUNCTION;flip=false;mirror=false\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], [\"csuper\"], [], [], [], [-1.0], [1.0], [1.0], [], [], [], [], list(), mlist([\"diagram\", \"props\", \"objs\"], tlist([\"params\", \"wpar\", \"title\", \"tol\", \"tf\", \"context\", \"void1\", \"options\", \"void2\", \"void3\", \"doc\"], [600.0, 450.0, 0.0, 0.0, 600.0, 450.0], [\"STEP_FUNCTION\"], [1.0E-4, 1.0E-6, 1.0E-10, 100001.0, 0.0, 0.0, 0.0], [14.0], [\" \"], [], tlist([\"scsopt\", \"3D\", \"Background\", \"Link\", \"ID\", \"Cmap\"], list([%t], [33.0]), [8.0, 1.0], [1.0, 5.0], list([5.0, 1.0], [4.0, 1.0]), [0.8, 0.8, 0.8]), [], [], list()), list(mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [202.230597, -160.0], [40.0, 40.0], [%t], [0.0], [\"1\" ; \"0\" ; \"1\"], [], [3.0], [4.0], [4.0], list([\"xstringb(orig(1),orig(2),\"\"STEP\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [], [\"E\"], [], [\"ExplicitOutputPort;align=right;verticalAlign=middle;spacing=10;rotation=0\"], [\"STEP\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], list([\"step_func\"], [4.0]), [], [], [], [1.0], [1.0], [1.0], [-1.0], [-1.0], [], [], list(), [0.0 ; 1.0], [], list(), [\"c\"], [1.0], [%f, %t], [\"bedc105:13552780191:-7fb7\"], [0.0], [0.0], list()), [\"STEP\"], list([\"bedc105:13552780191:-7fb7\"])), mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [270.80203, -150.0], [20.0, 20.0], [%t], [0.0], [\"1\"], [3.0], [], [], [], list([\"xstringb(orig(1),orig(2),\"\"OUT_f\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [\"E\"], [], [\"ExplicitInputPort;align=left;verticalAlign=middle;spacing=10;rotation=0\"], [], [\"OUT_f\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], [\"output\"], [-1.0], [-2.0], [-1.0], [], [], [], [], [], [], [], list(), [], [1.0], list(), [\"c\"], [], [%f, %f], [\"bedc105:13552780191:-7fb4\"], [0.0], [0.0], list()), [\"OUT_f\"], list([\"bedc105:13552780191:-7fb4\"])), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [246.230597 ; 266.80203], [-100.0 ; -120.0], [\"drawlink\"], [0.0, 0.0], [1.0, 1.0], [1.0, 1.0, 0.0], [2.0, 1.0, 1.0]), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [222.230597 ; 270.2306 ; 231.708992 ; 231.708992 ; 270.2306 ; 222.230597], [-124.0 ; -262.39289999999994 ; -262.39289999999994 ; -172.69678 ; -172.69678 ; -76.0], [\"drawlink\"], [0.0, 0.0], [5.0, -1.0], [1.0, 1.0, 1.0], [1.0, 1.0, 0.0])), [\"\"]), [], list(), [\"h\"], [], [%f, %f], [\"3bcfd9d2:12c7e695a9b:-7fd6\"], [0.0], [0.0], list()), [\"STEP_FUNCTION\"], list([\"3bcfd9d2:12c7e695a9b:-7fd6\"])), mlist([\"Block\", \"graphics\", \"model\", \"gui\", \"doc\"], mlist([\"graphics\", \"orig\", \"sz\", \"flip\", \"theta\", \"exprs\", \"pin\", \"pout\", \"pein\", \"peout\", \"gr_i\", \"id\", \"in_implicit\", \"out_implicit\", \"in_style\", \"out_style\", \"style\"], [430.0, -290.0], [40.0, 40.0], [%t], [0.0], [\"1 3 5 7 9 11 13 15\" ; \"-1\" ; \"[]\" ; \"[600;400]\" ; \"0.2\" ; \"1.1\" ; \"10\" ; \"20\" ; \"0\" ; \"\"], [6.0], [], [7.0], [], list([\"xstringb(orig(1),orig(2),\"\"CSCOPE\"\",sz(1),sz(2));\"], [8.0]), [\"\"], [\"E\"], [], [\"ExplicitInputPort;align=center;labelPosition=right;verticalLabelPosition=middle;rotation=0;spacingLeft=5;flip=false;mirror=false\"], [], [\"CSCOPE;flip=false;mirror=false\"]), mlist([\"model\", \"sim\", \"in\", \"in2\", \"intyp\", \"out\", \"out2\", \"outtyp\", \"evtin\", \"evtout\", \"state\", \"dstate\", \"odstate\", \"rpar\", \"ipar\", \"opar\", \"blocktype\", \"firing\", \"dep_ut\", \"label\", \"nzcross\", \"nmode\", \"equations\"], list([\"cscope\"], [4.0]), [-1.0], [1.0], [1.0], [], [], [], [-1.0], [], [], [], list(), [0.0 ; 0.2 ; 1.1 ; 10.0], [-1.0 ; 1.0 ; 20.0 ; 1.0 ; 3.0 ; 5.0 ; 7.0 ; 9.0 ; 11.0 ; 13.0 ; 15.0 ; -1.0 ; -1.0 ; 600.0 ; 400.0], list(), [\"c\"], [], [%t, %f], [\"3bcfd9d2:12c7e695a9b:-7fcd\"], [0.0], [0.0], list()), [\"CSCOPE\"], list([\"3bcfd9d2:12c7e695a9b:-7fcd\"])), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [212.0 ; 266.0], [-230.0 ; -200.0], [\"drawlink\"], [0.0, 0.0], [1.0, 1.0], [3.0, 1.0, 0.0], [2.0, 1.0, 1.0]), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [374.0 ; 426.0], [-200.0 ; -230.0], [\"drawlink\"], [0.0, 0.0], [1.0, 1.0], [2.0, 1.0, 0.0], [4.0, 1.0, 1.0]), mlist([\"Link\", \"xx\", \"yy\", \"id\", \"thick\", \"ct\", \"from\", \"to\"], [450.0 ; 450.0], [-144.0 ; -206.0], [\"drawlink\"], [0.0, 0.0], [5.0, -1.0], [1.0, 1.0, 1.0], [4.0, 1.0, 0.0])), [\"\"])";
        assertTrue(sci.exec("ml=" + mlistcode));
        ScilabMList mlFromScilab = (ScilabMList)sci.get("ml");
        sci.put("ml1", mlFromScilab);

        ScilabMList ml1FromScilab = (ScilabMList)sci.get("ml1");
        assertTrue(mlFromScilab.equals(ml1FromScilab));
    }

    //    @Test( expected = UnsupportedTypeException.class)
    @Test()
    public void ReadSparseTypeTest() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("W=sparse([1,2;4,5;3,10],[1,2,3]);"));
        assertEquals(sci.getVariableType("W"), ScilabTypeEnum.sci_sparse);
        ScilabSparse aFromScilab = (ScilabSparse)sci.get("W");
        assertTrue(aFromScilab.toString().equals("sparse([1, 2 ; 3, 10 ; 4, 5], [1.0 ; 3.0 ; 2.0], [4, 10])"));
        assertTrue(sci.exec("AZE= " + aFromScilab.toString()));
        ScilabSparse aFromScilab2 = (ScilabSparse)sci.get("AZE");

        assertTrue(Arrays.deepEquals(aFromScilab.getFullRealPart(), aFromScilab2.getFullRealPart()));

        ScilabSparse mySparse = new ScilabSparse(100, 100, 5, new int[] { 1, 1, 1, 1, 1}, new int[] { 1, 25, 50, 75, 99}, new double[] { 1.0, 2.0, 3.0, 4.0, 5.0});
        // sci.put with a sparse is not yet functional
        //        assertTrue(sci.put("mySparse", mySparse));
        //        String ref="mySparseRef = sparse([1, 2 ; 2, 26 ; 3, 51 ; 4, 76 ; 5, 100], [1.0 ; 2.0 ; 3.0 ; 4.0 ; 5.0], [100, 100]);";
        //        sci.exec("isEqual=(mySparseRef==mySparse)");
        //        ScilabBoolean isEqual = (ScilabBoolean)sci.get("isEqual");
        //        System.out.println("isequal " +isEqual);


    }

    @Test()
    public void ReadStructTest() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("myDate=struct('day',25,'month' ,'DEC','year',2006)"));
        assertEquals(sci.getVariableType("myDate"), ScilabTypeEnum.sci_mlist);

        ScilabMList myDate = (ScilabMList)sci.get("myDate");
        assertTrue(myDate.toString().equals("mlist([\"st\", \"dims\", \"day\", \"month\", \"year\"], int32([1, 1]), [25.0], [\"DEC\"], [2006.0])"));
        assertEquals(myDate.getHeight(), 1);
        assertEquals(myDate.getWidth(), 5);
        assertTrue(myDate.getVarName().equals("myDate"));
        assertTrue(myDate.getMListType().equals("st"));
        Map<String, ScilabType> listFields = myDate.getMListFields();
        ScilabString month = (ScilabString)listFields.get("month");
        assertTrue(month.getData()[0][0].equals("DEC"));
        ScilabDouble year = (ScilabDouble)listFields.get("year");
        assertEquals(year.getRealPart()[0][0], 2006.0, 1e-8);
        ScilabDouble day = (ScilabDouble)listFields.get("day");
        assertEquals(day.getRealPart()[0][0], 25.0, 1e-8);

    }

    @Test( expected = UndefinedVariableException.class)
    public void UndefinedVariableExceptionTest() throws NullPointerException, JavasciException {
        sci.get("undefinedVar"); /* Will launch an UnsupportedTypeException exception */
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();
    }
}