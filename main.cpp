/***********************************************************\
|*                                                         *|
|*   Teoria e Aplicação de Grafos, Turma A, 1/2018         *|
|*   Grupo:     Wladimir Gramacho & Leonardo de Almeida    *|
|*   Matrícula: 15/0048718        & 15/0135491             *|
|*   Projeto 1: Karate Classroom - Node Degrees & Cliques  *|
|*                                                         *|
\***********************************************************/

#include <bits/stdc++.h>
#include <algorithm>
#include <list>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

const int MAX_VERTEX = 35;

list<vector<int>> result_cliques;

int end_program() {
  exit(1);
}

void print_red () {
  cout << "\033[1;31m";
}

void reset_print_color () {
  cout << "\033[0m";
}

int checks_arguments(int argc, char const argv[]) {
  if(argc == 1 || (argc > 1 && strcmp(argv,"karate.gml")) ) {
    cout << "The program you are trying to execute must be ran with the file karate.gml as an argument.\n";
    cout << "To do so, you must run ";
    print_red();
    cout << "./<name_of_the_executable>.out karate.gml";
    reset_print_color();
    cout << " in your terminal.\n";
    return 0;
  }
  return 1;
}

void create_graph_from_file(vector<vector<int>>& graph, char const argv[]) {
  ifstream data_file(argv);
  string line, temp_str;
  int node1, node2;

  while(getline(data_file, line)) {
    if( (line.compare("  edge")) ) continue;
    getline(data_file, line); // ignore "  [" string

    getline(data_file, line);
    node1 = stoi(line.substr(11)); // offset of 11 chars to find number

    getline(data_file, line);
    node2 = stoi(line.substr(11)); // offset of 11 chars to find number

    graph[node1].push_back(node2);
    graph[node2].push_back(node1);

    getline(data_file, line); // ignore "  [" string
  }
}

void calculates_nodes_degrees(vector<vector<int>>& graph, vector<pair<int, int>>& nodesDegrees){
  for(int u = 1; u < MAX_VERTEX; u++){
    nodesDegrees[u].first = u;
    nodesDegrees[u].second = graph[u].size();
  }
}

bool sort_desc_by_second(const pair<int,int> &a, const pair<int,int> &b){
    return (a.second > b.second);
}

bool sort_by_vector_size(const vector<int>& first, const vector<int>& second){
  return first.size() > second.size();
}

void sort_nodes_by_degrees(vector<pair<int, int>>& nodesDegrees){
  sort(nodesDegrees.begin()+1, nodesDegrees.end()+1, sort_desc_by_second);
}

void print_sorted_nodes(vector<pair<int, int>>& nodesDegrees){
  cout << "List of nodes sorted by degree in descending order:\n";
  cout << "node[i] = <degree_of_node[i]>\n";
  for(int u = 1; u <= 34;u++){
    cout << "node[" << nodesDegrees[u].first << "] = " << nodesDegrees[u].second << "\n";
  }
  cout << "\n\n--------------------\n\n";
}

void init_P(vector<int>& P){
  for (int i = 1; i < MAX_VERTEX; i++) {
    P.push_back(i);
  }
}

vector<int> intersection(vector<int> v1, vector<int> v2){
    vector<int> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));

    return v3;
}

void BronKerbosch(vector<int> R, vector<int> P, vector<int> X, vector<vector<int>>& graph){
  if (P.empty() && X.empty()){
    result_cliques.insert(result_cliques.begin(), R);
  }

  for (auto node = P.begin(); node != P.end();){
    auto R_with_node = R;

    R_with_node.push_back(*node);
    BronKerbosch(R_with_node, intersection(P, graph[*node]), intersection(X, graph[*node]), graph);
    X.push_back(*node);
    node = P.erase(node);
  }
}

void print_largest_clique(string str){
  cout << str << " = ( ";
  for (int node : result_cliques.front()){
    cout << node << " ";
  }
  cout << ")\n";
}

void print_maximum_maximal_cliques(){
  result_cliques.sort(sort_by_vector_size);
  print_largest_clique("MAXIMUM CLIQUE");
  result_cliques.pop_front();
  print_largest_clique("MAXIMAL CLIQUE");

}

int main(int argc, char const *argv[]) {
  if(!checks_arguments(argc, argv[1])) end_program();

  vector<vector<int>> graph(MAX_VERTEX);
  vector<pair<int,int>> nodesDegrees(MAX_VERTEX);
  vector<int> R,P,X;

  create_graph_from_file(graph, argv[1]);

  calculates_nodes_degrees(graph, nodesDegrees);
  sort_nodes_by_degrees(nodesDegrees);
  print_sorted_nodes(nodesDegrees);

  init_P(P);
  BronKerbosch(R, P, X, graph);
  print_maximum_maximal_cliques();

  end_program();
}