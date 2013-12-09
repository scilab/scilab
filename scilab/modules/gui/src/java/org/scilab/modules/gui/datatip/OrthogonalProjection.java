/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import java.util.ArrayList;

/**
 * Find the minimum distance between a point and a polyline
 * @author Gustavo Barbosa Libotte
 */
public class OrthogonalProjection {

    static int i, j, k;

    /**
    * Computes the orthogonal projection of a point in a gives polyline
    *
    * @param polylineData Polyline data points
    * @param pt the point (x, y) in graphic coordinates
    * @return projection coefficient and index of minimum distace point
    */
    public static double[] orthogonalProj(double[][] polylineData, double[] pt) {

        double[][] X = new double[(polylineData.length) - 1][polylineData[0].length];
        for (i = 1 ; i < polylineData.length ; i++) {
            for (j = 0 ; j < polylineData[0].length ; j++) {
                X[i - 1][j] = polylineData[i][j] - polylineData[i - 1][j];
            }
        }

        double[][] Y = new double[(polylineData.length) - 1][polylineData[0].length];
        for (i = 1 ; i < polylineData.length ; i++) {
            for (j = 0 ; j < polylineData[0].length ; j++) {
                if (j != 2) {
                    Y[i - 1][j] = pt[j] - polylineData[i - 1][j];
                } else {
                    Y[i - 1][j] = 0 - polylineData[i - 1][j];
                }
            }
        }

        double[] L = new double[X.length];
        for (i = 0 ; i < X.length ; i++) {
            L[i] = (Math.pow(X[i][0], 2)) + (Math.pow(X[i][1], 2));
        }

        ArrayList<Integer> nz = new ArrayList<Integer>();
        for (i = 0 ; i < L.length ; i++) {
            if (L[i] > 0) {
                nz.add(i);
            }
        }

        double[][] newX = new double[nz.size()][3];
        double[][] newY = new double[nz.size()][3];
        for (i = 0 ; i < nz.size() ; i++) {
            for (j = 0 ; j < 3 ; j++) {
                newX[i][j] = X[nz.get(i)][j];
                newY[i][j] = Y[nz.get(i)][j];
            }
        }

        double[] P = new double[newX.length];
        for (i = 0 ; i < P.length ; i++) {
            P[i] = ((newX[i][0] * newY[i][0]) + (newX[i][1] * newY[i][1])) / L[i];
        }

        ArrayList<Integer> i_in = new ArrayList<Integer>();
        for (i = 0 ; i < P.length ; i++) {
            if (P[i] >= 0 && P[i] < 1) {
                i_in.add(i);
            }
        }

        if (i_in.size() != 0) {
            double[] sum_XPY = new double[i_in.size()];
            for (i = 0 ; i < sum_XPY.length ; i++) {
                sum_XPY[i] = Math.pow(((newX[i_in.get(i)][0] * P[i_in.get(i)]) - newY[i_in.get(i)][0]), 2) +
                             Math.pow(((newX[i_in.get(i)][1] * P[i_in.get(i)]) - newY[i_in.get(i)][1]), 2);
            }

            double d = sum_XPY[0];
            for (i = 0 ; i < sum_XPY.length ; i++) {
                if (sum_XPY[i] <= d) {
                    d = sum_XPY[i];
                    k = i;
                }
            }

            d = Math.sqrt(d);
            int i_in_k = i_in.get(k);
            double c = P[i_in_k];
            int ind = nz.get(i_in_k);

            return new double[] {ind, c};

        } else {

            return new double[] {1.0, 1.0};

        }
    }
}