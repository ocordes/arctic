# written by: Oliver Cordes 2013-01-07
# changed by: Oliver Cordes 2016-01-07
#
#  $Id: Makefile.am 947 2016-01-07 11:19:14Z ocordes $
#
AUTOMAKE_OPTIONS = gnu subdir-objects
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = python_acs

AM_CPPFLAGS = -Wall -g -O2


bin_PROGRAMS = arctic


arctic_SOURCES = src/arctic.cc \
			src/cte_image.cc \
			src/cte_image_euclid.cc \
			src/image.cc \
			src/image_acs.cc \
			src/image_euclid.cc \
			src/image_fits.cc \
			src/julian.cc \
			src/output.cc \
			src/params.cc \
			src/params_acs.cc \
			src/params_euclid.cc \
			src/params_fits.cc \
			src/strstr.cc


include_HEADERS = src/cte_image.hh \
		src/cte_image_euclid.hh \
		src/image.hh \
		src/image_acs.hh \
		src/image_euclid.hh \
		src/image_fits.hh \
		src/julian.hh \
		src/output.hh \
		src/params.hh \
		src/params_acs.hh \
		src/params_euclid.hh \
		src/params_fits.hh \
		src/strstr.hh 


EXTRA_DIST = INSTALL.fink src/config.h.in





