/* definition of the cte_image class

written by: Oliver Cordes 2017-03-23
changed by: Oliver Cordes 2017-07-19

*/

class params_fits : public params {
  public:
    // variables
    int     static_trap_definitions;
    // functions
    params_fits();
    virtual void check_params ( void );
};
