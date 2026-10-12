#!/bin/env bash

ARCH=$(cc -print-multiarch)

function usage() {
    echo "This script compiles dependencies of Scilab for Linux $ARCH."
    echo
    echo "Syntax : $0 <dependency> with dependency equal to:"
    echo " - 'versions': display versions of dependencies,"
    echo " - 'download': download all dependencies,"
    echo " - 'all': compile all dependencies,"
    echo " - 'binary': configure dev-tools for binary version of Scilab,"
    echo " - 'jar': configure JARs for binary version of Scilab,"
    echo " - 'fromscratch': 'download' + 'all' + 'binary',"
    echo " - 'archive': Create prerequirements archive."
    echo
}

if test $# -eq 0; then
    usage
    exit 42
fi

# Generate needed variables
if [ -z "$BRANCH" ]; then
    BRANCH=$(git rev-parse --abbrev-ref HEAD)
    echo "BRANCH was not set, set to $BRANCH via git" >&2
fi
if [ -z "$CI_DEFAULT_BRANCH" ]; then
    CI_DEFAULT_BRANCH="main"
    echo "CI_DEFAULT_BRANCH was not set, set to $CI_DEFAULT_BRANCH" >&2
fi
if [ -z "$CI_COMMIT_SHORT_SHA" ]; then
    CI_COMMIT_SHORT_SHA=$(git rev-parse --short HEAD)
    echo "CI_COMMIT_SHORT_SHA was not set, set to $CI_COMMIT_SHORT_SHA via git" >&2
fi
if [ -z "$ARCH" ]; then
    ARCH=$(cc -print-multiarch)
    echo "ARCH was not set, set to $ARCH vi cc." >&2
fi
if [ -z "$SCI_VERSION_STRING" ]; then
    echo "SCI_VERSION_STRING was not set, set to scilab-branch-${BRANCH}-${CI_COMMIT_SHORT_SHA}" >&2
fi

echo "Scilab prerequirements for $ARCH in branch $BRANCH"

INSTALLROOTDIR=$(pwd)/install
INSTALLUSRDIR=$(pwd)/install/usr
DOWNLOADDIR=$(pwd)/downloads
BUILDDIR=$(pwd)/builds
LOGDIR=$(pwd)/$SCI_VERSION_STRING

echo
echo "INSTALLROOTDIR = $INSTALLROOTDIR"
echo "INSTALLUSRDIR  = $INSTALLUSRDIR"
echo "DOWNLOADDIR    = $DOWNLOADDIR"
echo "BUILDDIR       = $BUILDDIR"
echo "LOGDIR         = $LOGDIR"
echo

create_folders() {
    mkdir -p "$INSTALLROOTDIR"
    mkdir -p "$INSTALLROOTDIR/java"
    mkdir -p "$INSTALLROOTDIR/thirdparty"
    mkdir -p "$INSTALLROOTDIR/lib"
    mkdir -p "$INSTALLROOTDIR/lib/thirdparty"
    mkdir -p "$INSTALLROOTDIR/include"
    mkdir -p "$INSTALLROOTDIR/modules"
    mkdir -p "$INSTALLUSRDIR"
    mkdir -p "$INSTALLUSRDIR/lib"
    mkdir -p "$INSTALLUSRDIR/bin"
    mkdir -p "$INSTALLUSRDIR/include"
    mkdir -p "$DOWNLOADDIR"
    mkdir -p "$BUILDDIR"
    mkdir -p "$LOGDIR"
}

export LD_LIBRARY_PATH="/usr/local/lib:$INSTALLUSRDIR/lib:$LD_LIBRARY_PATH"
export PATH="/usr/local/bin:$INSTALLUSRDIR/bin:$PATH"

################################
##### DEPENDENCIES VERSION #####
################################
JDK_VERSION=21.0.11+10
JRE_VERSION=21.0.11_10
JAVAFX_VERSION=21.0.11
ANT_VERSION=1.10.17
OPENBLAS_VERSION=0.3.28
ARPACK_VERSION=3.9.1
CURL_VERSION=8.10.1
EIGEN_VERSION=5.0.1
FFTW_VERSION=3.3.3
HDF5_VERSION=1.14.4
NCURSES_VERSION=6.4
LIBXML2_VERSION=2.9.9
MATIO_VERSION=1.5.28
OPENSSL_VERSION=1.1.1c
PCRE2_VERSION=10.43
SUITESPARSE_VERSION=4.4.5
TCL_VERSION=8.5.15
TK_VERSION=8.5.15
BWIDGET_VERSION=1.9.16
ZLIB_VERSION=1.2.11
XZ_VERSION=5.4.4
JOGL_VERSION=2.5.0
JCEF_VERSION=d3de827+cef-146.0.10+g8219561+chromium-146.0.7680.179
LIBARCHIVE_VERSION=3.7.1
RAPIDJSON_VERSION=24b5e7a
LIBXSLT_VERSION=1.1.35
XLNT_VERSION=1.6.1
ARROW_VERSION=19.0.0
ARROW_BOOST_VERSION=1_81_0
ARROW_THRIFT_VERSION=0.20.0
ARROW_MIMALLOC_VERSION=2.0.6
ARROW_XSIMD_VERSION=13.0.0
ARROW_SNAPPY_VERSION=1.1.10
ARROW_ZSTD_VERSION=1.5.6

# CppServer and its deps
CPPSERVER_VERSION=1.0.6.0
ASIO_VERSION=1-38-1
CPPCOMMON_VERSION=1.0.6.0
FMT_VERSION=12.2.0
CPPCMAKESCRIPTS_VERSION=24ed6954308b

# Variables used by ant to build Java deps in Java 21
export JAVA_HOME="$BUILDDIR/java/jdk-$JDK_VERSION/"
export ANT_HOME="$INSTALLROOTDIR/java/ant"
export SOURCE_LEVEL=1.8
export TARGET_LEVEL=1.8
export TARGET_RT_JAR="$INSTALLROOTDIR/java/jre/lib/rt.jar"

make_versions() {
    echo "JDK_VERSION             = $JDK_VERSION"
    echo "JRE_VERSION             = $JRE_VERSION"
    echo "JAVAFX_VERSION          = $JAVAFX_VERSION"
    echo "ANT_VERSION             = $ANT_VERSION"
    echo "OPENBLAS_VERSION        = $OPENBLAS_VERSION"
    echo "ARPACK_VERSION          = $ARPACK_VERSION"
    echo "CURL_VERSION            = $CURL_VERSION"
    echo "EIGEN_VERSION           = $EIGEN_VERSION"
    echo "FFTW_VERSION            = $FFTW_VERSION"
    echo "HDF5_VERSION            = $HDF5_VERSION"
    echo "NCURSES_VERSION         = $NCURSES_VERSION"
    echo "LIBXML2_VERSION         = $LIBXML2_VERSION"
    echo "MATIO_VERSION           = $MATIO_VERSION"
    echo "OPENSSL_VERSION         = $OPENSSL_VERSION"
    echo "PCRE2_VERSION           = $PCRE2_VERSION"
    echo "SUITESPARSE_VERSION     = $SUITESPARSE_VERSION"
    echo "TCL_VERSION             = $TCL_VERSION"
    echo "TK_VERSION              = $TK_VERSION"
    echo "BWIDGET_VERSION         = $BWIDGET_VERSION"
    echo "ZLIB_VERSION            = $ZLIB_VERSION"
    echo "XZ_VERSION              = $XZ_VERSION"
    echo "JOGL_VERSION            = $JOGL_VERSION"
    echo "JCEF_VERSION           = $JCEF_VERSION"
    echo "LIBARCHIVE_VERSION      = $LIBARCHIVE_VERSION"
    echo "RAPIDJSON_VERSION       = $RAPIDJSON_VERSION"
    echo "LIBXSLT_VERSION         = $LIBXSLT_VERSION"
    echo "XLNT_VERSION            = $XLNT_VERSION"
    echo "ARROW_VERSION           = $ARROW_VERSION"
    echo "ARROW_BOOST_VERSION     = $ARROW_BOOST_VERSION"
    echo "ARROW_THRIFT_VERSION    = $ARROW_THRIFT_VERSION"
    echo "ARROW_MIMALLOC_VERSION  = $ARROW_MIMALLOC_VERSION"
    echo "ARROW_XSIMD_VERSION     = $ARROW_XSIMD_VERSION"
    echo "ARROW_SNAPPY_VERSION    = $ARROW_SNAPPY_VERSION"
    echo "ARROW_ZSTD_VERSION      = $ARROW_ZSTD_VERSION"
    echo "CPPSERVER_VERSION       = $CPPSERVER_VERSION"
    echo "CPPCOMMON_VERSION       = $CPPCOMMON_VERSION"
    echo "CPPCMAKESCRIPTS_VERSION = $CPPCMAKESCRIPTS_VERSION"
    echo "ASIO_VERSION            = $ASIO_VERSION"
    echo "FMT_VERSION             = $FMT_VERSION"
}

####################
##### DOWNLOAD #####
####################
download_dependencies() {
    cd "$DOWNLOADDIR" || exit 1

    [ ! -f jre-$JRE_VERSION.tar.gz ] && curl -L -o jre-$JRE_VERSION.tar.gz "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/OpenJDK21U-jre_x64_linux_hotspot_$(echo ${JRE_VERSION} |sed 's/-//g').tar.gz"
    [ ! -f jdk-$JDK_VERSION.tar.gz ] && curl -L -o jdk-$JDK_VERSION.tar.gz "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/ibm-semeru-open-jdk_x64_linux_$(echo ${JDK_VERSION} |sed 's/+/_/g')_openj9-0.59.0.tar.gz"
    [ ! -f openjfx-${JAVAFX_VERSION}_linux-x64_bin-sdk.zip ] && curl -L -o openjfx-${JAVAFX_VERSION}_linux-x64_bin-sdk.zip "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/openjfx-${JAVAFX_VERSION}_linux-x64_bin-sdk.zip"

    [ ! -f OpenBLAS-$OPENBLAS_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/OpenBLAS-$OPENBLAS_VERSION.tar.gz
    [ ! -f apache-ant-$ANT_VERSION-bin.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/apache-ant-$ANT_VERSION-bin.tar.gz
    [ ! -f arpack-ng-$ARPACK_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/arpack-ng-$ARPACK_VERSION.tar.gz
    [ ! -f curl-$CURL_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/curl-$CURL_VERSION.tar.gz
    [ ! -f eigen-$EIGEN_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/eigen-$EIGEN_VERSION.tar.gz
    [ ! -f fftw-$FFTW_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/fftw-$FFTW_VERSION.tar.gz
    [ ! -f hdf5-$HDF5_VERSION.zip ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/hdf5-$HDF5_VERSION.zip
    [ ! -f libxml2-$LIBXML2_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/libxml2-$LIBXML2_VERSION.tar.gz
    [ ! -f matio-$MATIO_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/matio-$MATIO_VERSION.tar.gz
    [ ! -f openssl-$OPENSSL_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/openssl-$OPENSSL_VERSION.tar.gz
    [ ! -f SuiteSparse-$SUITESPARSE_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/SuiteSparse-$SUITESPARSE_VERSION.tar.gz
    [ ! -f pcre2-$PCRE2_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/pcre2-$PCRE2_VERSION.tar.gz
    [ ! -f ncurses-$NCURSES_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/ncurses-$NCURSES_VERSION.tar.gz
    [ ! -f tcl$TCL_VERSION-src.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/tcl$TCL_VERSION-src.tar.gz
    [ ! -f tk$TK_VERSION-src.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/tk$TK_VERSION-src.tar.gz
    [ ! -f bwidget-$BWIDGET_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/bwidget-$BWIDGET_VERSION.tar.gz
    [ ! -f zlib-$ZLIB_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/zlib-$ZLIB_VERSION.tar.gz
    [ ! -f xz-$XZ_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/xz-$XZ_VERSION.tar.gz

    [ ! -f gluegen-v$JOGL_VERSION.tar.xz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/gluegen-v$JOGL_VERSION.tar.xz
    [ ! -f jcpp-v$JOGL_VERSION.tar.xz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/jcpp-v$JOGL_VERSION.tar.xz
    [ ! -f jogl-v$JOGL_VERSION.tar.xz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/jogl-v$JOGL_VERSION.tar.xz

    # This archive contains .jar and directories that have been copied from Scilab prerequirements
    # JavaFX/openjfx is only shipped as JARs, no rebuild is needed for now
    curl -L --time-cond thirdparty.zip -o thirdparty.zip "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/thirdparty-scilab-branch-${CI_MERGE_REQUEST_SOURCE_BRANCH_NAME}.zip"
    if ! unzip -qt "thirdparty.zip"; then
        # use thirdparty from the target branch
        curl -L -o thirdparty.zip "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/thirdparty-scilab-branch-${BRANCH}.zip"
    fi
    if ! unzip -qt "thirdparty.zip"; then
        # fallback to the default branch
        curl -L -o thirdparty.zip "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/thirdparty-scilab-branch-${CI_DEFAULT_BRANCH}.zip"
    fi
    if ! unzip -qt "thirdparty.zip"; then
        echo "Unable to retrieve thirdparty.zip"
        exit 1
    fi

    [ ! -f libarchive-$LIBARCHIVE_VERSION.tar.xz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/libarchive-$LIBARCHIVE_VERSION.tar.xz

    # $RAPIDJSON_VERSION=24b5e7a
    # curl -o rapidjson-$RAPIDJSON_VERSION.tar.gz https://github.com/Tencent/rapidjson/archive/$RAPIDJSON_VERSION.tar.gz
    [ ! -f rapidjson-$RAPIDJSON_VERSION.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/rapidjson-$RAPIDJSON_VERSION.tar.gz

    [ ! -f libxslt-$LIBXSLT_VERSION.tar.xz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/libxslt-$LIBXSLT_VERSION.tar.xz

    [ ! -f xlnt-${XLNT_VERSION}_with_submodules.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/xlnt-${XLNT_VERSION}_with_submodules.tar.gz

    [ ! -f apache-arrow-${ARROW_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/apache-arrow-${ARROW_VERSION}.tar.gz
    [ ! -f boost_${ARROW_BOOST_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/boost_${ARROW_BOOST_VERSION}.tar.gz
    [ ! -f thrift-${ARROW_THRIFT_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/thrift-${ARROW_THRIFT_VERSION}.tar.gz
    [ ! -f mimalloc-${ARROW_MIMALLOC_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/mimalloc-${ARROW_MIMALLOC_VERSION}.tar.gz
    [ ! -f xsimd-${ARROW_XSIMD_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/xsimd-${ARROW_XSIMD_VERSION}.tar.gz
    [ ! -f snappy-${ARROW_SNAPPY_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/snappy-${ARROW_SNAPPY_VERSION}.tar.gz
    [ ! -f zstd-${ARROW_ZSTD_VERSION}.tar.gz ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/zstd-${ARROW_ZSTD_VERSION}.tar.gz

    # CppServer and its deps
    [ ! -f CppServer-$CPPSERVER_VERSION.zip ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/CppServer-$CPPSERVER_VERSION.zip
    [ ! -f asio-$ASIO_VERSION.zip ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/asio-$ASIO_VERSION.zip
    [ ! -f CppCommon-$CPPCOMMON_VERSION.zip ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/CppCommon-$CPPCOMMON_VERSION.zip
    [ ! -f fmt-$FMT_VERSION.zip ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/fmt-$FMT_VERSION.zip
    [ ! -f CppCMakeScripts-$CPPCMAKESCRIPTS_VERSION.zip ] && curl -LO https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/CppCMakeScripts-$CPPCMAKESCRIPTS_VERSION.zip

    true;
}

make_all() {
    build_openjdk
    build_ant
    build_gluegen
    build_jogl
    build_openblas
    build_eigen
    build_zlib
    build_lzma
    build_hdf5
    build_pcre2
    build_fftw
    build_libxml2
    build_arpack
    build_suitesparse
    build_tcl
    build_tk
    build_matio
    build_openssl
    build_zstd
    build_curl
    build_ncurses
    build_libarchive
    build_rapidjson
    build_libxslt
    build_xlnt
    build_arrow
    build_cppserver
}

make_binary_directory() {
    cd "$INSTALLROOTDIR" || exit 1

    ########################
    ##### TCL/TK stuff #####
    ########################
    TCL_DIR=$INSTALLROOTDIR/modules/tclsci/tcl/
    [ -d "$TCL_DIR/BWidget" ] && rm -fr "$TCL_DIR/BWidget"
    mkdir -p "$TCL_DIR/BWidget"
    tar -xzf "$DOWNLOADDIR/bwidget-$BWIDGET_VERSION.tar.gz" \
        -C "$TCL_DIR/BWidget" \
        --strip-components=1 \
        --exclude=demo \
        bwidget-$BWIDGET_VERSION/images bwidget-$BWIDGET_VERSION/lang --wildcards bwidget-$BWIDGET_VERSION/*.tcl
    # fix permissions to fix issue #17231
    find "$TCL_DIR" -type d -print0 | xargs -0 chmod 755
    find "$TCL_DIR" -type f -not -name '*.tcl' -print0 | xargs -0 chmod 644
    find "$TCL_DIR" -type f -name '*.tcl' -print0 | xargs -0 chmod 755

    ####################
    ##### INCLUDES #####
    ####################
    cp -R "$INSTALLUSRDIR/include/Eigen/" "$INSTALLROOTDIR/include/"
    cp -R "$INSTALLUSRDIR/include/rapidjson/" "$INSTALLROOTDIR/include/"
    cp -R "$INSTALLUSRDIR/include/libxml2/" "$INSTALLROOTDIR/include/"
    cp -R "$INSTALLUSRDIR/include/pcre2.h" "$INSTALLROOTDIR/include/"
    cp -R "$INSTALLUSRDIR/include/xlnt/" "$INSTALLROOTDIR/include/"
    cp -R "$INSTALLUSRDIR/include/arrow/" "$INSTALLROOTDIR/include/"
    cp -R "$INSTALLUSRDIR/include/parquet/" "$INSTALLROOTDIR/include/"

    #####################################
    ##### lib/thirdparty/ directory #####
    #####################################
    LIBTHIRDPARTYDIR=$INSTALLROOTDIR/lib/thirdparty
    mkdir -p "$LIBTHIRDPARTYDIR"

    # Provide OpenBLAS blas and lapack
    # ensure that ld.so always load the same "real" file to reduce overhead
    rm -f "$LIBTHIRDPARTYDIR"/libatlas.*
    rm -f "$LIBTHIRDPARTYDIR"/lib*blas.*
    rm -f "$LIBTHIRDPARTYDIR"/liblapack.*
    cp "$INSTALLUSRDIR/lib/libopenblas.so.$OPENBLAS_VERSION" "$LIBTHIRDPARTYDIR/"
    ln -fs libopenblas.so.$OPENBLAS_VERSION "$LIBTHIRDPARTYDIR/libopenblas.so.0"
    ln -fs libopenblas.so.$OPENBLAS_VERSION "$LIBTHIRDPARTYDIR/libblas.so.3"
    ln -fs libopenblas.so.$OPENBLAS_VERSION "$LIBTHIRDPARTYDIR/liblapack.so.3"

    rm -f "$LIBTHIRDPARTYDIR"/libarpack.*
    cp -d "$INSTALLUSRDIR"/lib/libarpack.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libcrypto.*
    cp -d "$INSTALLUSRDIR"/lib/libcrypto.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libscicurl.*
    cp -d "$INSTALLUSRDIR"/lib/libscicurl.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libarchive.*
    cp -d "$INSTALLUSRDIR"/lib/libarchive.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libfftw3.*
    cp -d "$INSTALLUSRDIR"/lib/libfftw3.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libhdf5_hl.*
    cp -d "$INSTALLUSRDIR"/lib/libhdf5_hl.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libhdf5.*
    cp -d "$INSTALLUSRDIR"/lib/libhdf5.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libhdf5_hl_cpp.*
    cp -d "$INSTALLUSRDIR"/lib/libhdf5_hl_cpp.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libhdf5_cpp.*
    cp -d "$INSTALLUSRDIR"/lib/libhdf5_cpp.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libmatio.*
    cp -d "$INSTALLUSRDIR"/lib/libmatio.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libpcre*.*
    cp -d "$INSTALLUSRDIR"/lib/libpcre2*.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libxlnt*.*
    cp -d "$INSTALLUSRDIR"/lib/libxlnt*.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libarrow*.*
    cp -d "$INSTALLUSRDIR"/lib/libarrow*.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libparquet*.*
    cp -d "$INSTALLUSRDIR"/lib/libparquet*.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libssl.*
    cp -d "$INSTALLUSRDIR"/lib/libssl.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libtcl*.*
    cp -d "$INSTALLUSRDIR"/lib/libtcl*.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libtk*.*
    cp -d "$INSTALLUSRDIR"/lib/libtk*.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libumfpack.*
    cp -d "$INSTALLUSRDIR"/lib/libumfpack.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libamd.*
    cp -d "$INSTALLUSRDIR"/lib/libamd.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libcholmod.*
    cp -d "$INSTALLUSRDIR"/lib/libcholmod.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libcolamd.*
    cp -d "$INSTALLUSRDIR"/lib/libcolamd.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libccolamd.*
    cp -d "$INSTALLUSRDIR"/lib/libccolamd.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libcamd.*
    cp -d "$INSTALLUSRDIR"/lib/libcamd.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libbtf.*
    cp -d "$INSTALLUSRDIR"/lib/libbtf.* "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libklu.*
    cp -d "$INSTALLUSRDIR"/lib/libklu.* "$LIBTHIRDPARTYDIR/"

    rm -f "$LIBTHIRDPARTYDIR"/libxslt*.*
    cp -d "$INSTALLUSRDIR"/lib/libxslt*.* "$LIBTHIRDPARTYDIR/"

    # Scilab dependencies where the system ones are not recent enough to be used.
    #
    # The system dependencies are supposed to be conformant to the latest
    # Linux Standard Base 5.0.0 ; Scilab requires more recent versions.

    rm -fr "$LIBTHIRDPARTYDIR/redist" && mkdir "$LIBTHIRDPARTYDIR/redist/"

    rm -f "$LIBTHIRDPARTYDIR"/libz.* "$LIBTHIRDPARTYDIR"/libsciz.*
    rm -f "$LIBTHIRDPARTYDIR"/redist/libz.* "$LIBTHIRDPARTYDIR"/redist/libsciz.*
    cp -d "$INSTALLUSRDIR"/lib/libsciz.* "$LIBTHIRDPARTYDIR/redist/"

    rm -f "$LIBTHIRDPARTYDIR"/liblzma.* "$LIBTHIRDPARTYDIR"/libscilzma.*
    rm -f "$LIBTHIRDPARTYDIR"/redist/liblzma.* "$LIBTHIRDPARTYDIR"/redist/libscilzma.*
    cp -d "$INSTALLUSRDIR"/lib/libscilzma.* "$LIBTHIRDPARTYDIR/redist/"

    rm -f "$LIBTHIRDPARTYDIR"/libzstd.* "$LIBTHIRDPARTYDIR"/libscizstd.*
    rm -f "$LIBTHIRDPARTYDIR"/redist/libzstd.* "$LIBTHIRDPARTYDIR"/redist/libscizstd.*
    cp -d "$INSTALLUSRDIR"/lib/libscizstd.* "$LIBTHIRDPARTYDIR/redist/"

    rm -f "$LIBTHIRDPARTYDIR"/libxml2.* "$LIBTHIRDPARTYDIR"/libscixml2.*
    rm -f "$LIBTHIRDPARTYDIR"/redist/libxml2.* "$LIBTHIRDPARTYDIR"/redist/libscixml2.*
    cp -d "$INSTALLUSRDIR"/lib/libscixml2.* "$LIBTHIRDPARTYDIR/redist/"

    rm -f "$LIBTHIRDPARTYDIR"/libncurses.* "$LIBTHIRDPARTYDIR"/libscincurses.*
    rm -f "$LIBTHIRDPARTYDIR"/redist/libncurses.* "$LIBTHIRDPARTYDIR"/redist/libscincurses.*
    cp -d "$INSTALLUSRDIR"/lib/libscincurses.so* "$LIBTHIRDPARTYDIR/redist/"

    # cppserver lib and its deps
    rm -f "$LIBTHIRDPARTYDIR"/libasio.so*
    cp -d "$INSTALLUSRDIR"/lib/libasio.so "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libfmt.so*
    cp -d "$INSTALLUSRDIR"/lib/libfmt.so "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libcppserver.so*
    cp -d "$INSTALLUSRDIR"/lib/libcppserver.so "$LIBTHIRDPARTYDIR/"
    rm -f "$LIBTHIRDPARTYDIR"/libcppcommon.so*
    cp -d "$INSTALLUSRDIR"/lib/libcppcommon.so "$LIBTHIRDPARTYDIR/"
    # cppserver include
    cp -R "$INSTALLUSRDIR/include/cppserver/" "$INSTALLROOTDIR/include/"

    # GCC libs could be there but are prefixed with "sci" to avoid clashing
    # system libraries static linked into scilab libraries instead.  This
    # avoid compilers (and support libraries) version mismatch between gcc
    # used here and user's gcc (probably more recent)
    cp -d /usr/local/lib/libsciquadmath.so* "$LIBTHIRDPARTYDIR/redist/"
    cp -d /usr/local/lib/libscigfortran.so* "$LIBTHIRDPARTYDIR/redist/"
    cp -d /usr/local/lib/libscigcc_s.so* "$LIBTHIRDPARTYDIR/redist/"
    cp -d /usr/local/lib/libscigomp.so* "$LIBTHIRDPARTYDIR/redist/"
    cp -d /usr/local/lib/libscistdc++.so* "$LIBTHIRDPARTYDIR/redist/"

    cp -d /usr/local/lib/libsciquadmath.so* "$INSTALLUSRDIR/lib/"
    cp -d /usr/local/lib/libscigfortran.so* "$INSTALLUSRDIR/lib/"
    cp -d /usr/local/lib/libscigcc_s.so* "$INSTALLUSRDIR/lib/"
    cp -d /usr/local/lib/libscigomp.so* "$INSTALLUSRDIR/lib/"
    cp -d /usr/local/lib/libscistdc++.so* "$INSTALLUSRDIR/lib/"

    # In case these libraries are not found on the system.
    #
    # The ".so" is not shipped on purpose for compilers support libraries,
    # the user should build on the reference system.
    # The mandatory libraries are the ones documented in the Linux Standard
    # Base 5.0 .

    #
    # use patchelf to cleanup and setup easy way to link against Scilab
    #

    # 1. strip libraries (exporting the debuginfo to another file) to
    # reduce file size and thus startup time
    # 2. remove rpath as LD_LIBRARY_PATH is set on the startup script
    find "$LIBTHIRDPARTYDIR" -name '*.so*' -type f -not -name '*.debug'| while read -r file ;
    do
        objcopy --only-keep-debug "$file" "$file.debug"
        [ -f  "$file.debug" ] && objcopy --strip-debug "$file"
        [ -f  "$file.debug" ] && (objcopy --add-gnu-debuglink="$file.debug" "$file" || true)

        patchelf --remove-rpath "$file"
    done
    # 3. add runpath dependency for thirdparty libs
    # shellcheck disable=SC2016
    find lib/thirdparty/*.so* -type f -exec patchelf \
        --set-rpath '$ORIGIN:$ORIGIN/redist' \
        {} \;
    # 3. add runpath dependency for thirdparty/redist libs
    # shellcheck disable=SC2016
    find lib/thirdparty/redist/*.so* -type f -exec patchelf \
        --set-rpath '$ORIGIN' \
        {} \;
    # 4. patchelf the JVM libs
    # shellcheck disable=SC2016
    patchelf --set-rpath '$ORIGIN:$ORIGIN/server' java/jre/lib/libjava.so
    # shellcheck disable=SC2016
    patchelf --set-rpath '$ORIGIN' java/jre/lib/server/libjvm.so
}

make_jar() {
    # JAR management
    # we do not need to recompile JARs and re-use some of them, only copy them
    # into the expected directory (shipped into the binary zip)

    JAVATHIRDPARTYDIR=$INSTALLROOTDIR/thirdparty
    mkdir -p "$JAVATHIRDPARTYDIR"

    LIBTHIRDPARTYDIR=$INSTALLROOTDIR/lib/thirdparty
    mkdir -p "$LIBTHIRDPARTYDIR"

    # copy .jar from scilab prerequirements
    cd "$DOWNLOADDIR" || exit 1
    rm -rf thirdparty
    mkdir thirdparty
    cd thirdparty || exit 1
    unzip -o "$DOWNLOADDIR/thirdparty.zip"

    # JavaFX
    ## Rename JARs (Remove -linux suffix)
    mv javafx-base-${JAVAFX_VERSION}-linux.jar javafx-base-${JAVAFX_VERSION}.jar 
    mv javafx-graphics-${JAVAFX_VERSION}-linux.jar javafx-graphics-${JAVAFX_VERSION}.jar 
    mv javafx-swing-${JAVAFX_VERSION}-linux.jar javafx-swing-${JAVAFX_VERSION}.jar
    ## Extract native libs
    unzip -q javafx-graphics-${JAVAFX_VERSION}.jar libglass.so libprism_sw.so libprism_es2.so -d "$LIBTHIRDPARTYDIR"
    ## Remove native libs from archive
    zip -d javafx-graphics-${JAVAFX_VERSION}.jar '*.so'

    # Copy all JARs from thirdparty.zip
    # JAR versions are enforced to trigger a prerequirement rebuild on upgrade
    cp -a -t "$JAVATHIRDPARTYDIR"           \
        Saxon-HE-12.4.jar                   \
        activation-1.1.1.jar                \
        antlr4-runtime-4.13.1.jar           \
        asm-3.3.1.jar                       \
        avalon-framework-4.1.4.jar          \
        batik-all-1.17.jar                  \
        checkstyle-10.17.0.jar              \
        checkstyle/                         \
        cobertura-2.1.1.jar                 \
        commons-beanutils-1.9.4.jar         \
        commons-codec-1.15.jar              \
        commons-collections-3.2.2.jar       \
        commons-io-2.11.0.jar               \
        commons-logging-1.1.1.jar           \
        docbook/                            \
        ecj-3.37.0.jar                      \
        flatlaf-3.4.1.jar                   \
        flexdock-1.2.5.jar                  \
        fontbox-2.0.27.jar                  \
        fonts/                              \
        fop-2.9.jar                         \
        fop-core-2.9.jar                    \
        fop-events-2.9.jar                  \
        fop-util-2.9.jar                    \
        freehep-graphics2d-2.4.jar          \
        freehep-graphicsbase-2.4.jar        \
        freehep-graphicsio-2.4.jar          \
        freehep-graphicsio-emf-2.4.jar      \
        freehep-io-2.2.2.jar                \
        gson-2.10.1.jar                     \
        jogl-all-2.5.0.jar                  \
        gluegen-rt-2.5.0.jar                \
        guava-33.2.0-jre.jar                \
        httpclient5-5.1.3.jar               \
        httpcore5-5.1.3.jar                 \
        istack-commons-runtime-4.2.0.jar    \
        jakarta.activation-2.0.1.jar        \
        jakarta.activation-api-2.1.3.jar    \
        javafx-base-21.0.11.jar             \
        javafx-graphics-21.0.11.jar         \
        javafx-swing-21.0.11.jar            \
        javax.activation-api-1.2.0.jar      \
        javax.annotation-api-1.3.2.jar      \
        jaxb-api-2.3.1.jar                  \
        jaxb-impl-2.3.1.jar                 \
        jaxb-runtime-2.3.1.jar              \
        jeuclid-core-3.1.14.jar             \
        jcef-146.0.10.jar                   \
        jgoodies-looks-2.7.0.jar            \
        jgraphx-2.1.0.7.jar                 \
        jlatexmath-1.0.7.jar                \
        jlatexmath-font-cyrillic-1.0.7.jar  \
        jlatexmath-font-greek-1.0.7.jar     \
        jlatexmath-fop-1.0.7.jar            \
        jna-5.6.0.jar                       \
        jna-platform-5.6.0.jar              \
        jrosetta-API-1.0.4.jar              \
        jrosetta-engine-1.0.4.jar           \
        junit-4.10.jar                      \
        lucene-analysis-common-9.10.0.jar   \
        lucene-core-9.10.0.jar              \
        lucene-queryparser-9.10.0.jar       \
        opencv-4.3.0-3.jar                  \
        qdox-1.12.jar                       \
        sikulixapi-2.0.5.jar                \
        skinlf-1.2.3.jar                    \
        slf4j-api-1.7.25.jar                \
        web/                                \
        xml-apis-1.4.01.jar                 \
        xml-apis-ext-1.3.04.jar             \
        xmlgraphics-commons-2.9.jar         \
        xmlresolver-6.0.4.jar               \
        || exit 1

    # JCEF (Copy native libs)
    mkdir -p "$LIBTHIRDPARTYDIR/jcef"
    unzip -p "jcef-natives-linux-amd64-jcef-$JCEF_VERSION.jar" "jcef-natives-linux-amd64-jcef-$JCEF_VERSION.tar.gz" \
        | tar -xzf - -C "$LIBTHIRDPARTYDIR/jcef" \
        || exit 1
}

make_archive() {
    make_versions > "$INSTALLROOTDIR/version.txt"

    cd "$INSTALLROOTDIR/.." || exit 1
    echo "Creating archive..."

    export XZ_OPT="-9T0"
    tar -cJf "prerequirements-${SCI_VERSION_STRING}.bin.${ARCH}.tar.xz" -C "$INSTALLROOTDIR" \
        -- java lib include modules thirdparty usr version.txt
}

####################
##### BUILDERS #####
####################

build_openblas() {
    cd "$BUILDDIR" || exit 1

    tar -xzf "$DOWNLOADDIR/OpenBLAS-$OPENBLAS_VERSION.tar.gz"
    cd OpenBLAS-$OPENBLAS_VERSION || exit 1
    make -j"$(nproc)" TARGET=NEHALEM DYNAMIC_ARCH=1 NO_AVX2=1 NO_STATIC=1 NO_LAPACKE=1

    # install openblas for runtime usage
    cp libopenblas.so "$INSTALLUSRDIR/lib/libopenblas.so.$OPENBLAS_VERSION"

    # Provides BLAS
    cp "$INSTALLUSRDIR/lib/libopenblas.so.$OPENBLAS_VERSION" "$INSTALLUSRDIR/lib/libblas.so.3"
    patchelf --set-soname libblas.so.3 "$INSTALLUSRDIR/lib/libblas.so.3"
    ln -fs libblas.so.3 "$INSTALLUSRDIR/lib/libblas.so"

    # Provides LAPACK
    cp "$INSTALLUSRDIR/lib/libopenblas.so.$OPENBLAS_VERSION" "$INSTALLUSRDIR/lib/liblapack.so.3"
    patchelf --set-soname liblapack.so.3 "$INSTALLUSRDIR/lib/liblapack.so.3"
    ln -fs liblapack.so.3 "$INSTALLUSRDIR/lib/liblapack.so"
}

build_openjdk() {
    [ -e "$BUILDDIR/java" ] && rm -fr "$BUILDDIR/java"
    [ -d "$INSTALLROOTDIR/java/jdk-$JDK_VERSION-jre" ] && rm -fr "$INSTALLROOTDIR/java/jdk-$JDK_VERSION-jre"
    [ -e "$INSTALLROOTDIR/java/jre" ] && rm -fr "$INSTALLROOTDIR/java/jre"

    mkdir -p "$BUILDDIR/java/"
    cd "$BUILDDIR/java/" || exit 1
    tar -xzf "$DOWNLOADDIR/jdk-$JDK_VERSION.tar.gz" # Needed to build other dependencies such as JoGL
    tar -xzf "$DOWNLOADDIR/jre-$JRE_VERSION.tar.gz"

    cp -a "jdk-$JDK_VERSION-jre" "$INSTALLROOTDIR/java/jdk-$JRE_VERSION-jre"
    ln -s "jdk-$JRE_VERSION-jre" "$INSTALLROOTDIR/java/jre"
}

build_ant() {
    [ -d "$INSTALLROOTDIR/java/ant" ] && rm -fr "$INSTALLROOTDIR/java/ant"
    [ -d "$INSTALLROOTDIR/java/apache-ant-$ANT_VERSION" ] && rm -fr "$INSTALLROOTDIR/java/apache-ant-$ANT_VERSION"

    cd "$INSTALLROOTDIR/java/" || exit 1
    tar -xzf "$DOWNLOADDIR/apache-ant-$ANT_VERSION-bin.tar.gz"
    ln -s apache-ant-$ANT_VERSION ant

    cp -a apache-ant-$ANT_VERSION/etc/checkstyle "$INSTALLROOTDIR/thirdparty/"

    # cleanup folder
    rm -rf "$INSTALLROOTDIR/java/ant/manual"
    rm "$INSTALLROOTDIR"/java/ant/lib/*.pom
}

build_arpack() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/arpack-ng-$ARPACK_VERSION/install_dir

    rm -rf arpack-ng-$ARPACK_VERSION
    tar -xzf "$DOWNLOADDIR/arpack-ng-$ARPACK_VERSION.tar.gz"
    cd arpack-ng-$ARPACK_VERSION || exit 1

    mkdir -p build || exit 1
    cd build || exit 1
    cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DBUILD_SHARED_LIBS=ON \
        -DBLAS_LIBRARIES="$INSTALLUSRDIR/lib/libblas.so" \
        -DLAPACK_LIBRARIES="$INSTALLUSRDIR/lib/liblapack.so" \
        -G "Unix Makefiles"
    cmake --build . --parallel --config Release
    cp -a lib/libarpack.so* "$INSTALLUSRDIR/lib/"
}

build_eigen() {
    cd "$BUILDDIR" || exit 1

    mkdir -p eigen-$EIGEN_VERSION
    tar -zxf "$DOWNLOADDIR/eigen-$EIGEN_VERSION.tar.gz" --strip-components=1 -C eigen-$EIGEN_VERSION
    cd eigen-$EIGEN_VERSION || exit 1
    rm -fr "$INSTALLUSRDIR/include/Eigen"
    cp -a Eigen "$INSTALLUSRDIR/include/"
}

build_hdf5() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/hdf5-$HDF5_VERSION/install_dir

    unzip -o "$DOWNLOADDIR/hdf5-$HDF5_VERSION.zip"
    cd hdf5-hdf5_$HDF5_VERSION || exit 1

    mkdir -p build
    cd build || exit 1
    cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DBUILD_SHARED_LIBS=ON \
        -DHDF5_BUILD_CPP_LIB=ON \
        -DHDF5_BUILD_HL_LIB=ON \
        -G "Unix Makefiles"
    cmake --build . --parallel --target install --config Release

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}

build_fftw() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/fftw-$FFTW_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/fftw-$FFTW_VERSION.tar.gz"
    cd fftw-$FFTW_VERSION || exit 1
    ./configure --enable-shared --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}

build_zlib() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/zlib-$ZLIB_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/zlib-$ZLIB_VERSION.tar.gz"
    cd zlib-$ZLIB_VERSION || exit 1
    ./configure --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    # Rename libz to libsciz
    cp "$INSTALL_DIR/lib/libz.so.$ZLIB_VERSION" "$INSTALLUSRDIR/lib/libsciz.so.$ZLIB_VERSION"
    cd "$INSTALLUSRDIR/lib" || exit 1
    ln -sf libsciz.so.$ZLIB_VERSION libz.so
    ln -sf libsciz.so.$ZLIB_VERSION libsciz.so.1
    ln -sf libsciz.so.1 libsciz.so
    patchelf --set-soname libsciz.so.1 libsciz.so.$ZLIB_VERSION
}

build_lzma() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/xz-$XZ_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/xz-$XZ_VERSION.tar.gz"
    cd "xz-$XZ_VERSION" || exit 1
    ./configure --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    # Rename liblzma to libscilzma
    cp "$INSTALL_DIR/lib/liblzma.so.$XZ_VERSION" "$INSTALLUSRDIR/lib/libscilzma.so.$XZ_VERSION"
    cd "$INSTALLUSRDIR/lib" || exit 1
    ln -sf "libscilzma.so.$XZ_VERSION" liblzma.so
    ln -sf "libscilzma.so.$XZ_VERSION" libscilzma.so.1
    ln -sf libscilzma.so.1 libscilzma.so
    patchelf --set-soname libscilzma.so.1 "libscilzma.so.$XZ_VERSION"
}

build_zstd() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/zstd-$ARROW_ZSTD_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/zstd-$ARROW_ZSTD_VERSION.tar.gz"
    cd "zstd-$ARROW_ZSTD_VERSION" || exit 1
    make -C lib "-j$(nproc)"
    make -C lib install PREFIX="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    # Rename libzstd to libscizstd
    cp "$INSTALL_DIR/lib/libzstd.so.$ARROW_ZSTD_VERSION" "$INSTALLUSRDIR/lib/libscizstd.so.$ARROW_ZSTD_VERSION"
    cd "$INSTALLUSRDIR/lib" || exit 1
    ln -sf "libscizstd.so.$ARROW_ZSTD_VERSION" libzstd.so
    ln -sf "libscizstd.so.$ARROW_ZSTD_VERSION" libscizstd.so.1
    ln -sf libscizstd.so.1 libscizstd.so
    patchelf --set-soname libscizstd.so.1 "libscizstd.so.$ARROW_ZSTD_VERSION"
}

build_openssl() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/openssl-$OPENSSL_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/openssl-$OPENSSL_VERSION.tar.gz"
    cd openssl-$OPENSSL_VERSION || exit 1
    ./config shared --prefix="$INSTALL_DIR" --openssldir="$INSTALL_DIR"
    make "-j$(nproc)" depend all
    make install

    # install at the right location
    cp -a "$INSTALL_DIR"/lib/libssl.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/lib/libcrypto.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}


build_tcl() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/tcl$TCL_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/tcl$TCL_VERSION-src.tar.gz"
    cd tcl$TCL_VERSION/unix || exit 1
    ./configure --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/lib/libtcl*.so* "$INSTALLUSRDIR/lib/"
    chmod 755 "$INSTALLUSRDIR"/lib/libtcl*.so*

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    TCL_DIR=$INSTALLROOTDIR/modules/tclsci/tcl/
    mkdir -p "$TCL_DIR"
    # install in module
    cp -a "$INSTALL_DIR"/lib/tcl* "$TCL_DIR"
    rm "$TCL_DIR/tclConfig.sh"
    rm -rf "$TCL_DIR/tcl*/tzdata"
    # install in usr/lib
    cp -a "$INSTALL_DIR"/lib/tcl* "$INSTALLUSRDIR/lib/"
    rm "$INSTALLUSRDIR/lib/tclConfig.sh"
    rm -rf "$INSTALLUSRDIR/tcl*/tzdata"
}

build_tk() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/tcl$TCL_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/tk$TK_VERSION-src.tar.gz"
    cd tk$TK_VERSION/unix || exit 1
    ./configure --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/lib/libtk*.so* "$INSTALLUSRDIR/lib/"
    chmod 755 "$INSTALLUSRDIR"/lib/libtk*.so*

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    TCL_DIR=$INSTALLROOTDIR/modules/tclsci/tcl/
    mkdir -p "$TCL_DIR"
    # install in module
    cp -a "$INSTALL_DIR"/lib/tk* "$TCL_DIR"
    rm "$TCL_DIR/tkConfig.sh"
    rm -rf "$TCL_DIR/tk8.5/demos/" # See bug #3869
    # install in usr/lib
    cp -a "$INSTALL_DIR"/lib/tk* "$INSTALLUSRDIR/lib/"
    rm "$INSTALLUSRDIR/lib/tkConfig.sh"
    rm -rf "$INSTALLUSRDIR/tk8.5/demos/"
}

build_matio() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/matio-$MATIO_VERSION/install_dir

    rm -rf matio-$MATIO_VERSION
    tar -xzf "$DOWNLOADDIR/matio-$MATIO_VERSION.tar.gz"
    cd matio-$MATIO_VERSION || exit 1

    ./autogen.sh
    ./configure --enable-shared --with-hdf5="$INSTALLUSRDIR" --with-zlib="$INSTALLUSRDIR" --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}

build_ncurses() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/ncurses-$NCURSES_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/ncurses-$NCURSES_VERSION.tar.gz"
    cd ncurses-$NCURSES_VERSION || exit 1
    ./configure --with-shared --without-cxx --with-terminfo-dirs="/etc/terminfo:/lib/terminfo:/usr/share/terminfo:/usr/lib/terminfo" --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    # Rename ncurses to scincurses
    cp "$INSTALL_DIR/lib/libncurses.so.$NCURSES_VERSION" "$INSTALLUSRDIR/lib/libscincurses.so.$NCURSES_VERSION"
    cd "$INSTALLUSRDIR/lib" || exit 1
    ln -sf libscincurses.so.$NCURSES_VERSION libncurses.so
    ln -sf libscincurses.so.$NCURSES_VERSION libscincurses.so.6
    ln -sf libscincurses.so.6 libscincurses.so
    patchelf --set-soname libscincurses.so.6 "$INSTALLUSRDIR/lib/libscincurses.so.$NCURSES_VERSION"
}

build_pcre2() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/pcre2-$PCRE2_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/pcre2-$PCRE2_VERSION.tar.gz"
    cd pcre2-$PCRE2_VERSION || exit 1
    cmake -B build . \
        -DBUILD_SHARED_LIBS=ON \
        -DBUILD_STATIC_LIBS=OFF \
        -DPCRE2_BUILD_PCRE2_8=OFF \
        -DPCRE2_BUILD_PCRE2_16=OFF \
        -DPCRE2_BUILD_PCRE2_32=ON

    cmake --build build/

    cp -a build/*.so* "$INSTALLUSRDIR/lib/"
    cp -a build/pcre2.h "$INSTALLUSRDIR/include/"
}

build_libxml2() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/libxml2-$LIBXML2_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/libxml2-$LIBXML2_VERSION.tar.gz"
    cd libxml2-$LIBXML2_VERSION || exit 1
    rm -rf "$INSTALL_DIR" && mkdir "$INSTALL_DIR"
    ./configure --without-python --with-zlib="$INSTALLUSRDIR" --without-lzma --prefix=
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    # shellcheck disable=SC2016
    sed -i -e 's|^prefix=.*|prefix=$( cd -- "$(dirname "$0")" >/dev/null 2>\&1 ; pwd -P )/..|' "$INSTALL_DIR/bin/xml2-config"
    cp "$INSTALL_DIR/bin/xml2-config" "$INSTALLUSRDIR/bin/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    # Rename xml2 to scixml2
    cp "$INSTALL_DIR/lib/libxml2.so.$LIBXML2_VERSION" "$INSTALLUSRDIR/lib/libscixml2.so.$LIBXML2_VERSION"
    cd "$INSTALLUSRDIR/lib" || exit 1
    ln -sf libscixml2.so.$LIBXML2_VERSION libxml2.so
    ln -sf libscixml2.so.$LIBXML2_VERSION libscixml2.so.2
    ln -sf libscixml2.so.2 libscixml2.so
    ln -sf libscixml2.so.2 libxml2.so
    patchelf --set-soname libscixml2.so.2 libscixml2.so.$LIBXML2_VERSION
}

build_curl() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/curl-$CURL_VERSION/install_dir

    tar -zxf "$DOWNLOADDIR/curl-$CURL_VERSION.tar.gz"
    cd curl-$CURL_VERSION || exit 1
    ./configure \
        --without-ca-bundle --with-ca-fallback \
        --with-ssl="$INSTALLUSRDIR" --without-nss \
        --with-zlib="$INSTALLUSRDIR" \
        --with-zstd="$INSTALLUSRDIR" \
	    --disable-ldap \
        --without-libpsl \
        --prefix= \
        CFLAGS="-O2 -g"
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    # shellcheck disable=SC2016
    sed -i -e 's|^prefix=.*|prefix=$( cd -- "$(dirname "$0")" >/dev/null 2>\&1 ; pwd -P )/..|' "$INSTALL_DIR/bin/curl-config"
    sed -i -e 's|lcurl|lscicurl|' "$INSTALL_DIR/bin/curl-config"
    cp "$INSTALL_DIR/bin/curl-config" "$INSTALLUSRDIR/bin/"

    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    # Rename libcurl to libscicurl
    cp "$INSTALL_DIR/lib/libcurl.so.4.8.0" "$INSTALLUSRDIR/lib/libscicurl.so.4.8.0"
    cd "$INSTALLUSRDIR/lib" || exit 1
    ln -sf "libscicurl.so.4.8.0" libcurl.so
    ln -sf "libscicurl.so.4.8.0" libscicurl.so.4
    ln -sf libscicurl.so.4 libscicurl.so
    patchelf --set-soname libscicurl.so.4 "libscicurl.so.4.8.0"
}

build_libarchive() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/libarchive-$LIBARCHIVE_VERSION/install_dir

    tar -xf "$DOWNLOADDIR/libarchive-$LIBARCHIVE_VERSION.tar.xz"
    cd libarchive-$LIBARCHIVE_VERSION || exit 1

    # this configure does not support passing path, overwrite variables
    saved_CFLAGS="$CFLAGS"
    saved_LDFLAGS="$LDFLAGS"
    CFLAGS="$CFLAGS -I$INSTALLUSRDIR/include -I$INSTALLUSRDIR/include/libxml2"
    LDFLAGS="$LDFLAGS -L$INSTALLUSRDIR/lib"

    ./configure --prefix= \
        --disable-posix-regex-lib
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    CFLAGS="$saved_CFLAGS"
    LDFLAGS="$saved_LDFLAGS"

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}

build_rapidjson() {
    rm -rf "$INSTALLUSRDIR/include/rapidjson/"
    tar -xzf "$DOWNLOADDIR"/rapidjson-$RAPIDJSON_VERSION.tar.gz -C "$INSTALLUSRDIR/include/" --strip-components=2 --wildcards rapidjson-$RAPIDJSON_VERSION*/include
}

build_xlnt() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/xlnt-$XLNT_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/xlnt-${XLNT_VERSION}_with_submodules.tar.gz"
    cd xlnt-$XLNT_VERSION || exit 1

    cmake -B build . --install-prefix="$INSTALL_DIR" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        -G "Unix Makefiles"
    cmake --build build/
    cmake --build build/ --target install

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    XLNT_MAJOR_VERSION=$(echo "$XLNT_VERSION" | awk -F \. '{ print $1}')
    cd "$INSTALLUSRDIR/lib/" || exit 1
    ln -fs "libxlnt.so.${XLNT_VERSION}" libxlnt.so.${XLNT_MAJOR_VERSION}
}

build_arrow() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/apache-arrow-$ARROW_VERSION/install_dir

    tar -xzf "$DOWNLOADDIR/apache-arrow-${ARROW_VERSION}.tar.gz"
    cd apache-arrow-$ARROW_VERSION/cpp/ || exit 1

    # Use local files as curl embedded with current cmake version does not support https protocol
    export ARROW_BOOST_URL=file://$DOWNLOADDIR/boost_$ARROW_BOOST_VERSION.tar.gz
    export ARROW_THRIFT_URL=file://$DOWNLOADDIR/thrift-$ARROW_THRIFT_VERSION.tar.gz
    sed -i 's/b5d8311a779470e1502c027f428a1db542f5c051c8e1280ccd2163fa935ff2d6/cd7b829d3d9d87f9f7d708e004eef7629789591ee1d416f4741913bc33e5c27d/' thirdparty/versions.txt
    export ARROW_MIMALLOC_URL=file://$DOWNLOADDIR/mimalloc-$ARROW_MIMALLOC_VERSION.tar.gz
    export ARROW_XSIMD_URL=file://$DOWNLOADDIR/xsimd-$ARROW_XSIMD_VERSION.tar.gz
    export ARROW_ZLIB_URL=file://$DOWNLOADDIR/zlib-$ZLIB_VERSION.tar.gz
    sed -i 's/9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23/c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1/' thirdparty/versions.txt
    export ARROW_SNAPPY_URL=file://$DOWNLOADDIR/snappy-$ARROW_SNAPPY_VERSION.tar.gz
    # Selected options similar to https://github.com/microsoft/vcpkg/blob/5825c5878e2ccab9b3c3128e4529a3b4a86776b4/ports/arrow/portfile.cmake
    cmake -B build . --install-prefix="$INSTALL_DIR" \
        -DCMAKE_BUILD_TYPE=Release \
        -DARROW_BUILD_SHARED=ON \
        -DARROW_BUILD_STATIC=OFF \
        -DARROW_BUILD_TESTS=OFF \
        -DARROW_PARQUET=ON \
        -DARROW_WITH_ZLIB=ON \
        -DARROW_WITH_SNAPPY=ON \
        -DSnappy_SOURCE=BUNDLED \
        -DARROW_WITH_ZSTD=ON \
        -Dzstd_SOURCE=SYSTEM \
        -DZSTD_ROOT="$INSTALLUSRDIR" \
        -G "Unix Makefiles"
    cmake --build build/
    cmake --build build/ --target install

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}

build_suitesparse() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/SuiteSparse-$SUITESPARSE_VERSION/install_dir

    rm -rf "$INSTALL_DIR"
    mkdir -p "$INSTALL_DIR/lib"
    mkdir -p "$INSTALL_DIR/include"

    tar -zxf "$DOWNLOADDIR/SuiteSparse-$SUITESPARSE_VERSION.tar.gz"
    cd SuiteSparse || exit 1
    sed -i -e 's|^\INSTALL_LIB = .*|\INSTALL_LIB = '"$INSTALL_DIR"'\/lib\/|' SuiteSparse_config/SuiteSparse_config.mk
    sed -i -e 's|^\INSTALL_INCLUDE = .*|\INSTALL_INCLUDE = '"$INSTALL_DIR"'\/include\/|' SuiteSparse_config/SuiteSparse_config.mk
    make "-j$(nproc)" library
    make install

    # cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"

    UMFPACK_VERSION=$(grep -m1 VERSION UMFPACK/Makefile | sed -e "s|\VERSION = ||")

    # See http://slackware.org.uk/slacky/slackware-12.2/development/suitesparse/3.1.0/src/suitesparse.SlackBuild
    # libamd.so
    AMD_VERSION=$(grep -m1 VERSION AMD/Makefile | sed -e "s|\VERSION = ||")
    AMD_MAJOR_VERSION=$(echo "$AMD_VERSION" | awk -F \. '{ print $1}')
    cd AMD/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libamd.so.${AMD_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libamd.so.${AMD_VERSION}" \
        $(ls ./*.o)
    rm -f "$INSTALLUSRDIR"/lib/libamd.so*
    cp "libamd.so.${AMD_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libcamd.so
    CAMD_VERSION=$(grep -m1 VERSION CAMD/Makefile | sed -e "s|\VERSION = ||")
    CAMD_MAJOR_VERSION=$(echo "$CAMD_VERSION" | awk -F \. '{print $1}')
    cd CAMD/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libcamd.so.${CAMD_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libcamd.so.${CAMD_VERSION}" \
        $(ls ./*.o)
    rm -f "$INSTALLUSRDIR"/lib/libcamd.so*
    cp "libcamd.so.${CAMD_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libcolamd.so
    COLAMD_VERSION=$(grep -m1 VERSION COLAMD/Makefile | sed -e "s|\VERSION = ||")
    COLAMD_MAJOR_VERSION=$(echo "$COLAMD_VERSION" | awk -F \. '{print $1}')
    cd COLAMD/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libcolamd.so.${COLAMD_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libcolamd.so.${COLAMD_VERSION}" \
        $(ls ./*.o)
    rm -f "$INSTALLUSRDIR"/lib/libcolamd.so*
    cp "libcolamd.so.${COLAMD_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libccolamd.so
    CCOLAMD_VERSION=$(grep -m1 VERSION CCOLAMD/Makefile | sed -e "s|\VERSION = ||")
    CCOLAMD_MAJOR_VERSION=$(echo "$CCOLAMD_VERSION" | awk -F \. '{print $1}')
    cd CCOLAMD/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libccolamd.so.${CCOLAMD_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libccolamd.so.${CCOLAMD_VERSION}" \
        $(ls ./*.o)
    rm -f "$INSTALLUSRDIR"/lib/libccolamd.so*
    cp "libccolamd.so.${CCOLAMD_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libcholmod.so
    CHOLMOD_VERSION=$(grep -m1 VERSION CHOLMOD/Makefile | sed -e "s|\VERSION = ||")
    CHOLMOD_MAJOR_VERSION=$(echo "$CHOLMOD_VERSION" | awk -F \. '{print $1}')
    cd CHOLMOD/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libcholmod.so.${CHOLMOD_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libcholmod.so.${CHOLMOD_VERSION}" \
        $(ls ./*.o)
    rm -f "$INSTALLUSRDIR"/lib/libcholmod.so*
    cp "libcholmod.so.${CHOLMOD_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libbtf.so
    BTF_VERSION=$(grep -m1 VERSION BTF/Makefile | sed -e "s|\VERSION = ||")
    BTF_MAJOR_VERSION=$(echo "$BTF_VERSION" | awk -F \. '{print $1}')
    cd BTF/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libbtf.so.${BTF_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libbtf.so.${BTF_VERSION}" \
        $(ls ./*.o)
    rm -f "$INSTALLUSRDIR"/lib/libbtf.so*
    cp "libbtf.so.${BTF_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libklu.so
    KLU_VERSION=$(grep -m1 VERSION KLU/Makefile | sed -e "s|\VERSION = ||")
    KLU_MAJOR_VERSION=$(echo "$KLU_VERSION" | awk -F \. '{print $1}')
    cd KLU/Lib/ || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libklu.so.${KLU_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libklu.so.${KLU_VERSION}" \
        $(ls ./*.o) \
        "$INSTALLUSRDIR/lib/libamd.so.${AMD_VERSION}" "$INSTALLUSRDIR/lib/libcolamd.so.${COLAMD_VERSION}" \
        "$INSTALLUSRDIR/lib/libbtf.so.${BTF_VERSION}"
    rm -f "$INSTALLUSRDIR"/lib/libklu.so*
    cp "libklu.so.${KLU_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    # libumfpack.so
    UMFPACK_VERSION=$(grep -m1 VERSION UMFPACK/Makefile | sed -e "s|\VERSION = ||")
    UMFPACK_MAJOR_VERSION=$(echo "$UMFPACK_VERSION" | awk -F \. '{print $1}')
    cd UMFPACK/Lib || exit 1
    # shellcheck disable=SC2046
    gcc -shared \
        "-Wl,-soname,libumfpack.so.${UMFPACK_MAJOR_VERSION}" \
        -Wl,-rpath='$ORIGIN' \
        -o "libumfpack.so.${UMFPACK_VERSION}" \
        $(ls ./*.o) \
        "$INSTALL_DIR/lib/libsuitesparseconfig.a" \
        /usr/local/lib/libscigfortran.so.5 /usr/local/lib/libsciquadmath.so.0 \
        "$INSTALLUSRDIR/lib/libblas.so.3" "$INSTALLUSRDIR/lib/liblapack.so.3" -lm -lrt \
        "$INSTALLUSRDIR/lib/libcholmod.so.${CHOLMOD_VERSION}" "$INSTALLUSRDIR/lib/libcolamd.so.${COLAMD_VERSION}" \
        "$INSTALLUSRDIR/lib/libccolamd.so.${CCOLAMD_VERSION}" "$INSTALLUSRDIR/lib/libcamd.so.${CAMD_VERSION}" \
        "$INSTALLUSRDIR/lib/libbtf.so.${BTF_VERSION}" "$INSTALLUSRDIR/lib/libklu.so.${KLU_VERSION}" \
        "$INSTALLUSRDIR/lib/libamd.so.${AMD_VERSION}"
    rm -f "$INSTALLUSRDIR"/lib/libumfpack.so*
    cp "libumfpack.so.${UMFPACK_VERSION}" "$INSTALLUSRDIR/lib/"
    cd ../..

    cd "$INSTALLUSRDIR/lib/" || exit 1
    ln -fs "libamd.so.${AMD_VERSION}" libamd.so
    ln -fs "libamd.so.${AMD_VERSION}" "libamd.so.${AMD_MAJOR_VERSION}"
    ln -fs "libcamd.so.${CAMD_VERSION}" libcamd.so
    ln -fs "libcamd.so.${CAMD_VERSION}" "libcamd.so.${AMD_MAJOR_VERSION}"
    ln -fs "libcolamd.so.${COLAMD_VERSION}" libcolamd.so
    ln -fs "libcolamd.so.${COLAMD_VERSION}" "libcolamd.so.${COLAMD_MAJOR_VERSION}"
    ln -fs "libccolamd.so.${CCOLAMD_VERSION}" libccolamd.so
    ln -fs "libccolamd.so.${CCOLAMD_VERSION}" "libccolamd.so.${CCOLAMD_MAJOR_VERSION}"
    ln -fs "libcholmod.so.${CHOLMOD_VERSION}" libcholmod.so
    ln -fs "libcholmod.so.${CHOLMOD_VERSION}" "libcholmod.so.${CHOLMOD_MAJOR_VERSION}"
    ln -fs "libbtf.so.${BTF_VERSION}" libbtf.so
    ln -fs "libbtf.so.${BTF_VERSION}" "libbtf.so.${BTF_MAJOR_VERSION}"
    ln -fs "libklu.so.${KLU_VERSION}" libklu.so
    ln -fs "libklu.so.${KLU_VERSION}" "libklu.so.${KLU_MAJOR_VERSION}"
    ln -fs "libumfpack.so.${UMFPACK_VERSION}" libumfpack.so
    ln -fs "libumfpack.so.${UMFPACK_VERSION}" "libumfpack.so.${UMFPACK_MAJOR_VERSION}"
}

build_gluegen() {
    cd "$BUILDDIR" || exit 1

    tar -xJf "$DOWNLOADDIR/gluegen-v$JOGL_VERSION.tar.xz"
    cd gluegen-v$JOGL_VERSION || exit 1
    tar -xJf "$DOWNLOADDIR/jcpp-v$JOGL_VERSION.tar.xz"
    rm -fr jcpp && mv jcpp-v$JOGL_VERSION jcpp

    cd make || exit 1
    "$ANT_HOME/bin/ant" || exit 1

    cd "$BUILDDIR/gluegen-v$JOGL_VERSION" || exit 1
    cp -a build/obj/libgluegen_rt.so "$INSTALLROOTDIR/lib/thirdparty"
    cp -a build/gluegen-rt.jar "$INSTALLROOTDIR/thirdparty"
}

build_jogl() {
    cd "$BUILDDIR" || exit 1

    tar -xJf "$DOWNLOADDIR/jogl-v$JOGL_VERSION.tar.xz"
    ln -fs gluegen-v$JOGL_VERSION gluegen

    cd jogl-v$JOGL_VERSION/make || exit 1

    "$ANT_HOME/bin/ant" || exit 1

    cd "$BUILDDIR/jogl-v$JOGL_VERSION" || exit 1
    cp -a -t "$INSTALLROOTDIR/lib/thirdparty" build/lib/libjogl_desktop.so build/lib/libnativewindow_awt.so build/lib/libnativewindow_drm.so build/lib/libnativewindow_x11.so build/lib/libnewt_drm.so build/lib/libnewt_head.so
    cp -a build/jar/jogl-all.jar "$INSTALLROOTDIR/thirdparty"
}

build_libxslt() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/libxslt-$LIBXSLT_VERSION/install_dir

    tar -xJf "$DOWNLOADDIR/libxslt-$LIBXSLT_VERSION.tar.xz"
    cd "libxslt-$LIBXSLT_VERSION" || exit 1

    ./configure --prefix= --without-python CFLAGS="-I$INSTALLUSRDIR/include/libxml2" LDFLAGS="-L$INSTALLUSRDIR/lib"
    make "-j$(nproc)"
    make install DESTDIR="$INSTALL_DIR"

    cp -a "$INSTALL_DIR"/lib/*.so* "$INSTALLUSRDIR/lib/"
    cp -a "$INSTALL_DIR"/include/* "$INSTALLUSRDIR/include/"
}

build_cppserver() {
    cd "$BUILDDIR" || exit 1

    INSTALL_DIR=$BUILDDIR/cppserver-$CPPSERVER_VERSION/install_dir

    rm -rf cppserver
    unzip -q "$DOWNLOADDIR/CppServer-$CPPSERVER_VERSION.zip"
    mv CppServer-$CPPSERVER_VERSION cppserver
    cd cppserver || exit 1
    # add cmake files
    unzip -q "$DOWNLOADDIR/CppCMakeScripts-$CPPCMAKESCRIPTS_VERSION.zip"
    mv CppCMakeScripts-* cmake

    # add cppserver needed modules
    cd modules || exit 1
    unzip -q "$DOWNLOADDIR/asio-$ASIO_VERSION.zip"
    mv asio-asio-"$(echo $ASIO_VERSION | tr "." "-")" asio
    unzip -q "$DOWNLOADDIR/CppCommon-$CPPCOMMON_VERSION.zip"
    mv CppCommon-$CPPCOMMON_VERSION CppCommon
    cd CppCommon/modules || exit 1
    unzip -q "$DOWNLOADDIR/fmt-$FMT_VERSION.zip"
    mv fmt-$FMT_VERSION fmt
    cd ../../../

    # link against our openssl
    export OPENSSL_ROOT_DIR="$INSTALLUSRDIR"

    ln -s "$BUILDDIR/cppserver/cmake" modules/CppCommon/cmake

    # update cmake files to include only needed sources
    sed -i '/Catch2/s/^/#/' modules/CMakeLists.txt
    sed -i '/CppBenchmark/s/^/#/' modules/CMakeLists.txt
    sed -i '/cpp-optparse/s/^/#/' modules/CMakeLists.txt
    sed -i '/Catch2/s/^/#/' modules/CppCommon/modules/CMakeLists.txt
    sed -i '/CppBenchmark/s/^/#/' modules/CppCommon/modules/CMakeLists.txt
    sed -i '/vld/s/^/#/' modules/CppCommon/modules/CMakeLists.txt

    # generate makefile
    mkdir -p build
    cd build || exit 1
    # DCPPSERVER_MODULE: remove execution of benchmarks and tests from build
    cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DBUILD_SHARED_LIBS=ON \
        -DCPPSERVER_MODULE=true \
        -G "Unix Makefiles"
    # build cppserver
    cmake --build . --parallel --config Release

    # copy libs
    cd ../
    cp -- $(find . -name *.so) "$INSTALLUSRDIR/lib/"

    # copy includes
    rm -rf "$INSTALLUSRDIR/include/cppserver/"
    rm -rf "$INSTALLUSRDIR/include/cppcommon/"
    rm -rf "$INSTALLUSRDIR/include/asio/"
    rm -rf "$INSTALLUSRDIR/include/fmt/"
    mkdir -p "$INSTALLUSRDIR/include/cppserver"
    mkdir -p "$INSTALLUSRDIR/include/cppcommon"
    mkdir -p "$INSTALLUSRDIR/include/asio"
    mkdir -p "$INSTALLUSRDIR/include/fmt"

    cp -r include/server "$INSTALLUSRDIR/include/cppserver"

    rm modules/asio/asio/include/Makefile.am
    cp -r modules/asio/asio/include/* "$INSTALLUSRDIR/include/asio/"

    cp -r modules/CppCommon/include/* "$INSTALLUSRDIR/include/cppcommon/"
    cp -r modules/CppCommon/modules/fmt/include/* "$INSTALLUSRDIR/include/fmt/"
}

#########################
##### DEFAULT FLAGS #####
#########################
export CFLAGS="-O2 -g"
export CXXFLAGS="-O2 -g"
export FFLAGS="-O2 -g"
export LDFLAGS="-O2 -g"

###############################
##### ARGUMENT MANAGEMENT #####
###############################
while [ $# -gt 0 ]
do
  case "$1" in
    "versions")
        make_versions
        shift
        ;;

    "fromscratch")
        echo "Create prerequirement folder tree"
        rm -rf "$INSTALLROOTDIR"
        create_folders
        echo "Download prerequirements sources"
        download_dependencies > "$LOGDIR/prebuild_downloads.log" 2>&1 || (tail --lines=100 "$LOGDIR/prebuild_downloads.log"; exit 1)
        echo "Build prerequirements"
        make_all > "$LOGDIR/prebuild_build_all.log" 2>&1 || (tail --lines=100 "$LOGDIR/prebuild_build_all.log"; exit 1)
        echo "Make jar"
        make_jar > "$LOGDIR/prebuild_jar.log" 2>&1 || (tail --lines=100 "$LOGDIR/prebuild_jar.log"; exit 1)
        echo "Create binary"
        make_binary_directory > "$LOGDIR/prebuild_binary.log" 2>&1 || (tail --lines=100 "$LOGDIR/prebuild_binary.log"; exit 1)
        make_archive
        shift
        ;;

    "download")
        create_folders
        download_dependencies
        shift
        ;;

    "binary")
        create_folders
        make_all
        make_binary_directory
        shift
        ;;

    "jar")
        create_folders
        make_jar
        shift
        ;;

    "all")
        create_folders
        make_all
        shift
        ;;

    "archive")
        create_folders
        make_archive
        shift
        ;;
    *)
        if declare -F "build_$1" > /dev/null; then
            echo "Building $1"
        else
            echo "Unknown function: build_$1"
            usage
            exit 1
        fi
        create_folders
        eval "build_$1"
        shift
        ;;
  esac
done
