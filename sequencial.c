/*Integração concorrente
  Usando o método dos trapézios*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

// função que será integrada
double f(double x){
  return (2*(x*x*x*x)-3*(x*x*x)-5*(x*x)+2*x-2)/(1.5*(x*x*x*x)+4); 
}

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
  
  GET_TIME(inicio);
  //leitura e avaliacao dos parametros de entrada

  if(argc<3) {
    printf("Digite: %s <extremo a do intervalo> <extremo b do intervalo> <número de subdivisões do intervalo>\n", argv[0]);
    return 1;
  }
  float a = atoi(argv[1]);
  float b = atoi(argv[2]);
  float n = atoi(argv[3]);
  
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