

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>

#include <iostream>
#include <memory>


int main( int argc, char *argv[] )
{
int prec = 15;
    double d = 0.12345678987654321;

std::cout << std::fixed << std::setprecision( prec ) << "d=" << d << std::endl;


double b = 0.0;

int count = atoi( argv[1] );

for (int i=0;i<count;i++)
  b += d;

std::cout << std::fixed << std::setprecision( prec ) << "b=" << b << std::endl;

double c = d;
c *= count;

std::cout << std::fixed << std::setprecision( prec ) << "c=" << c << std::endl;

return 0;
}
