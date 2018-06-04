/*
    Instrucoes de compilacao:
    1. Abrir o terminal
    2. Digitar os seguintes comandos:
        g++ -std=c++11 -c galeshapley.cpp
        g++ -g -std=c++11 galeshapley.o -o galeshapley
        ./galeshapley
    OBS: é necessario que arquivo 'input.txt' esteja no mesmo diretorio

 */

#include <bits/stdc++.h>

#define FILE_NAME "entrada.txt"
#define NUM_PROFESSORS 100
#define NUM_SCHOOLS 50
#define MAX_PROFS 2

using namespace std;

class School;
class Professor;

class Professor {

    public:
        int id;
        int linked_to;
        int num_skills;
        int prefs[5];
        int idx;

        Professor(int _id, int _n, int * _arr) {
            this->id = _id;
            this->num_skills = _n;
            this->linked_to = -1;
            this->idx = -1;

            for (int i = 0; i < 5; i++) {
                this->prefs[i] = _arr[i];
            }
        }

        bool isFree(void) {
            return (this->linked_to == -1);
        }

        int applies(void) {
            this->idx++;
            if (this->idx >= 5)
                throw invalid_argument ("Professor.applies: out of range");
            return prefs[this->idx];
        }

        void link (int sch_id) {
            if (sch_id < 0)
                throw invalid_argument ("Professor.link: Invalid school.");
            this->linked_to = sch_id;
        }

        void getsDumped (void) {
            this->linked_to = -1;
        }
};

class School {

    public:
        int id;
        int req_skills;
        int linked_to[MAX_PROFS];
        int vagas;

        School (int _id, int _sk, int vagas) {
            this->id = _id;
            this->req_skills = _sk;
            this->vagas = vagas;
            for (int i = 0; i < MAX_PROFS; i++) {
                this->linked_to[i] = -1;
            }
        }

        bool isFree (void) {
            return (this->linked_to[0] == -1 || this->linked_to[1] == -1);
        }

        bool prefers_new_professor(Professor * a, Professor * b) {
            int skill_a = a->num_skills;
            int skill_b = b->num_skills;

            if (skill_a >= this->req_skills && skill_b >= this->req_skills) {
                return (skill_a >= skill_b);
            }
            if (skill_a < this->req_skills && skill_b < this->req_skills) {
                return (skill_a <= skill_b);
            }
            if (skill_a < this->req_skills && skill_b >= this->req_skills) {
                return false;
            }
            if (skill_a >= this->req_skills && skill_b < this->req_skills) {
                return true;
            }
        }

        void link (int index, int prof) {
            this->linked_to[index] = prof;
        }

        int kicks (int i) {
            int prof_id = this->linked_to[i];
            this->linked_to[i] = -1;
            return prof_id;
        }
};
/* Funcao que retorna um professor da lista que esteja sem vinculo com escolas.
   Usada no Algoritmo Gale-Shapley como condicao de parada do Algoritmo.
 */
int getFreeProfessor (Professor ** prof) {
    for (int i = 0; i < NUM_PROFESSORS; i++) {
        if (prof[i] -> isFree()) {
            return i;
        }
    }
    return -1;
}

/* Funcao que cria o vinculo entre professor e escola */
void make_new_engagement(int fprof_id, int index, School * s_cur, Professor * p_cur) {
    s_cur -> link(index, fprof_id);
    p_cur -> link(s_cur -> id - 1);
}

void become_engaged(Professor * p_cur, School * s_cur, int fprof_id) {
    for (int i = 0; i < MAX_PROFS; i++) {
        if (s_cur -> linked_to[i] == -1) {
            make_new_engagement(fprof_id, i, s_cur, p_cur);
            break;
        }
    }
}

/* Funcao que possibilita a escola quebrar a relacao com um professor.
   Utilizada caso seja feita uma oferta de professor melhor para a escola.
 */
void break_engagement(School * s_cur, int index, Professor ** profArray) {
    int break_index = s_cur -> kicks(index);
    profArray[break_index] -> getsDumped();
}

/* Funcao que possibilita a escola decidir, dentre dois professores, qual contratar */
void school_makes_choice(Professor * p_cur, School * s_cur, Professor ** profArray, int fprof_id) {
    Professor * p_engaged;

    for (int i = 0; i < MAX_PROFS; i++) {
        p_engaged = profArray[s_cur -> linked_to[i]];
        if (s_cur -> prefers_new_professor(p_cur, p_engaged) && (p_engaged -> idx < 4)) { // sucesso do professor em conseguir uma vaga em relação ao seu concorrente
            break_engagement(s_cur, i, profArray);
            make_new_engagement(fprof_id, i, s_cur, p_cur);
            break;
        }
    }
}

/*
    Implementacao do Algoritmo Gale-Shapley para realizar o emparelhamento
    entre escolas e professores. A implementacao favorece as escolas, uma vez
    que a mesma tem a possibilidade de, dado dois professores, escolher o que
    mais lhe interessa.

*/
void gale_shapley(Professor ** profArray, School ** schArray) {
    int fprof_id, provoked_school;
    Professor * p_cur;
    School * s_cur;

    fprof_id = getFreeProfessor(profArray);
    while (fprof_id != -1) {
        cout << fprof_id << endl;
        p_cur = profArray[fprof_id];
        provoked_school = p_cur -> applies() - 1;
        s_cur = schArray[provoked_school];

        if (s_cur -> isFree()) {
            become_engaged(p_cur, s_cur, fprof_id);
        } else {
            school_makes_choice(p_cur, s_cur, profArray, fprof_id);
        }
        fprof_id = getFreeProfessor(profArray);
    }
}

void init(int * distribution) {
    for (int i = 0; i < 5; i++) {
        distribution[i] = 0;
    }
}

void show_results(Professor ** profArray, School ** schArray) {
    string schools[] = {"primeira", "segunda", "terceira", "quarta", "quinta"};
    int distribution[5];

    init(distribution);
    for (int i = 0; i < NUM_PROFESSORS; i++) {
        distribution[profArray[i] -> idx]++;
    }
    for (int i = 0; i < NUM_SCHOOLS; i++) {
        printf("\t[E%2d] => [P%3d, P%3d]\n", schArray[i]->id, schArray[i]->linked_to[0]+1, schArray[i]->linked_to[1]+1);
    }
    cout << endl;
    cout << "Resumo: " << endl;
    for (int i = 0; i < 5; i++) {
        printf("\t + %2d professores contratados na %s escola\n", distribution[i], schools[i].c_str());
    }
}

int main(int argc, char const *argv[]) {

    ifstream fp;
    string line;

    School * schArray[NUM_SCHOOLS];
    Professor * profArray[NUM_PROFESSORS];

    int id, skill, preference[5];
    int vagas;

    fp.open(FILE_NAME);
    if (!fp.is_open()) {
        cout << "ERRO. Arquivo de entrada nao encontrado" << endl;
        exit(1);
    }

    /* Faz a leitura do arquivo filtrando as informacoes dos professores */
    fp.seekg (130, ios_base::beg);
    for (int i = 0; i < NUM_PROFESSORS; i++) {
        getline (fp, line);
        sscanf (line.c_str(), "(P%d, %d): (E%d, E%d, E%d, E%d, E%d)",
            &id, &skill, &preference[0], &preference[1], &preference[2], &preference[3], &preference[4]);

        profArray[i] = new Professor(id, skill, preference);
    }

    /* Faz a leitura do arquivo filtrando as informacoes das escolas  */
    fp.seekg (3719, ios_base::beg);
    for (int i = 0; i < NUM_SCHOOLS; i++) {
        getline (fp, line);
        sscanf (line.c_str(), "(E%d):(%d):(%d)", &id, &skill, &vagas);

        schArray[i] = new School(id, skill, vagas);
    }

    // gale_shapley(profArray, schArray);
    // show_results(profArray, schArray);
    fp.close();

    return 0;
}
