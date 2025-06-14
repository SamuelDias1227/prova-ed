#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 50
#define MAX_MEDICOES 50

typedef struct
{
    int timestamp;
    double valor;
} Medicao;

typedef struct
{
    char id_sensor[MAX];
    int qtd_medicoes_sensor;
    Medicao medicao[MAX_MEDICOES];

} MedicoesPorTipo;

void lerConteudoArquivo(MedicoesPorTipo medicoes_por_tipo[]);
void separarMedicoesPorTipoSensor(MedicoesPorTipo medicoes_por_tipo[], int *qtd_sensor_diferentes, int aux_timestamp, char aux_id_sensor[MAX], double aux_valor);
void ordenarMedicoesPorTimestamp(MedicoesPorTipo medicoes_por_tipo[], int qtd_sensor_diferentes);
void gerarArquivosComMedicoes(MedicoesPorTipo medicoes_por_tipo[], int qtd_sensor_diferentes);

void lerConteudoArquivo(MedicoesPorTipo medicoes_por_tipo[])
{

    int qtd_sensor_diferentes = 0;

    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int qtd_leituras = 0;

    char linha[256];

    int aux_timestamp;
    char aux_id_sensor[MAX];
    double aux_valor;

    // Ignora a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), arquivo);
    while (fgets(linha, sizeof(linha), arquivo))
    {
        char *token;

        token = strtok(linha, " ");
        if (token)
            aux_timestamp = atoi(token);

        token = strtok(NULL, " ");
        strncpy(aux_id_sensor, token, MAX);

        token = strtok(NULL, " ");
        if (token)
            aux_valor = atof(token);

        qtd_leituras++;

        separarMedicoesPorTipoSensor(medicoes_por_tipo, &qtd_sensor_diferentes, aux_timestamp, aux_id_sensor, aux_valor);
    }
    ordenarMedicoesPorTimestamp(medicoes_por_tipo, qtd_sensor_diferentes);
    gerarArquivosComMedicoes(medicoes_por_tipo, qtd_sensor_diferentes);
    fclose(arquivo);
}

void separarMedicoesPorTipoSensor(MedicoesPorTipo medicoes_por_tipo[], int *qtd_sensor_diferentes, int aux_timestamp, char aux_id_sensor[MAX], double aux_valor)
{

    bool static jaAdd = false;
    bool existe = false;

    int i = 0;
    do
    {
        if (strcmp(medicoes_por_tipo[i].id_sensor, aux_id_sensor) == 0)
        {
            medicoes_por_tipo[i].medicao[medicoes_por_tipo[i].qtd_medicoes_sensor].timestamp = aux_timestamp;
            medicoes_por_tipo[i].medicao[medicoes_por_tipo[i].qtd_medicoes_sensor].valor = aux_valor;
            medicoes_por_tipo[i].qtd_medicoes_sensor++;
            existe = true;
        }
        i++;
    } while (i < *qtd_sensor_diferentes);

    if (!existe)
    {
        if (jaAdd == true)
        {
            strncpy(medicoes_por_tipo[i].id_sensor, aux_id_sensor, MAX);
            medicoes_por_tipo[i].medicao[medicoes_por_tipo[i].qtd_medicoes_sensor].timestamp = aux_timestamp;
            medicoes_por_tipo[i].medicao[medicoes_por_tipo[i].qtd_medicoes_sensor].valor = aux_valor;
            medicoes_por_tipo[i].qtd_medicoes_sensor++;
    
            (*qtd_sensor_diferentes)++;
        } else
        {
            strncpy(medicoes_por_tipo[i-1].id_sensor, aux_id_sensor, MAX);
            medicoes_por_tipo[i-1].medicao[medicoes_por_tipo[i-1].qtd_medicoes_sensor].timestamp = aux_timestamp;
            medicoes_por_tipo[i-1].medicao[medicoes_por_tipo[i-1].qtd_medicoes_sensor].valor = aux_valor;
            medicoes_por_tipo[i-1].qtd_medicoes_sensor++;
            (*qtd_sensor_diferentes)++;
            jaAdd = true;
        }
    }
}

void ordenarMedicoesPorTimestamp(MedicoesPorTipo medicoes_por_tipo[], int qtd_sensor_diferentes)
{
    int aux_timestamp;
    double aux_valor;

    for (int p = 0; p < qtd_sensor_diferentes; p++)
    {
        int i, j;
        for (i = 0; i < medicoes_por_tipo[p].qtd_medicoes_sensor - 1; i++)
        {
            printf("\n[%d] ", i);

            for (j = 0; j < medicoes_por_tipo[p].qtd_medicoes_sensor - i - 1; j++)
            {
                printf("%d, ", j);

                if ((medicoes_por_tipo[p].medicao[j].timestamp) > (medicoes_por_tipo[p].medicao[j + 1].timestamp))
                {
                    aux_timestamp = medicoes_por_tipo[p].medicao[j + 1].timestamp;
                    medicoes_por_tipo[p].medicao[j + 1].timestamp = medicoes_por_tipo[p].medicao[j].timestamp;
                    medicoes_por_tipo[p].medicao[j].timestamp = aux_timestamp;

                    aux_valor = medicoes_por_tipo[p].medicao[j + 1].valor;
                    medicoes_por_tipo[p].medicao[j + 1].valor = medicoes_por_tipo[p].medicao[j].valor;
                    medicoes_por_tipo[p].medicao[j].valor = aux_valor;
                }
            }
        }
    }
}

void gerarArquivosComMedicoes(MedicoesPorTipo medicoes_por_tipo[], int qtd_sensor_diferentes)
{

    char nome_arquivo[MAX + 5];

    for (int p = 0; p < qtd_sensor_diferentes; p++)
    {
        strcpy(nome_arquivo, medicoes_por_tipo[p].id_sensor);

        strcat(nome_arquivo, ".txt");

        FILE *file = fopen(nome_arquivo, "w");
        if (file == NULL)
        {
            perror("Erro ao abrir o arquivo");
            return;
        }

        fprintf(file, "<TIMESTAMP><ID_SENSOR><VALOR>\n");

        for (int i = 0; i < medicoes_por_tipo[p].qtd_medicoes_sensor; i++)
        {

            fprintf(file, "%d,%s,%.2lf\n",
                    medicoes_por_tipo[p].medicao[i].timestamp,
                    medicoes_por_tipo[p].id_sensor,
                    medicoes_por_tipo[p].medicao[i].valor);
        }
        fclose(file);
    }
}

int main()
{

    // Struct para armazenar os dados
    MedicoesPorTipo medicoes_por_tipo[MAX_MEDICOES];

    lerConteudoArquivo(medicoes_por_tipo);

    return 0;
}