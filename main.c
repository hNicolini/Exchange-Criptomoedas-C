#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int usuario(void);

// aqui estão as criptomoedas
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
    printf("\nAumentou/Diminuiu %.2f%c \n",numero_gerado*100.0,37);
    //  calculando os bytes para percorrer os valores 1 por 1 (4 em 4 bytes) na struct
   size_t len_bolsa = sizeof(valores);
   for(size_t indice = 0; indice < len_bolsa; indice += sizeof(float)){
        float* pos_bolsa = (float*)((char*)&valores + indice); // char (1 byte) de moedas é somado com valor atual de indice, após e realizado conversão para float, podendo caminhar pelas variaveis floats(4 bytes) do struct

        *pos_bolsa += (*pos_bolsa)*numero_gerado;
   }
   salvar_cota(&valores);
    
}
// tela de menu pós login
char menu(void){
    char opcao;

    do{
        puts("\t\tMenu Principal\n");
        puts("1 - Consultar Saldo");
        puts("2 - Consultar Extrato");
        puts("3 - Depositar Reais");
        puts("4 - Sacar Reais");
        puts("5 - Comprar Criptomoedas");
        puts("6 - Vender Criptomoedas");
        puts("7 - Atualizar Cotacao");
        puts("8 - Sair");

        printf("\n\t\tOpcao: ");
        scanf("%c", &opcao);
        // vendo se a opcao escolhida da dentro do escopo possivel em ascii
        if(opcao < 49 || opcao > 56){
            puts("Opcao Invalida");
        }
        
    }while(opcao < 49 || opcao > 56);


    return opcao;
}

char consultar_saldo(){
    char opcao;

    puts("Aqui você consulta o saldo");
    
    do{
        fflush(stdin); // limpando buffer para não conflitar com scanf
        puts("1 - Voltar");
        printf("Opcao: ");
        scanf("%c", &opcao);
    }while(opcao != 49);

    return opcao;
}
char consultar_extrato(){
    char opcao;

    puts("Aqui você consulta o extrato");

   do{
        fflush(stdin); // limpando buffer para não conflitar com scanf
        puts("1 - Voltar");
        printf("Opcao: ");
        scanf("%c", &opcao);
    }while(opcao != 49);

    return opcao;
}
void depositar(){
    puts("Aqui você deposita");
}
void sacar(){
    puts("Aqui você saca");
}
void comprar_cripto(){
    puts("Aqui você compra criptomoedas");
}
void vender_cripto(){
    puts("Aqui você vende suas criptomoedas");
}


int main(void){
    char resposta;
    // usuario();
    BolsaCripto moedas;
    
    // pós login Menu
    resposta = menu();
    
    switch(resposta){
        case '1':
            consultar_saldo();
            break;

        case '2':
            consultar_extrato();
            break;

        case '3':
            depositar();
            break;

        case '4':
            sacar();
            break;
        
        case '5':
            comprar_cripto();
            break;

        case '6':
            vender_cripto();
            break;

        case '7':
            puts("Valor atual da moeda");

            ler_cota(&moedas);
            printf("bitcoin: %.2f\nethereum: %.2f\nripple: %.2f\n", moedas.bitcoin, moedas.ethereum, moedas.ripple);

            alterar_valor_moeda(moedas);
            ler_cota(&moedas); // salvando valor alterado

            puts("\nCotação atualizada!");
            printf("\nbitcoin: %.2f\nethereum: %.2f\nripple: %.2f", moedas.bitcoin, moedas.ethereum, moedas.ripple);
            break;
        
        case '8':
            puts("\nFim da Sessão.");
            break;
    }
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
