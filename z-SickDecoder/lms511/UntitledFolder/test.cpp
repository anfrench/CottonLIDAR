#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
#include "interp.hpp"


using namespace std;

void test04 ( int data_num );
double *f_runge ( int m, int n, double x[] );

int main()
{
	test04 ( 101);
}

void test04 ( int data_num )

//****************************************************************************80
//
//  Purpose:
//
//    TEST04 tests INTERP_LAGRANGE on 1-dimensional data, Clenshaw-Curtis data.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 March 2014
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DATA_NUM, the number of data values.
//
{
  int after;
  int before;
  int fat;
  int i;
  int interp;
  int interp_num;
  int j;
  double p;
  double *p_data;
  double *p_interp;
  double t;
  double *t_data;
  double *t_interp;
  double t_max;
  double t_min;

  cout << " \n";
  cout << "TEST04\n";
  cout << "  INTERP_LAGRANGE evaluates a polynomial interpolant.\n";
  cout << " \n";
  cout << "  In this example, the function we are interpolating is\n";
  cout << "  Runge's function, with Clenshaw Curtis knots.\n";

  t_min = -1.0;
  t_max = +1.0;

  t_data = cc_abscissas_ab ( t_min, t_max, data_num );

  p_data = f_runge ( 1, data_num, t_data );

  cout << "\n";
  cout << "  The data to be interpolated:\n";
  cout << "\n";
  cout << "  Number of data values = " << data_num << "\n";
  cout << "\n";
  cout << "       T_data        P_data\n";
  cout << "\n";
  for ( j = 0; j < data_num; j++ )
  {
    cout << "  " << setw(14) << t_data[j]
         << "  " << setw(14) << p_data[0+j] << "\n";
  }
//
//  Our interpolation values will include the original T values, plus
//  3 new values in between each pair of original values.
//
  before = 4;
  fat = 3;
  after = 2;

  interp_num = before + 1 + ( data_num - 1 ) * ( fat + 1 ) + after;

  t_interp = r8vec_expand_linear2 ( data_num, t_data, before, fat, after );

  p_interp = interp_lagrange ( 1, data_num, t_data, p_data, interp_num, 
    t_interp );

  

  cout << "\n";
  cout << "  Interpolation:\n";
  cout << "\n";
  cout << "    T_interp      P_interp   \n";
  cout << "\n";

  for ( j = 0; j < interp_num; j++ )
  {
    cout << "  " << setw(10) << t_interp[j]
         << "  " << setw(14) << p_interp[0+j]<<endl;
  }

  delete [] p_data;
  delete [] p_interp;
  delete [] t_data;
  delete [] t_interp;

  return;
}





double *f_runge ( int m, int n, double x[] )
{
  double *f;
  int i;
  int j;
  double t;

  f = new double[n];

  for ( j = 0; j < n; j++ )
  {
    t = 0.0;
    for ( i = 0; i < m; i++ )
    {
      t = t + pow ( x[i+j*m], 2 );
    }
    f[j] = 1.0 / ( 1.0 + 25.0 * t );
  }

  return f;
}




























