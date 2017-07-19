/* definition of the cte_image class

written by: Oliver Cordes 2017-03-23
changed by: Oliver Cordes 2017-07-19

*/

#define PARSE_OK            0
#define PARSE_UNKNOWN       1
#define PARSE_ERROR         100


// algorithm definitions
#define ALGORITHM_CLASSIC  0
#define ALGORITHM_NEO      1
#define ALGORITHM_NEO2     2

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
  bool                     charge_injection;
  unsigned int             n_iterations;
  unsigned int             express;
  unsigned int             n_levels;
  bool                     unclock;
  unsigned int             readout_offset;
  bool                     dark_mode;
  long                     start_x;
  long                     end_x;
  long                     start_y;
  long                     end_y;
  unsigned int             n_species;
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

  int load_config( std::string filename );

};


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
    {
      vec[i] = $self->trap_density[i];
    }
  }

  void get_trap_lifetime( size_t dim1, double *vec )
  {
    int i;

    for (i=0;i<dim1;++i)
    {
      vec[i] = $self->trap_lifetime[i];
    }
  }

}
