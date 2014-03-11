
#OMNI_INC = /usr/local/omniorb-4.1.6/include
#TANGO_INC = /usr/local/tango-8.1.2.c/include/tango
include ../../Make-hdb++.in

CFLAGS = -Wall -DRELEASE='"N$Name:  $ "' -I$(TANGO_INC) -I$(OMNI_INC)  -std=gnu++98 -D_MYSQL
#CFLAGS = -Wall -DRELEASE='"N$Name:  $ "' -I$(TANGO_INC) -I$(OMNI_INC)  -std=gnu++0x -D_MYSQL
CXX = g++


##############################################
# support for shared libray versioning
#
LFLAGS_SONAME = -Wl,-soname,
SHLDFLAGS = -shared
BASELIBNAME       =  libhdb++
SHLIB_SUFFIX = so

#  release numbers for libraries
#
 LIBVERSION    = 1
 LIBRELEASE    = 0
 LIBSUBRELEASE = 0
#

LIBRARY       = $(BASELIBNAME).a
DT_SONAME     = $(BASELIBNAME).$(SHLIB_SUFFIX).$(LIBVERSION)
DT_SHLIB      = $(BASELIBNAME).$(SHLIB_SUFFIX).$(LIBVERSION).$(LIBRELEASE).$(LIBSUBRELEASE)
SHLIB         = $(BASELIBNAME).$(SHLIB_SUFFIX)



.PHONY : install clean

lib/LibHdb++: lib obj obj/LibHdb++.o
	$(CXX) obj/LibHdb++.o $(SHLDFLAGS) $(LFLAGS_SONAME)$(DT_SONAME) -l$(LIBHDBIMPL) -L$(LIBHDBIMPL_LIB) -o lib/$(DT_SHLIB)
	ln -sf $(DT_SHLIB) lib/$(SHLIB)
	ln -sf $(SHLIB) lib/$(DT_SONAME)
	ar rcs lib/$(LIBRARY) obj/LibHdb++.o

obj/LibHdb++.o: src/LibHdb++.cpp src/LibHdb++.h
	$(CXX) $(CFLAGS) -fPIC -c src/LibHdb++.cpp -o $@

clean:
	rm -f obj/*.o lib/*.so* lib/*.a

lib obj:
	@mkdir $@
