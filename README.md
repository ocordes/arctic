# arctic
artic - Algorithm foR Charge Transfer Inefficency Correction (Astronomy)


The arctic (Algorithm foR CTI Correction ) (formerly named as acs-cte) project
was founded to rewrite an existing IDL program to clear the artefacts of the CTI
of the ACS camera (Advanced Camera for Surveys) which is installed in the
Hubble Space Telescope (HST). Details can be found on the original webpage by
[Richard Massey](http://community.dur.ac.uk/r.j.massey/acs/CTE/).

The new code can now also be used for all CCD images which have CTI artefacts.
In 2016 the main focus of the development switched from HST to the Euclid project.



Installation/Compilation:

After download of the source tree, please change into the arctic directory:

CMAKE tools:

```
mkdir build; cd build
cmake ..

make

make Install
```
-----

automake/autoconf tools (**_DEPRECATED_**):

```
mkdir build; cd build
../configure

make

make install
```
