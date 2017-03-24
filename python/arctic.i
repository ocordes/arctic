%module arctic
%{
#include "cte_image.hh"
#include "params.hh"
#include "params_fits.hh"
%}

%include "std_string.i"

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
  std::valarray<long>      xrange;
    std::valarray<long>      yrange;
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
