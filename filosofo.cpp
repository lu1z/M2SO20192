//g++ -o filosofo filosofo.cpp -lpthread
//./filosofo


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

void show() {
	std::cout << var << std::endl;
}

void* operation1(void* arg) {
	sem_wait(&dataUse);
	sem_wait(&varUse);
	
	int a, b;
	readFile(&a, &b);
	var = a + b;
	show();
	
	sem_post(&dataUse);
	sem_post(&varUse);
	
	pthread_exit(NULL);
}

void* operation2(void* arg) {
	sem_wait(&dataUse);
	sem_wait(&logUse);
	
	int a, b;
	readFile(&a, &b);
	var = a * b;
	std::string s = std::to_string(var);
	writeFile(s);
	
	sem_post(&dataUse);
	sem_post(&logUse);

	pthread_exit(NULL);
}

void* operation3(void* arg) {
	sem_wait(&logUse);
	sem_wait(&varUse);
	
	writeFile("calculo desconhecido ????????? iniciado");
	writeFile("exibindo lixo de memoria de var na tela");
	show();
	
	sem_post(&logUse);
	sem_post(&varUse);
	
	pthread_exit(NULL);
}

int main() {
	sem_init(&dataUse, 0, 1);
	sem_init(&logUse, 0, 1);
	sem_init(&varUse, 0, 1);

	pthread_t f1, f2, f3;
	
	pthread_create(&f1, NULL, &operation1, NULL);
	pthread_create(&f2, NULL, &operation2, NULL);
	pthread_create(&f3, NULL, &operation3, NULL);
	
	pthread_join(f1, NULL);
	pthread_join(f2, NULL);
	pthread_join(f3, NULL);
	
	return 0;
}
