#
#   Translator from Modelica 2.x to flat Modelica
# 
#   Copyright (C) 2005 – 2007 Imagine S.A.
#   For more information or commercial use please contact us at www.amesim.com
# 
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
# 
#

all: release


release: 
	$(MAKE) opt RELEASE=yes

debug:
	$(MAKE) bytecode RELEASE=no

depend:
	$(MAKE) depends RELEASE=yes

depends:
	cd parsing ; $(MAKE) depend 
	cd compilation ; $(MAKE) depend
	cd instantiation ; $(MAKE) depend
	cd exceptionHandling ; $(MAKE) depend
	cd translation ; $(MAKE) depend

opt: 
	$(MAKE) depend
	cd parsing ; $(MAKE)  opt  RELEASE=$(RELEASE)
	cd compilation ; $(MAKE)  opt RELEASE=$(RELEASE)
	cd instantiation ; $(MAKE)  opt RELEASE=$(RELEASE)
	cd exceptionHandling ; $(MAKE) opt RELEASE=$(RELEASE)
	cd translation ; $(MAKE) opt RELEASE=$(RELEASE)

custom: 
	$(MAKE) depend
	cd parsing ; $(MAKE)  bytecode RELEASE=$(RELEASE)
	cd compilation ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd instantiation ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd exceptionHandling ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd translation ; $(MAKE)  custom RELEASE=$(RELEASE)

bytecode: 
	$(MAKE) depend
	cd parsing ; $(MAKE)  bytecode RELEASE=$(RELEASE)
	cd compilation ; $(MAKE)  bytecode RELEASE=$(RELEASE)
	cd instantiation ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd exceptionHandling ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd translation ; $(MAKE)  bytecode RELEASE=$(RELEASE)

clean:
	cd parsing ; $(MAKE) clean
	cd compilation ; $(MAKE) clean
	cd instantiation ; $(MAKE) clean
	cd exceptionHandling ; $(MAKE) clean
	cd translation ; $(MAKE) clean
