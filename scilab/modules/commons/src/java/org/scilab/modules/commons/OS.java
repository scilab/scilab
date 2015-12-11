/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.commons;

/**
 * Operating Systems utility class
 */
public enum OS {
    WINDOWS {
        /**
         * {@inheritDoc}
         */
        @Override
        public Object getVersion() {
            return Double.valueOf(System.getProperty("os.version"));
        }
    },
    MAC {
        /**
         * {@inheritDoc}
         */
        @Override
        public Object getVersion() {
            final String[] numbers = System.getProperty("os.version").split(
                "\\.");

            int[] ret = new int[numbers.length];
            for (int i = 0; i < numbers.length; i++) {
                ret[i] = Integer.parseInt(numbers[i]);
            }
            return ret;
        }
    },
    UNIX;

    /**
     * @return the OS type
     */
    public static OS get() {
        final String name = System.getProperty("os.name").toLowerCase();

        if (name.contains("windows")) {
            return OS.WINDOWS;
        } else if (name.contains("mac")) {
            return MAC;
        } else {
            return UNIX;
        }
    }

    /**
     * @return the OS version
     */
    public Object getVersion() {
        return null;
    }

    public static String getVersionName() {
        switch (get()) {
            case WINDOWS:
                return "Windows";
            case MAC:
                return "Mac";
            default:
                return "Linux";
        }
    }
}
