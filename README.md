# T1-Paralela
Trabalho 1 da matéria de Programação Paralela

Ci1316 2sem2024  - trabalho 1 - versao 1.1
---------------

DATA da Entrega: 01/nov/24
Local de entrega: UFPR Virtual
Equipe de alunos:
   Esse trabalho pode ser feito em grupo de NO MAXIMO dois alunos(as)

----------------------    
Historico:
versao 1.0 versao inicial
Versao 1.1 pequenas melhorias nas descricoes
Versao 1.2 pequenas melhorias nas descricoes *anotadas com (v1.2)*
     
OBS: essa especificacao pode receber pequenas alteracoes em breve
     para melhor clareza das atividades      
--------------------------

Nesse trabalho 1 vamos paralelizar a função abaixo,
conforme essa especificacao inicial.

- As as funções devem ENCAPSULAR tudo
  para a paralelização 
  (conforme exemplo visto em aula para a redução).
  
  OU SEJA: voce DEVE basear suas soluçoes para as funcoes
           abaixo tomando como base 
           a solucao disponibilizada pelo prof. de reducao paralela com pool de threads

- As suas novas soluceos devem ser feitas TAMBEM com pool de threads.

- Voce pode fazer as experiencias se baseando nas planilhas feitas pelo prof.
  (experiencias baseadas na planilha de reducao paralela com pool de threads)
  adaptando suas experiencias para os casos de bsearch_lower_bound (ver abaixo)
  
- Para as experiencias:
  Somente serao aceitos resultados obtidos 
  nas maquinas do lab 12 (intel i5) preferencialmente
  ou com maquinas de lab 5 (AMD FX-6300)  
  
  A quantidade de elementos para as experiencias:
  Vamos especificar aqui em breve.
  Tipicamente teremos:
  - Vetor Input de 
     1Milhao, 2Milhoees, 4Milhoes, 8Milhoes e 16Milhoes de elementos long long
  - Vetor de pesquisa Q, e vetor Pos  (parte B do trabalho)
     podemos pensar em 100mil elementos nesse vetor (em principio)
  - (v1.2) o mesmo vetor de pesquisa DEVE ser usado no loop de medicao da parte A
    tambem, ver observacoes abaixo.    
     
  - Como vamos gerar os vetores ?
    Vamos gerar numeros aleatorios para colocar nesses vetores
    * (v1.2) Para o vetor de entrada:
      como a pesquisa binária só funciona no vetor ordenado
      após gerar o vetor de entrada voce DEVE chamar uma funçao de biblioteca de C
      para ordenar esse vetor (no inicio do programa)
      Esse ordenaçao NAO precisa ser paralela.
    * (v1.2) Para o vetor de busca:
      O vetor de busca pode ser gerado aleariamente também no início do programa
      e usaremos esse vetor SEM ordenar   
      Esse vetor pode ser usado tanto na parte A (para faze varias pesquisas e medir)
      quanto na parte B
      Note que a parte A e B sao algoritmos DIFERENTES (v1.2)
      ------------------------------------------------
      o vetor de pesquisa na parte A, eh usado apenas para medir varias
      chamadas da pesquisa (chamadas um a um, serialmente, para cada pesquisa)
      Entao o vetor de pesquisa, aqui na parte A eh apenas uma conveniencia 
      para realizar as MESMAS pesquisas da parte B, porém na parte B vai usar o vetor de pesquisa
      internamente já que as varias pesquisas executam em paralelo. Já na parte B
      o paralelismo será tentado apenas dentro da pesquisa de cada valor.
      Assim poderemos comparar os dois métodos para as mesmas pesquisas,
      sendo que, potencialmente, a parte B será bem mais eficiente.
      
   * (V1.2) Minimizar efeitos de cache nas mediçoes:
     Como faremos as medicoes pelo menos 10 vezes para tirar medias,
     *conforme feito na versao da reduçao paralela*, 
     TANTO o vetor de entrada QUANTO o vetor Q 
     DEVEM SER ser replicados a cada para que se faça uma pesquisa
     em uma copia de cada vetor, a cada nova pesquisa. Assim
     evitamos medir errado, quando os efeitos de cache mudam nossos resultados
     de desempenho.  
   
       

O que entregar?
---------------

- dois codigos (apenas 1 aquivo por codigo)
   . 1 para a parte A e outro para a parte B
   . script para compilar
   . scripts para rodar os experimentos (pode se basear nos scripts do prof)     
   . duas planilhas, uma para a parte A, outra para a parte B 
       (DEVE se basear nas planilhas do prof de reducao paralela com pool de threads)
       adaptando a planilha para as nossas experiencias aqui descritas
       
       ATENCAO: A vazao deve ser adaptada: 
             - para calcular numero de operacoes de busca feitas por segundo (OPS)
        
  

A FUNÇÂO a ser paralelizada
---------------------------

  A função bsearch_lower_bound 
  - é uma versão da pesquisa binária: 
  - Tenta encontrar o valor do elemento em um intervalo ordenado [primeiro, último). 
  - Especificamente, ele retorna a primeira posição onde o valor poderia ser inserido sem violar a ordem.
  
Conforme especificado, vamos paralelizar DUAS versoes:
 - parte A do trabalho: tentar paralelizar UMA unica pesquisa binária
 - parte B do trabalho: paralelizar a busca de um conjuto Pos de elementos
      vetor Pos: é o vetor de pesquisa (Queueries)
  
Exemplo para pequeuenos vetores:
// apenas como exemplo, 
// para o trabalho pesquisaremos em vetores de milhares ou milhoes de elementos

   long long Input = { 0, 2, 5, 7, 8 };
   int n = 5;   // numero de elementos preenchidos no vetor

 - parte A do trabalho: tentar paralelizar UMA unica pesquisa binária
 
    bsearch_lower_bound(Input, n, 0); // retorna 0
    bsearch_lower_bound(Input, n, 1); // retorna 1
    bsearch_lower_bound(Input, n, 2); // retorna 1
    bsearch_lower_bound(Input, n, 3); // retorna 2
    bsearch_lower_bound(Input, n, 8); // retorna 4
    bsearch_lower_bound(Input, n, 9); // retorna 5

 - parte B do trabalho: paralelizar a busca de um conjuto Pos de elementos
 
   long long Q = { 3, 8, 9 };
   int nQ = 3;   // numero de elementos preenchidos no vetor
    
    bsearch_lower_bound(Input, n, Q, nQ, Pos); 
      // retorna em Pos os valores {2, 4, 5}
      
    nQ = 2;  
    bsearch_lower_bound(Input, n, Q, nQ, Pos); 
      // retorna em Pos os valores {2, 4}



