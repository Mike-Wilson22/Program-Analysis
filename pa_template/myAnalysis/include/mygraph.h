
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
#include <unordered_map>


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
        // Iterate over nodes, return the node that has no in edges
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
        // Iterate over nodes, return the node that has no out edges
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

    myGraph operator+(myGraph& graph2) {
        myGraph newGraph;
        unordered_map<unsigned, myNode*> nodeMapping;
        unsigned id_offset = 0;

        // Find the highest existing ID to offset graph2's nodes
        for (auto I = begin(), E = end(); I != E; ++I) {
            id_offset = max(id_offset, I->second->getId());
        }

        // Copy nodes from this and graph2 to newGraph
        for (auto I = begin(), E = end(); I != E; ++I) {
            myNode* newNode = new myNode(I->second->getId()); 
            newGraph.addNode(newNode->getId(), newNode);
            nodeMapping[I->second->getId()] = newNode;
        }
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I) {
            myNode* newNode = new myNode(I->second->getId() + id_offset);
            newGraph.addNode(newNode->getId(), newNode);
            nodeMapping[I->second->getId() + id_offset] = newNode;
        }

        // Copy edges from this and graph2 to newGraph
        for (auto I = begin(), E = end(); I != E; ++I) {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S) {
                unsigned srcId = (*S)->getSrcNode()->getId();
                unsigned dstId = (*S)->getDstNode()->getId();
                myEdge* edge = new myEdge(nodeMapping[srcId], nodeMapping[dstId]);
                newGraph.addEdge(edge);
            }
        }
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I) {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S) {
                unsigned srcId = (*S)->getSrcNode()->getId() + id_offset;
                unsigned dstId = (*S)->getDstNode()->getId() + id_offset;
                myEdge* edge = new myEdge(nodeMapping[srcId], nodeMapping[dstId]);
                newGraph.addEdge(edge);
            }
        }

        // Connect `this` graph's exit node to `graph2`'s entry node
        myNode* nodeExit = nodeMapping[getExit()->getId()];
        myNode* nodeEntry = nodeMapping[graph2.getEntry()->getId() + id_offset];
        myEdge* edge = new myEdge(nodeExit, nodeEntry);
        newGraph.addEdge(edge);

        return newGraph;
    }

    bool operator==(myGraph graph2) 
    {
        std::set<unsigned> ids_1;
        std::set<unsigned> ids_2;
        std::set<myEdge*> edges_1;
        std::set<myEdge*> edges_2;

        // Iterate over nodes and save IDs
        for (auto I = begin(), E = end(); I != E; ++I) {
            ids_1.insert(I->second->getId());
        }
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I) {
            ids_2.insert(I->second->getId());
        }
        if (ids_1 != ids_2)
        {
            return false;
        }

        // Iterate through edges, for each edge, find an edge with corresponding src and dst id in other graph
        for (auto I = begin(), E = end(); I != E; ++I) {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S) {
                myNode* src = (*S)->getSrcNode();
                myNode* dst = (*S)->getDstNode();
                unsigned id = src->getId();
                myNode* graph2_src = graph2.getMap()[src->getId()];

                bool marker = true;
                for (auto K = graph2_src->outEdgeBegin(), L = graph2_src->outEdgeEnd(); K != L; ++K) {
                    if ((*K)->getDstID() == dst->getId())
                    {
                        edges_1.insert((*K));
                        marker = false;
                        break;
                    }
                }
                if (marker)
                {
                    return false;
                }
            }
        }

        // Confirm that there are no edges in graph2 not in graph1
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I) {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S) {
                edges_2.insert((*S));
            }
        }
        if (edges_1 != edges_2)
        {
            return false;
        }

        return true;
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
        assert(nodePointer == node1);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node4, node1);
        graph.addEdge(edge4);

        nodePointer = graph.getEntry();

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
        assert(nodePointer == node3);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node3, node4);
        graph.addEdge(edge4);

        nodePointer = graph.getExit();

        assert(nodePointer == node4);
        cout << "testGetExit passed!" << endl;
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

        myGraph newGraph = graph + graph2;
        assert(newGraph.getEntry() != newGraph.getMap()[4]);
        assert(newGraph.getExit() != newGraph.getMap()[3]);
        assert(newGraph.getEntry() == newGraph.getMap()[1]);
        assert(newGraph.getExit() == newGraph.getMap()[6]);

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

        assert(graph == graph2);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node3, node4);
        graph.addEdge(edge4);

        assert(!(graph == graph2));

        cout << "testEquivalence passed!" << endl;
    }
};
#endif 
