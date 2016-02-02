/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.types;

import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

/**
 * Class to handle the Scilab data retrievment and conversion to a ScilabType
 * object. All the functions sendFoo are used from C++ and should not be used on
 * the Java side.
 *
 * An handler which implements ScilabVariablesHandler must be used to achieve
 * the retrievment. The id returned by addScilabVariablesHandler must be passed
 * from C/C++ to functions in the class org_modules_types::ScilabToJava to
 * guarantee that the correct handler would receive its datas.
 */
public final class ScilabVariables {

    private static final Map<Thread, ArrayList<ScilabType>> lists = new HashMap<Thread, ArrayList<ScilabType>>();
    private static final Vector<ScilabVariablesHandler> handlers = new Vector<ScilabVariablesHandler>();

    /**
     * Register a new handler
     *
     * @param handler
     *            the handler
     * @return the id to use from C/C++
     */
    public static final int addScilabVariablesHandler(ScilabVariablesHandler handler) {
        int i = 0;
        for (; i < handlers.size(); i++) {
            ScilabVariablesHandler h = handlers.get(i);
            if (h == handler) {
                return i;
            } else if (h == null) {
                handlers.set(i, handler);
                return i;
            }
        }
        handlers.add(handler);

        return i;
    }

    /**
     * Unregister an handler (the id must be considered as lost !!)
     *
     * @param handler
     *            the handler to remove
     */
    public static final void removeScilabVariablesHandler(ScilabVariablesHandler handler) {
        if (handler != null) {
            for (int i = 0; i < handlers.size(); i++) {
                if (handlers.get(i) == handler) {
                    handlers.set(i, null);
                }
            }
        }
    }

    /**
     * Unregister an handler with a given id (the id must be considered as lost
     * !!)
     *
     * @param id
     *            the handler id to remove
     */
    public static final void removeScilabVariablesHandler(int id) {
        if (id >= 0 && id < handlers.size()) {
            handlers.set(id, null);
        }
    }

    /**
     * Send double matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, double[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabDouble(null, data, null, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabDouble(varName, data, null, swaped));
        }
    }

    /**
     * Send double matrix as DoubleBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendDataAsBuffer(String varName, int[] indexes, DoubleBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabDoubleReference(null, data, null, rows, cols));
        } else {
            handlers.get(handlerId).handle(new ScilabDoubleReference(varName, data, null, rows, cols));
        }
    }

    /**
     * Send complex matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param real
     *            the real data
     * @param img
     *            the imaginary data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, double[][] real, double[][] img, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabDouble(null, real, img, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabDouble(varName, real, img, swaped));
        }
    }

    /**
     * Send complex matrix as DoubleBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendDataAsBuffer(String varName, int[] indexes, DoubleBuffer real, DoubleBuffer imag, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabDoubleReference(null, real, imag, rows, cols));
        } else {
            handlers.get(handlerId).handle(new ScilabDoubleReference(varName, real, imag, rows, cols));
        }
    }

    /**
     * Send int32 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send int32 matrix as IntBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendDataAsBuffer(String varName, int[] indexes, IntBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, false));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, false));
        }
    }

    /**
     * Send uint16 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedData(String varName, int[] indexes, short[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, true, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, true, swaped));
        }
    }

    /**
     * Send uint16 matrix as ShortBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedDataAsBuffer(String varName, int[] indexes, ShortBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, true));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, true));
        }
    }

    /**
     * Send int16 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, short[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send int16 matrix as ShortBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendDataAsBuffer(String varName, int[] indexes, ShortBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, false));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, false));
        }
    }

    /**
     * Send uint8 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedData(String varName, int[] indexes, byte[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, true, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, true, swaped));
        }
    }

    /**
     * Send uint8 matrix as ByteBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedDataAsBuffer(String varName, int[] indexes, ByteBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, true));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, true));
        }
    }

    /**
     * Send boolean matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, boolean[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabBoolean(null, data, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabBoolean(varName, data, swaped));
        }
    }

    /**
     * Send boolean matrix as reference
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendBooleanDataAsBuffer(String varName, int[] indexes, IntBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabBooleanReference(null, data, rows, cols));
        } else {
            handlers.get(handlerId).handle(new ScilabBooleanReference(varName, data, rows, cols));
        }
    }

    /**
     * Send int8 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, byte[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send int8 matrix as ByteBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendDataAsBuffer(String varName, int[] indexes, ByteBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, false));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, false));
        }
    }

    /**
     * Send int64 or uint64 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, long[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send int64 or uint64 matrix as LongBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendDataAsBuffer(String varName, int[] indexes, LongBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, false));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, false));
        }
    }

    /**
     * Send uint32 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedData(String varName, int[] indexes, int[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, true, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, true, swaped));
        }
    }

    /**
     * Send uint32 matrix as IntBuffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedDataAsBuffer(String varName, int[] indexes, IntBuffer data, int rows, int cols, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabIntegerReference(null, data, rows, cols, true));
        } else {
            handlers.get(handlerId).handle(new ScilabIntegerReference(varName, data, rows, cols, true));
        }
    }

    /**
     * Send string matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, String[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabString(null, data, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabString(varName, data, swaped));
        }
    }

    /**
     * Send double sparse matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of non null elements
     * @param nbItemRow
     *            the number by row of non null elements
     * @param colPos
     *            the column position of the non null elements
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int row, int col, int nbItem, int[] nbItemRow, int[] colPos, double[] data, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabSparse(null, row, col, nbItem, nbItemRow, colPos, data, null));
        } else {
            handlers.get(handlerId).handle(new ScilabSparse(varName, row, col, nbItem, nbItemRow, colPos, data, null));
        }
    }

    /**
     * Send double sparse matrix as buffer
     * Useless for now due to Scilab limitations (A=sparse(...);A(1,1)=123; a new A is created so references are lost)
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of non null elements
     * @param nbItemRow
     *            the number by row of non null elements
     * @param colPos
     *            the column position of the non null elements
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    /*public static final void sendDataAsBuffer(String varName, int[] indexes, int row, int col, int nbItem, IntBuffer nbItemRow, IntBuffer colPos, DoubleBuffer data, int handlerId) {
      if (indexes.length != 0) {
      addElement(indexes, new ScilabSparseReference(null, row, col, nbItem, nbItemRow, colPos, data, null));
      } else {
      handlers.get(handlerId).handle(new ScilabSparseReference(varName, row, col, nbItem, nbItemRow, colPos, data, null));
      }
      }*/

    /**
     * Send complex sparse matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of non null elements
     * @param nbItemRow
     *            the number by row of non null elements
     * @param colPos
     *            the column position of the non null elements
     * @param real
     *            the real data
     * @param imag
     *            the imaginary data
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int row, int col, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabSparse(null, row, col, nbItem, nbItemRow, colPos, real, imag));
        } else {
            handlers.get(handlerId).handle(new ScilabSparse(varName, row, col, nbItem, nbItemRow, colPos, real, imag));
        }
    }

    /**
     * Send complex sparse matrix as buffer
     * Useless for now due to Scilab limitations (A=sparse(...);A(1,1)=123; a new A is created so references are lost)
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of non null elements
     * @param nbItemRow
     *            the number by row of non null elements
     * @param colPos
     *            the column position of the non null elements
     * @param real
     *            the real data
     * @param imag
     *            the imaginary data
     * @param handlerId
     *            the handler id
     */
    /*private static final void sendDataAsBuffer(String varName, int[] indexes, int row, int col, int nbItem, IntBuffer nbItemRow, IntBuffer colPos, DoubleBuffer real, DoubleBuffer imag, int handlerId) {
      if (indexes.length != 0) {
      addElement(indexes, new ScilabSparse(null, row, col, nbItem, nbItemRow, colPos, real, imag));
      } else {
      handlers.get(handlerId).handle(new ScilabSparse(varName, row, col, nbItem, nbItemRow, colPos, real, imag));
      }
      }*/

    /**
     * Send boolean sparse matrix
     * Useless for now due to Scilab limitations (A=sparse(...);A(1,1)=123; a new A is created so references are lost)
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of true elements
     * @param nbItemRow
     *            the number by row of true elements
     * @param colPos
     *            the column position of the true elements
     * @param handlerId
     *            the handler id
     */
    private static final void sendData(String varName, int[] indexes, int row, int col, int nbItem, int[] nbItemRow, int[] colPos, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabBooleanSparse(null, row, col, nbItem, nbItemRow, colPos));
        } else {
            handlers.get(handlerId).handle(new ScilabBooleanSparse(varName, row, col, nbItem, nbItemRow, colPos));
        }
    }

    /**
     * Send boolean sparse matrix as Buffer
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of true elements
     * @param nbItemRow
     *            the number by row of true elements
     * @param colPos
     *            the column position of the true elements
     * @param handlerId
     *            the handler id
     */
    /*public static final void sendDataAsBuffer(String varName, int[] indexes, int row, int col, int nbItem, IntBuffer nbItemRow, IntBuffer colPos, int handlerId) {
      if (indexes.length != 0) {
      addElement(indexes, new ScilabBooleanSparse(null, row, col, nbItem, nbItemRow, colPos));
      } else {
      handlers.get(handlerId).handle(new ScilabBooleanSparse(varName, row, col, nbItem, nbItemRow, colPos));
      }
      }*/

    /**
     * Send double polynomial matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param polyVarName
     *            the polynomial variable name
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendPolynomial(String varName, int[] indexes, String polyVarName, double[][][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabPolynomial(null, polyVarName, data, null, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabPolynomial(varName, polyVarName, data, null, swaped));
        }
    }

    /**
     * Send complex polynomial matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param polyVarName
     *            the polynomial variable name
     * @param real
     *            the real data
     * @param img
     *            the imaginary data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendPolynomial(String varName, int[] indexes, String polyVarName, double[][][] real, double[][][] img, boolean swaped,
                                            int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabPolynomial(null, polyVarName, real, img, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabPolynomial(varName, polyVarName, real, img, swaped));
        }
    }

    /**
     * Send list, tlist and mlist
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param type
     *            a char which must take the values 'l' for list or 'm' for
     *            mlist or 't' for tlist
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int nbItems, int[] indexes, char type, int handlerId) {
        String name = null;
        if (indexes.length == 0) {
            name = varName;
        }

        ScilabType var = null;
        switch (type) {
            case 'l':
                var = new ScilabList(name, nbItems);
                break;
            case 'm':
                var = new ScilabMList(name, nbItems);
                break;
            case 't':
                var = new ScilabTList(name, nbItems);
                break;
        }

        if (indexes.length == 0) {
            lists.put(Thread.currentThread(), (ArrayList<ScilabType>) var);
        } else {
            addElement(indexes, var);
        }
    }

    /**
     * Call when the list filling is finished a * @param indexes an integer
     * array with the indexes of the (sub)*-list which will contain the data
     *
     * @param handlerId
     *            the handler id
     */
    public static final void closeList(int[] indexes, int handlerId) {
        Thread t = Thread.currentThread();
        ArrayList<ScilabType> var = lists.get(t);
        if (var != null && indexes.length == 0) {
            handlers.get(handlerId).handle((ScilabType) var);
            lists.remove(t);
        }
    }

    /**
     * Add an element to the list
     *
     * @param indexes
     *            the indexes where to put the variable
     * @param var
     *            the variable to put
     */
    private static final void addElement(int[] indexes, ScilabType data) {
        ArrayList<ScilabType> list = lists.get(Thread.currentThread());

        for (int i = 0; i < indexes.length - 1; i++) {
            list = (ArrayList<ScilabType>) list.get(indexes[i] - 1);
        }

        int n = indexes[indexes.length - 1] - 1;
        if (n < list.size()) {
            list.set(indexes[indexes.length - 1] - 1, data);
        } else {
            list.add(data);
        }
    }
}
