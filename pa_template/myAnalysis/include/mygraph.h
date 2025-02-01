
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

    bool operator==(myGraph& graph2) 
    {
        std::set<unsigned> ids_1;
        std::set<unsigned> ids_2;
        std::set<std::pair<unsigned, unsigned>> edges_1, edges_2;

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

        // Record all edges with src/dst ids, to be compared
        for (auto I = begin(), E = end(); I != E; ++I) {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S) {
                edges_1.insert({(*S)->getSrcID(), (*S)->getDstID()});
            }
        }
        for (auto I = graph2.begin(), E = graph2.end(); I != E; ++I) {
            for (auto S = I->second->outEdgeBegin(), T = I->second->outEdgeEnd(); S != T; ++S) {
                edges_2.insert({(*S)->getSrcID(), (*S)->getDstID()});
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
        unsigned ids[] = {1, 2, 3};
        myGraph graph = stdGraph(ids);

        myNode* nodePointer = graph.getEntry();
        assert(nodePointer == graph.getMap()[1]);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(node4, graph.getMap()[1]);
        graph.addEdge(edge4);

        nodePointer = graph.getEntry();

        assert(nodePointer == node4);
        cout << "testGetEntry passed!" << endl;
    }

    void testGetExit()
    {
        cout << "Running testGetExit..." << endl;
        unsigned ids[] = {1, 2, 3};
        myGraph graph = stdGraph(ids);

        myNode* nodePointer = graph.getExit();
        assert(nodePointer == graph.getMap()[3]);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);
        myEdge* edge4 = new myEdge(graph.getMap()[3], node4);
        graph.addEdge(edge4);

        nodePointer = graph.getExit();

        assert(nodePointer == node4);
        cout << "testGetExit passed!" << endl;
    }

    void testAddition()
    {
        cout << "Running testAddition..." << endl;
        unsigned ids[] = {1, 2, 3};
        myGraph graph = stdGraph(ids);
        myGraph graph2 = stdGraph(ids);

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
        
        unsigned ids[] = {1, 2, 3};
        myGraph graph = stdGraph(ids);
        myGraph graph2 = stdGraph(ids);

        assert(graph == graph2);

        myNode* node4 = new myNode(4);
        graph.addNode(node4->getId(), node4);

        assert(!(graph == graph2));

        myGraph graph3 = stdGraph(ids);
        myEdge* edge = new myEdge(graph3.getMap()[3], graph3.getMap()[2]);
        graph3.addEdge(edge);

        assert(!(graph == graph3));
        

        cout << "testEquivalence passed!" << endl;
    }

    myGraph stdGraph(unsigned ids[])
    {
        myGraph graph;

        myNode* node1 = new myNode(ids[0]);
        myNode* node2 = new myNode(ids[1]);
        myNode* node3 = new myNode(ids[2]);
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
        
        return graph;
    }


};
#endif 
