#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BolsaCripto{
    float bitcoin, ethereum, ripple;
} BolsaCripto;

void salvar_cota(FILE* arquivo, BolsaCripto* valores){
    fwrite(valores,sizeof(BolsaCripto),1, arquivo);
}
void ler_cota(FILE* arquivo, BolsaCripto* valores){
    fread(valores, sizeof(BolsaCripto), 1, arquivo);
}

int main(void){
    BolsaCripto moedas;
    moedas.bitcoin = 312444.10;
    moedas.ethereum = 16233.20;
    moedas.ripple = 231.03;
    // Salvando cotas em arquivo binario
    FILE* cotas = fopen("cotas", "wb");

    if(cotas == NULL){
        printf("Não foi possivel abrir o arquivo para escrever");
        return 1;
    }

    salvar_cota(cotas, &moedas);

    fclose(cotas);

    // limpando a variavel bitcoin em moedas para teste de recuperacao dos dados
    moedas.bitcoin = 0;
    moedas.ethereum = 0;
    moedas.ripple = 0;

    // Lendo as cotas do arquivo binario
    puts("Conteudo do arquivo");
    cotas = fopen("cotas", "rb");
    if(cotas == NULL){
        printf("Não foi possivel abrir o arquivo para ler");
        return 1;
    }
    ler_cota(cotas, &moedas);
    printf("bitcoin: %.2f\nethereum: %.2f\nripple: %.2f", moedas.bitcoin, moedas.ethereum, moedas.ripple);
    fclose(cotas);
    
    return 0;
}
