%module arctic

// written by: Oliver Cordes 2017-03-23
// changed by: Oliver Cordes 2017-04-24

%{
#define SWIG_FILE_WITH_INIT
#include "cte_image.hh"
#include "cte_image_neo.hh"
#include "params.hh"
#include "params_fits.hh"
#include "params_acs.hh"
#include "test_valarray.hh"
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


%rename (clock_charge) clock_charge2;

// cte_image class

class cte_image {
public:
  std::shared_ptr<params>              parameters;

  cte_image( void );
  cte_image( std::shared_ptr<params> );
  void     setup( long, long );
  virtual ~cte_image();
  void   clock_charge( std::shared_ptr<std::valarray<double> > );
};

%apply (int DIM1, int DIM2, double* INPLACE_ARRAY2 ) {( size_t dim1, size_t dim2, double *vec )}
%extend cte_image {
  void clock_charge2( size_t dim1_inplace, size_t dim2_inplace, double *vec_inplace )
  {
    std::cout << "Requested size is " << dim1_inplace << "x" << dim2_inplace << std::endl;

    std::shared_ptr<std::valarray<double>> v = std::shared_ptr<std::valarray<double>>( new std::valarray<double>(vec_inplace, dim1_inplace*dim2_inplace ) );

    $self->cte_image::clock_charge( v );

    memcpy( vec_inplace, &((*v)[0]), sizeof( double ) * v->size() );
  }
}

%clear ( size_t dim1, size_t dim2, double *vec );

%apply (int DIM1, int DIM2, double* INPLACE_ARRAY2 ) {(size_t dim1_in, size_t dim2_int, double *vec_in),
                                                      (size_t dim1_out, size_t dim2_out, double *vec_out)}

%extend cte_image {
  void clock_charge3( size_t dim1_in, size_t dim2_in, double *vec_in,
                      size_t dim1_out, size_t dim2_out, double *vec_out )

  {
    std::shared_ptr<std::valarray<double >> v = std::shared_ptr<std::valarray<double>>( new std::valarray<double>(vec_in, dim1_in*dim2_in ) );

    $self->cte_image::clock_charge( v );

    memcpy( vec_out, &((*v)[0]), sizeof( double ) * v->size() );
  }
}

%ignore clock_charge2;

class cte_image_neo : public cte_image {
  public:
    cte_image_neo( void );
    cte_image_neo( std::shared_ptr<params> );

};

class params {
public:
    // CTI variables
    unsigned  int            algorithm;
    double                   well_depth;
    double                   well_notch_depth;
    double                   well_fill_power;
    double                   empty_trap_limit;
    double                   empty_trap_limit_neo2;
    bool                     check_empty_traps;
    unsigned int             n_iterations;
    int                      express;
    int                      n_levels;
    bool                     unclock;
    int                      readout_offset;
    bool                     dark_mode;
    long                     start_x;
    long                     end_x;
    long                     start_y;
    long                     end_y;
    int                      n_species;
    std::valarray<double>    trap_density;
    std::valarray<double>    trap_lifetime;
    bool                     cut_upper_limit;
    double                   upper_limit;
    double                   neo2_split_limit;

    // image variables
    bool                     rotate;
    bool                     direction;
    bool                     force;

    // standard variables
    std::string              working_mode;
    std::string              config_filename;
    std::vector<std::string> config_entries;


    params();
    virtual ~params();

    void load_config( std::string filename );

    virtual void parse_args( std::string &, std::string &, int & );
    void set_args(  int *argc, char **argv[] );
  };

%apply (int DIM1, double* IN_ARRAY1) {(size_t dim1, double* vec1), (size_t dim2, double* vec2 )}
%apply (int DIM1, double* ARGOUT_ARRAY1 ) { ( size_t dim1, double *vec )}
%extend params {
    void set_traps( size_t dim1, double *vec1, size_t dim2, double *vec2 )
    {
      if ( dim1 != dim2 )
      {
        std::cout << "set_traps: array size mismatch! Both arrays need to have the same size!" << std::endl;
      }
      else
      {
        $self->trap_density  = std::valarray<double>( vec1, dim1 );
        $self->trap_lifetime = std::valarray<double>( vec2, dim2 );
        $self->n_species     = dim1;
      }
    }

   void get_trap_density( size_t dim1, double *vec )
   {
     int i;

     for (i=0;i<dim1;++i)
       vec[i] = $self->trap_density[i];
   }

   void get_trap_lifetime( size_t dim1, double *vec )
   {
     int i;

     for (i=0;i<dim1;++i)
       vec[i] = $self->trap_lifetime[i];
   }

}

%clear (size_t dim1, double* vec1), (size_t dim2, double* vec2);
%clear (size_t dim1, double *vec);

class params_fits : public params {
  public:
    // variables
    int     static_trap_definitions;
    // functions
    params_fits();
    virtual void parse_args( std::string & key, std::string & val, int & error );
    virtual void check_params ( void );
};


class params_acs : public params_fits {
  public:
    // variables

    // functions
    params_acs();
    virtual void parse_args( std::string &, std::string &, int & );

    void calc_trap_config( double date );
    virtual void check_params ( void );
};



%apply (int DIM1, int* INPLACE_ARRAY1) {(size_t len_, int* vec_)}

void test_function( std::valarray<int> & );

%rename (test_function) my_test_function2;
%inline %{
void my_test_function(size_t len_, int* vec_) {
    std::valarray<int> v( vec_, len_ );

    test_function(v);

    // back copy ...
    //for( unsigned int i=0;i<v.size();++i)
    //  vec_[i] = v[i];
    memcpy( vec_, &v[0], sizeof( int ) * v.size() );
}
%}

void my_test_function(size_t len_, int* vec_);
