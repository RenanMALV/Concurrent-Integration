/*Integração concorrente
  Usando o método dos trapézios*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int nthreads; //numero de threads

// função que será integrada
double f(double x){
  return 2*x; 
}

typedef struct{
  int id; //identificador do intervalo que a thread ira processar
  float a; //intervalo [a,b] de integração
  double (* fu)(double) = &f;
} tArgs;

//funcao que as threads executarao
void * integrate(void *arg) {
  tArgs *args = (tArgs*) arg;

  
  pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char* argv[]) {
   
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta; // variáveis para tomada de tempo
   
   GET_TIME(inicio);
   //leitura e avaliacao dos parametros de entrada
   if(argc<4) {
      printf("Digite: %s <numero de threads> <extremo a do intervalo> <extremo b do intervalo> <número de subdivisões do intervalo>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   entrada = (int *) malloc(sizeof(int) * dim);
   if (entrada == NULL) {printf("ERRO--malloc\n"); return 2;}
   saida = (float *) malloc(sizeof(float) * dim);
   if (saida == NULL) {printf("ERRO--malloc\n"); return 2;}
   result = (float *) malloc(sizeof(float) * dim);
   if (result == NULL) {printf("ERRO--malloc\n"); return 2;}

   srand((unsigned) time(NULL));
   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
    entrada[i] = rand(); 
    saida[i] = 0;
    result[i] = 0;
   }

   //--inicilaiza o mutex (lock de exclusao mutua)
   pthread_mutex_init(&mutex, NULL);
  
   GET_TIME(fim);
   delta = fim - inicio;
   //printf("Tempo inicializacao:%lf\n", delta);
  
   //início da tarefa
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, integrate, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim);   
   delta = fim - inicio;
   printf("Tempo concorrente: %lf\n", delta);

  //calculando o resultado sequencial
  GET_TIME(inicio);
  for(int i=0; i<dim; i++) {
    if (ehPrimo(entrada[i]))
      result[i] = sqrt(entrada[i]);
    else
      result[i] = entrada[i];
  }
  GET_TIME(fim);   
  delta = fim - inicio;
  printf("Tempo sequencial: %lf\n", delta);
  for(int i=0; i<dim; i++) {
    if(result[i] != saida[i]){
      printf("Falhou no teste de corretude\n");
      free(entrada);
      free(saida);
      free(result);
      free(args);
      free(tid);
      pthread_mutex_destroy(&mutex);
      exit(-1);
    }
  }
  //liberacao da memoria
  GET_TIME(inicio);
  free(entrada);
  free(saida);
  free(result);
  free(args);
  free(tid);
  pthread_mutex_destroy(&mutex);
  GET_TIME(fim);   
  delta = fim - inicio;
  //printf("Tempo finalizacao:%lf\n", delta);

  printf("Passou no teste de corretude\n");

  return 0;
}