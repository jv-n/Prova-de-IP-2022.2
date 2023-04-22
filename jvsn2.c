//Joao Victor da Silva Nascimento

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{

    char nome[101];
    char cpf[12];
    int qtdNotas;
    float* notas;
    float media;

} Aluno;

Aluno* carregaAlunos(int *qtdAlunos);
float* leNotas(int* qtdNotas);
float calcmedia(float** notas, int qtdNotas);
void ordena (Aluno** alunos, int qtdAlunos);
void salvaAlunos(Aluno* alunos, int qtdAlunos);

int main(){

    Aluno* alunos = NULL;
    int qtdAlunos = 0;

    alunos = carregaAlunos(&qtdAlunos); //vai carregar o arquivo de notas
    
    for (int i = 0; i<qtdAlunos; i++)
    {
        printf("Digite as notas de %s:\n", alunos[i].nome);
        alunos[i].qtdNotas = 0;
        alunos[i].notas = leNotas(&alunos[i].qtdNotas);
        alunos[i].media = calcmedia(&alunos[i].notas, alunos[i].qtdNotas);
    }

    ordena(&alunos, qtdAlunos); // ordena o vetor

    salvaAlunos(alunos, qtdAlunos); // salva o vetor em uma file .bin

    printf("Aluno\tCPF\tMEDIA\n"); //printa uma tabela com nome cpf e media do aluno
    for (int i = 0; i<qtdAlunos; i++)
    {
       printf("%s\t%s\t%.1f\n", alunos[i].nome, alunos[i].cpf, alunos[i].media);
    }

    free(alunos);

    return 0;
}

Aluno* carregaAlunos(int *qtdAlunos)
{
    //Considerando o arquivo .txt formatado como: 1 linha do arquivo contem o nome e o cpf do aluno, o cpf nao esta separado por '.' ou '-'//
    FILE* arq;
    Aluno* temp = NULL; //vai armazenar o nome e o cpf dos alunos
        
    arq = fopen("alunos.txt", "r");
    if(arq == NULL) printf("Falha\n");
    char linha[115]; // vai armazenar uma linha do arquivo
    char nome[101]; //vai armazenar o nome;
    char cpf[12]; // vai armazenar o cpf;

    while(!feof(arq))
    {
        fscanf(arq, "%100[^\n]\n", linha); //lê a linha inteira contendo nome e cpf do aluno

        int cont = 0;
        while((linha[cont] < '0')||(linha[cont]>'9')) cont++; // enquanto n achar um numero adiciona ao contador
        
        strncpy(nome, linha, cont-1);// a posicao linha[cont] vai indicar o começo do cpf, então, o nome irá até linha[cont-1]
        
        nome[cont-1] = '\0'; //define o caractere espçao ' ' que separa nome e cpf como final da string de nomes
        
        int i = 0;
        while(linha[cont]!='\0')// da linha[cont] até o final da string linha teremos o cpf
        {
            cpf[i] = linha[cont];//adiciona os numeros do cpf no char* cpf
            cont++; i++; //passa para o proximo numero;
        }

        cpf[i] = '\0';//o ultimo digito do cpf sera '\0'

        (*qtdAlunos)++; //+1 aluno
        temp = (Aluno*) realloc(temp, sizeof(Aluno)*(*qtdAlunos)); //aloca o espaço necessario em um vetor temporario
        
        if(temp == NULL) 
        {
            printf ("falha na alocacao"); 
            exit(1);
        }
        
        strcpy(temp[(*qtdAlunos) - 1].nome, nome); //adiciona o nome do aluno em temp
        strcpy(temp[(*qtdAlunos) - 1].cpf, cpf); //adiciona o cpf do aluno em temp
        
    }
    
    fclose(arq);
    printf("Arquivo carregado!\n");

    return temp;
}

float* leNotas(int* qtdNotas)
{
    float* notas = NULL; //vai armazenar TODAS as notas daquele aluno;
    float nota_temp = 0; //vai armazenar temporariamente 1 nota do aluno
    while(nota_temp>-1) // lê enquanto a nota for positiva;
    {
        scanf("%f", &nota_temp); //recebe uma nota do aluno
        if(nota_temp>-1) // se a nota for negativa, nao adiciona ela ao vetor de notas;
        {
            (*qtdNotas)++; //aumenta a qtd de notas para aquele aluno
            notas = (float*) realloc(notas, sizeof(float)*(*qtdNotas)); //aloca o espaço de uma nota no vetor de notas
            if(notas == NULL) printf("FALHA DE ALOCACAO");
            notas[(*qtdNotas)-1] = nota_temp; // armazena a nota no vetor de notas
        }
    }

    return notas; //retorna as notas de 1 aluno
}

float calcmedia(float** notas, int qtdNotas)
{
    float media_temp = 0;//media temporaria, vai guardar a soma de todas as notas

    for(int i = 0; i<qtdNotas; i++)
    {
        media_temp = media_temp + (*notas)[i];//vai somando as notas
    }

    float media = media_temp/qtdNotas; //calcula a media, dividindo a soma de todas as notas pela quantidade de notas
    
    return media; //retorna a media de 1 aluno
}

void ordena (Aluno** alunos, int qtdAlunos)
{
    int troca = 0; // variavel booleana q vai determinar se realiza uma troca
    do
    {
        troca = 0;
        for (int i = 1; i<qtdAlunos; i++) //roda por todo o vetor de alunos
        {
            if((*alunos)[i].media>(*alunos)[i-1].media) //achou alguma media crescente
            {
                troca = 1; //troca é verdadeira
                Aluno temp; //variavel temporaria que vai armazenar o valor fora de ordem

                temp = (*alunos)[i-1];
                (*alunos)[i-1] = (*alunos)[i];
                (*alunos)[i] = temp;

                //fez a troca para aqueles dois vetores, como troca == 1, o programa irá rodar o ciclo no vetor novamente para achar alguma media fora de ordem
            }
        }
    
    } while(troca == 1); //vai trocar enquanto troca == 1
    
}

void salvaAlunos(Aluno* alunos, int qtdAlunos)
{
    FILE* arq;
    arq = fopen("alunos.bin", "w"); //vai escrever em um .bin
    if (arq == NULL) printf("ERRO AO ABRIR\n"); //se falhar ao abrir;

    fwrite(alunos, sizeof(Aluno), qtdAlunos, arq); //vai escrever o conteudo do vetor alunos na file .bin
    fclose(arq); //fecha o arquivo
    printf("Salvo em Arquivo!\n"); //confirmacao de salvo!
}
