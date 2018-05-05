/***********************************************************\
|*                                                         *|
|*   Teoria e Aplicação de Grafos, Turma A, 1/2018         *|
|*   Grupo:     Wladimir Gramacho & Leonardo de Almeida    *|
|*   Matrícula: 15/0048718        & 15/0135491             *|
|*   Projeto 2: Topological Sorting                        *|
|*                                                         *|
\***********************************************************/

#include <bits/stdc++.h>
using namespace std;

vector< vector<int> > graph;
int graph_size;

void show_kahn_topological_sorting(vector<int> top_sorting) {
  cout << "Kahn" << endl;
  for(int i = 0; i < top_sorting.size(); i++) {
    cout << top_sorting[i] << " ";
  }
  cout << endl;
}

bool has_topological_sorting(int node_counter) {
  if(node_counter == graph_size) {
    return true;
  } else {
    return false;
  }
}

void init_kahn(vector<int> &in_degree, queue<int> &q) {
  for(int i = 0; i < graph_size; i++) {
    vector<int>::iterator it;
    for(it = graph[i].begin(); it != graph[i].end(); it++) {
      in_degree[*it]++;
    }
  }
  for(int i = 0; i < graph_size; i++) {
    if(in_degree[i] == 0) {
      q.push(i);
    }
  }
}

void kahn() {
  vector<int> in_degree(graph_size, 0);
  vector<int> top_order;
  queue<int> q;
  int node_counter = 0;

  init_kahn(in_degree, q);

  while(!q.empty()) {
    int u = q.front();
    q.pop();
    top_order.push_back(u);

    vector<int>::iterator it;
    for(it = graph[u].begin(); it != graph[u].end(); it++) {
      if(--in_degree[*it] == 0) {
        q.push(*it);
      }
    }
    node_counter++;
  }

  if(has_topological_sorting(node_counter)) {
    show_kahn_topological_sorting(top_order);
  } else {
    cout << "Erro. O grafo possui pelo menos um ciclo." << endl;
    return;
  }
}

void dfs_visit(int v, vector<bool> &visited, stack<int> &s) {
  visited[v] = true;

  vector<int>::iterator it;
  for(it = graph[v].begin(); it != graph[v].end(); it++) {
    if(!visited[*it]) {
      dfs_visit(*it, visited, s);
    }
  }
  s.push(v);
}

void show_dfs_topological_sorting(stack<int> &s) {
  cout << "DFS" << endl;
  while(!s.empty()) {
    cout << s.top() << " ";
    s.pop();
  }
  cout << endl;
}

void dfs_mod() {
  vector<bool> visited(graph_size, false);
  stack<int> s;

  for(int i = 0; i < graph_size; i++) {
    if(visited[i] == false) {
      dfs_visit(i, visited, s);
    }
  }
  show_dfs_topological_sorting(s);
}

void add_edge(int start_node, int target_node) {
  graph[start_node].push_back(target_node);
}

void build_graph(const string graph_file) {
  ifstream input_file(graph_file.c_str());
  string line;
  int start_node, target_node;
  if (input_file.is_open()) {
    getline(input_file, line);
    stringstream ss(line);
    ss >> graph_size;
    for(int i = 0; i < graph_size; i++) {
      graph.push_back(vector<int>());
    }
    string arr[2];
    while(getline(input_file, line)) {
      int i = 0;
      stringstream ssin(line);
      while(ssin.good() && i < 2) {
        ssin >> arr[i];
        ++i;
      }
      start_node = atoi(arr[0].c_str());
      target_node = atoi(arr[1].c_str());
      add_edge(start_node, target_node);
    }
    input_file.close();
  } else {
    cout << "Erro durante a leitura do arquivo." << endl;
  }
}

void set_kahn_runtime_info(float kahn_total_time) {
  float kahn_time_per_node = kahn_total_time/graph_size;
  float runtime_seconds = 0.0;
  ofstream kahn_info;

  kahn_info.open("kahn_info.txt", ios::out | ios::trunc);
  for (int i = 0; i < graph_size; i++) {
    kahn_info << runtime_seconds << " " << i + 1 << endl;
    runtime_seconds+= kahn_time_per_node;
  }
  kahn_info.close();
}

void set_dfs_runtime_info(float dfs_total_time) {
  float dfs_time_per_node = dfs_total_time/graph_size;
  float runtime_seconds = 0.0;
  ofstream dfs_info;

  dfs_info.open("dfs_info.txt", ios::out | ios::trunc);
  for(int i = 0; i < graph_size; i++) {
    dfs_info << runtime_seconds << " " << i + 1 << endl;
    runtime_seconds+= dfs_time_per_node;
  }
  dfs_info.close();
}

void plot_comparison() {
  FILE * fp;
  fp = popen("gnuplot -persist", "w");
  if(fp == NULL) {
    cout << "Erro ao executar o gnuplot. Verificar se o mesmo está instalado." << endl;
    return;
  } else {
    fprintf(fp, "set title 'Kahn vs DFS for topological sorting'\n");
    fprintf(fp, "set autoscale\n");
    fprintf(fp, "set xlabel 'Time(seconds)'\n");
    fprintf(fp, "set ylabel 'Nodes'\n");
    fprintf(fp, "plot 'kahn_info.txt' lt rgb '#2E8B57' with lines smooth mcsplines, \
                      'dfs_info.txt' lt rgb '#B22222' with lines smooth mcsplines\n");
  }
}

void set_runtime_comparison(float kahn_time, float dfs_time) {
  set_kahn_runtime_info(kahn_time);
  set_dfs_runtime_info(dfs_time);
  plot_comparison();
}

void compare_algorithms(const string graph_file) {
  build_graph(graph_file.c_str());
  clock_t t;

  t = clock();
  kahn();
  t = clock() - t;
  float kahn_time = ((float)t)/CLOCKS_PER_SEC;

  t = clock();
  dfs_mod();
  t = clock() - t;
  float dfs_time = ((float)t)/CLOCKS_PER_SEC;

  set_runtime_comparison(kahn_time, dfs_time);
}

void init() {
  int option;

  cout << "Select a graph file" << endl;
  cout << "0 - top_small.txt" << endl;
  cout << "1 - top_med.txt" << endl;
  cout << "2 - top_large.txt" << endl;
  cout << "3 - top_huge.txt" << endl;
  cout << ">> ";
  cin >> option;

  switch (option) {
    case 0:
      compare_algorithms("top_small.txt");
      cout << "Observando o gráfico pode-se concluir que o DFS possui um desempenho melhor" << endl;
      break;
    case 1:
      compare_algorithms("top_med.txt");
      cout << "Observando o gráfico pode-se concluir que o DFS apresenta desempenho melhor na maioria dos casos" << endl;
      break;
    case 2:
      compare_algorithms("top_large.txt");
      cout << "Observando o gráfico pode-se concluir que o algoritmo de Kahn apresenta desempenho melhor" << endl;
      break;
    case 3:
      compare_algorithms("top_huge.txt");
      cout << "Observando o gráfico pode-se concluir que o DFS apresenta desempenho melhor na maioria dos casos" << endl;
      break;
  }
}

int main(int argc, char const *argv[]) {
  init();
  return 0;
}
