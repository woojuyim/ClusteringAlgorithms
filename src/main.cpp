#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "dataanalysis.h"

using namespace alglib;


int main(int argc, char **argv)
{

    //real_2d_array xy = "[[1,1],[1,2],[4,1],[2,3],[4,1.5]]";
    double x[6][2] = {{0,3},{3,0},{7,1},{2,2},{1,2},{2,0}};
    real_2d_array xy;
    xy.setlength(6, 2);
    for ( int i = 0; i < 6; i++ ){
        for ( int j = 0; j < 2; j++ ){
            xy(i,j) = x[i][j];
        }
    }
    clusterizerstate s;
    ahcreport rep;
    integer_1d_array cidx;
    integer_1d_array cz;

    clusterizercreate(s);
    //clusterizersetdistances(s, xy, true);
    clusterizersetpoints(s, xy, 2);
    clusterizerrunahc(s, rep);
    clusterizersetahcalgo(s, 1);
    clusterizergetkclusters(rep, 2, cidx, cz);

    printf("%s\n", cidx.tostring().c_str()); // EXPECTED: [0,1,2,3,4]

    // printf("%s\n", rep.z.tostring().c_str());

    // for ( int i = 0; i < 6; i++ ){
    //     for ( int j = 0; j < 2; j++ ){
    //         xy(i,j) = rep.z[i][j];
    //     }
    // }

    //Last item is main cluster
    //If cluster number is > row_length, there's a cluster below
    // for(int i = 5; i > 0; --i){
    //     std::cout << x[i];
    // }

    return 0;
}


