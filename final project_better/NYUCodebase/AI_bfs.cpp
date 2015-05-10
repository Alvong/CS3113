
#include "AI_bfs.h"
#include <queue>
Node::Node(float x, float y):x(x),y(y),visited(false),cameFrom(nullptr){}

void Node::addEdge(Node* one)
{
    connected.push_back(one);
}

bool Node::BFS(Node* end)
{
    
      // Create a queue for BFS
    std::queue<Node*> queue;
    
    queue.push(this);
    this->visited=true;
    Node* temp=nullptr;
    while(!queue.empty())
    {
        
        temp = queue.front();
        
        queue.pop();
        
        if (temp->x==end->x&&temp->y==end->y) {
            return true;
        }
        for(int i =0 ; i < temp->connected.size(); ++i)
        {
            if(!(temp->connected[i]->visited))
            {
                temp->connected[i]->visited=true;
                temp->connected[i]->cameFrom=temp;
                queue.push(temp->connected[i]);
            }
        }
        
    }
    return false;
}