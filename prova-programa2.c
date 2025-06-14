#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 50
#define MAX_MEDICOES 50

typedef struct
{
    int timestamp;
    char id_sensor[MAX];
    double valor;

} Medicao;

int lerConteudoArquivo(Medicao medicoes[], char nome_arquivo[30], int timestamp);
int buscaBinaria(Medicao medicoes[MAX_MEDICOES], int timestamp, int qtd_leituras);
int verificaTimestampMaisProximo(int i, int timestamp, Medicao medicoes[MAX_MEDICOES]);
time_t capturar_timestamp_valido();

int lerConteudoArquivo(Medicao medicoes[], char nome_arquivo[30], int timestamp)
{

    char nome_arquivo_formatado[MAX + 5];

    strcpy(nome_arquivo_formatado, nome_arquivo);

    strcat(nome_arquivo_formatado, ".txt");

    FILE *arquivo = fopen(nome_arquivo_formatado, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    int qtd_leituras = 0;

    char linha[256];

    // Ignora a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), arquivo);
    while (fgets(linha, sizeof(linha), arquivo))
    {
        char *token;

        // Lê duração
        token = strtok(linha, ",");
        if (token)
            medicoes[qtd_leituras].timestamp = atoi(token);

        // Lê nome do filme
        token = strtok(NULL, ",");
        strncpy(medicoes[qtd_leituras].id_sensor, token, MAX);

        // Lê nota
        token = strtok(NULL, ",");
        if (token)
            medicoes[qtd_leituras].valor = atof(token);

        qtd_leituras++;
    }
    fclose(arquivo);

    return buscaBinaria(medicoes, timestamp, qtd_leituras);
}

int buscaBinaria(Medicao medicoes[MAX_MEDICOES], int timestamp, int qtd_leituras)
{
    int i = 0, inicio = 0;
    int fim = qtd_leituras - 1;

    while (inicio <= fim)
    { /* Condição de parada */

        i = (inicio + fim) / 2; /* Calcula o meio do sub-vetor */

        if (medicoes[i].timestamp == timestamp)
        { /* Item encontrado */
            return i;
        }

        if (medicoes[i].timestamp < timestamp)
        { /* Item está no sub-vetor à direita */
            inicio = i + 1;
        }
        else
        { /* Item está no sub-vetor à esquerda */
            fim = i;
        }
    }

    return verificaTimestampMaisProximo(i, timestamp, medicoes);
}

int verificaTimestampMaisProximo(int i, int timestamp, Medicao medicoes[MAX_MEDICOES])
{
    int diferenca3 = medicoes[i - 1].timestamp - timestamp;
    int diferenca1 = medicoes[i].timestamp - timestamp;
    int diferenca2 = medicoes[i + 1].timestamp - timestamp;

    int indice = 0;

    int maior = diferenca1;
    indice = i;

    if (diferenca2 > maior)
    {
        maior = diferenca2;
        indice = i + 1;
    }

    if (diferenca3 > maior)
    {
        maior = diferenca3;
        indice = i - 1;
    }
    return indice;
}

time_t capturar_timestamp_valido()
{
    int dia, mes, ano, hora, min, seg;
    struct tm t;

    while (1)
    {
        printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
        if (scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &min, &seg) != 6)
        {
            while (getchar() != '\n')
                ;
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        t.tm_year = ano - 1900;
        t.tm_mon = mes - 1;
        t.tm_mday = dia;
        t.tm_hour = hora;
        t.tm_min = min;
        t.tm_sec = seg;
        t.tm_isdst = -1;

        time_t timestamp = mktime(&t);
        if (timestamp == -1)
        {
            printf("Data inválida. Tente novamente.\n");
        }
        else
        {
            return timestamp;
        }
    }
}

int main()
{
    char tipo_sensor[30];
    int timestamp;

    int i = 0;

    // Struct para armazenar os dados
    Medicao medicoes[MAX_MEDICOES];

    printf("Informe o tipo do sensor que deseja consultar suas medicoes:");
    scanf("%s", tipo_sensor);

    timestamp = capturar_timestamp_valido();

    i = lerConteudoArquivo(medicoes, tipo_sensor, timestamp);

    printf("----i=%d----\n", i);

    if (i == -1)
    {
        printf("Arquivo não encontrado. Certifique de ter digitado o tipo do sensor corretamente.");
    }
    else
    {
        printf("A medicao com a data mais próxima da informada é:\n");
        printf("Sensor: %s\n", medicoes[i].id_sensor);
        printf("timestamp: %d\n", medicoes[i].timestamp);
        printf("valor: %.2lf\n", medicoes[i].valor);
    }
    return 0;
}