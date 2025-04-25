#include "icfg.h"
#include "otf_pta.h"
#include "dfg.h"
#include "generic_graph.h"

using namespace std;

void findUnreachedFunctions(ICFG* icfg) {
    ICFGVisual vis = ICFGVisual("dcd", icfg);

    std::set<std::set<CFGNode*>> splits;
    for (auto nodeEntry = icfg->cfg_begin(); nodeEntry != icfg->cfg_end(); ++nodeEntry) {
        std::stack<CFGNode*> stack;
        CFGNode* entryNode = nodeEntry->second->getEntryNode();
        bool doContinue = false;
        for (auto visitedset = splits.begin(); visitedset != splits.end(); ++visitedset) {
            if ((*visitedset).find(entryNode) != (*visitedset).end()) {
                doContinue = true;
                break;
            }
        }
        if (doContinue) {
            continue;
        }
        stack.push(entryNode);

        std::set<CFGNode*> visited;

        while (!stack.empty()) {
            CFGNode* node = stack.top();
            stack.pop();
            visited.insert(node);
            for (auto child = node->outEdgeBegin(); child != node->outEdgeEnd(); ++child) {
                CFGNode* childNode = (*child)->getDstNode();
                if (visited.find(childNode) == visited.end()) {
                    stack.push(childNode);
                }
            }
            for (auto parent = node->inEdgeBegin(); parent != node->inEdgeEnd(); ++parent) {
                CFGNode* parentNode = (*parent)->getSrcNode();
                if (visited.find(parentNode) == visited.end()) {
                    stack.push(parentNode);
                }
            }
        }

        std::set<CFGNode*> entries;
        for (auto node = visited.begin(); node != visited.end(); ++node) {
            if ((*node)->isEntry()) {
                entries.insert(*node);
            }
        }
        splits.insert(entries);
    }

    if (splits.size() == 1) {
        std::cout << "All functions are reachable from each other" << std::endl;
    } else {
        std::cout << "There are " << splits.size() << " separate sections in the code" << std::endl;
        for (auto split = splits.begin(); split != splits.end(); ++split) {
            std::set<CFGNode*> section = *split;
            std::cout << "\nThere are " << section.size() << " function(s) in this section. \nThey are: " << std::endl;
            for (auto function = section.begin(); function != section.end(); ++function) {
                std::cout << vis.getNodeLabel(*function) << "()" << std::endl;
            }
        }
    }
}

void solveDeadCode(ICFG* icfg, int function) {
    // Detect uncalled functions -- limitation with libraries (only works on programs with a single entry point)
    
    // Identify node 1
    // Begin BFS from node 1, mark all nodes reached
    // Print out nodes that are unreached, mark them as possible dead code
    // Possibly--add all entry nodes to a list, if unmarked, code is never used

    // Detect unused values 

    // Needs: USE sets, GEN sets, or equivalent
    // Iterate through each node where a variable is defined
    // Look for outflow edges--if none detected, definition is dead code
    // Possibly--mark code as dead, restart analysis to find more dead code
}

void findDeadDeclarations(DFG *dfg) {
    DFGVisual vis = DFGVisual("dcd", dfg);

    std::map<int, CFGNode*> deadMap;
    std::set<CFGNode*> dead;
    int deadSize = 0;

    bool once = false;

    ICFG* icfg = dfg->getICFG();
    int i = 0;
    while (!once || deadSize != dead.size()) {
        deadSize = dead.size();
        for (auto it = icfg->cfg_begin (), end = icfg->cfg_end (); it != end; it++) {
            CFG *cfg = it->second;

            std::map<CFGNode*, std::set<llvm::Value*>> *genSet = dfg->getGEN();
            for (auto cfgNode = cfg->begin (), end = cfg->end (); cfgNode != end; cfgNode++) {
                CFGNode *node = cfgNode->second;
                if (dead.find(node) != dead.end()) {
                    continue;
                }

                if (!(*genSet)[node].empty() && !node->isExit()) {
                    bool goodNode = false;
                    for (auto cfgEdge = node->outEdgeBegin (), cfgEnd = node->outEdgeEnd (); cfgEdge != cfgEnd; cfgEdge++) {
                        CFGEdge *edge = *cfgEdge;

                        if ((edge->edgeType&EDGE_DFG || edge->edgeType == 3) && dead.find(edge->getDstNode()) == dead.end() && node->getId() < edge->getDstNode()->getId()) {
                            goodNode = true;
                        }
                    }

                    if (!goodNode) {
                        dead.insert(node);
                        deadMap[i] = node;
                        i++;
                    }
                }
            }
        }
        once = true;
    }



    if (dead.empty()) {
        std::cout << "No dead code detected" << std::endl;
    } else {
        std::cout << "Lines of LLVM IR that are dead code: " << std::endl;

        for (auto node = dead.begin(); node != dead.end(); ++node) {
            std::cout << vis.getNodeLabel(*node) << std::endl;
        }
    }
}
