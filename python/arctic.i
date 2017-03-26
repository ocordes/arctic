%module arctic

// written by: Oliver Cordes 2017-03-23
// changed by: Oliver Cordes 2017-03-26

%{
#define SWIG_FILE_WITH_INIT
#include "cte_image.hh"
#include "params.hh"
#include "params_fits.hh"
#include "test_valarray.hh"
%}

%include "std_string.i"

%include "numpy.i"

%init %{
import_array();
%}


// Very simple C++ example for linked list

class cte_image {
public:
  cte_image( std::shared_ptr<params> );
};

class params_fits {
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


    params_fits();
    virtual ~params_fits();

    virtual void parse_args( std::string &, std::string &, int & );
    void set_args(  int *argc, char **argv[] );
  };


%apply (int DIM1, int* IN_ARRAY1) {(size_t len_, int* vec_)}

void test_function( std::valarray<int> & );

%rename (test_function) my_test_function;
%inline %{
void my_test_function(size_t len_, int* vec_) {
    std::valarray<int> v;

    test_function(v);
}
%}

void my_test_function(size_t len_, int* vec_);
