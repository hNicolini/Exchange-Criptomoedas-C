#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LIMITE_USUARIOS 10
#define LIMITE_EXTRATOS 100

// saldo de cada moeda do usuario
typedef struct Saldo{
    int idcoin;
    float qtd;
}Saldo;

typedef struct User{

    char cpf[80];
    char nome[50];
    char senha[80];
    char extrato[LIMITE_EXTRATOS][200];
    int qntd_extrato;
    int qtd_coins;
    float reais;
    Saldo* saldos;

} User;


void writeUser(User* user){
    FILE* wfile = fopen("usuarios", "ab");
    if(wfile == NULL){
        puts("First User!");
    }
    printf("Gravando: CPF: %s, Nome: %s, Senha: %s, qntd_extrato: %d, qtd_coins: %d, reais: %.2f\n",
           user->cpf, user->nome, user->senha, user->qntd_extrato, user->qtd_coins, user->reais);

    puts("Escrevendooo");
    fwrite(user,sizeof(User) - sizeof(Saldo*),1,wfile); // escrevendo credenciais + extrato do usuario + saldo de reais
    puts("Usuario foi salvo sem o saldo");
     // Gravação do conteúdo do array dinâmico `saldos`
    if (user->saldos != NULL && user->qtd_coins > 0) {
        fwrite(user->saldos, sizeof(Saldo), user->qtd_coins, wfile);
        puts("Saldo do usuário foi salvo");
    } else {
        puts("Nenhum saldo para salvar");
    }
    
    free(user->saldos);
    fclose(wfile);
}

void findUser(User* user, const char* cpf, const char* senha){
    FILE* rfile = fopen("usuarios", "rb");
    if(rfile == NULL){
        puts("No User registered");
        exit(1);
    }
  
    while(fread(user,sizeof(User) - sizeof(Saldo*),1,rfile)){
   
        printf("Comparando CPF: %s e Senha: %s com wCPF: %s e wSenha: %s\n",cpf, senha, user->cpf, user->senha);
        
        if(!strcmp(cpf,user->cpf) && !strcmp(senha, user->senha)){ // usuario encontrado !
            
            puts("User There is");
            user->saldos = malloc(user->qtd_coins * sizeof(Saldo)); // alocando memoria referente a qtd de moedas do usuario
            
            fread(user->saldos,sizeof(Saldo),user->qtd_coins,rfile); // coletando todas as moedas + saldos do usuario
            
            fclose(rfile); // fechando arquivo apos coletar os dados
        
            return;
        }
        /// as credenciais nao batam de primeira
        fseek(rfile,sizeof(Saldo) * user->qtd_coins, SEEK_CUR); // pular todo o array de saldos do respectivo usuario
    }
    puts("User There isn't");
    fclose(rfile);

}
int main(void){
    User usuario;
    // strcpy(usuario.cpf,"0000000000");
    // strcpy(usuario.nome,"Kaio");
    // strcpy(usuario.senha, "123");
    // // memset(usuario.extrato, 0, sizeof(usuario.extrato));
    // usuario.qntd_extrato = 2;
    // usuario.qtd_coins = 1;
    // usuario.reais = 23.9;
    // usuario.saldos = malloc(usuario.qtd_coins * sizeof(Saldo));
    // usuario.saldos[0].idcoin = 0;
    // usuario.saldos[0].qtd = 2.3;

    // for(int indice = 0; indice < usuario.qtd_coins; ++indice){
    //     printf("[%d] idcoin: %d  -> qtd: %.4f",indice, usuario.saldos[indice].idcoin,usuario.saldos[indice].qtd);
    // }

    // writeUser(&usuario);

    findUser(&usuario, "0000000000", "123");

    printf("Nome: %s saldo[0]: id[%d] qtnd[%.4f]", usuario.nome, usuario.saldos[0].idcoin, usuario.saldos[0].qtd);
    free(usuario.saldos);

}