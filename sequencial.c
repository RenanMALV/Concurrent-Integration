/*Integração concorrente
  Usando o método dos trapézios*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include "curves.h"

// ponteiro para a função que será integrada
double (*f)(double);

// funcao que integra a função f no intervalo [a, b],
// subdividinto este intervalo em n partes 
double integrate(double (*f)(double), double a, double b, double n) {
  // resolução (tamanho do intervalo) h de cada parcela de trapézios
  double h = (b-a)/n;
  double soma = 0;
  double x;
  // calculando os termos do meio
  for(int i=1; i<=n-1; i++){
    // define o ponto x dentro do intervalo dividido
    x = a + (h*i);
    // calcula o valor da parcela e soma no acumulador
    soma += f(x);
  }
  // multiplica por 2 os termos do meio e
  // calcula as funções nos extremos e obtém o resultado 
  return (h/2 * (f(a) + (2*soma) + f(b))); 
}

//fluxo principal
int main(int argc, char* argv[]) {

  double inicio, fim, delta; // variáveis para tomada de tempo
  
  //leitura e avaliacao dos parametros de entrada

  char aux[100];
  printf("digite qual curva carregada será integrada <1/2/3>\n");
  fgets(aux, sizeof aux, stdin);
  int curve = atoi(aux);
  if(curve != 1 && curve != 2 && curve != 3){
    printf("curva %d digitada inválida\n", curve);
    exit(-1);
  }
  printf("digite o <extremo a do intervalo>\n");
  fgets(aux, sizeof aux, stdin);
  double a = atof(aux);
  printf("digite o <extremo b do intervalo>\n");
  fgets(aux, sizeof aux, stdin);
  double b = atof(aux);
  printf("digite o <número de subdivisões do intervalo>\n");
  fgets(aux, sizeof aux, stdin);
  double n = atoi(aux);

  GET_TIME(inicio);
  
  switch(curve){
    case 1:
      f = &c1;
      break;
    case 2:
      f = &c2;
      break;
    case 3:
      f = &c3;
  }
  
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo inicializacao:%lf\n", delta);
  
  //início da tarefa
  GET_TIME(inicio);

  printf("\nResultado: %lf\n\n", integrate(f, a, b, n));
  
  GET_TIME(fim);   
  delta = fim - inicio;
  printf("Tempo sequencial: %lf\n", delta);

  
  //liberacao da memoria...
  //nada alocado dinamicamente, logo, delta = 0
  GET_TIME(inicio);
  GET_TIME(fim);   
  delta = fim - inicio;
  printf("Tempo finalizacao: %lf\n", delta);

  return 0;
}