
#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <stack>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RandomData.h"
#include <chrono>
#include <boost/sort/sort.hpp>
#include "CppConsoleTable.hpp"

using namespace std;


class Edge{
public:
    char src;
    char dst;
    Edge(char _src, char _dst) {
        this->src = _src;
        this->dst = _dst;
    }
    bool operator==(const Edge& edge) const {
       return  this->src== edge.src && this->dst == edge.dst;
    }
};

class DAGRedundancyCheckerV1 {


    std::map<char, std::vector<char> > buildGraph(const vector<Edge>&  edges)
    {
        std::map<char, std::vector<char> >  graph;
        for(auto& edge: edges) {
            graph[edge.src].push_back(edge.dst);
        }
        return std::move(graph);
    }

    int searchLongestPath(std::map<char, std::vector<char> > & graph, char src, char dst)
    {
        int longest = -1;
        if(src == dst) return 0;
        const auto& nexts =  graph[src];
        for(char node : nexts) {
            if(node == dst && longest < 1) { 
                longest =  1 ;
                //break;
            }
            int length =  searchLongestPath(graph, node, dst);
            if(length > 0 && longest < length + 1) {
                longest = length + 1;
            }     
        }

        return longest;
    }
 public:
    vector<Edge>  findRedundancyEdges( vector<Edge>&  edges) 
    {
        vector<Edge>  redundancy_edges;
        auto graph = buildGraph(edges);
        for(auto& edge :  edges) {
            int longest = searchLongestPath(graph, edge.src, edge.dst);
            std::cout <<  edge.src << "-->" << edge.dst  << ",longest=" << longest <<"\n";
             if( longest > 1) {
                 redundancy_edges.push_back(edge);
             }
        }
        return std::move(redundancy_edges);
    }

};

TEST(DAGRedundancyCheckerV1, simple_test){


 
    // ASSERT_EQ(x, 3); 
    vector<Edge> edges;
    edges.push_back(Edge('A', 'B'));
    edges.push_back(Edge('A', 'C'));
    edges.push_back(Edge('B', 'C'));
    edges.push_back(Edge('B', 'D'));
    edges.push_back(Edge('C', 'D'));
    edges.push_back(Edge('C', 'E'));
    edges.push_back(Edge('D', 'E'));

    auto red_edges =  DAGRedundancyCheckerV1().findRedundancyEdges(edges);

    for(auto& edge: red_edges) {
            std::cout << "redundancy edge: " << edge.src << "-->" << edge.dst <<"\n";
    }


    ASSERT_EQ(red_edges.size(), 3); 

  
   // ASSERT_EQ(std::find(red_edges.begin(), red_edges.end(), Edge('A', 'C')), true);
   // ASSERT_EQ(std::find(red_edges.begin(), red_edges.end(), Edge('B', 'D')), true);
   // ASSERT_EQ(std::find(red_edges.begin(), red_edges.end(), Edge('C', 'E')), true);

    //ASSERT_THAT(red_edges, Elements)
      
}
