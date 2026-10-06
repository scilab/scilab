/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2023 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __ARCHIVE_UTILS_HXX__
#define __ARCHIVE_UTILS_HXX__

#include <map>
#include "function.hxx"

extern "C"
{
    #include "charEncoding.h"
    #include "localization.h"
    #include "Scierror.h"
    #include "Sciwarning.h"

    #include <archive.h>
    #include <archive_entry.h>
}

int fillArchiveOpt(struct archive* archive, types::optional_list& opt, const char* fname, const wchar_t* filename = nullptr);

void cleanup(struct archive* ar, struct archive* disk = nullptr, struct archive_entry* entry = nullptr);
bool check_error(int res, struct archive* ar, const char* fname);

static std::map<std::wstring, int> format = {
    {L"tar", ARCHIVE_FORMAT_TAR},
    {L"tar_ustar", ARCHIVE_FORMAT_TAR_USTAR},
    {L"tar_pax_interchange", ARCHIVE_FORMAT_TAR_PAX_INTERCHANGE},
    {L"tar_pax_restricted", ARCHIVE_FORMAT_TAR_PAX_RESTRICTED},
    {L"tar_gnutar", ARCHIVE_FORMAT_TAR_GNUTAR},
    {L"zip", ARCHIVE_FORMAT_ZIP},
    {L"7zip", ARCHIVE_FORMAT_7ZIP},
    {L"raw", ARCHIVE_FORMAT_RAW}
    // {L"empty", ARCHIVE_FORMAT_EMPTY},
    // {L"base_mask", ARCHIVE_FORMAT_BASE_MASK},
    // {L"cpio", ARCHIVE_FORMAT_CPIO},
    // {L"cpio_posix", ARCHIVE_FORMAT_CPIO_POSIX},
    // {L"cpio_bin_le", ARCHIVE_FORMAT_CPIO_BIN_LE},
    // {L"cpio_bin_be", ARCHIVE_FORMAT_CPIO_BIN_BE},
    // {L"cpio_svr4_nocrc", ARCHIVE_FORMAT_CPIO_SVR4_NOCRC},
    // {L"cpio_svr4_crc", ARCHIVE_FORMAT_CPIO_SVR4_CRC},
    // {L"cpio_afio_large", ARCHIVE_FORMAT_CPIO_AFIO_LARGE},
    // {L"cpio_pwb", ARCHIVE_FORMAT_CPIO_PWB},
    // {L"shar", ARCHIVE_FORMAT_SHAR},
    // {L"shar_base", ARCHIVE_FORMAT_SHAR_BASE},
    // {L"shar_dump", ARCHIVE_FORMAT_SHAR_DUMP},
    // {L"iso9660", ARCHIVE_FORMAT_ISO9660},
    // {L"iso9660_rockridge", ARCHIVE_FORMAT_ISO9660_ROCKRIDGE},
    // {L"ar", ARCHIVE_FORMAT_AR},
    // {L"ar_gnu", ARCHIVE_FORMAT_AR_GNU},
    // {L"ar_bsd", ARCHIVE_FORMAT_AR_BSD},
    // {L"mtree", ARCHIVE_FORMAT_MTREE},
    // {L"xar", ARCHIVE_FORMAT_XAR},
    // {L"lha", ARCHIVE_FORMAT_LHA},
    // {L"cab", ARCHIVE_FORMAT_CAB},
    // {L"rar", ARCHIVE_FORMAT_RAR},
    // {L"warc", ARCHIVE_FORMAT_WARC},
    // {L"rar_v5", ARCHIVE_FORMAT_RAR_V5}
};

static std::map<std::wstring, int> compression = {
    {L"none", ARCHIVE_FILTER_NONE},
    {L"gzip", ARCHIVE_FILTER_GZIP},
    {L"lzma", ARCHIVE_FILTER_LZMA},
    {L"xz", ARCHIVE_FILTER_XZ},
    {L"zstd", ARCHIVE_FILTER_ZSTD}
    // {L"bzip2", ARCHIVE_FILTER_BZIP2},
    // {L"compress", ARCHIVE_FILTER_COMPRESS},
    // {L"program", ARCHIVE_FILTER_PROGRAM},
    // {L"uu", ARCHIVE_FILTER_UU},
    // {L"rpm", ARCHIVE_FILTER_RPM},
    // {L"lzip", ARCHIVE_FILTER_LZIP},
    // {L"lrzip", ARCHIVE_FILTER_LRZIP},
    // {L"lzop", ARCHIVE_FILTER_LZOP},
    // {L"grzip", ARCHIVE_FILTER_GRZIP},
    // {L"lz4", ARCHIVE_FILTER_LZ4},
    // {L"zstd", ARCHIVE_FILTER_ZSTD}
};
#endif /* __ARCHIVE_UTILS_HXX__ */
