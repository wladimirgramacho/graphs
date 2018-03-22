#include <stdio.h>
#include <vector>
#include <array>
#include <string.h>

using namespace std;


int checks_arguments(int argc, char const *argv[]) {
  if(argc == 1 || (argc > 1 && strcmp(argv[1],"karate.gml")) ) {
    printf("É necessário passar o arquivo de entrada karate.gml na execução do programa\n");
    return 0;
  }
  return 1;
}

int end_program() {
  return 0;
}


int main(int argc, char const *argv[]) {
  if(!checks_arguments(argc, argv)) end_program();





  end_program();
}