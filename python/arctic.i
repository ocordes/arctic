%module arctic

// written by: Oliver Cordes 2017-03-23
// changed by: Oliver Cordes 2017-07-19

%{
#define SWIG_FILE_WITH_INIT
#include "cte_image.hh"
#include "cte_image_neo.hh"
#include "cte_image_classic.hh"
#include "cte_image_watermark.hh"
#include "params.hh"
#include "params_fits.hh"
#include "params_acs.hh"
#include <cstring>
#include <iostream>
%}

%include "std_string.i"
%include <std_shared_ptr.i>

%shared_ptr(params)
%shared_ptr(params_fits)
%shared_ptr(params_acs)

%include "numpy.i"

%init %{
import_array();
%}


%rename (clock_charge) clock_chargeX;

// cte_image class

%apply ( int DIM1, int DIM2, double* INPLACE_ARRAY2 ) { ( size_t dim1_in, size_t dim2_in, double *vec_in ) }
%apply ( int DIM1, int DIM2, double* INPLACE_ARRAY2 ) { ( size_t dim1_in, size_t dim2_in, double *vec_in ),
                                                        ( size_t dim1_out, size_t dim2_out, double *vec_out )}


%include "cte_image.i"
%include "cte_image_neo.i"
%include "cte_image_classic.i"
%include "cte_image_watermark.i"

%apply (int DIM1, double* IN_ARRAY1) {(size_t dim1, double* vec1), (size_t dim2, double* vec2 )}
%apply (int DIM1, double* ARGOUT_ARRAY1 ) { ( size_t dim1, double *vec )}

%include "params.i"
%include "params_fits.i"
%include "params_acs.i"
