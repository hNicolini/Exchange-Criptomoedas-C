#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct User{

    char cpf[80];
    char nome[50];
    char senha[80];
    float saldo_reais;
    float saldo_btc;
    float saldo_eth;
    float saldo_xrp;
} User;

// limpa o buffer de entrada
void limpaBuffer() {
    int c;
    // Lê e descarta todos os caracteres até o '\n' ou EOF
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// procura o usuario
long seekUser(User* loginUsuario, FILE* arquivo){
    User usuario;

    while (fread(&usuario, sizeof(User), 1, arquivo)) {
        if (strcmp(usuario.cpf, loginUsuario->cpf) == 0 && strcmp(usuario.senha, loginUsuario->senha) == 0) {
            return ftell(arquivo) - sizeof(User);

        }
       
    }
    return -1;
}

//Função Login

int login(User *loginUsuario) {


    printf("Digite o CPF:\n");
    fgets(loginUsuario->cpf,sizeof(loginUsuario->cpf),stdin);
    printf("Digite sua senha:\n");
    fgets(loginUsuario->senha,sizeof(loginUsuario->senha),stdin);
    FILE* arquivo = fopen("Usuario", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }
    long pos = seekUser(loginUsuario, arquivo);
    fseek(arquivo, pos, SEEK_SET);
    fread(loginUsuario,sizeof(User),1, arquivo);
    fclose(arquivo);

    if (pos != -1) {
        printf("Logado com sucesso! Bem-Vindo, %s\n", loginUsuario->nome);
        fflush(stdin);
        return 1;
    } else {
        printf("Nome de usuário ou senha incorreto(s)\n");
        return 0;
    }
    printf("ERRROOOO");
}

//Função de deposito de Dinheiro

int AdicionarSaldo(User *loginUsuario) {
    float valor;

    printf("Digite o valor a adicionar ao saldo: R$ ");
    scanf("%f", &valor);
    limpaBuffer();
    if(valor > 0){

    loginUsuario->saldo_reais += valor;  
    }
    else{
        puts("Digite um valor maior que 0");
    }
    FILE *arquivo1 = fopen ("Usuario", "r+b");
    long pos = seekUser(loginUsuario, arquivo1);
    fseek(arquivo1,pos,SEEK_SET); // move para o ponto onde está localizado o usuario logado
    fwrite(loginUsuario,sizeof(User),1,arquivo1); // atualiza o saldo
    printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->saldo_reais);
    fclose(arquivo1);

    // FILE *arquivo = fopen("Usuario", "rb");
    // fread(loginUsuario,sizeof(User),1,arquivo);
    // printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->saldo_reais);
    // fclose(arquivo);
    return 0;
}

//Função de saque de dinheiro

int SacarSaldo(User *loginUsuario) {
    float valor;

    printf("Digite o valor a ser sacado do saldo: R$ ");
    scanf("%f", &valor);
    limpaBuffer();

    if (valor > loginUsuario->saldo_reais){
        printf("Saque Invalido, Dinheiro insuficiente!");
    }
    else{
        loginUsuario->saldo_reais -= valor;
        FILE *arquivo1 = fopen ("Usuario", "r+b");
        long pos = seekUser(loginUsuario, arquivo1);
        fseek(arquivo1,pos,SEEK_SET);
        fwrite(loginUsuario,sizeof(User),1,arquivo1);
        printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->saldo_reais);
        fclose(arquivo1);
    }

    return 0;
}

//função de Verificação de usuario

int usuarioExiste(char* nomeArquivo, char* nome) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    User usuariolido;
    while (fread(&usuariolido, sizeof(User), 1, arquivo)) {
        if (strcmp(usuariolido.cpf, nome) == 0) {
            fclose(arquivo);
            return 1;  
        }
    }

    fclose(arquivo);
    return 0;  
}

//Função de Cadastro

int cadastro(void) {
    User novousuario;
    printf("Digite seu CPF:\n");
    fgets(novousuario.cpf,sizeof(novousuario.cpf),stdin);
    if (usuarioExiste("Usuario", novousuario.cpf)) {
        printf("Usuário existente!\n");
        return 0;
    }
    printf("Digite seu nome: ");
    fgets(novousuario.nome,sizeof(novousuario.nome),stdin);
    printf("Digite sua senha:\n");
    fgets(novousuario.senha,sizeof(novousuario.senha),stdin);

    novousuario.saldo_reais = 0.0;  

    FILE* arquivo = fopen("Usuario", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    fwrite(&novousuario, sizeof(User), 1, arquivo);
    fclose(arquivo);
    printf("\nUsuário registrado com sucesso!\n");
    return 1;
}

//Função de acesso a pagina de login e cadastro de usuarios

int usuario(void) {
    int resposta;
    printf("Já possui um Login?\n1- Sim\n2- Nao\n");
    scanf("%d", &resposta);
    fflush(stdin);
    if (resposta == 1) {
        return 1;  
    } else if (resposta == 2) {
        cadastro();
        return 1;  
    } else {
        printf("Numero invalido!\n");
        exit(0);
    }
}

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
        opcao = getchar();
        limpaBuffer(); // tirando o \n
        // vendo se a opcao escolhida da dentro do escopo possivel em ascii
        if(opcao < 49 || opcao > 56){
            puts("Opcao Invalida");
        }
        
    }while(opcao < 49 || opcao > 56);


    return opcao;
}

char consultar_saldo(User* usuario){
    char opcao;
    puts("Saldo na Carteira");
    printf("Reais: %.2f\n\n",usuario->saldo_reais);
    
    do{
        fflush(stdin); // limpando buffer para não conflitar com scanf
        puts("1 - Voltar");
        printf("Opcao: ");
        scanf("%c", &opcao);
        limpaBuffer();
    }while(opcao != 49);

    return opcao;
}
char consultar_extrato(){
    char opcao;

    puts("Aqui você consulta o extrato");

   do{
        limpaBuffer();
        puts("1 - Voltar");
        printf("Opcao: ");
        scanf("%c", &opcao);
        limpaBuffer();
    }while(opcao != 49);

    return opcao;
}
void comprar_cripto(){
    puts("Aqui você compra criptomoedas");
}
void vender_cripto(){
    puts("Aqui você vende suas criptomoedas");
}


int main(void){
    BolsaCripto moedas;
    User loginUsuario;

    char resposta,opcao;

    if (usuario()) {  
        if (login(&loginUsuario)) {
    // pós login Menu
            do{
                resposta = menu();

                switch(resposta){
                    case '1':
                        consultar_saldo(&loginUsuario);
                        break;

                    case '2':
                        consultar_extrato();
                        break;

                    case '3':
                        AdicionarSaldo(&loginUsuario);
                        break;

                    case '4':
                        SacarSaldo(&loginUsuario);
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
                        printf("\nbitcoin: %.2f\nethereum: %.2f\nripple: %.2f\n\n", moedas.bitcoin, moedas.ethereum, moedas.ripple);
                        
                        do{
                            limpaBuffer(); // limpando buffer para não conflitar com scanf
                            puts("1 - Voltar");
                            printf("Opcao: ");
                            scanf("%c", &opcao);
                            limpaBuffer();
                        }while(opcao != 49);

                        break;
                    
                    case '8':
                        puts("\nFim da Sessao.");
                        break;
                }
            }while(resposta != '8');
        }
    }
    return 0;
}
