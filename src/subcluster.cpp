#include "../inc/subclustering.h"
#include "../inc/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "../inc/dataanalysis.h"
#include "../inc/node.h"
#include <queue>
#include <algorithm>

using namespace alglib;

void SubClustering::run(){
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

    // //K Clusters
    // std::vector<std::vector<int>> kclusters;
    // std::vector<int> kcluster1;
    // std::vector<int> kcluster2;
    // clusterizergetkclusters(rep, 2, cidx, cz);
    // printf("%s\n", cidx.tostring().c_str());

    // for(int i = 0; i < cidx.length() ; ++i){
    //     if(cidx[i] == 1){
    //         kcluster2.push_back(i);
    //     } else if(cidx[i] == 0){
    //         kcluster1.push_back(i);
    //     }
    // }
    // kclusters.push_back(kcluster1);
    // kclusters.push_back(kcluster2);

    //Manual Subclustering
    printf("%s\n", rep.z.tostring().c_str());

    Node *root = new Node(-1, nullptr, nullptr, nullptr);

    //recursive(rep.z, rep.z.rows() - 1, root);

    subcluster(rep.z, rep.z.rows() - 1, root);
    calcLeafNodesNum(root);

    std::vector<std::vector<int>> clusters;
    std::vector<Node *> clusteredNodes;

    FillClusterNodes(root, clusteredNodes, 6);

    nodesIntoClusters(clusteredNodes, clusters, rep.z.rows());

    // std::cout << std::boolalpha << "Two Clusters are the same:\n"
    //     << compareClusters(kclusters, clusters) << std::endl;

    //inordernumofLeafChilds(root);

    //inorderTraversal(root);

    print2dvector(clusters);

    deleteTree(root);
}

int SubClustering::calcLeafNodesNum(Node *node)
{
    if (node->leftChild == nullptr && node->rightChild == nullptr)
    {
        node->numofLeafChilds = 1;
        return 1;
    }
    int leftc = calcLeafNodesNum(node->leftChild);
    int rightc = calcLeafNodesNum(node->rightChild);
    node->numofLeafChilds = leftc + rightc;
    return node->numofLeafChilds;
}

void SubClustering::FillClusterNodes(Node *root, std::vector<Node*> &clusteredNodes, int limit)
{
    if (root->numofLeafChilds < limit)
    {
        clusteredNodes.push_back(root);
        return;
    }
    FillClusterNodes(root->leftChild, clusteredNodes, limit);
    FillClusterNodes(root->rightChild, clusteredNodes, limit);
}

void SubClustering::findLeafNodes(Node *node, std::vector<int> &cluster, int limit)
{
    if (node == nullptr){
        return;
    }

    findLeafNodes(node->leftChild, cluster, limit);
    if (node->val <= limit)
    {
        cluster.push_back(node->val);
    }
    findLeafNodes(node->rightChild, cluster, limit);
}

void SubClustering::nodesIntoClusters(std::vector<Node *> &clusteredNodes, std::vector<std::vector<int>> &clusters, int limit)
{
    std::vector<int> temp;
    for(auto node: clusteredNodes){
        findLeafNodes(node, temp, limit);
        clusters.push_back(temp);
        temp.clear();
    }
}

//Use a BFS iterative method
// 1. Input current [int, int] into queue(FIFO)
// 2. Pop and then input child nodes right first, left second
// 3. Continue until queue is empty and arr is empty as well
void SubClustering::subcluster(integer_2d_array &arr, int currIndex, Node *headNode)
{
    std::queue<ae_int_t *> qe;
    std::queue<Node *> listofNodes;
    const int numofRows = arr.rows();

    qe.push(arr[currIndex]);
    listofNodes.push(headNode);

    while (!qe.empty())
    {
        ae_int_t *val = qe.front();
        qe.pop();
        //std::cout << "VAL:" << val[0] << " " << val[1] << std::endl;
        Node *curr = listofNodes.front();
        listofNodes.pop();

        Node *rightHead = new Node(val[1], curr, nullptr, nullptr);
        Node *leftHead = new Node(val[0], curr, nullptr, nullptr);
        curr->rightChild = rightHead;
        curr->leftChild = leftHead;

        //Right Child has a subcluster
        if (val[1] > numofRows)
        {
            qe.push(arr[--currIndex]);
            listofNodes.push(rightHead);
            //std::cout << "RIGHT: " << val[1] << std::endl;
        }
        //Left Child has a subcluster
        if (val[0] > numofRows)
        {
            qe.push(arr[--currIndex]);
            listofNodes.push(leftHead);
            //std::cout << "LEFT: " << val[0] << std::endl;
        }
        //std::cout << "INDEX: " << currIndex << " " << std::endl;
    }
}

//Recursive implementation fails to work
void SubClustering::recursiveSubclustering(integer_2d_array &arr, int currIndex, Node *currNode)
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
        recursiveSubclustering(arr, currIndex - 2, left);
    }
    //rightchild
    if (rightChild)
    {
        //std::cout << currIndex << " ";
        recursiveSubclustering(arr, currIndex - 1, right);
    }
}

void SubClustering::inorderTraversal(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    inorderTraversal(node->leftChild);
    std::cout << node->val << " ";
    inorderTraversal(node->rightChild);
}

void SubClustering::inordernumofLeafChilds(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    inordernumofLeafChilds(node->leftChild);
    std::cout << "VAL: " << node->val << " NUMCHILD: " << node->numofLeafChilds << std::endl;
    inordernumofLeafChilds(node->rightChild);
}

void SubClustering::deleteTree(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->leftChild);
    deleteTree(node->rightChild);
    delete node;
}

bool SubClustering::compareClusters(std::vector<std::vector<int>> &vec1, std::vector<std::vector<int>> &vec2)
{
    for (auto &vec : vec1)
    {
        std::sort(vec.begin(), vec.end());
    }
    for (auto &vec : vec2)
    {
        std::sort(vec.begin(), vec.end());
    }
    print2dvector(vec1);
    print2dvector(vec2);

    for (int i = 0; i < vec1.size(); ++i)
    {
        for (int j = 0; j < vec1[i].size(); ++j)
        {
            if (vec1[i][j] != vec2[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
void SubClustering::print2dvector(std::vector<std::vector<int>> &vectors)
{
    for (auto vector : vectors)
    {
        for (auto vec : vector)
        {
            std::cout << vec << " ";
        }
        std::cout << std::endl;
    }
}