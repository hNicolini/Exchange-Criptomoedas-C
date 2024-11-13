#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define LIMITE_USUARIOS 10
#define LIMITE_EXTRATOS 100


// aqui estão as criptomoedas
typedef struct BolsaCripto{
    int idcoin;
    char sigla[4];
    char nome[255];
    float cota, txbuy, txsell;
    
} BolsaCripto;

// aqui fica as taxas
// typedef struct TxCripto{
//     int idcoin;
//     float txbuy;
//     float txsell;
// } TxCripto;

typedef struct Saldo{
    int idcoin;
    float saldo;
}Saldo;

typedef struct User{

    char cpf[80];
    char nome[50];
    char senha[80];
    char extrato[LIMITE_EXTRATOS][200];
    int qntd_extrato;
    int qtd_coins;
    Saldo* saldos;

} User;


void limpaBuffer() {
    int c;
    // Lê e descarta todos os caracteres até o '\n' ou EOF
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int gera_idcoin(){
    BolsaCripto recentCoin;

    FILE* moedas = fopen("moedas", "rb");
    if(moedas == NULL){
        puts("Primeira moeda do programa.");
        return 0;
    }

    if(fseek(moedas,-(sizeof(BolsaCripto)),SEEK_END) != 0){
        puts("Nao foi possivel posicionar o ponteiro no ultimo registro do arquivo.");
        fclose(moedas);

        return -1;
    }
    
    if(fread(&recentCoin,sizeof(BolsaCripto),1,moedas) != 1){
        puts("Erro na leitura do arquivo");
        fclose(moedas);

        return -1;
    }

    int id_gerado = recentCoin.idcoin + 1;

    fclose(moedas);

    return id_gerado;

}

bool coinExists(char* coinName, char* coinTicker){
    BolsaCripto coins;

    FILE* criptomoedas = fopen("moedas", "rb");
    if(criptomoedas == NULL){

        return false;

    }

    while(fread(&coins,sizeof(BolsaCripto),1,criptomoedas)){

        if(strcmp(coinName, coins.nome) == 0){
            puts("Este nome de moeda ja existe");
            return true;
        }
        if(strcmp(coinTicker, coins.sigla) == 0){
            puts("Esta sigla de moeda ja existe");
            return true;
        }
    }
    return false;
}

void adicionar_moeda(BolsaCripto* moeda){

    puts("\t\tCadastrar Criptomoeda");

    int idcoin = gera_idcoin();

    FILE* arquivo = fopen("moedas", "ab");
    if(arquivo == NULL){
        puts("Erro na abertura do arquivo moedas");
        exit(1);
    }

    if(idcoin != -1){

        moeda->idcoin = idcoin;
        
        while(true){
            printf("Digite o nome da moeda: ");
            fgets(moeda->nome,sizeof(moeda->nome),stdin);
            // limpaBuffer();

            if(!coinExists(moeda->nome, "")){
                break;
            }
        }

        while(true){
            printf("Digite a sigla da moeda: ");
            fgets(moeda->sigla,sizeof(moeda->sigla),stdin);
            // limpaBuffer();

            if(!coinExists("", moeda->sigla)){
                break;
            }
        }

        printf("Digite a cotacao");
        scanf("%f", &moeda->cota);
        limpaBuffer();

        printf("Digite a taxa de compra: ");
        scanf("%f", &moeda->txbuy);
        limpaBuffer();

        printf("Digite a taxa de venda: ");
        scanf("%2f", &moeda->txsell);
        limpaBuffer();

        fwrite(moeda,sizeof(BolsaCripto),1,arquivo);

        puts("Criptomoeda Cadastrada com Sucesso");

    }
    else{
        puts("Erro na criacao de uma nova moeda");
    }
    
    fclose(arquivo);

    

}

int menu(){
    int opcao;
    puts("\t\tMENU ADMIN");
    puts("[1] Cadastrar Investidor");
    puts("[2] Remover Investidor");
    puts("[3] Cadastrar Criptomoeda");
    puts("[4] Remover Criptomoeda");
    puts("[5] Consultar Saldo do Investidor");
    puts("[6] Consultar Extrato do Investidor");
    puts("[7] Atualizar Cotacao");
    puts("[8] Sair");

    printf("Opcao: ");
    scanf("%d",&opcao);
    limpaBuffer();

    return opcao;
}

int main(void){
    // User usuario;
    BolsaCripto moeda;
    
    while(true){
        switch(menu()){
            case 1:
                puts("Cadastrar Investidor");
                break;
            case 2:
                puts("Remover Investidor");
                break;
            case 3:
                adicionar_moeda(&moeda);
                break;
            case 4:
                puts("Remover Criptomoeda");
            case 5:
                puts("Consultar Saldo do Investidor");
                break;
            case 6:
                puts("Consultar Extrato do Investidor");
                break;
            case 7:
                puts("Atualiza Cotacao/ Reciclar do outro programa");
            case 8:
                puts("Finalizando sessao");
                exit(1);
                break;
            default:
                puts("Opcao invalida");
                break;
        }
    }
    return 0;
    
}