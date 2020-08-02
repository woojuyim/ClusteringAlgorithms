#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "dataanalysis.h"
#include "node.h"
#include <stack>

using namespace alglib;
using namespace std;

void recursive(integer_2d_array &arr, int currIndex, Node *currNode, stack<int> &indexes);
void inOrder(Node *node);
void deleteTree(Node *node);

int main(int argc, char **argv)
{

    //real_2d_array xy = "[[1,1],[1,2],[4,1],[2,3],[4,1.5]]";
    int row = 6;
    double x[row][2] = {{0, 3}, {3, 0}, {7, 1}, {2, 2},{4,5},{6,2}};
    //{1,2},{2,0}};
    real_2d_array xy;
    xy.setlength(row, 2);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            xy(i, j) = x[i][j];
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
    //clusterizergetkclusters(rep, 3, cidx, cz);

    //printf("%s\n", cidx.tostring().c_str()); // EXPECTED: [0,1,2,3,4]

    printf("%s\n", rep.z.tostring().c_str());

    std::stack<int> indexes;

    Node *root = new Node(-1, nullptr, nullptr, nullptr);
    recursive(rep.z, rep.z.rows() - 1, root, indexes);


    //inOrder(root);


    //cout << root->rightChild->rightChild->leftChild->val;
    deleteTree(root);

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

void inOrder(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    inOrder(node->leftChild);
    cout << node->val << " ";
    inOrder(node->rightChild);

}

void deleteTree(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->leftChild);
    deleteTree(node->rightChild);
    delete node;
}

void recursive(integer_2d_array &arr, int currIndex, Node *currNode, stack<int> &indexes)
{
    if (currIndex < 0)
    {
        return;
    }

    bool leftChild = arr[currIndex][0] > arr.rows();
    bool rightChild = arr[currIndex][1] > arr.rows();
    Node *left = new Node(arr[currIndex][0], currNode, nullptr, nullptr);
    Node *right = new Node(arr[currIndex][1], currNode, nullptr, nullptr);
    currNode->leftChild = left;
    currNode->rightChild = right;

    //leftchild
    if (leftChild)
    {
        //cout << currIndex << " ";
        recursive(arr, currIndex - 2, left, indexes);
    }
    //rightchild
    if (rightChild)
    {
        //cout << currIndex << " ";
        recursive(arr, currIndex - 1, right, indexes);
    }
}
