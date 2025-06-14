#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 50
#define MAX_MEDICOES 50

typedef struct
{
    int timestamp;
    char id_sensor[MAX];
    double valor;

} Medicao;

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano) {
    struct tm t;
    time_t timestamp_inicial, timestamp_final;
    
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    
    timestamp_inicial = mktime(&t);
    if (timestamp_inicial == -1) {
        printf("Data inválida.\n");
        return -1;
    }

    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;
    
    timestamp_final = mktime(&t);
    if (timestamp_final == -1) {
        printf("Data inválida.\n");
        return -1;
    }

    time_t timestamp_aleatorio = timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
    
    return timestamp_aleatorio;
}

void gerarArquivo(char nomes_arquivo[5][10], FILE *file, int dia, int mes, int ano){

    int static i = 0;

    if (i == 5)
    {
        printf("numero de chamadas a funcao excedido!");
        return;
    }
    
    file = fopen(nomes_arquivo[i], "w");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "<TIMESTAMP><ID_SENSOR><VALOR>\n");

    for (int p = 0; p < 100; p++)
    {
        fprintf(file, "%ld,%s,%.2lf\n",
            gerar_timestamp_aleatorio(dia, mes, ano),
            nomes_arquivo[i],
            5.0 + ((double)rand() / RAND_MAX) * (15.0 - 5.0));
    }
    
    i++;  // Avança para o próximo nome (PRES.txt)
}

int main()
{
    char tipo_sensor[30];
    char nomes_arquivo[5][10] = {"TEMP.txt","PRES.txt","VIBR.txt","UMID.txt","FLUX.txt"};
    FILE *file;
    Medicao medicoes[MAX_MEDICOES];
    int dia, mes, ano;

    printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
    if (scanf("%d %d %d", &dia, &mes, &ano) != 3) {
        while (getchar() != '\n');
        printf("Entrada inválida. Tente novamente.\n");
    }

    for (int i = 0; i < 5; i++)
    {
        gerarArquivo(nomes_arquivo, file, dia, mes, ano);
    }

    return 0;
}