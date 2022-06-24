# Concurrent integration
Integração numérica trapezoidal concorrente.

### Instruções para execução
``` bash
# Compilar programa na versão sequencial
  gcc -c curves.c -Wall -lm && gcc sequencial.c -o sequencial curves.o -Wall -lm
# Executar a versão sequencial
  ./sequencial

# Compilar programa na versão concorrente
  gcc -c curves.c -Wall -lm && gcc -pthread concorrente.c -o concorrente curves.o -Wall -lm
# Executar a versão concorrente
# ./concorrente <numero de threads>
  ./concorrente 2
```

### Referências
- Aula de cálculo numérico do professor João Paixão - IC - UFRJ;
- Documento PDF sobre o método do trapézio: https://www1.univap.br/spilling/CN/CN_Capt6.pdf;
- Exemplos de funções da biblioteca pthread com valores de retorno por referência: https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/ThreadArgs.html;
- Solver usado para resolver as integrais definidas a fim de ratificar o algoritmo sequencial: https://pt.symbolab.com/solver/;