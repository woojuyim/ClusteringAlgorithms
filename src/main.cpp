#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "dataanalysis.h"
#include "node.h"
#include <stack>
#include <queue>

using namespace alglib;

void subcluster(integer_2d_array &arr, int currIndex, Node *headNode);
void recursive(integer_2d_array &arr, int currIndex, Node *currNode);
void inOrder(Node *node);
void deleteTree(Node *node);


int main(int argc, char **argv)
{
    //real_2d_array xy = "[[1,1],[1,2],[4,1],[2,3],[4,1.5]]";
    int row = 6;
    //Corect: 1 8 2 6 5 -1 4 9 0 7 3
    //double x[row][2] = {{0, 3}, {3, 0}, {7, 1}, {2, 2}, {4, 5}, {6, 2}};

    //Correct: 3 -1 1 7 2 9 5 8 0 6 4
    double x[row][2] = {{1, 2}, {0, 1}, {-1, 1}, {-5, 2}, {1, 2}, {3, 2}};

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

    //printf("%s\n", cidx.tostring().c_str());

    printf("%s\n", rep.z.tostring().c_str());

    Node *root = new Node(-1, nullptr, nullptr, nullptr);

    //recursive(rep.z, rep.z.rows() - 1, root);

    subcluster(rep.z, rep.z.rows() - 1, root);

    inOrder(root);
    deleteTree(root);
    return 0;
}

//Use a BFS iterative method
// 1. Input current [int, int] into queue(FIFO)
// 2. Pop and then input child nodes right first, left second
// 3. Continue until queue is empty and arr is empty as well
void subcluster(integer_2d_array &arr, int currIndex, Node *headNode)
{
    std::queue<int*> qe;
    std::queue<Node *> listofNodes;
    const int numofRows = arr.rows();

    qe.push(arr[currIndex]);
    listofNodes.push(headNode);

    while(!qe.empty()){
        int* val = qe.front();
        qe.pop();
        //std::cout << "VAL:" << val[0] << " " << val[1] << std::endl;
        Node* curr = listofNodes.front();
        listofNodes.pop();

        Node* rightHead = new Node(val[1], curr, nullptr, nullptr);
        Node *leftHead = new Node(val[0], curr, nullptr, nullptr);
        curr->rightChild = rightHead;
        curr->leftChild = leftHead;

        //Right Child has a subcluster
        if(val[1] > numofRows){
            qe.push(arr[--currIndex]);
            listofNodes.push(rightHead);
            //std::cout << "RIGHT: " << val[1] << std::endl;
        } 
        //Left Child has a subcluster
        if(val[0] > numofRows){
            qe.push(arr[--currIndex]);
            listofNodes.push(leftHead);
            //std::cout << "LEFT: " << val[0] << std::endl;
        }
        //std::cout << "INDEX: " << currIndex << " " << std::endl;
    }
}


//Recursive implementation fails to work
void recursive(integer_2d_array &arr, int currIndex, Node *currNode)
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
        //std::cout << currIndex << " ";
        recursive(arr, currIndex - 2, left);
    }
    //rightchild
    if (rightChild)
    {
        //std::cout << currIndex << " ";
        recursive(arr, currIndex - 1, right);
    }
}
void inOrder(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    inOrder(node->leftChild);
    std::cout << node->val << " ";
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