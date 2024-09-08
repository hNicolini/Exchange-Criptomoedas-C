#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int usuario(void);
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
    usuario();
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
