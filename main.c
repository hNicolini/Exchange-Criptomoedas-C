#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct User{
    char usuario[80];
    char senha[80];
    float Saldo;
} User;

//Função Login

int login(User *loginUsuario) {
    char usuario[80];
    char senha[80];

    printf("Digite o nome de usuario:\n");
    scanf("%s", usuario);
    printf("Digite sua senha:\n");
    scanf("%s", senha);

    FILE* arquivo = fopen("Usuario", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    User usuarioLogado;
    int encontrado = 0;

    while (fread(&usuarioLogado, sizeof(User), 1, arquivo)) {
        if (strcmp(usuarioLogado.usuario, usuario) == 0 && strcmp(usuarioLogado.senha, senha) == 0) {
            *loginUsuario = usuarioLogado;
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (encontrado) {
        printf("Logado com sucesso! Bem-Vindo, %s\n", usuario);
        return 1;
    } else {
        printf("Nome de usuário ou senha incorreto(s)\n");
        return 0;
    }
}

//Função de deposito de Dinheiro

int AdicionarSaldo(User *loginUsuario) {
    float valor;

    printf("Digite o valor a adicionar ao saldo: R$ ");
    scanf("%f", &valor);

    loginUsuario->Saldo += valor;  

    FILE *arquivo1 = fopen ("Usuario", "wb+");

    fwrite(loginUsuario,sizeof(User),1,arquivo1);
    fclose(arquivo1);

    FILE *arquivo = fopen("Usuario", "rb");
    fread(loginUsuario,sizeof(User),1,arquivo);
    printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->Saldo);
    fclose(arquivo);
    return 0;
}

//Função de saque de dinheiro

int SacarSaldo(User *loginUsuario) {
    float valor;

    printf("Digite o valor a ser sacado do saldo: R$ ");
    scanf("%f", &valor);
    if (valor > loginUsuario->Saldo){
        printf("Saque Invalido, Dinheiro insuficiente!");
    }
    loginUsuario->Saldo -= valor;  

    FILE *arquivo1 = fopen ("Usuario", "wb+");

    fwrite(loginUsuario,sizeof(User),1,arquivo1);
    fclose(arquivo1);

    FILE *arquivo = fopen("Usuario", "rb");
    fread(loginUsuario,sizeof(User),1,arquivo);
    printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->Saldo);
    fclose(arquivo);
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
        if (strcmp(usuariolido.usuario, nome) == 0) {
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
    printf("Digite seu nome de usuario:\n");
    scanf("%s", novousuario.usuario);

    if (usuarioExiste("Usuario", novousuario.usuario)) {
        printf("Usuário existente!\n");
        return 0;
    }

    printf("Digite sua senha:\n");
    scanf("%s", novousuario.senha);

    novousuario.Saldo = 0.0;  

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
    printf("Já possui um Login?\n1- Sim\n2- Não\n");
    scanf("%d", &resposta);

    if (resposta == 1) {
        return 1;  
    } else if (resposta == 2) {
        cadastro();
        return 1;  
    } else {
        printf("Número inválido!\n");
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
    char resposta,opcao;
    // usuario();
    BolsaCripto moedas;
    
    // pós login Menu
    do{
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
                printf("\nbitcoin: %.2f\nethereum: %.2f\nripple: %.2f\n\n", moedas.bitcoin, moedas.ethereum, moedas.ripple);
                
                do{
                    fflush(stdin); // limpando buffer para não conflitar com scanf
                    puts("1 - Voltar");
                    printf("Opcao: ");
                    scanf("%c", &opcao);
                }while(opcao != 49);

                break;
            
            case '8':
                puts("\nFim da Sessao.");
                break;
        }
    }while(resposta != '8');

    return 0;
}
