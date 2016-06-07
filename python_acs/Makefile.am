# Makefile.am for python_arctic

# written by: Oliver Cordes 2016-01-07
# changed by: Oliver Cordes 2016-01-07


PYTHON_TOOLS=arctic_acs
${PYTHON_TOOLS}: python_wrapper
		cp $< $@

BUILT_SOURCES = ${PYTHON_TOOLS}
CLEANFILES = ${PYTHON_TOOLS}
bin_SCRIPTS = ${PYTHON_TOOLS}


pythonlibdir=$(libdir)/python


pythonlib_DATA= \
		arctic_acs.py \
		acs_image.py \
		arctic_prg.py 

pythonlib_SCRIPTS= \
                arctic_acs.py

EXTRA_DIST = ${pythonlib_DATA}
