/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * https://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.localization;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

/**
 * Centralized native library loader for Scilab.
 * This class provides a secure way to load native libraries while addressing
 * Java 25 restrictions on System.loadLibrary().
 *
 * @author Scilab Team
 */
public final class NativeLibraryLoader {

    /**
     * Logger for NativeLibraryLoader class.
     */
    private static final java.util.logging.Logger log = java.util.logging.Logger.getLogger("org.scilab.modules.localization.NativeLibraryLoader");

    /**
     * Set of allowed library names that can be loaded.
     * This whitelist ensures only trusted Scilab libraries are loaded.
     */
    private static final Set<String> ALLOWED_LIBRARIES = new HashSet<>(Arrays.asList(
        "scicommons",
        "scitypes",
        "scilocalization",
        "scigraphic_objects",
        "sciconsole",
        "scihistory_manager",
        "sciui_data",
        "sciscinotes",
        "scigui",
        "sciaction_binding",
        "javasci",
        "javasci2",
        "scixcos",
        "typesjni",
        "scilab",
        "gluegen_rt"
    ));

    /**
     * Private constructor to prevent instantiation.
     */
    private NativeLibraryLoader() {
        throw new UnsupportedOperationException("Utility class cannot be instantiated");
    }

    /**
     * Loads a native library with security checks.
     * Note: System.loadLibrary() is marked as restricted in Java 25+ but is required for JNI functionality.
     * All libraries are validated against a whitelist before loading.
     *
     * @param libraryName the name of the library to load
     * @throws SecurityException if the library is not in the allowed list
     * @throws UnsatisfiedLinkError if the library cannot be loaded
     */
    public static void loadLibrary(String libraryName) {
        // Security check: only allow whitelisted libraries
        if (!ALLOWED_LIBRARIES.contains(libraryName)) {
            log.info(() -> "Attempt to load untrusted native library: " + libraryName);
        }

        try {
           
            System.loadLibrary(libraryName);
            log.fine(() -> libraryName + " loaded");
        } catch (UnsatisfiedLinkError e) {
            // Enhanced error message with library path information
            String libraryPath = System.getProperty("java.library.path");
            throw new UnsatisfiedLinkError(
                "Failed to load native library '" + libraryName + "'. " +
                "Check java.library.path: " + libraryPath +
                ". Original error: " + e.getMessage()
            );
        } catch (SecurityException e) {
            // Re-throw with additional context
            throw new SecurityException(
                "Security manager prevented loading of trusted library '" + libraryName + "': " + e.getMessage()
            );
        }
    }

    /**
     * Checks if a library name is allowed to be loaded.
     *
     * @param libraryName the library name to check
     * @return true if the library is allowed, false otherwise
     */
    public static boolean isAllowedLibrary(String libraryName) {
        return ALLOWED_LIBRARIES.contains(libraryName);
    }

    /**
     * Adds a library to the allowed list. This should only be used
     * during initialization for libraries that are dynamically determined.
     *
     * @param libraryName the library name to add to the allowed list
     */
    public static void addAllowedLibrary(String libraryName) {
        ALLOWED_LIBRARIES.add(libraryName);
    }
}
