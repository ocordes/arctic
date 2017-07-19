/* definition of the cte_image class

written by: Oliver Cordes 2017-03-23
changed by: Oliver Cordes 2017-07-19

*/

class params_acs : public params_fits {
  public:
    // variables

    // functions
    params_acs();

    void calc_trap_config( double date );
    virtual void check_params ( void );
};
