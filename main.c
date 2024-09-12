#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int usuario(void);
typedef struct BolsaCripto{
    float bitcoin, ethereum, ripple;
} BolsaCripto;

void salvar_cota(BolsaCripto* valores){
    FILE* cotas = fopen("cotas", "wb");
    // Salvando cotas em arquivo binario
    if(cotas == NULL){
        printf("Não foi possivel abrir o arquivo para escrever");
    }
    fwrite(valores,sizeof(BolsaCripto),1, cotas);
    fclose(cotas);
}
void ler_cota(BolsaCripto* valores){
    FILE* cotas = fopen("cotas", "rb");
    if(cotas == NULL){
        printf("Não foi possivel abrir o arquivo para ler");
    }
    fread(valores, sizeof(BolsaCripto), 1, cotas);
    fclose(cotas);
}
void alterar_valor_moeda(BolsaCripto valores){
    // gerando o numero aleatorio entre -5% e 5%
    srand(time(NULL));
    float numero_gerado =((rand() % 5) + 1.0)/100.0;
    int escolhe_sinal = rand()% 2;
    if(escolhe_sinal == 1){
        numero_gerado*=-1;
    }

    //  calculando os bytes para percorrer os valores 1 por 1 (4 em 4 bytes) na struct
   size_t len_bolsa = sizeof(valores);
   for(size_t indice = 0; indice < len_bolsa; indice += sizeof(float)){
        float* pos_bolsa = (float*)((char*)&valores + indice); // char (1 byte) de moedas é somado com valor atual de indice, após e realizado conversão para float, podendo caminhar pelas variaveis floats(4 bytes) do struct

        *pos_bolsa += (*pos_bolsa)*numero_gerado;
   }
   salvar_cota(&valores);
    
}


int main(void){
    usuario();
    BolsaCripto moedas;
    // moedas.bitcoin = 330667.00;
    // moedas.ethereum = 13443.73;
    // moedas.ripple = 3.05;
    //salvar_cota(&moedas);

    // Lendo as cotas do arquivo binario
    puts("Conteudo do arquivo");
    ler_cota(&moedas);
    printf("bitcoin: %.2f\nethereum: %.2f\nripple: %.2f\n", moedas.bitcoin, moedas.ethereum, moedas.ripple);
    alterar_valor_moeda(moedas);
    ler_cota(&moedas); // salvando valor alterado
    puts("\nApos alterar valor");
    printf("\nbitcoin: %.2f\nethereum: %.2f\nripple: %.2f", moedas.bitcoin, moedas.ethereum, moedas.ripple);

    return 0;
}
int login(void){
    // Função dreservada para login
}
int cadastro(void){
    
    //função reservada para cadastro de usuarios

    char usuario[80];
    char senha[30];
    
    //perguntando o cpf e senha e salvando em um arquivo binario

    FILE *arquivo = fopen("Usuario", "ab");
    printf("Digite seu CPF \n");
    scanf("%s", &usuario);
    fprintf(arquivo,"%s",&usuario);
    
    
    printf("Digite sua senha: \n");
    scanf("%s", &senha);
    fprintf(arquivo,"Senha: %s",&senha);
}
int usuario(void){
    int resposta;
    printf("Ja possui um Login?\n1- Sim\n2- Nao\n");
    scanf("%d", &resposta);
    if (resposta == 1)
    {
        login();
    }
    else if(resposta == 2)
    {
        cadastro();
    }
    else{
        printf("Numero invalido!\n");
        usuario();
    }
        
}
