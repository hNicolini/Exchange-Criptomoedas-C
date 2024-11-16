#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LIMITE_USUARIOS 10
#define LIMITE_EXTRATOS 100

// aqui estão as criptomoedas
typedef struct BolsaCripto{
    int idcoin;
    char sigla[4];
    char nome[255];
    float cota, txbuy, txsell;
    
} BolsaCripto;

// saldo de cada moeda do usuario
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
    while(fread(usuario,sizeof(User),1,arquivo)) {
      
        if (strcmp(usuario.cpf, loginUsuario->cpf) == 0 && strcmp(usuario.senha, loginUsuario->senha) == 0) {
           
            pos =  ftell(arquivo) - (sizeof(User));

        }
    }
    return pos;

}

void writeUser(User* user){
    FILE* wfile = fopen("usuarios", "ab");
    if(wfile == NULL){
        puts("First User!");
    }

    fwrite(user,sizeof(User),1,wfile); // escrevendo credenciais + extrato do usuario + saldo de reais

    fclose(wfile);
}

void findUser(User* user, const char* cpf, const char* senha){
    FILE* rfile = fopen("usuarios", "rb");
    if(rfile == NULL){
        puts("No User registered");
        exit(1);
    }
  
    while(fread(user,sizeof(User),1,rfile)){
        
        if(!strcmp(cpf,user->cpf) && !strcmp(senha, user->senha)){ // usuario encontrado !
            
            puts("User There is");
            fclose(rfile); // fechando arquivo apos coletar os dados
        
            return;
        }

    }
    puts("User There isn't");
    fclose(rfile);

}
// verificando se é possivel cadastrar novos usuarios
int user_limit_over(char* nome_arquivo){
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
    printf("\t\tLogin\n\n");
    FILE* arquivo = fopen("Usuario", "rb");
    if (arquivo == NULL) {
        printf("\x1b[31mNenhum usuario cadastrado.\x1b[0m\n");
        return 0;
    }
    printf("Digite o CPF: ");
    fgets(loginUsuario->cpf,sizeof(loginUsuario->cpf),stdin);
    printf("Digite sua senha: ");
    fgets(loginUsuario->senha,sizeof(loginUsuario->senha),stdin);
    
    

    long pos = seekUser(loginUsuario, arquivo);
   if (pos != -1) {
        fseek(arquivo, pos, SEEK_SET);
        fread(loginUsuario,sizeof(User),1, arquivo);
        fclose(arquivo);
        printf("Logado com sucesso! Bem-Vindo, %s\n", loginUsuario->nome);
        return 1;
    } 
    else if(pos == -1) {
        printf("Nome de usuario ou senha incorreto(s)\n");
        fclose(arquivo);
        return 0;
    }
    
}

// salva  o usuario no arquivo
void save_user(User* usuario){
    FILE* arquivo = fopen("Usuario", "r+b");
    if(arquivo == NULL){
        printf("Erro ao abrr o arquivo para salvar usuario");
        fclose(arquivo);
        exit(1);
        }
    
    long pos = seekUser(usuario, arquivo);
    if(pos != -1){
        fseek(arquivo,pos,SEEK_SET);
        fwrite(usuario,sizeof(User),1,arquivo);
    }else{
        printf("NAO ACHOU  O USUARIO");
    }
    fclose(arquivo);
}

// salva acao pra colocar no extrato ::::: ARRUMEI? ### PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
void save_acao(int tipo,User* loginUsuario,float valor,BolsaCripto* criptomoedas){
    
    time_t seconds;
    
    enum tipo_acao realizou = tipo;    
    time(&seconds); // pega a data (dds,mmm,ddd,hour,minute,seconds,year)
    char* data_hora = ctime(&seconds); // pega a string gerada pelo ctime()
    data_hora[strcspn(data_hora, "\n")] = 0; // Retira um conteudo indesejado na string, no caso o \n

    char signal;

    if(loginUsuario->qntd_extrato >=  LIMITE_EXTRATOS){
        for(int i = 1; i < LIMITE_EXTRATOS; i++){
            strcpy(loginUsuario->extrato[i-1],loginUsuario->extrato[i]);
        }
        loginUsuario->qntd_extrato--;
    }
    switch(realizou){
        case VENDA:

            signal = '-';
            sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f",data_hora,signal,valor,criptomoedas->nome,criptomoedas->cota,criptomoedas->txsell); //  formatando string para mandar no extrato
            break;

        case COMPRA:

            signal = '+';
            sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f",data_hora,signal,valor,criptomoedas->nome,criptomoedas->cota,criptomoedas->txbuy); //  formatando string para mandar no extrato
            break;

    }
    loginUsuario->qntd_extrato++;
    save_user(loginUsuario);
}

//Função de deposito de Dinheiro :::::: ARRUMEI? ### PRECISA SER ALTERADA PARA FICAR FICAR PROGRAMAVEL
int AdicionarSaldo(User *loginUsuario,BolsaCripto* moedas) {
    float valor;
    printf("\t\tDepositar Real\n");

    printf("Digite o valor a adicionar ao saldo: R$ ");
    scanf("%f", &valor);
    limpaBuffer();
    if(valor > 0){

    loginUsuario->reais += valor;  
    }
    else{
        puts("Digite um valor maior que 0");
    }
    
    printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->reais);
   
    save_user(loginUsuario);
    save_acao(COMPRA,loginUsuario,valor,moedas);

    return 0;
}


// funcao para verificar  a senha do usuario
int check_password(char* user_senha){
    char senha[80];
    printf("Digite sua senha: ");
    fgets(senha,sizeof(senha),stdin);
    if(strcmp(senha,user_senha) == 0){
        return 1;
    }
    return 0;
}

//Função de saque de dinheiro :::::: ARRUMEI? ### PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
int SacarSaldo(User *loginUsuario, BolsaCripto* moedas) {
    float valor;
    printf("\t\tSacar Real\n");

    if(check_password(loginUsuario->senha)){
        printf("Quanto voce deseja sacar: ");
        scanf("%f", &valor);
        limpaBuffer();
        if (valor > loginUsuario->saldo_reais){
            printf("Saque Invalido, Dinheiro insuficiente!");
        }
        else{
            loginUsuario->saldo_reais -= valor;
            printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->reais);
            
            save_user(loginUsuario);
            save_acao(VENDA,loginUsuario,valor,moedas);

        }
    }
    else{
        puts("Senha Inválida.");
    }
    return 0;
}

//função de Verificação de usuario
int usuarioExiste(char* nomeArquivo, char* nome) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("\n\x1b[32m Primeiro usuario do programa. Ola.\x1b[0m\n\n");
        
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

//Função de Cadastro :::::: ARRUMEI?  ### PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
int cadastro(void) {
    User novousuario;
    printf("\t\tCadastro\n");
    printf("Digite seu CPF: ");
    fgets(novousuario.cpf,sizeof(novousuario.cpf),stdin);
    if (usuarioExiste("Usuario", novousuario.cpf)) {
        printf("Usuario existente ");
        return 0;
    }
    printf("Digite seu nome: ");
    fgets(novousuario.nome,sizeof(novousuario.nome),stdin);

    printf("Digite sua senha: ");
    fgets(novousuario.senha,sizeof(novousuario.senha),stdin);

    novousuario.reais = 0;
    novousuario.qntd_extrato = 0;
    novousuario.qtd_coins = 0;
    

    FILE* arquivo = fopen("usuario", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    salvar_usuario(&novousuario);
    fclose(arquivo);
    return 1;
}

//Função de acesso a pagina de login e cadastro de usuarios
int usuario(void) {
    int resposta;
    printf("Ja possui um Login?\n1- Sim\n2- Nao\n");
    printf("Opcao: ");
    scanf("%d", &resposta);
    limpaBuffer();
    if (resposta == 1) {
        return 1;  
    } else if (resposta == 2) {
        int users_excedido = user_limit_over("Usuario");
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

// ##### ALTERAR :::: ALTEREI CERTO?
void salvar_cota(BolsaCripto* moedas, const size_t qtd_moedas){
    FILE* arquivo = fopen("cotas", "wb");
    // Salvando cotas em arquivo binario
    if(arquivo == NULL){
        printf("Não foi possivel abrir o arquivo para salvar cotas");
        exit(1);
    }
    
    

    fwrite(moedas,sizeof(BolsaCripto),qtd_moedas, arquivo);
    fclose(arquivo);
}

// ### ler_cota :::: ARRUMEI? PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
size_t ler_moedas(BolsaCripto* moedas){
    FILE* readFile = fopen("moedas", "rb");
    if(readFile == NULL){
        printf("Não foi possivel abrir o arquivo para ler\n");
        return;
    }

    fseek(readFile,0,SEEK_END);
    size_t pos = ftell(readFile);
    size_t qtd_moedas = pos/sizeof(BolsaCripto);
    moedas = malloc(qtd_moedas*sizeof(BolsaCripto));
    fseek(readFile,0,SEEK_SET);

    if (fread(moedas, sizeof(BolsaCripto), qtd_moedas, readFile) != 1) {
        printf("Erro ao ler as cotas.\n");
    }
    fclose(readFile);

    return qtd_moedas;
}

// alterar_valor_moeda ::::: ARRUMEI? ### PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
void alterar_valor_moeda(BolsaCripto* moedas, const size_t qtd_moedas){
    // gerando o numero aleatorio entre -5% e 5%
    srand(time(NULL));
    float numero_gerado =((rand() % 5) + 1.0)/100.0;
    int escolhe_sinal = rand()% 2;
    if(escolhe_sinal == 1){
        numero_gerado*=-1;
    }
    printf("\nAumentou/Diminuiu %.2f%c \n",numero_gerado*100.0,37);
    
    for(const int i = 0; i <= qtd_moedas; ++i){
        moedas[i].cota += numero_gerado;
    }

   salvar_cota(moedas, qtd_moedas);
}

/// ### ESSAS DUAS FUNCOES NAO PRECISAM EXISITR (TEORIA)

// void salvar_taxa(TxCripto* taxas){
//     FILE* arquivo = fopen("taxas", "wb");

//     if(arquivo == NULL){
//         printf("Não foi possivel abrir o arquivo de taxas");
//         exit(1);
//     }
//     fwrite(taxas, sizeof(TxCripto),1,arquivo);
//     fclose(arquivo);
// }
// void ler_taxa(TxCripto* taxas){
//     FILE* arquivo = fopen("taxas", "rb");

//     if(arquivo == NULL){
//         printf("Não foi possivel ler o arquivo de taxas\n");
//         taxas->buy_bitcoin = 0.02;
//         taxas->sell_bitcoin = 0.03;
//         taxas->buy_ethereum = 0.01;
//         taxas->sell_ethereum = 0.02;
//         taxas->buy_ripple = 0.01;
//         taxas->sell_ripple = 0.01;
//         taxas->real = 0.0;
//         return;
//     }
//     if (fread(taxas, sizeof(TxCripto), 1, arquivo) != 1){
//         printf("Erro ao ler as taxas.\n");
//         taxas->buy_bitcoin = 0.02;
//         taxas->sell_bitcoin = 0.03;
//         taxas->buy_ethereum = 0.01;
//         taxas->sell_ethereum = 0.02;
//         taxas->buy_ripple = 0.01;
//         taxas->sell_ripple = 0.01;
//         taxas->real = 0.0;
//     }
//     fclose(arquivo);
// }

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

// consultar_saldo :::::: ARRUMEI? ## PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
char consultar_saldos(User* usuario, BolsaCripto* moedas, size_t qtd_moedas){
    char opcao;
    Saldo* saldos;

    FILE* readSaldos = ("saldos", "rb");
    if(readSaldos == NULL){
        puts("Nenhum Saldo Existente no sistema");
        exit(1);
    }
    puts("\t\tSaldo na Carteira\n\n");
    while(fread(saldos,sizeof(Saldo),1,readSaldos)){
        if(!strcmp(saldos->cpf, usuario->cpf)){

            for(const int i = 0; i <= qtd_moedas; ++i){
                if(saldos->idcoin == moedas[i].idcoin ){
                    printf("%s: R$%.2f\n",moedas[i].nome, saldos->saldo);
                }
            }
        }
    }
    
    do{
        puts("1 - Voltar");
        printf("Opcao: ");
        opcao = getchar();
        limpaBuffer();
    }while(opcao != 49);

    return opcao;
}

char consultar_extrato(User* usuario){
    char opcao;
    int limite = (usuario->qntd_extrato);
    
    printf("\t\tExtrato da conta de %s\n",usuario->nome);
    for(int i = 0; i < limite; i++ ){
        printf("%s",usuario->extrato[i]);
    }

   do{
        puts("1 - Voltar");
        printf("Opcao: ");
        opcao = getchar();
        limpaBuffer();
    }while(opcao != 49);

    return opcao;
}
/// ##### VERIFICAR SE PRECISA ALTERAR
int check_buy_sell(int tipo, float valor, float  cota, float possui_reais,float possui_moeda,float taxa){

    switch(tipo){
        case COMPRA:
            if(valor <= 0 || valor > possui_reais){
                return 0; // não pode comprar
            }
            
            else if((possui_reais-=valor + (valor*taxa)) < 0){
                return 0;
            }
            return 1; // pode comprar
        case VENDA:
            if(valor <= 0 || valor > (possui_moeda*cota)){
                return 0; //  pode comprar
            }
            return 1; // pode vender
    }
}

// comprar_cripto ### PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
void comprar_cripto(User* usuario, BolsaCripto* cotas){
    char opcao;
    float valor;
    char confirm;
    int pode;
    
    puts("\t\tComprar criptomoedas\n");
    printf("1 - Bitcoin: %.2f\n",cotas->bitcoin);
    printf("2 - Ethereum: %.2f\n",cotas->ethereum);
    printf("3 - Ripple: %.2f\n",cotas->ripple);

    printf("Qual das opcoes deseja  comprar: ");
    opcao = getchar();
    limpaBuffer();
    
    switch(opcao){
        case '1':
            printf("Quantos R$ deseja aplicar em bitcoin: ");
            scanf("%f",&valor);
            limpaBuffer();
            pode = check_buy_sell(COMPRA,valor,cotas->bitcoin,usuario->saldo_reais,usuario->saldo_btc,taxas->buy_bitcoin);
            if(pode){
                if(check_password(usuario->senha)){
                    printf("Voce vai ter %.4f e sera combrado uma taxa de %.0f%c\n", valor/cotas->bitcoin,taxas->buy_bitcoin*100,37);
                    printf("[1] Confirmar / [0] Cancelar: ");
                    confirm = getchar();
                    limpaBuffer();
                    if(confirm == '1'){
                        usuario->saldo_reais -= valor  + (valor*taxas->buy_bitcoin);
                        usuario->saldo_btc += valor/cotas->bitcoin;
                        save_acao(COMPRA,usuario,valor,"BTC",BTC,cotas,taxas);

                        puts("Compra Realizada!");

                    }
                    else{
                        puts("Compra Cancelada");
                    }
                }
                else{
                    puts("Senha invalida");
                }
            }
            else{
                printf("Impossivel realizar compra.");
            }
            break;

         case '2':
            printf("Quantos R$ deseja aplicar em Ethereum: ");
            scanf("%f",&valor);
            limpaBuffer();
            pode = check_buy_sell(COMPRA,valor,cotas->ethereum,usuario->saldo_reais,usuario->saldo_eth,taxas->buy_ethereum);
            if(pode){
                if(check_password(usuario->senha)){
                    printf("Voce vai ter %.4f e sera combrado uma taxa de %.0f%c\n", valor/cotas->ethereum,taxas->buy_ethereum*100,37);
                    printf("[1] Confirmar / [0] Cancelar: ");
                    confirm = getchar();
                    limpaBuffer();
                    if(confirm == '1'){
                        usuario->saldo_reais -= valor  + (valor*taxas->buy_ethereum);
                        usuario->saldo_eth += valor/cotas->ethereum;

                        save_acao(COMPRA,usuario,valor,"ETH",ETH,cotas,taxas);
                        puts("Compra Realizada!");

                    }
                    else{
                        puts("Compra Cancelada");
                    }
                }
                else{
                    puts("Senha invalida");
                }
            }
            else{
                printf("Impossivel realizar compra.");
            }
            break;

         case '3':
            printf("Quantos R$ deseja aplicar em ripple: ");
            scanf("%f",&valor);
            limpaBuffer();
            pode = check_buy_sell(COMPRA,valor,cotas->ripple,usuario->saldo_reais,usuario->saldo_xrp,taxas->buy_ripple);
            if(pode){
                if(check_password(usuario->senha)){
                    printf("Voce vai ter %.4f e sera combrado uma taxa de %.0f%c\n", valor/cotas->ripple,taxas->buy_ripple*100,37);
                    printf("[1] Confirmar / [0] Cancelar: ");
                    confirm = getchar();
                    limpaBuffer();
                    if(confirm == '1'){
                        usuario->saldo_reais -= valor  + (valor*taxas->buy_ripple);
                        usuario->saldo_xrp += valor/cotas->ripple;
                        save_acao(COMPRA,usuario,valor,"XRP",XRP,cotas,taxas);
                        puts("Compra Realizada!");
                    }
                    else{
                        puts("Compra Cancelada");
                    }
                }
                else{
                    puts("Senha invalida");
                }
            }
            else{
                printf("Impossivel realizar compra.");
            }
            break;
    }


    do{
        puts("\n1 - Voltar");
        printf("Opcao: ");
        opcao = getchar();
        limpaBuffer();
    }while(opcao != 49);
}

// vender_cripto ### PRECISA SER ALTERADA PARA FICAR PROGRAMAVEL
void vender_cripto(User* usuario, BolsaCripto* cotas, TxCripto* taxas){
    char opcao;
    float valor;
    char confirm;
    int pode;
    
    puts("\t\tVender criptomoedas\n");
    printf("1 - Bitcoin: %.2f\n",usuario->saldo_btc*cotas->bitcoin);
    printf("2 - Ethereum: %.2f\n",usuario->saldo_eth*cotas->ethereum);
    printf("3 - Ripple: %.2f\n",usuario->saldo_xrp*cotas->ripple);

    printf("Qual criptomoeda deseja  vender: ");
    opcao = getchar();
    limpaBuffer();
    
    switch(opcao){
        case '1':
            printf("Quantos R$ deseja vender em bitcoin: ");
            scanf("%f",&valor);
            limpaBuffer();
            pode = check_buy_sell(VENDA,valor,cotas->bitcoin,usuario->saldo_reais,usuario->saldo_btc,taxas->sell_bitcoin);
            if(pode){
                if(check_password(usuario->senha)){
                    printf("Voce vai vender %.4f e sera combrado uma taxa de %.0f%c\n", valor/cotas->bitcoin,taxas->sell_bitcoin*100,37);
                    printf("[1] Confirmar / [0] Cancelar: ");
                    confirm = getchar();
                    limpaBuffer();
                    if(confirm == '1'){
                        usuario->saldo_reais += valor  - (valor*taxas->sell_bitcoin);
                        usuario->saldo_btc -= valor/cotas->bitcoin;
                        save_acao(VENDA,usuario,valor,"BTC",BTC,cotas,taxas);

                        puts("Venda Realizada!");

                    }
                    else{
                        puts("Venda Cancelada");
                    }
                }
                else{
                    puts("Senha invalida");
                }
            }
            else{
                printf("Impossivel realizar venda.");
            }
            break;

         case '2':
            printf("Quantos R$ deseja vender em Ethereum: ");
            scanf("%f",&valor);
            limpaBuffer();
            pode = check_buy_sell(VENDA,valor,cotas->ethereum,usuario->saldo_reais,usuario->saldo_eth,taxas->sell_ethereum);
            if(pode){
                if(check_password(usuario->senha)){
                    printf("Voce vai vender %.4f e sera combrado uma taxa de %.0f%c\n", valor/cotas->ethereum,taxas->sell_ethereum*100,37);
                    printf("[1] Confirmar / [0] Cancelar: ");
                    confirm = getchar();
                    limpaBuffer();
                    if(confirm == '1'){
                        usuario->saldo_reais += valor  - (valor*taxas->sell_ethereum);
                        usuario->saldo_eth -= valor/cotas->ethereum;

                        save_acao(VENDA,usuario,valor,"ETH",ETH,cotas,taxas);
                        puts("Venda Realizada!");

                    }
                    else{
                        puts("Venda Cancelada");
                    }
                }
                else{
                    puts("Senha invalida");
                }
            }
            else{
                printf("Impossivel realizar venda.");
            }
            break;

         case '3':
            printf("Quantos R$ deseja vender em ripple: ");
            scanf("%f",&valor);
            limpaBuffer();
            pode = check_buy_sell(VENDA,valor,cotas->ripple,usuario->saldo_reais,usuario->saldo_xrp,taxas->sell_ripple);
            if(pode){
                if(check_password(usuario->senha)){
                    printf("Voce vai vender %.4f e sera combrado uma taxa de %.0f%c\n", valor/cotas->ripple,taxas->sell_ripple*100,37);
                    printf("[1] Confirmar / [0] Cancelar:");
                    confirm = getchar();
                    limpaBuffer();
                    if(confirm == '1'){
                        usuario->saldo_reais += valor  - (valor*taxas->sell_ripple);
                        usuario->saldo_xrp -= valor/cotas->ripple;
                        save_acao(VENDA,usuario,valor,"XRP",XRP,cotas,taxas);
                        puts("Venda Realizada!");
                    }
                    else{
                        puts("Venda Cancelada");
                    }
                }
                else{
                    puts("Senha invalida");
                }
            }
            else{
                printf("Impossivel realizar venda.");
            }
            break;
    }


    do{
        puts("\n1 - Voltar");
        printf("Opcao: ");
        opcao = getchar();
        limpaBuffer();
    }while(opcao != 49);
}


int main(void){
    BolsaCripto moedas;
    User loginUsuario;
    TxCripto taxas;
    ler_cota(&moedas);
    
    salvar_cota(&moedas);
    
    ler_taxa(&taxas);
    salvar_taxa(&taxas);
    

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
                        AdicionarSaldo(&loginUsuario,&moedas,&taxas);
                        break;

                    case '4':
                        SacarSaldo(&loginUsuario,&moedas,&taxas);
                        break;
                    
                    case '5':
                        comprar_cripto(&loginUsuario,&moedas,&taxas);
                        break;

                    case '6':
                        vender_cripto(&loginUsuario,&moedas,&taxas);
                        break;

                    case '7':
                    
                        puts("Valor atual da moeda\n");

                        ler_cota(&moedas);
                        printf("bitcoin: %.2f\nethereum: %.2f\nripple: %.2f\n", moedas.bitcoin, moedas.ethereum, moedas.ripple); /// ### ALTERAR PARA SER PROGRAMAVEL

                        alterar_valor_moeda(&moedas);
                        ler_cota(&moedas); // salvando valor alterado

                        puts("\nCotacao atualizada!");
                        printf("\nbitcoin: %.2f\nethereum: %.2f\nripple: %.2f\n\n", moedas.bitcoin, moedas.ethereum, moedas.ripple); /// ### ALTERAR PARA SER PROGRAMAVEL
                        
                        do{
                            puts("1 - Voltar");
                            printf("Opcao: ");
                            opcao = getchar();
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
