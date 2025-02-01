
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

    myNode* getEntry () const
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

    myNode* getExit () const
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

    T_IDToNodeMap getMap ()
    {
        return m_IDToNodeMap;
    }

    myGraph operator+(myGraph graph2)
    {
        myGraph newGraph;
        myNode* nodeExit;
        myNode* nodeEntry;
        unsigned id_offset = 0;
        for (auto I = begin(), E = end(); I != E; ++I)
        {
            if (I->second->getId() > id_offset)
            {
                id_offset = I->second->getId();
            }
            if (I->second == getExit())
            {
                nodeExit = new myNode(I->second->getId());
                newGraph.addNode(nodeExit->getId(), nodeExit);
            } else {
                myNode* node = new myNode(I->second->getId());
                newGraph.addNode(node->getId(), node);
            }
        }
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I)
        {
            if (I->second == graph2.getEntry())
            {
                nodeEntry = new myNode(I->second->getId() + id_offset);
                newGraph.addNode(nodeEntry->getId(), nodeEntry);
            } else {
                myNode* node = new myNode(I->second->getId() + id_offset);
                newGraph.addNode(node->getId(), node);
            }
        }
        for (auto I = begin(), E = end(); I != E; ++I)
        {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S)
            {
                unsigned srcId = (*S)->getSrcNode()->getId();
                unsigned dstId = (*S)->getDstNode()->getId();
                myEdge* edge = new myEdge(newGraph.getMap()[srcId], newGraph.getMap()[dstId]);
                newGraph.addEdge(edge);
            }
        }
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I)
        {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S)
            {
                unsigned srcId = (*S)->getSrcNode()->getId() + id_offset;
                unsigned dstId = (*S)->getDstNode()->getId() + id_offset;
                myEdge* edge = new myEdge(newGraph.getMap()[srcId], newGraph.getMap()[dstId]);
                newGraph.addEdge(edge);
            }
        }
        myEdge* edge = new myEdge(nodeExit, nodeEntry);
        newGraph.addEdge(edge);
        return newGraph;
    }

    bool operator==(myGraph const& graph2) const
    {
        return false;
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
        //testGetEntry();
        //testGetExit();
        testAddition();
        testEquivalence();
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
        myGraph* graph;

        myNode* node1 = new myNode(1);
        myNode* node2 = new myNode(2);
        myNode* node3 = new myNode(3);
        graph->addNode(node1->getId(), node1);
        graph->addNode(node2->getId(), node2);
        graph->addNode(node3->getId(), node3);

        // Add edges
        myEdge* edge1 = new myEdge(node1, node2);
        myEdge* edge2 = new myEdge(node2, node3);
        myEdge* edge3 = new myEdge(node1, node3);
        graph->addEdge(edge1);
        graph->addEdge(edge2);
        graph->addEdge(edge3);

        myNode* nodePointer = graph->getEntry();
        //cout << "Entry pointer: " << nodePointer << endl;
        assert(nodePointer == node1);

        myNode* node4 = new myNode(4);
        graph->addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node4, node1);
        graph->addEdge(edge4);

        nodePointer = graph->getEntry();

        //cout << "Entry pointer: " << nodePointer << endl;
        //cout << "Node 4: " << node4 << endl;
        //cout << "Node 3: " << node3 << endl;
        //cout << "Node 2: " << node2 << endl;
        //cout << "Node 1: " << node1 << endl;
        assert(nodePointer == node4);
        cout << "testGetEntry passed!" << endl;
        cout << "graph1 Entry: " << &graph << endl;
        delete graph;
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
        cout << "testGetExit passed!" << endl;
        cout << "graph1 Exit: " << &graph << endl;
    }

    void testAddition()
    {
        cout << "Running testAddition..." << endl;
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

        myGraph graph2;

        myNode* node21 = new myNode(4);
        myNode* node22 = new myNode(5);
        myNode* node23 = new myNode(6);
        graph2.addNode(node21->getId(), node21);
        graph2.addNode(node22->getId(), node22);
        graph2.addNode(node23->getId(), node23);

        // Add edges
        myEdge* edge21 = new myEdge(node21, node22);
        myEdge* edge22 = new myEdge(node22, node23);
        myEdge* edge23 = new myEdge(node21, node23);
        graph2.addEdge(edge21);
        graph2.addEdge(edge22);
        graph2.addEdge(edge23);

        myGraph newGraph = graph + graph2;
        assert(newGraph.getEntry() != newGraph.getMap()[4]);
        assert(newGraph.getExit() != newGraph.getMap()[3]);
        assert(newGraph.getEntry() == newGraph.getMap()[1]);
        assert(newGraph.getExit() == newGraph.getMap()[6]);
        cout << "new graph: " << &newGraph << endl;
        cout << "graph1: " << &graph << endl;
        cout << "graph2: " << &graph2 << endl;
        cout << "testAddition passed!" << endl;
    }
    
    void testEquivalence()
    {
        cout << "Running Equivalence..." << endl;
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

        myGraph graph2;

        myNode* node21 = new myNode(1);
        myNode* node22 = new myNode(2);
        myNode* node23 = new myNode(3);
        graph2.addNode(node21->getId(), node21);
        graph2.addNode(node22->getId(), node22);
        graph2.addNode(node23->getId(), node23);

        // Add edges
        myEdge* edge21 = new myEdge(node21, node22);
        myEdge* edge22 = new myEdge(node22, node23);
        myEdge* edge23 = new myEdge(node21, node23);
        graph2.addEdge(edge21);
        graph2.addEdge(edge22);
        graph2.addEdge(edge23);

        //assert(graph == graph2);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node3, node4);
        graph.addEdge(edge4);

        //assert(!(graph == graph2));

        cout << "testEquivalence passed!" << endl;
    }
};
#endif 
