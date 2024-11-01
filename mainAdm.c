#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIMITE_USUARIOS 10
#define LIMITE_EXTRATOS 100

typedef struct User {
    int id;
    char cpf[80];
    char nome[50];
    char senha[80];
    float saldo_reais;
    float saldo_btc;
    float saldo_eth;
    float saldo_xrp;
    char extrato[LIMITE_EXTRATOS][200];
    int qntd_extrato;
} User;

// aqui estão as criptomoedas
typedef struct BolsaCripto {
    float bitcoin, ethereum, ripple, real;
} BolsaCripto;

// aqui ficam as taxas
typedef struct TxCripto {
    float buy_bitcoin, sell_bitcoin;
    float buy_ethereum, sell_ethereum;
    float buy_ripple, sell_ripple;
    float real;
} TxCripto;

// pegando se o tipo de ação do usuário foi venda ou compra
enum tipo_acao { VENDA, COMPRA };
enum indice_moeda { BTC = 1, ETH = 2, XRP = 3, REAL = 4 };

// limpa o buffer de entrada
void limpaBuffer() {
    int c;
    // Lê e descarta todos os caracteres até o '\n' ou EOF
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// Remove a nova linha de uma string
char* clear_newLine(char* string) {
    string[strcspn(string, "\n")] = 0;
    return string;
}
int gera_id_user() {
    int id;
    User ultimo_usuario;

    FILE* pegar_id = fopen("Usuario.bin", "rb");
    if (pegar_id == NULL) {
        id = 1;  // Inicia em 1 se o arquivo não existe
    } else {
        fseek(pegar_id, -(long)sizeof(User), SEEK_END);  // Casting para long
        fread(&ultimo_usuario, sizeof(User), 1, pegar_id);
        id = ultimo_usuario.id + 1;  // Incrementa o último ID lido
        fclose(pegar_id);
    }

    return id;
}
void AddInvestidor(User* user) { 
    puts("\n\t\tTodos os Usuarios Disponíveis \n");
    
    FILE *arquivo = fopen("Usuario.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Adicione o Investidor\n");
    printf("CPF: ");
    fgets(user->cpf, sizeof(user->cpf), stdin);
    strcpy(user->cpf, clear_newLine(user->cpf));

    printf("Nome: ");
    fgets(user->nome, sizeof(user->nome), stdin);
    strcpy(user->nome, clear_newLine(user->nome));

    printf("Senha: ");
    fgets(user->senha, sizeof(user->senha), stdin);
    strcpy(user->senha, clear_newLine(user->senha));

    user->saldo_reais = 0;
    user->saldo_btc = 0;
    user->saldo_xrp = 0;
    user->saldo_eth = 0;
    strcpy(user->extrato[0], " ");
    user->qntd_extrato = 0;

    user->id = gera_id_user();



    //--------------------------------------------------------------------------

    fwrite(user, sizeof(User), 1, arquivo); 
    fclose(arquivo);

    printf("Investidor adicionado com sucesso!\n");
}
void ConsultarUsuarios(User* usuarios){
    
    puts("\t\tLista de todos os usuarios!");
    FILE*arquivo = fopen("Usuario.bin", "rb");
    printf("\n-----------------------------------------------------------\n");

    while (fread(usuarios, sizeof(User),1,arquivo))
    {
        printf("ID: %d CPF: %s Nome: %s\n",usuarios->id, usuarios->cpf, usuarios->nome);
    }
}   
void RemoverUsuarios(){
    int id;
    int opcao;

    puts("\t\tPagina de remover usuarios");
    printf("Insira o ID do Usuario que deseja remover: ");
    scanf("%d",&id);
    
    printf("\n Tem certeza que deseja remover o Usuario de ID: %d", id);
    printf("\n 1 - SIM");
    printf("\n 2 - NAO");
    printf("\n Opcao: ");
    limpaBuffer();
    scanf("%d", &opcao);

    if (opcao == 1){
            FILE* coletarusers = fopen("Usuario.bin", "rb+");
        if(coletarusers == NULL){
            puts("Nenhum usuario cadastrado");
            return;
        }

        // Obtendo numero total de usuarios
        fseek(coletarusers,0, SEEK_END);
        size_t pos = ftell(coletarusers);
        size_t qtd_usuarios = pos/sizeof(User);


        User* all_users = malloc(qtd_usuarios * sizeof(User));
        fseek(coletarusers,0,SEEK_SET);
        fread(all_users, sizeof(User), qtd_usuarios, coletarusers);
        if(all_users == NULL){
            puts("Erro na alocacao de memoria para all_users");
            return;
        }
        fclose(coletarusers);

        User* filteredUsers = malloc((qtd_usuarios)*sizeof(User));
        if(filteredUsers == NULL){
            puts("Erro na alocacao de memoria para filteredUsers");
            free(all_users);
            return;
        }
        int cont = 0;

        // Pegando os usuarios que nao foram removidos
        for(unsigned int i = 0; i < qtd_usuarios; i++){
            if(all_users[i].id != id){
                filteredUsers[cont] = all_users[i];
                cont++;
            }
        }
        free(all_users);

        // Escrevendo os usuarios que nao foram removidos no arquivo
        FILE* rewriteUsers = fopen("Usuario.bin", "wb");
        if(rewriteUsers == NULL){
            puts("Impossivel reescrever usuarios");
            free(filteredUsers);
            return;
        }
        
        fwrite(filteredUsers,sizeof(User),cont,rewriteUsers);
    
        free(filteredUsers);
    
        fclose(rewriteUsers);
    
        printf("Usuario com ID [%d] foi removido.",id);
    }else{
        printf("Operação de remoção de usuario cancelada!");
        exit(1);
    }
        
    }
    

    
   

    


int main() {
    User novo_investidor;
    int opcao;
    printf("Opcao: ");
    scanf("%d", &opcao);
    limpaBuffer();
    switch(opcao){
        case 1:
            AddInvestidor(&novo_investidor);
            break;
        case 2:
            ConsultarUsuarios(&novo_investidor);
            break;
        case 3:
            RemoverUsuarios();
            break;
    }
    
    
    
    
    return 0;
}
