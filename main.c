#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LIMITE_USUARIOS 10
typedef struct User{

    char cpf[80];
    char nome[50];
    char senha[80];
    float saldo_reais;
    float saldo_btc;
    float saldo_eth;
    float saldo_xrp;
    char extrato[100][200];
    int qntd_extrato;
} User;

// aqui estão as criptomoedas
typedef struct BolsaCripto{
    float bitcoin, ethereum, ripple,real;
} BolsaCripto;

// aqui fica as taxas
typedef struct TxCripto{
    float buy_bitcoin, sell_bitcoin;
    float buy_ethereum, sell_ethereum;
    float buy_ripple, sell_ripple;
    float real;
} TxCripto;
//pegando se o tipo de acao do usuario foi venda ou compra
enum tipo_acao {VENDA, COMPRA};
// limpa o buffer de entrada
void limpaBuffer() {
    int c;
    // Lê e descarta todos os caracteres até o '\n' ou EOF
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// procura o usuario
long seekUser(User* loginUsuario, FILE* arquivo){
    User usuario;
    long pos = -1;
    while (fread(&usuario, sizeof(User), 1, arquivo)) {
      
        if (strcmp(usuario.cpf, loginUsuario->cpf) == 0 && strcmp(usuario.senha, loginUsuario->senha) == 0) {
           
            pos =  ftell(arquivo) - sizeof(User);

        }
       
    }
    return pos;
}

// verificando se é possivel cadastrar novos usuarios
int usuer_limit_over(char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "rb");
    if(arquivo == NULL){
        return 0; // se o arquivo não existir é possivel cadastrar usuarios
    }
    fseek(arquivo,0,SEEK_END);
    unsigned int tamanho_arquivo = ftell(arquivo); // pegando o tamanho total do arquivo
    fclose(arquivo);
    if(tamanho_arquivo < sizeof(User)*LIMITE_USUARIOS){
        return 0;
    }
    return 1;
}
//Função Login

int login(User *loginUsuario) {
    FILE* arquivo = fopen("Usuario", "rb");
    if (arquivo == NULL) {
        printf("\x1b[31mNenhum usuario cadastrado.\x1b[0m\n");
        return 0;
    }
    printf("Digite o CPF:\n");
    fgets(loginUsuario->cpf,sizeof(loginUsuario->cpf),stdin);
    printf("Digite sua senha:\n");
    fgets(loginUsuario->senha,sizeof(loginUsuario->senha),stdin);
    

    long pos = seekUser(loginUsuario, arquivo);
    printf("%ld\n", pos);
    fseek(arquivo, pos, SEEK_SET);
    fread(loginUsuario,sizeof(User),1, arquivo);
    fclose(arquivo);

    if (pos != -1) {
        printf("Logado com sucesso! Bem-Vindo, %s\n", loginUsuario->nome);
        fflush(stdin);
        return 1;
    } else {
        printf("Nome de usuario ou senha incorreto(s)\n");
        return 0;
    }
}
// salva  o usuario no arquivo
void save_user(User* usuario){
    FILE* arquivo = fopen("Usuario", "r+b");
    if(arquivo == NULL){printf("Erro ao abrr o arquivo para salvar usuario");}
    
    long pos = seekUser(usuario, arquivo);
    if(pos != -1){
        fseek(arquivo,pos,SEEK_SET);
        fwrite(usuario,sizeof(User),1,arquivo);
    }else{
        printf("NAO ACHOU  O USUARIO");
    }
    fclose(arquivo);
}
// salva acao pra colocar no extrato
void save_acao(int tipo,User* loginUsuario,float valor, char cripto[4],int cota_moeda){
    BolsaCripto criptomoedas;
    TxCripto taxas = {0.02, 0.03, 0.01, 0.02, 0.01, 0.01,0.0};
    time_t seconds;
    // FILE* arquivo = fopen("Usuario", "r+b");

    // if (arquivo == NULL) {
    // printf("Erro ao abrir o arquivo de usuários em save_acao.\n");
    // }
    enum tipo_acao realizou = tipo;    
    time(&seconds); // pega a data (dds,mmm,ddd,hour,minute,seconds,year)
    char* data_hora = ctime(&seconds); // pega a string gerada pelo ctime()
    data_hora[strcspn(data_hora, "\n")] = 0; // Retira um conteudo indesejado na string, no caso o \n

    char signal;

    switch(realizou){
        case VENDA:
            signal = '-';
             switch(cota_moeda){
                case 1: // BTC
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.bitcoin,taxas.buy_bitcoin,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
                case 2: // ETH
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.ethereum,taxas.buy_ethereum,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
                case 3: // XRP
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.ripple,taxas.buy_ripple,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
                case 4: // REAL
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.real,taxas.real,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
             }
            break;
        case COMPRA:
            signal = '+';
            switch(cota_moeda){
                case 1: // BTC
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.bitcoin,taxas.buy_bitcoin,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
                case 2: // ETH
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.ethereum,taxas.buy_ethereum,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
                case 3: // XRP
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.ripple,taxas.buy_ripple,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
                case 4: // REAL
                    sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f BTC: %.2f ETH: %.2f XRP: %.2f\n",data_hora,signal,valor,cripto,criptomoedas.real,taxas.real,loginUsuario->saldo_reais,loginUsuario->saldo_btc,loginUsuario->saldo_eth,loginUsuario->saldo_xrp); //  formatando string para mandar no extrato
                    break;
            }
            break;
    }
    loginUsuario->qntd_extrato++;
    save_user(loginUsuario);
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
    
    printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->saldo_reais);
   
    save_user(loginUsuario);
    save_acao(COMPRA,loginUsuario,valor,"REAL",4);

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
        printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->saldo_reais);
        
        save_user(loginUsuario);
        save_acao(VENDA,loginUsuario,valor,"REAL",4);

    }

    return 0;
}

//função de Verificação de usuario

int usuarioExiste(char* nomeArquivo, char* nome) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("\n\x1b[32m Primeiro usuario do programa. Olá.\x1b[0m\n\n");
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
    novousuario.saldo_btc = 0.0;
    novousuario.saldo_eth = 0.0;
    novousuario.saldo_xrp = 0.0;
    novousuario.qntd_extrato = 0;
    

    FILE* arquivo = fopen("Usuario", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    fwrite(&novousuario, sizeof(User), 1, arquivo);
    fclose(arquivo);
    return 1;
}

//Função de acesso a pagina de login e cadastro de usuarios

int usuario(void) {
    int resposta;
    printf("Ja possui um Login?\n1- Sim\n2- Nao\n");
    scanf("%d", &resposta);
    fflush(stdin);
    if (resposta == 1) {
        return 1;  
    } else if (resposta == 2) {
        int users_excedido = usuer_limit_over("Usuario");
        if(users_excedido){
            printf("Numero maximo de usuarios atingidos!");
            exit(0);
        }
        else{
            cadastro();
            return 1;
        }
    } else {
        printf("Numero invalido!\n");
        exit(0);
    }
}

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
char consultar_extrato(User* usuario){
    char opcao;
    int limite = (usuario->qntd_extrato);
    printf("\n%d\n", limite);
    printf("\t\tExtrato da conta de %s\n",usuario->nome);
    for(int i = 0; i < limite; i++ ){
        printf("%s\n",usuario->extrato[i]);
    }

   do{
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
    ler_cota(&moedas);
    salvar_cota(&moedas);
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
                        consultar_extrato(&loginUsuario);
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
                    
                        puts("Valor atual da moeda\n");

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
