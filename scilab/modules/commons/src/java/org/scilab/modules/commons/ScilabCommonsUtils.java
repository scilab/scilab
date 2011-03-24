/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Basic utils
 * @author Calixte DENIZET
 */
public final class ScilabCommonsUtils {

    private static MessageDigest MD5;
    static {
        try {
            MD5 = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            System.err.println(e);
            MD5 = null;
        }
    }

    /**
     * Get a MD5 string of the input string
     * @param str the string to convert
     * @return the MD5 representation of str
     */
    public static String getMD5(String str) {
        if (MD5 == null) {
            return null;
        }

        byte[] bytes = MD5.digest(str.getBytes());
        return new BigInteger(1, bytes).toString(16);
    }

}
