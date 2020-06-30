#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dataanalysis.h"

using namespace alglib;


int main(int argc, char **argv)
{

    //real_2d_array xy = "[[1,1],[1,2],[4,1],[2,3],[4,1.5]]";
    double x[5][2] = {{1,1},{1,2},{4,1},{2,3},{4,1.5}};
    real_2d_array xy;
    xy.setlength(5, 2);
    for ( int i = 0; i < 5; i++ ){
        for ( int j = 0; j < 2; j++ ){
            xy(i,j) = x[i][j];
        }
    }
    clusterizerstate s;
    ahcreport rep;
    integer_1d_array cidx;
    integer_1d_array cz;

    clusterizercreate(s);
    clusterizersetpoints(s, xy, 2);
    clusterizerrunahc(s, rep);

    // with K=5, every points is assigned to its own cluster:
    // C0=P0, C1=P1 and so on...
    clusterizergetkclusters(rep, 5, cidx, cz);
    printf("%s\n", cidx.tostring().c_str()); // EXPECTED: [0,1,2,3,4]

    // with K=1 we have one large cluster C0=[P0,P1,P2,P3,P4,P5]
    clusterizergetkclusters(rep, 1, cidx, cz);
    printf("%s\n", cidx.tostring().c_str()); // EXPECTED: [0,0,0,0,0]

    // with K=3 we have three clusters C0=[P3], C1=[P2,P4], C2=[P0,P1]
    clusterizergetkclusters(rep, 3, cidx, cz);
    printf("%s\n", cidx.tostring().c_str()); // EXPECTED: [2,2,1,0,1]
    return 0;
}


