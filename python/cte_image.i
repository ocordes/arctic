
/* definition of the cte_image class

written by: Oliver Cordes 2017-03-23
changed by: Oliver Cordes 2017-07-19

*/

class cte_image {
public:
  std::shared_ptr<params>              parameters;

  cte_image( void );
  cte_image( std::shared_ptr<params> );
  void     setup( unsigned int, unsigned int );
  virtual ~cte_image();
  void   clock_charge( std::valarray<double> & );
};

%extend cte_image {
  void clock_chargeX( size_t dim1_in, size_t dim2_in, double *vec_in )
  {
    std::cout << "Requested size is " << dim1_in << "x" << dim2_in << std::endl;

    std::valarray<double> v = std::valarray<double>(vec_in, dim1_in*dim2_in);

    $self->cte_image::clock_charge( v );

    memcpy( vec_in, &(v[0]), sizeof( double ) * v.size() );
  }


  void clock_charge2( size_t dim1_in, size_t dim2_in, double *vec_in,
                      size_t dim1_out, size_t dim2_out, double *vec_out )
  {
    size_t size;

    size = dim1_in * dim2_in;

    std::valarray<double> v = std::valarray<double>(vec_in, size );

    $self->cte_image::clock_charge( v );

    size = dim1_out * dim2_out;
    if ( size > v.size() )
      size = v.size();
    memcpy( vec_out, &(v[0]), sizeof( double ) * size );
  }
}


%ignore clock_chargeX;
