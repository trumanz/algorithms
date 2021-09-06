#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <set>
#include <stdio.h>
#include "utility/CppConsoleTable.hpp"

using namespace std;

class AlienDict {
public:

    enum VisitFlag {
        NOT_VISIT,
        VISITING,
        VISITED
    };

    typedef map<char, set<char> >  DAG;
    DAG dag;
    map<char, VisitFlag> node_visit_flag;
    std::string node_stack;
    bool have_circle;

    void genDag(vector<string>& words) {
        for(auto& word : words) {
            for(char c : word) {
                dag[c] = set<char>();
                node_visit_flag[c] = NOT_VISIT;
            }
        }
        for(int i = 0; i + 1 < words.size(); i++) {
            const string& first_word = words[i];
            const string& second_word = words[i+1];
            //why first_word < second_word, because there are one char of first_word  < second_word
            for(int j = 0;  j < first_word.length() && j < second_word.length(); j++) {
                if( first_word[j] != second_word[j] ) {
                    dag[first_word[j]].insert(second_word[j]);
                    break;
                }
            }
        }
    }
    void printDag() {
        printf("dag.size()=%lu\n", dag.size());
        for(auto it = dag.begin(); it != dag.end(); it++) {
            char from = it->first;
            set<char>& tos = it->second;
            if(tos.size() > 0) {
                for(char to : tos) {
                    printf("%c --> %c \n", from, to);
                }
            }else {
                printf("%c -->  \n", from);
            }
        }
    }

    void dfsSearch(char node) {
        node_visit_flag[node] = VISITING;
        for(auto next_node : dag[node] ) {
            const auto flag = node_visit_flag[next_node];
            if (flag == NOT_VISIT) {
                dfsSearch(next_node);
            } else if(flag == VISITING) { //visiting again, incorrect DAG
                have_circle = true;
                return;
            }
        }
        node_visit_flag[node] = VISITED;
        node_stack.push_back(node);
    }



    bool checkAgainOneRoot(char start_node) {
        return true;
        for(auto it = node_visit_flag.begin(); it != node_visit_flag.end(); it ++) {
            it->second = NOT_VISIT;
        }
        dfsSearch(start_node);
        for(auto it = node_visit_flag.begin(); it != node_visit_flag.end(); it ++) {
            if( it->second != VISITED ) {
                return false;
            }
        }
        return true;
    }

    string solve(vector<string>& words) {
        have_circle = false;
        genDag(words);
        //printDag();
        for(auto it = node_visit_flag.begin(); it != node_visit_flag.end(); it++) {
            if(it->second == NOT_VISIT ) {
                dfsSearch(it->first);
            }
        }
        if(have_circle)  {
         //   printf("have circle\n");
            return  "";
        }
        std::reverse(node_stack.begin(), node_stack.end());

        string result = node_stack;
        if(result.length() > 0 && checkAgainOneRoot(result[0]) == false) {
            return "";
        }

        return result;
    }

    string alienOrder(vector<string>& words) {
        return solve(words);
    }


};





TEST(AlienDict, sample_test){

    vector<string> words = {
            "wrt","wrf","er","ett","rftt"
    };

    EXPECT_EQ("wertf", AlienDict().alienOrder(words));

    words = {
            "z", "x"
    };
    EXPECT_EQ("zx", AlienDict().alienOrder(words));

    words = {
            "z", "x", "z"
    };
    EXPECT_EQ("", AlienDict().alienOrder(words));

};

