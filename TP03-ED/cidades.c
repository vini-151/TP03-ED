#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cidades.h"

Estrada *getEstrada(const char *NomeArquivo){

    FILE *arquivo;

    arquivo = fopen(NomeArquivo, "r");

    if (arquivo == NULL){
        printf("Erro, nao foi possivel abrir arquivo. Verifique se o arquivo encontra-se no mesmo diretorio que o arquivo executavel.\n\n");
        exit(1);
    }

    // alocando memoria para estrada
    Estrada *x = (Estrada *)(malloc(sizeof(Estrada)));

    if (x == NULL){
        printf("Erro: nao foi possivel alocar memoria.\n");
        free(x);
        exit(1);
    }

    fscanf(arquivo, "%d\n", &x->T); // tamanho da estrada
    fscanf(arquivo, "%d\n", &x->N); //numero de cidades

    //alocando memoria para as cidades
    x->C = malloc(x->N * sizeof(Cidade));

    if (x->C == NULL){
        printf("Erro, nao foi possivel alocar memoria para array de cidades.\n");
        free(x->C);
        exit(1);
    }

    int i = 0; //tamanho do array
    

    // verificando restrições   

    if (x->T < 3 || x->T > pow(10, 6)){ //restrição 1 (3 ≤ T ≤ 10^6)
        printf("Erro: Tamanho da estrada nao suportado.\n");
        return NULL;
    }else if (x->N < 2 || x->N > pow(10, 4)){ //restrição 2 ( 2 ≤ N ≤ 10^4)
        printf("Erro, Numero de cidades incompativel.\n");
        return NULL;
    }
    
   char linha[256];

    // alocando memoria conforme a quantidade de cidades na estrada e preenchendo a lista
    while (fgets(linha,sizeof(linha), arquivo)){

        sscanf(linha, "%d %[^#]#", &x->C[i].Posicao, &x->C[i].Nome);

        if (x->C[i].Posicao < 0 || x->C[i].Posicao > x->T){ //restrição 3 (para cada cidade, 0 < Xi < T)
            printf("Erro, distacia da fronteira incompativel.\n");
            return NULL;
        }

        if (i > 0){
            if (x->C[i].Posicao == x->C[i - 1].Posicao){ //restrição 5 (Xi ≠ Xj) compara distancia da atual cidade com a cidade anterior
            printf("Erro. Restricao 05, essa cidade possui Xi = Xj.\n");
            return NULL;
            }
        }
        
    
        i++; //conta quantas cidades foram lidas (posicao do array)
    }

    if(i < 1 || i > x->N){ //restrição 4 (1 ≤ i ≤ N ) i sendo a quantidade de informações de cidades lidas pelo while, que deve ser inferior ou igual ao número de cidades "n" no arquivo.
        printf("Erro, numeros de cidades lidas nao condiz com o numero de cidades esperadas.\n");
        return NULL;
    }
    
    printf("Estrada e suas cidades inicializados com sucesso!\n");

    //ordenando as cidades na estrada

        for (int i = 0; i < x->N; i++){ 
        int minIndex = i;

        for (int j = i + 1; j < x->N; j++){
            if (x->C[j].Posicao < x->C[minIndex].Posicao){
                minIndex = j;
            }
        }
        Cidade temp = x->C[minIndex];
        x->C[minIndex] = x->C[i];
        x->C[i] = temp;
    }

    fclose(arquivo);

    return x;

}

double calcularMenorVizinhanca(const char *nomeArquivo){

    //inicializando o TAD da estrada pelo getEstrada();

    Estrada *e = getEstrada(nomeArquivo);

    //inicializando um array que conterá a posição das divisas entre cidades

    int numeroDeDivisas = e->N - 1;

    double posicaoDivisa[numeroDeDivisas];
    double areaCidade[e->N]; //array que armazena a área da cidade em relação à estrada (primeira cidade à última cidade da estrada)

    //determinando divisas

    for (int i = 0; i < numeroDeDivisas; i++){
        posicaoDivisa[i] = ((double)(e->C[i].Posicao + e->C[i + 1].Posicao) / 2); 
    }

    // calculando areas a partir das divisas

    areaCidade[0] = posicaoDivisa [0]; //determina que a área da primeira cidade que a estrada passa é igual à distância da primeira divisa em relação ao ponto de início da estrada


    //calculando as próximas áreas

    int fimDaEstrada = e->T;
    for (int i = 1; i < e->N; i++){
        if (i + 1 == e->N){ //se a cidade a ser lida for a última, a sua área será a distância da estrada - a última divisa
            areaCidade[i] = fimDaEstrada - posicaoDivisa[i - 1];
        }else{        
        areaCidade[i] = posicaoDivisa[i] - posicaoDivisa[i - 1];
        }
    }

    //retornar menor vizinhança

    double menorVizinhanca = areaCidade[0];
    int menorCidadeIndex;

    for (int i = 0; i < e->N; i++){
        if (areaCidade[i] < menorVizinhanca){
            menorVizinhanca = areaCidade[i];
            menorCidadeIndex = i; //salva o índice do array da menor área para podermos retornar o nome da cidade que possui essa área
        }
    }

    printf("Menor area: %.2lf\n", menorVizinhanca);

    

    //escrevendo no arquivo o nome da cidade que possui menor área

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL){
        printf("Erro ao gravar nome da cidade no arquivo.\n");
        exit(1);
    }
    
    fprintf(arquivo, "%s\0", e->C[menorCidadeIndex].Nome);

    fclose(arquivo);

    //retornando o valor da menor área

    return menorVizinhanca;
    
}

char *cidadeMenorVizinhanca(const char *nomeArquivo){
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    char* menorCidade = NULL;
    char linha[100]; 

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remover nova linha se existir
        linha[strcspn(linha, "\n")] = '\0';

        if (menorCidade == NULL || strlen(linha) < strlen(menorCidade)) {
            if (menorCidade != NULL) {
                free(menorCidade);
            }
            menorCidade = strdup(linha); // Alocar memória e copiar string
        }
    }

    fclose(arquivo);
    return menorCidade;
}