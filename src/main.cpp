#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dataanalysis.h"

using namespace alglib;


int main(int argc, char **argv)
{

    //real_2d_array xy = "[[1,1],[1,2],[4,1],[2,3],[4,1.5]]";
    double x[3][3] = {{0,3,1},{3,0,3},{1,3,0}};
    real_2d_array xy;
    xy.setlength(3, 3);
    for ( int i = 0; i < 3; i++ ){
        for ( int j = 0; j < 3; j++ ){
            xy(i,j) = x[i][j];
        }
    }
    clusterizerstate s;
    ahcreport rep;
    integer_1d_array cidx;
    integer_1d_array cz;

    clusterizercreate(s);
    clusterizersetdistances(s, xy, true);
    clusterizerrunahc(s, rep);
    clusterizersetahcalgo(s, 1);

    printf("%s\n", rep.z.tostring().c_str());
    return 0;
}


