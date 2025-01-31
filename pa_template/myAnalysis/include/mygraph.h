
#ifndef _MYGRAPH_H_
#define _MYGRAPH_H_
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include "generic_graph.h"
#include "graph_visual.h"
#include "graph_test.h"
#include "graph_generator.h"

using namespace std;

class myNode;
class myEdge : public GenericEdge<myNode> 
{
public:
    myEdge(myNode* s, myNode* d):GenericEdge<myNode>(s, d) {}

    ~myEdge() {}
};


class myNode : public GenericNode<myEdge> 
{  
public:
    myNode(unsigned Id): GenericNode<myEdge>(Id) {}
};


class myGraph : public GenericGraph<myNode, myEdge> 
{
public:
    myGraph() {} 
    ~myGraph() {}

    myNode* getEntry ()
    {
        for (auto I = m_IDToNodeMap.begin(), E = m_IDToNodeMap.end(); I != E; ++I)
        {
            //cout << I->second << endl;
            if (I->second->inEdgeBegin() == I->second->inEdgeEnd())
            {
                return I->second;
            }
        }
        return 0;
    }

    myNode* getExit ()
    {
        for (auto I = m_IDToNodeMap.begin(), E = m_IDToNodeMap.end(); I != E; ++I)
        {
            if (I->second->outEdgeBegin() == I->second->outEdgeEnd())
            {
                return I->second;
            }
        }
        return 0;
    }
};


class myGraphVisual: public GraphVis <myNode, myEdge, myGraph>
{
public:
    myGraphVisual (string graphName, myGraph *graph)
        :GraphVis <myNode, myEdge, myGraph>(graphName, graph) {}

    ~myGraphVisual () {}
};

class myGraphTest: public GraphTest <myNode, myEdge, myGraph>
{
public:
    myGraphTest (): GraphTest <myNode, myEdge, myGraph>() {}
    ~myGraphTest () {}

    void runTests ()
    {
        // add your own test here
        testGetEntry();
        testGetExit();
        testGraphDump();

        GraphTest::runTests ();
    }

private:
    void testGraphDump ()
    {
        GraphGenerator<myNode, myEdge, myGraph> generator;
        myGraph randomGraph = generator.generateRandomGraph(10);

        myGraphVisual myGV ("mygraph", &randomGraph);
        myGV.witeGraph();
    }

    void testGetEntry()
    {
        cout << "Running testGetEntry..." << endl;
        myGraph graph;

        myNode* node1 = new myNode(1);
        myNode* node2 = new myNode(2);
        myNode* node3 = new myNode(3);
        graph.addNode(node1->getId(), node1);
        graph.addNode(node2->getId(), node2);
        graph.addNode(node3->getId(), node3);

        // Add edges
        myEdge* edge1 = new myEdge(node1, node2);
        myEdge* edge2 = new myEdge(node2, node3);
        myEdge* edge3 = new myEdge(node1, node3);
        graph.addEdge(edge1);
        graph.addEdge(edge2);
        graph.addEdge(edge3);

        myNode* nodePointer = graph.getEntry();
        //cout << "Entry pointer: " << nodePointer << endl;
        assert(nodePointer == node1);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node4, node1);
        graph.addEdge(edge4);

        nodePointer = graph.getEntry();

        //cout << "Entry pointer: " << nodePointer << endl;
        //cout << "Node 4: " << node4 << endl;
        //cout << "Node 3: " << node3 << endl;
        //cout << "Node 2: " << node2 << endl;
        //cout << "Node 1: " << node1 << endl;
        assert(nodePointer == node4);
        cout << "testGetEntry passed!" << endl;
    }

    void testGetExit()
    {
        cout << "Running testGetExit..." << endl;
        myGraph graph;

        myNode* node1 = new myNode(1);
        myNode* node2 = new myNode(2);
        myNode* node3 = new myNode(3);
        graph.addNode(node1->getId(), node1);
        graph.addNode(node2->getId(), node2);
        graph.addNode(node3->getId(), node3);

        // Add edges
        myEdge* edge1 = new myEdge(node1, node2);
        myEdge* edge2 = new myEdge(node2, node3);
        myEdge* edge3 = new myEdge(node1, node3);
        graph.addEdge(edge1);
        graph.addEdge(edge2);
        graph.addEdge(edge3);

        myNode* nodePointer = graph.getExit();
        //cout << "Exit pointer: " << nodePointer << endl;
        assert(nodePointer == node3);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node3, node4);
        graph.addEdge(edge4);

        nodePointer = graph.getExit();

        //cout << "Exit pointer: " << nodePointer << endl;
        //cout << "Node 4: " << node4 << endl;
        //cout << "Node 3: " << node3 << endl;
        //cout << "Node 2: " << node2 << endl;
        //cout << "Node 1: " << node1 << endl;
        assert(nodePointer == node4);
        cout << "testGetEntry passed!" << endl;
    }
    
};
#endif 
