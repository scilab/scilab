#  Scicos
#
#  Copyright (C) DIGITEO - 2010 - Allan CORNET
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file ./license.txt

MAKE=nmake /f makefile.mak

all::
	@cd compilation
	@$(MAKE) all
	@cd ..
	
	@cd instantiation
	@$(MAKE) all
	@cd ..	
	
	@cd exceptionHandling
	@$(MAKE) all
	@cd ..	
	
	@cd translation
	@$(MAKE) all
	@cd ..	
	
clean::
	@cd compilation
	@$(MAKE) clean
	@cd ..
	
	@cd instantiation
	@$(MAKE) clean
	@cd ..	
	
	@cd exceptionHandling
	@$(MAKE) clean
	@cd ..	
	
	@cd translation
	@$(MAKE) clean
	@cd ..	
	
distclean::	
	@cd compilation
	@$(MAKE) distclean
	@cd ..
	
	@cd instantiation
	@$(MAKE) distclean
	@cd ..	
	
	@cd exceptionHandling
	@$(MAKE) distclean
	@cd ..	
	
	@cd translation
	@$(MAKE) distclean
	@cd ..	
	