#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef struct bolsa{
    // declarando as moedas
    float bitcoin, ethereum, ripple;
} bolsa;

int main(void){
    bolsa moedas;
    moedas.bitcoin = 312444.10;
    moedas.ethereum = 16233.20;
    moedas.ripple = 231.03;
    // Salvando cotas em arquivo binario
    FILE* cotas = fopen("cotas", "wb");
    fwrite(&moedas,sizeof(bolsa),1, cotas);
    fclose(cotas);
    // limpando a variavel bitcoin em moedas para teste
    moedas.bitcoin = 0;
    moedas.ethereum = 0;
    printf("Moeda zerada: %.2f\n", moedas.bitcoin);
    // Lendo as cotas do arquivo binario
    puts("Conteudo do arquivo");
    cotas = fopen("cotas", "rb");
    fread(&moedas, sizeof(bolsa), 1, cotas);
    printf("bitcoin: %.2f\nethereum: %.2f\nripple: %.2f", moedas.bitcoin, moedas.ethereum, moedas.ripple);
    fclose(cotas);

}