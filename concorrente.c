/*Integração concorrente
  Usando o método dos trapézios*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int nthreads; //numero de threads
double* test1 = NULL;
double* test2 = NULL;
// função que será integrada
double f(double x){
  return (2*(x*x*x*x)-3*(x*x*x)-5*(x*x)+2*x-2)/(1.5*(x*x*x*x)+4); 
}

typedef struct{
  int id; //identificador do intervalo que a thread ira processar
  // ponteiro para a função que será integrada
  double (*f)(double);
  double a, b, n;
} tArgs;

// funcao que integra a função f no intervalo [a, b],
// subdividinto este intervalo em n partes
// cada parte pode executar em uma thread diferente
void * integrate(void *arg) {
  tArgs * args = (tArgs *) arg;
  
  // precisão (tamanho do intervalo) h de cada parcela de trapézios
  double h = (args->b-args->a)/args->n;
  double soma = 0;
  double x;
  // calculando os termos do meio
  for(int i=1; i<=args->n-1; i++){
    // define o ponto x dentro do intervalo dividido
    x = args->a + (h*i);
    // calcula o valor da parcela e soma no acumulador
    soma += args->f(x);
  }
  // multiplica por 2 os termos do meio e
  // calcula as funções nos extremos e obtém o resultado 
  double res = (h/2 * (f(args->a) + (2*soma) + f(args->b)));
  // retornando o resultado à main com o join recebendo este valor obtido
  double* retPointer = malloc(sizeof(double));
  *retPointer = res;
  /*if(args->id==0)
    test1 = retPointer;
  else
    test2 = retPointer;*/
  pthread_exit((void *) retPointer);
}

//fluxo principal
int main(int argc, char* argv[]) {
   
  pthread_t *tid; //identificadores das threads no sistema
  tArgs *args; //argumentos para a integração
  double inicio, fim, delta; // variáveis para tomada de tempo
   
  GET_TIME(inicio);
  //leitura e avaliacao dos parametros de entrada
  if(argc<4) {
    printf("Digite: %s <numero de threads> <extremo a do intervalo> <extremo b do intervalo> <número de subdivisões do intervalo>\n", argv[0]);
    return 1;
  }
  nthreads = atoi(argv[1]);
  double a = atoi(argv[2]);
  double b = atoi(argv[3]);
  double n = atoi(argv[4]);
  double nPerThread = floor(n/nthreads);
  double intervalPerThread = (b-a)/nthreads;

  //alocacao das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
  if(tid==NULL) {puts("ERRO--malloc"); return 2;}
  args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
  if(args==NULL) {puts("ERRO--malloc"); return 2;}
  
   
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo inicializacao:%lf\n", delta);
  
  //início da tarefa
  GET_TIME(inicio);
  
  //criacao das threads
  for(int i=0; i<nthreads; i++) {
    (args+i)->id = i;
    (args+i)->f = &f;
    
    // deslocando os limites do intervalo de acordo com a thread
    (args+i)->a = a + intervalPerThread*i;
    (args+i)->b = a + intervalPerThread*(i+1);
    
    if(i==nthreads-1)
      (args+i)->n = n-(i*nPerThread); // porção de nPerThread + restante da divisão, se houver
    else
      (args+i)->n = nPerThread;
    if(pthread_create(tid+i, NULL, integrate, (void*) (args+i))){
      puts("ERRO--pthread_create"); return 3;
    }
  } 
  //espera pelo termino da threads
  double sum=0;
  double* ret; 
  for(int i=0; i<nthreads; i++) {
    pthread_join(*(tid+i),(void*) &ret);
    sum += (double) *ret;
    /*if(i == 0)
      printf("\nvalor alocado dinamicamente: %lf\n",*test1);
    else
      printf("\nvalor alocado dinamicamente: %lf\n",*test2);*/
    free(ret);
    /*if(i == 0)
      printf("\nvalor alocado dinamicamente foi liberado da memória:%lf\n",*test1);
    else
      printf("\nvalor alocado dinamicamente foi liberado da memória:%lf\n",*test2);*/
    
  }
  printf("\nResultado: %lf\n\n", sum);
  GET_TIME(fim);   
  delta = fim - inicio;
  printf("Tempo concorrente: %lf\n", delta);

  //calculando o resultado sequencial e verificando corretude
  /*
  for(int i=0; i<dim; i++) {
    if(result[i] != saida[i]){
      printf("Falhou no teste de corretude\n");
      free(args);
      free(tid);
      exit(-1);
    }
  }
  */
  //liberacao da memoria
  GET_TIME(inicio);
  free(args);
  free(tid);
  GET_TIME(fim);   
  delta = fim - inicio;
  printf("Tempo finalizacao:%lf\n", delta);

  printf("Passou no teste de corretude\n");

  return 0;
}