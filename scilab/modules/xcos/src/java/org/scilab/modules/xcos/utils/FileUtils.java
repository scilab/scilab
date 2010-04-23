/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;

import org.apache.commons.logging.LogFactory;

/**
 * Contains useful method for managing files.
 */
public final class FileUtils {
	/**
	 * Default constructor
	 */
	private FileUtils() {
	}

	/**
	 * Copy in to out
	 * 
	 * @param in
	 *            the input file
	 * @param out
	 *            the output file
	 * @throws IOException
	 *             when an errors has occured
	 */
	public static void copy(File in, File out) throws IOException {
		FileChannel inChannel = new FileInputStream(in).getChannel();
		FileChannel outChannel = new FileOutputStream(out).getChannel();
		try {
			inChannel.transferTo(0, inChannel.size(), outChannel);
		} catch (IOException e) {
			LogFactory.getLog(FileUtils.class).warn(e);
			throw e;
		} finally {
			if (inChannel != null) {
				inChannel.close();
			}
			if (outChannel != null) {
				outChannel.close();
			}
		}
	}

	/**
	 * Force the copy from in to out
	 * 
	 * @param in
	 *            the input file
	 * @param out
	 *            the output file
	 */
	public static void forceCopy(File in, File out) {
		FileChannel inChannel = null;
		FileChannel outChannel = null;
		
		if (!out.exists()) {
			try {
				out.createNewFile();
			} catch (IOException e) {
				LogFactory.getLog(FileUtils.class).warn(e);
			}
		}
		
		try {
			inChannel = new FileInputStream(in).getChannel();
			outChannel = new FileOutputStream(out).getChannel();
			inChannel.transferTo(0, inChannel.size(), outChannel);
		} catch (IOException e) {
			LogFactory.getLog(FileUtils.class).warn(e);
		} finally {
			if (inChannel != null) {
				try {
					inChannel.close();
				} catch (IOException e) {
					LogFactory.getLog(FileUtils.class).warn(e);
				}
			}
			if (outChannel != null) {
				try {
					outChannel.close();
				} catch (IOException e) {
					LogFactory.getLog(FileUtils.class).warn(e);
				}
			}
		}
	}
	
	/**
	 * Create a temporary file and return it
	 * @return a new unique temporary file.
	 * @throws IOException when an error occurs
	 */
	public static File createTempFile() throws IOException {
		return File.createTempFile(XcosFileType.XCOS.getExtension(),
				XcosFileType.HDF5.getDottedExtension(), XcosConstants.TMPDIR);
	}
}
