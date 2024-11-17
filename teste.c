#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LIMITE_USUARIOS 10
#define LIMITE_EXTRATOS 100

// saldo de cada moeda do usuario
typedef struct BolsaCripto{
    int idcoin;
    char sigla[4];
    char nome[255];
    float cota, txbuy, txsell;
    
} BolsaCripto;
typedef struct Saldo{
    char cpf[80];
    int idCoin;
    float saldo;
}Saldo;
typedef struct User{
    char cpf[80];
    char nome[50];
    char senha[80];
    char extrato[LIMITE_EXTRATOS][200];
    int qntd_extrato;
    int qtd_coins;
    float reais;

} User;

void listar_users(){
    User usuarios;
    FILE* rUsers = fopen("usuarios", "rb");
    if(rUsers == NULL){
        puts("Sem usuarios para ler");
        return;
    }
    while(fread(&usuarios,sizeof(User),1,rUsers)){
        printf("CPF: %s\n", usuarios.cpf);
        printf("Nome: %s\n",usuarios.nome);
        puts("-----------------");
    }
}

void listar_moedas() {
    FILE* arquivo = fopen("moedas", "rb");
    if (arquivo == NULL) {
        puts("Nenhuma criptomoeda registrada.");
        return;
    }

    BolsaCripto moeda;
    puts("\t\tLista de Criptomoedas");
    while (fread(&moeda, sizeof(BolsaCripto), 1, arquivo) == 1) {
        printf("ID: %d\n", moeda.idcoin);
        printf("Nome: %s\n", moeda.nome); // fgets inclui \n, não precisa adicionar
        printf("Sigla: %s\n", moeda.sigla);
        printf("Cotacao: %.2f\n", moeda.cota);
        printf("Taxa de Compra: %.2f\n", moeda.txbuy);
        printf("Taxa de Venda: %.2f\n", moeda.txsell);
        puts("------------------------------");
    }

    fclose(arquivo);
}

void listar_saldos(){
    FILE* arquivo = fopen("saldos", "rb");
    if(arquivo == NULL){
        puts("Nenhum Saldo Registrado");
        return;
    }
    Saldo saldos;
    puts("Lista de saldos");
    while(fread(&saldos, sizeof(Saldo),1,arquivo) == 1){
        printf("CPF: %s", saldos.cpf);
        printf("IdCOin: %d\n",saldos.idCoin);
        printf("Saldo: %.2f\n",saldos.saldo);
        puts("-------------------");
    }
    fclose(arquivo);
}
int main(void){
    listar_moedas();
    listar_saldos();
    listar_users();
    

}