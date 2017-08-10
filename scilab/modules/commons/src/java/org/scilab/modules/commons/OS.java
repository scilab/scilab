/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Clement DAVID
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
