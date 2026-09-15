#!/usr/bin/env bash
#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2022 - Dassault Systèmes S.E. - Clément DAVID
#
#
# Builder script for building linux version.
#
# NOTE: log all commands to LOG_PATH to avoid hitting Gitlab log limit
# NOTE: nproc is used to limit memory usage


echo "defined variables"
echo "    CI_PROJECT_DIR:        $CI_PROJECT_DIR"
echo "    ARCH:                  $ARCH"
echo "    SCI_VERSION_STRING:    $SCI_VERSION_STRING"
echo "    CI_COMMIT_SHORT_SHA:   $CI_COMMIT_SHORT_SHA"
echo "    SCI_VERSION_TIMESTAMP: $SCI_VERSION_TIMESTAMP"
echo "    BRANCH:                $BRANCH"
echo ""

LOG_PATH=$SCI_VERSION_STRING
[ ! -d "$LOG_PATH" ] && mkdir "$LOG_PATH"

# checkout pre-requirements
echo -e "\e[0Ksection_start:$(date +%s):prerequirements[collapsed=true]\r\e[0KGetting prerequirements"

rm -f prereq.tar.xz
OVERRIDE_THIRDPARTY=0

# custom build for this commit or tag
mv -f "prerequirements-${SCI_VERSION_STRING}.bin.${ARCH}.tar.xz" "prereq.tar.xz" || true
if [ ! -f "prereq.tar.xz" ]; then
	# custom build for this branch
	cp -a "prerequirements-scilab-branch-${BRANCH}.bin.${ARCH}.tar.xz" "prereq.tar.xz" || true
fi
if [ ! -f "prereq.tar.xz" ]; then
	# download prebuild for the MR branch
	curl -Lk -o "prereq.tar.xz" "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-${CI_MERGE_REQUEST_SOURCE_BRANCH_NAME}.bin.${ARCH}.tar.xz"
	xz -qt "prereq.tar.xz" &>/dev/null || rm -f "prereq.tar.xz"
fi
if [ ! -f "prereq.tar.xz" ]; then
	# download prebuild for the target branch
	curl -Lk -o "prereq.tar.xz" "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-${BRANCH}.bin.${ARCH}.tar.xz"
	xz -qt "prereq.tar.xz" &>/dev/null || rm -f "prereq.tar.xz"
	OVERRIDE_THIRDPARTY=1
fi
if [ ! -f "prereq.tar.xz" ]; then
	# fallback to the default branch
	curl -Lk -o "prereq.tar.xz" "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-${CI_DEFAULT_BRANCH}.bin.${ARCH}.tar.xz"
	xz -qt "prereq.tar.xz" &>/dev/null || rm -f "prereq.tar.xz"
	OVERRIDE_THIRDPARTY=1
fi
if [ ! -f "prereq.tar.xz" ]; then
	# fallback to the default branch main
	curl -Lk -o "prereq.tar.xz" "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-main.bin.${ARCH}.tar.xz"
	xz -qt "prereq.tar.xz" &>/dev/null || rm -f "prereq.tar.xz"
	OVERRIDE_THIRDPARTY=1
fi

# cleanup and extract
git clean -fxd scilab/java scilab/lib scilab/thirdparty scilab/usr scilab/modules/tclsci/tcl
rm -f scilab/svn-info.txt scilab/version.txt
tar -xvf prereq.tar.xz -C scilab >"${LOG_PATH}/build_prereq_${CI_COMMIT_SHORT_SHA}.log" || exit 1
# display svn revision
cat scilab/svn-info.txt || cat scilab/version.txt || exit 1
echo -e "\e[0Ksection_end:$(date +%s):prerequirements\r\e[0K"

# patch thirdparty JARs on WIP Merge-Request
if [ "$OVERRIDE_THIRDPARTY" -ne 0 ]; then
	curl -Lk -o "thirdparty.zip" "https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/thirdparty-scilab-branch-${CI_MERGE_REQUEST_SOURCE_BRANCH_NAME}.zip"
	unzip -qt "thirdparty.zip" || rm -f "thirdparty.zip"
fi
if [ -f "thirdparty.zip" ]; then
	rm -rf scilab/thirdparty/
	mkdir scilab/thirdparty/
	unzip -o thirdparty.zip -d scilab/thirdparty/
fi

# patch version numbers
sed -i \
 -e "s/SCI_VERSION_STRING .*/SCI_VERSION_STRING \"${SCI_VERSION_STRING}\"/" \
 -e "s/SCI_VERSION_WIDE_STRING .*/SCI_VERSION_WIDE_STRING L\"${SCI_VERSION_STRING}\"/" \
 -e "s/SCI_VERSION_REVISION .*/SCI_VERSION_REVISION \"${CI_COMMIT_SHA}\"/" \
 -e "s/SCI_VERSION_TIMESTAMP .*/SCI_VERSION_TIMESTAMP ${SCI_VERSION_TIMESTAMP}/" \
 scilab/modules/core/includes/version.h.in
echo SCIVERSION="${SCI_VERSION_STRING}" >scilab/Version.incl

# predefined env
CCACHE_DIR="${CI_PROJECT_DIR}/ccache"
export CCACHE_DIR
LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(pwd)/scilab/usr/lib/"
export LD_LIBRARY_PATH

# configure (with reconfigure for up to date info)
echo -e "\e[0Ksection_start:$(date +%s):configure[collapsed=true]\r\e[0KConfigure"
cd scilab ||exit 1
./configure --prefix='' --enable-stop-on-warning | tee -a "../${LOG_PATH}/build_configure_${CI_COMMIT_SHORT_SHA}.log"
CONFIGURE_STATUS="${PIPESTATUS[0]}"
cp -a config.log "../${LOG_PATH}/build_config.log_${CI_COMMIT_SHORT_SHA}.log"
if [ "${CONFIGURE_STATUS}" -ne 0 ]; then
	exit "${CONFIGURE_STATUS}"
fi
echo -e "\e[0Ksection_end:$(date +%s):configure\r\e[0K"

# make
echo -e "\e[0Ksection_start:$(date +%s):make\r\e[0KMake"
make --jobs="$(nproc)" all &>>"../${LOG_PATH}/build_make_${CI_COMMIT_SHORT_SHA}.log" ||(tail --lines=100 "../${LOG_PATH}/build_make_${CI_COMMIT_SHORT_SHA}.log"; exit 1)
make help &>"../${LOG_PATH}/build_help_${CI_COMMIT_SHORT_SHA}.log" ||(tail --lines=100 "../$LOG_PATH/build_help_${CI_COMMIT_SHORT_SHA}.log"; exit 1)
echo -e "\e[0Ksection_end:$(date +%s):make\r\e[0K"

# install to tmpdir
echo -e "\e[0Ksection_start:$(date +%s):install\r\e[0KInstall"
make install DESTDIR="/tmp/${SCI_VERSION_STRING}" &>>"../${LOG_PATH}/build_install_${CI_COMMIT_SHORT_SHA}.log" ||(tail --lines=100 "../$LOG_PATH/build_install_${CI_COMMIT_SHORT_SHA}.log"; exit 1)
echo -e "\e[0Ksection_end:$(date +%s):install\r\e[0K"

echo -e "\e[0Ksection_start:$(date +%s):patch[collapsed=true]\r\e[0KPatch binary"
# copy extra files
cp -a ACKNOWLEDGEMENTS "/tmp/${SCI_VERSION_STRING}/"
cp -a CHANGES.md "/tmp/${SCI_VERSION_STRING}/"
cp -a COPYING "/tmp/${SCI_VERSION_STRING}/"
cp -a README.md "/tmp/${SCI_VERSION_STRING}/"

# copy thirdparties
cp -a lib/thirdparty "/tmp/${SCI_VERSION_STRING}/lib/"
cp -r include/* "/tmp/${SCI_VERSION_STRING}/include/"
cp -a thirdparty "/tmp/${SCI_VERSION_STRING}/"
cp -r java/jre/ "/tmp/${SCI_VERSION_STRING}/thirdparty/java"
cp -a modules/tclsci/tcl "/tmp/${SCI_VERSION_STRING}/share/scilab/modules/tclsci/"

# copy gcc libs from docker customs build gcc if available
cp -a /usr/local/lib/libsci*  "/tmp/${SCI_VERSION_STRING}/lib/thirdparty/redist"

# Update the classpath
sed -i "s#$(pwd)#\$SCILAB/../../#g" "/tmp/${SCI_VERSION_STRING}/share/scilab/etc/classpath.xml"

# Update the rpath and ELF NEEDED
cd "/tmp/${SCI_VERSION_STRING}/" ||exit
export PATH="${CI_PROJECT_DIR}/scilab/usr/bin/:$PATH"

# shellcheck disable=SC2016
patchelf --set-rpath '$ORIGIN:$ORIGIN/../lib/scilab:$ORIGIN/../lib/thirdparty:$ORIGIN/../lib/thirdparty/redist' \
	bin/scilab-cli-bin bin/scilab-bin
# shellcheck disable=SC2016
find lib/scilab/*.so* -type f -exec patchelf \
	--set-rpath '$ORIGIN:$ORIGIN/../thirdparty:$ORIGIN/../thirdparty/redist' \
	{} \;

cd "${CI_PROJECT_DIR}" ||exit

echo -e "\e[0Ksection_end:$(date +%s):patch\r\e[0K"

# package as a tar xz file
echo -e "\e[0Ksection_start:$(date +%s):package\r\e[0KPackage"
XZ_OPT="-9T0" && [ "${CI_PIPELINE_SOURCE}" = "merge_request_event" ] && XZ_OPT="-0T0"
export XZ_OPT
tar -cJf "${SCI_VERSION_STRING}.bin.${ARCH}.tar.xz" -C "/tmp" "${SCI_VERSION_STRING}"
echo -e "\e[0Ksection_end:$(date +%s):package\r\e[0K"

# error if artifact does not exist
du -h "${SCI_VERSION_STRING}.bin.${ARCH}.tar.xz"
ccache -s

