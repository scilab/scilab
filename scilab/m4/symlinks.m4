
dnl ===============================================================
dnl FOLLOW_SYMLINKS <path>
dnl Follows symbolic links on <path>,
dnl
dnl Arguments:
dnl the file which want to resolve the real file
dnl the real name of the file (for example, if we are looking for javac and 
dnl it is a symlink to /usr/bin/gcj, we want to stay in the java dir where
dnl javac is found
dnl 
dnl VARIABLES SET:
dnl SYMLINK_FOLLOWED_TO the "real" file
dnl ===============================================================

AC_DEFUN([FOLLOW_SYMLINKS],[
	
	dnl find the include directory relative to the executable
	_cur="$1"
	if test ! -z "$2"; then
		_fileNameWanted=$2
	else
		_fileNameWanted=$1
	fi
	while ls -ld "$_cur" 2>/dev/null | grep " -> " >/dev/null; do
		AC_MSG_CHECKING(Symlink for $_cur)

		_slink=`ls -ld "$_cur" | sed 's/.* -> //'`	
		if test "$_fileNameWanted" != "`basename $_slink`"; then
			AC_MSG_RESULT(Filename changed... Keeping the one found before)
			break
		fi
		case "$_slink" in
			/*)
				_cur="$_slink";;
			# 'X' avoids triggering unwanted echo options.
			*)
				_cur=`echo "X$_cur" | sed -e 's/^X//' -e 's:[[^/]]*$::'`"$_slink";;
		esac
		AC_MSG_RESULT($_cur)
	done
	SYMLINK_FOLLOWED_TO="$_cur"
])# FOLLOW_SYMLINKS

