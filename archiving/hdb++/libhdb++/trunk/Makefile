
include ../../Make-hdb++.in

CXXFLAGS += -Wall -DRELEASE='"N$Name:  $ "' -I$(TANGO_INC) -I$(OMNI_INC)
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
	$(CXX) $(CXXFLAGS) -fPIC -c src/LibHdb++.cpp -o $@

clean:
	rm -f obj/*.o lib/*.so* lib/*.a

lib obj:
	@mkdir $@
