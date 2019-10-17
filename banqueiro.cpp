//g++ -o banqueiro banqueiro.cpp -lpthread
//./banqueiro

#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <semaphore.h>

void* operation1(void* arg);
void* operation2(void* arg);
void* operation3(void* arg);

sem_t dataUse;
sem_t logUse;
sem_t varUse;

float var;
// P0, P1, P2 are the Process names here 
pthread_t f1, f2, f3;
int i, j, k; 
const int n = 3;
const int m = 3;

int alloc[3][3] = { { 0, 0, 0 }, // P0    // Allocation Matrix 
                    { 0, 0, 0 }, // P1 
                    { 0, 0, 0 } };// P2
  
int max[3][3] = { { 1, 1, 1 }, // P0    // MAX Matrix 
                  { 1, 1, 1 }, // P1 
                  { 1, 1, 1 } }; // P2 
  
int avail[3] = { 1, 1, 1 }; // Available Resources 
  
int f[n], ind = 0;
int need[n][m];

void checkAndExecute() 
{
    int y = 0;
    for (k = 0; k < 3; k++) { 
        for (i = 0; i < n; i++) { 
            if (f[i] == 0) { 
  
                int flag = 0; 
                for (j = 0; j < m; j++) { 
                    if (need[i][j] > avail[j]){ 
                        flag = 1; 
                         break; 
                    } 
                } 
  
                if (flag == 0) {
                	//printf("i=%d\n", i);
          			if (i == 0)
                    pthread_create(&f1, NULL, &operation1, NULL);
                    if (i == 1)
					pthread_create(&f2, NULL, &operation2, NULL);
					if( i == 2)
					pthread_create(&f3, NULL, &operation3, NULL);
					
                    for (y = 0; y < m; y++) 
                        avail[y] += alloc[i][y];
                    if(i == 0)
                    pthread_join(f1, NULL);
                    if(i == 1)
					pthread_join(f2, NULL);
					if(i == 2)
					pthread_join(f3, NULL);
                    f[i] = 1; 
                } 
            } 
        } 
    }
  
}

void readFile(int *a, int *b) {
	std::ifstream infile("data.txt");
	int _a, _b;
	infile >> _a >> _b;
	*a = _a;
	*b = _b;
}

void writeFile(std::string s) {
	std::ofstream outfile("log.txt", std::ios_base::out | std::ios_base::app);
	outfile << s << std::endl;
}

void show(std::string s) {
	std::cout << s << std::endl;
}

int main() {
	sem_init(&dataUse, 0, 1);
	sem_init(&logUse, 0, 1);
	sem_init(&varUse, 0, 1);
	
	for (k = 0; k < n; k++) { 
        f[k] = 0;
    } 
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) 
            need[i][j] = max[i][j] - alloc[i][j]; 
    }
	
	checkAndExecute();
	

}

void* operation1(void* arg) {
	sem_wait(&dataUse);	
	int a, b;
	readFile(&a, &b);	
	sem_post(&dataUse);

	sem_wait(&varUse);
	var = a + b;
	sem_post(&varUse);

	sem_wait(&logUse);
	writeFile("arquivo de leitura aberto");
	writeFile("dados lidos");
	std::string s = std::to_string(var);
	writeFile(s);
	sem_post(&logUse);

	sem_wait(&varUse);
	std::string ss = std::to_string(var);
	show(ss);
	sem_post(&varUse);
	

	pthread_exit(NULL);
}

void* operation2(void* arg) {
	sem_wait(&dataUse);	
	int a, b;
	readFile(&a, &b);	
	sem_post(&dataUse);
	
	sem_wait(&varUse);
	var = a * b;
	sem_post(&varUse);
	
	sem_wait(&logUse);
	writeFile("arquivo de leitura aberto");
	writeFile("dados lidos");
	std::string s = std::to_string(var);
	writeFile(s);
	sem_post(&logUse);
	
	sem_wait(&varUse);
	std::string ss = std::to_string(var);
	show(ss);
	sem_post(&varUse);
	
	pthread_exit(NULL);
}

void* operation3(void* arg) {
	sem_wait(&dataUse);	
	int a, b;
	readFile(&a, &b);	
	sem_post(&dataUse);
	
	sem_wait(&varUse);
	var = a / b;
	sem_post(&varUse);
	
	sem_wait(&logUse);
	writeFile("arquivo de leitura aberto");
	writeFile("dados lidos");
	std::string s = std::to_string(var);
	writeFile(s);
	sem_post(&logUse);
	
	sem_wait(&varUse);
	std::string ss = std::to_string(var);
	show(ss);
	sem_post(&varUse);
	
	pthread_exit(NULL);
}
