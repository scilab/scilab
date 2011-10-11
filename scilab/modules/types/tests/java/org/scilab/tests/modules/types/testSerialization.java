/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.tests.modules.types;

import org.testng.annotations.*;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabTypeEnum;

public class testSerialization {

    @BeforeTest
    public void loadLibrary() {
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

        ObjectOutputStream oos= new ObjectOutputStream(fos);
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

            ObjectInputStream ois= new ObjectInputStream(fis);
            try {
                a = (ScilabType) ois.readObject();
            } finally {
                try {
                    ois.close();
                } finally {
                    fis.close();
                }
            }
        } catch(IOException ioe) {
            ioe.printStackTrace();
        } catch(ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
        return a;

    }

    private String scilabDoubleFile="double.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeDoubleFileReference() throws IOException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aMatrix = new ScilabDouble(a);
        writeReference(aMatrix, scilabDoubleFile);
    }

    private String scilabDoubleComplexFile="double.complex.data";
// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeDoubleComplexFileReference() throws IOException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]aImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};
        ScilabDouble aMatrix = new ScilabDouble(a,aImg);
        writeReference(aMatrix, scilabDoubleComplexFile);
    }

    private String scilabStringFile="string.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeStringFileReference() throws IOException {
        String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
        ScilabString aMatrix = new ScilabString(a);
        writeReference(aMatrix, scilabStringFile);
    }


    @Test
    public void readJavaSerializedDoubleTest() throws IOException {

        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aMatrix = new ScilabDouble(a);
        assert aMatrix.equals(readFromFile(scilabDoubleFile)) == true;
    }

    @Test
    public void readJavaSerializedComplexTest() throws IOException {

        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]aImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};

        ScilabDouble aMatrix = new ScilabDouble(a, aImg);
        assert aMatrix.equals(readFromFile(scilabDoubleComplexFile)) == true;
    }


    @Test
    public void readJavaSerializedStringTest() throws IOException {

        String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
        ScilabString aMatrix = new ScilabString(a);
        assert aMatrix.equals(readFromFile(scilabStringFile)) == true;
    }

    private String scilabInt8File="Int8.data";
    private String scilabInt8SignedFile="Int8.signed.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt8FileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt8File);
    }


    @Test
    public void readJavaSerializedInt8Test() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assert aMatrix.equals(readFromFile(scilabInt8File)) == true;
    }

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt8SignedFileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt8SignedFile);
    }


    @Test
    public void readJavaSerializedInt8SignedTest() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assert aMatrix.equals(readFromFile(scilabInt8SignedFile)) == true;
    }

    private String scilabInt16File="Int16.data";
    private String scilabInt16SignedFile="Int16.signed.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt16FileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt16File);
    }


    @Test
    public void readJavaSerializedInt16Test() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assert aMatrix.equals(readFromFile(scilabInt16File)) == true;
    }

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt16SignedFileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt16SignedFile);
    }


    @Test
    public void readJavaSerializedInt16SignedTest() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assert aMatrix.equals(readFromFile(scilabInt16SignedFile)) == true;
    }


    private String scilabInt32File="Int32.data";
    private String scilabInt32SignedFile="Int32.signed.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt32FileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt32File);
    }


    @Test
    public void readJavaSerializedInt32Test() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assert aMatrix.equals(readFromFile(scilabInt32File)) == true;
    }

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt32SignedFileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt32SignedFile);
    }


    @Test
    public void readJavaSerializedInt32SignedTest() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assert aMatrix.equals(readFromFile(scilabInt32SignedFile)) == true;
    }

    private String scilabInt64File="Int64.data";
    private String scilabInt64SignedFile="Int64.signed.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt64FileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        writeReference(aMatrix, scilabInt64File);
    }


    @Test
    public void readJavaSerializedInt64Test() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        assert aMatrix.equals(readFromFile(scilabInt64File)) == true;
    }

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeInt64SignedFileReference() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        writeReference(aMatrix, scilabInt64SignedFile);
    }


    @Test
    public void readJavaSerializedInt64SignedTest() throws IOException {
        byte [][]a={{32,42,41}, {12,13,32}};
        ScilabInteger aMatrix = new ScilabInteger(a, false);
        assert aMatrix.equals(readFromFile(scilabInt64SignedFile)) == true;
    }
    private String scilabBooleanFile="Boolean.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
    public void writeBooleanFileReference() throws IOException {
        boolean [][]a={{true,false,true}, {true,true,true}};
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        writeReference(aMatrix, scilabBooleanFile);
    }


    @Test
    public void readJavaSerializedBooleanTest() throws IOException {
        boolean [][]a={{true,false,true}, {true,true,true}};
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        assert aMatrix.equals(readFromFile(scilabBooleanFile)) == true;
    }

    private String scilabListFile="List.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
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
        assert data.equals(readFromFile(scilabListFile)) == true;
    }

    private String scilabTListFile="TList.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
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

        assert data.equals(readFromFile(scilabTListFile)) == true;
    }
    private String scilabMListFile="MList.data";

// Uncomment to regenerate a new reference file
    //@BeforeMethod
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
        assert data.equals(readFromFile(scilabMListFile)) == true;
    }

}

