acs-cte Configuration file
=====================


Example 
--------

Here is an example of a test configuration file: ::

   WELL_DEPTH = 84700.0
   WELL_NOTCH_DEPTH = 1e-9  #  old value  96.5 
   WELL_FILL_POWER = 0.465
   #XRANGE = 1,50
   #YRANGE = 1,2
   N_ITERATIONS = 1
   EXPRESS = 1
   #N_LEVELS=2500
   ACS_MODE=Y
   N_SPECIES=1
   TRAP_DENSITY=0.1
   TRAP_LIFETIME=0.1


This configuration enables the default ACS mode to calculate the trap behaviour from the observing date of the file to be corrected. However, additionally a simple trap definition is given in the file but deactivated by the ``ACS_MODE=Y`` line.


Keywords
---------

.. index:: WELL_DEPTH
* ``WELL_DEPTH``:  full well value of the CCD  (double)
.. index:: WELL_NOTCH_DEPTH
* ``WELL_NOTCH_DEPTH``: 
.. index:: WELL_FILL_POWER
* ``WELL_FILL_POWER``: power fo fill a trap
.. index:: XRANGE
* ``XRANGE``: columns to correct starting with 1 (integer)
.. index:: YRANGE
* ``YRANGE``:: rows to correct starting with 1 (integer)
.. index:: N_ITERATIONS
* ``N_ITERATIONS``: number of iterations during trail calculations (integer)
.. index:: EXPRESS
* ``EXPRESS``: express level of optimazation (integer)
.. index:: N_LEVELS
* ``N_LEVELS`` : number of trap levels in the corrections routines
.. index:: ACS_MODE
* ``ACS_MODE``: ``=Y`` (default) uses the internal model for the trap definitions, ``=N`` expects a trap definition in the config file
.. index:: N_SPECIES
* ``N_SPECIES``: number of defined trap sepcies in the non-ACS_MODE (integer)
.. index:: TRAP_DENSITY
* ``TRAP_DENSITY``: trap densitiy in the non-ACS_MODE (comma separated list of ``N_SPECIES`` doubles )
.. index:: TRAP_LIFETIME
* ``TRAP_LIFETIME``: trap lifetime in the non-ACS_MODE (comma separated list of ``N_SPECIES`` doubles )
   
