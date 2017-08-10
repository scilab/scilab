/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
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
package org.scilab.tests.modules.types;

import static org.junit.Assert.assertTrue;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.junit.BeforeClass;
import org.junit.Test;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;

public class testSerialization {

    @BeforeClass
    public static final void loadLibrary() {
        if (!System.getProperty("os.name").toLowerCase().contains("windows")) {
            System.loadLibrary("scilab");
        }
    }

    private String getReferenceDataPath() {
        if (!System.getProperty("os.name").toLowerCase().contains("windows")) {
            return ScilabConstants.SCI + "/modules/types/tests/java/org/scilab/tests/modules/types/referenceData/";
        } else {
            return "tests/java/org/scilab/tests/modules/types/referenceData/";
        }
    }

    private void writeReference(ScilabType a, String to) throws IOException {

        FileOutputStream fos = new FileOutputStream(getReferenceDataPath() + to);

        ObjectOutputStream oos = new ObjectOutputStream(fos);
        try {
            oos.writeObject(a);
            oos.flush();
        } finally {

            try {
                oos.close();
            } finally {
                fos.close();
            }
        }
    }

    private ScilabType readFromFile(String filename) {
        ScilabType a = null;
        try {
            FileInputStream fis = new FileInputStream(getReferenceDataPath() + filename);

            ObjectInputStream ois = new ObjectInputStream(fis);
            try {
                a = (ScilabType) ois.readObject();
            } finally {
                try {
                    ois.close();
                } finally {
                    fis.close();
                }
            }
        } catch (IOException ioe) {
            ioe.printStackTrace();
        } catch (ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
        return a;

    }

    private final String scilabDoubleFile = "double.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeDoubleFileReference() throws IOException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        ScilabDouble aMatrix = new ScilabDouble(a);
        writeReference(aMatrix, scilabDoubleFile);
    }

    private final String scilabDoubleComplexFile = "double.complex.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeDoubleComplexFileReference() throws IOException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] aImg = { { 210.2, 220.0, 420.0, 390.0 }, { 230.2, 240.0, 440.0, 400.0 } };
        ScilabDouble aMatrix = new ScilabDouble(a, aImg);
        writeReference(aMatrix, scilabDoubleComplexFile);
    }

    private final String scilabStringFile = "string.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeStringFileReference() throws IOException {
        String[][] a = { { "This", "is", "my", "string" }, { "and", "I want to", "compare", " them" } };
        ScilabString aMatrix = new ScilabString(a);
        writeReference(aMatrix, scilabStringFile);
    }

    @Test
    public void readJavaSerializedDoubleTest() throws IOException {

        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        ScilabDouble aMatrix = new ScilabDouble(a);
        assertTrue(aMatrix.equals(readFromFile(scilabDoubleFile)) == true);
    }

    @Test
    public void readJavaSerializedComplexTest() throws IOException {

        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] aImg = { { 210.2, 220.0, 420.0, 390.0 }, { 230.2, 240.0, 440.0, 400.0 } };

        ScilabDouble aMatrix = new ScilabDouble(a, aImg);
        assertTrue(aMatrix.equals(readFromFile(scilabDoubleComplexFile)) == true);
    }

    @Test
    public void readJavaSerializedStringTest() throws IOException {

        String[][] a = { { "This", "is", "my", "string" }, { "and", "I want to", "compare", " them" } };
        ScilabString aMatrix = new ScilabString(a);
        assertTrue(aMatrix.equals(readFromFile(scilabStringFile)));
    }

    private final String scilabInt8File = "Int8.data";
    private final String scilabInt8SignedFile = "Int8.signed.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt8FileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt8File);
    }

    @Test
    public void readJavaSerializedInt8Test() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assertTrue(aMatrix.equals(readFromFile(scilabInt8File)) == true);
    }

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt8SignedFileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt8SignedFile);
    }

    @Test
    public void readJavaSerializedInt8SignedTest() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assertTrue(aMatrix.equals(readFromFile(scilabInt8SignedFile)) == true);
    }

    private final String scilabInt16File = "Int16.data";
    private final String scilabInt16SignedFile = "Int16.signed.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt16FileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt16File);
    }

    @Test
    public void readJavaSerializedInt16Test() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assertTrue(aMatrix.equals(readFromFile(scilabInt16File)) == true);
    }

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt16SignedFileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt16SignedFile);
    }

    @Test
    public void readJavaSerializedInt16SignedTest() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assertTrue(aMatrix.equals(readFromFile(scilabInt16SignedFile)) == true);
    }

    private final String scilabInt32File = "Int32.data";
    private final String scilabInt32SignedFile = "Int32.signed.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt32FileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt32File);
    }

    @Test
    public void readJavaSerializedInt32Test() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assertTrue(aMatrix.equals(readFromFile(scilabInt32File)) == true);
    }

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt32SignedFileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt32SignedFile);
    }

    @Test
    public void readJavaSerializedInt32SignedTest() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assertTrue(aMatrix.equals(readFromFile(scilabInt32SignedFile)) == true);
    }

    private final String scilabInt64File = "Int64.data";
    private final String scilabInt64SignedFile = "Int64.signed.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt64FileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt64File);
    }

    @Test
    public void readJavaSerializedInt64Test() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assertTrue(aMatrix.equals(readFromFile(scilabInt64File)) == true);
    }

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeInt64SignedFileReference() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt64SignedFile);
    }

    @Test
    public void readJavaSerializedInt64SignedTest() throws IOException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assertTrue(aMatrix.equals(readFromFile(scilabInt64SignedFile)) == true);
    }

    private final String scilabBooleanFile = "Boolean.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeBooleanFileReference() throws IOException {
        boolean[][] a = { { true, false, true }, { true, true, true } };
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        writeReference(aMatrix, scilabBooleanFile);
    }

    @Test
    public void readJavaSerializedBooleanTest() throws IOException {
        boolean[][] a = { { true, false, true }, { true, true, true } };
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        assertTrue(aMatrix.equals(readFromFile(scilabBooleanFile)) == true);
    }

    private final String scilabListFile = "List.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeListFileReference() throws IOException {
        ScilabList data = new ScilabList();
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));

        data.add(new ScilabString("hello"));
        writeReference(data, scilabListFile);
    }

    @Test
    public void readJavaSerializedListTest() throws IOException {
        ScilabList data = new ScilabList();
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));
        data.add(new ScilabString("hello"));
        assertTrue(data.equals(readFromFile(scilabListFile)) == true);
    }

    private final String scilabTListFile = "TList.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeTListFileReference() throws IOException {
        ScilabTList data = new ScilabTList();
        data.add(new ScilabString("hello"));
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));
        writeReference(data, scilabTListFile);
    }

    @Test
    public void readJavaSerializedTListTest() throws IOException {
        ScilabTList data = new ScilabTList();
        data.add(new ScilabString("hello"));
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));

        assertTrue(data.equals(readFromFile(scilabTListFile)));
    }

    private final String scilabMListFile = "MList.data";

    // Uncomment to regenerate a new reference file
    // @org.junit.Before
    public void writeMListFileReference() throws IOException {
        ScilabMList data = new ScilabMList();
        data.add(new ScilabString("hello"));
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));
        writeReference(data, scilabMListFile);
    }

    @Test
    public void readJavaSerializedMListTest() throws IOException {
        ScilabMList data = new ScilabMList();
        data.add(new ScilabString("hello"));
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));
        assertTrue(data.equals(readFromFile(scilabMListFile)) == true);
    }
}
