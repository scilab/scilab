/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io.scicos;

import java.util.List;

import org.scilab.modules.localization.Messages;

/**
 * Default exception for a Xcos - Scicos communication
 */
public abstract class ScicosFormatException extends Exception {
    private static final String UNABLE_TO_DECODE_INVALID_DATA = Messages
            .gettext("Unable to decode \"%s\" : invalid data.");
    private static final String UNABLE_TO_DECODE_INVALID_FIELD = Messages
            .gettext("Unable to decode \"%s.%s\" : invalid field.");

    /**
     * Used when the {@link Element} cannot be used to decode/encode the
     * instance.
     */
    @SuppressWarnings(value = { "serial" })
    public static class WrongElementException extends ScicosFormatException {

        /**
         * Default constructor
         */
        public WrongElementException() {
            super();

            printStackTrace();
        }
    }

    /**
     * Used when the expected data cannot be casted safely.
    @SuppressWarnings(value = { "serial" })
     */
    public static class WrongTypeException extends ScicosFormatException {

        /**
         * Default cstr
         */
        public WrongTypeException() {
        }

        /**
         * @param message
         *            the message
         * @param cause
         *            the cause
         */
        public WrongTypeException(String message, Throwable cause) {
            super(message, cause);
        }

        /**
         * @param cause
         *            the cause
         */
        public WrongTypeException(Throwable cause) {
            super(cause);
        }

        /**
         * Constructor with field.
         *
         * @param fields
         *            the field list
         * @param index
         *            the buggy index
         */
        public WrongTypeException(List<String> fields, int index) {
            super(String.format(UNABLE_TO_DECODE_INVALID_FIELD, fields.get(0),
                                fields.get(index)));
        }
    }

    /**
    @SuppressWarnings(value = { "serial" })
     * Used when the expected data are not well formatted.
     */
    public static class WrongStructureException extends ScicosFormatException {

        /**
         * Default constructor
         */
        public WrongStructureException() {
        }

        /**
         * Constructor with a field.
         *
         * @param field
         *            the erroneous field
         */
        public WrongStructureException(String field) {
            super(String.format(UNABLE_TO_DECODE_INVALID_DATA, field));
        }

        /**
         * Constructor with field descriptor.
         *
         * @param fields
         *            the erroneous fields
         */
        public WrongStructureException(List<String> fields) {
            super(String.format(UNABLE_TO_DECODE_INVALID_DATA, fields.get(0)));
        }
    }

    @SuppressWarnings(value = { "serial" })
    /**
     * Used when the data doesn't have a valid version number.
     */
    public static class VersionMismatchException extends ScicosFormatException {
        private final String wrongVersion;

        /**
         * Default constructor
         *
         * @param wrongVersion
         *            the wrong version number.
         */
        public VersionMismatchException(String wrongVersion) {
            this.wrongVersion = wrongVersion;
        }

        /**
         * @return the wrong version number
         */
        public String getWrongVersion() {
            return wrongVersion;
        }
    }

    /**
     * Default constructor
     */
    protected ScicosFormatException() {
        if (!(this instanceof VersionMismatchException)) {
            printStackTrace();
        }
    }

    /**
     * @param message
     *            the message
     */
    public ScicosFormatException(String message) {
        super(message);
    }

    /**
     * {@link ScicosFormatException} with message and cause.
     *
     * @param message
     *            the message to be printed
     * @param cause
     *            the cause
     */
    public ScicosFormatException(String message, Throwable cause) {
        super(message, cause);
    }

    /**
     * {@link ScicosFormatException} with cause.
     *
     * @param cause
     *            the cause
     */
    public ScicosFormatException(Throwable cause) {
        super(cause);
    }
}
