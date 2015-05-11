

#ifndef __NYUCodebase__AI_bfs__
#define __NYUCodebase__AI_bfs__

#include <stdio.h>
#include<iostream>
#include <list>
#include <vector>

using namespace std;

class Node
{
public:
Node(float x, float y);
float x;
float y;
bool visited;
Node *cameFrom;
std::vector<Node*> connected;

void addEdge(Node* one);
bool BFS(Node* end);
};
#endif /* defined(__NYUCodebase__AI_bfs__) */
