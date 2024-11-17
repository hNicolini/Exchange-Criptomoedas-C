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
    while(fread(&usuario,sizeof(User),1,arquivo)) {
      
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
    FILE* arquivo = fopen("usuarios", "rb");
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
    fclose(arquivo);
    return 0;
    
}

// salva  o usuario no arquivo
void save_user(User* usuario){
    FILE* arquivo = fopen("usuarios", "r+b");
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

// salva acao pra colocar no extrato ::::: 
void save_acao(int tipo,User* loginUsuario,float valor,char sigla[4], float cota, float tx){
    
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
            sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f",data_hora,signal,valor,sigla,cota,tx, loginUsuario->reais); //  formatando string para mandar no extrato
            break;

        case COMPRA:

            signal = '+';
            sprintf(loginUsuario->extrato[loginUsuario->qntd_extrato], "%s %c %.2f  %s  CT: %.3f TX: %.2f REAL: %.2f",data_hora,signal,valor,sigla,cota,tx, loginUsuario->reais); //  formatando string para mandar no extrato
            break;

    }
    loginUsuario->qntd_extrato++;
    save_user(loginUsuario);
}

// Funcao para salvar o saldo
void save_saldo(Saldo* saldo){  
    int saldoExiste = 0;
    FILE* readSaldos = fopen("saldos", "rb+");
    
    Saldo allSaldos;
    long pos = -1;
    while(fread(&allSaldos,sizeof(User),1,readSaldos)) {
      
        if (strcmp(allSaldos.cpf, saldo->cpf) == 0 && allSaldos.idCoin == saldo->idCoin) {
           
            pos =  ftell(readSaldos) - (sizeof(Saldo));
            saldoExiste = 1;

        }
    }
    if(saldoExiste){
        if(pos != -1){
            fseek(readSaldos,pos,SEEK_SET);
            fwrite(saldo,sizeof(Saldo),1,readSaldos);
        }else{
            printf("NAO ACHOU  O Saldo");
        }
    }
    else{
        fseek(readSaldos,0,SEEK_SET);
        fwrite(saldo,sizeof(Saldo),1,readSaldos);
    }
    fclose(readSaldos);
}

//Função de deposito de Dinheiro :::::: 
int AdicionarSaldo(User *loginUsuario) {
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
    save_acao(COMPRA,loginUsuario,valor,"REAL", 0, 0);

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

//Função de saque de dinheiro :::::: 
int SacarSaldo(User *loginUsuario) {
    float valor;
    printf("\t\tSacar Real\n");

    if(check_password(loginUsuario->senha)){
        printf("Quanto voce deseja sacar: ");
        scanf("%f", &valor);
        limpaBuffer();
        if (valor > loginUsuario->reais){
            printf("Saque Invalido, Dinheiro insuficiente!");
        }
        else{
            loginUsuario->reais -= valor;
            printf("Saldo atualizado com sucesso! Saldo atual: R$ %.2f\n",loginUsuario->reais);
            
            save_user(loginUsuario);
            save_acao(VENDA,loginUsuario,valor,"REAL",0.0,0.0);

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

//Função de Cadastro :::::: 
int cadastro(void) {
    User novousuario;
    printf("\t\tCadastro\n");
    printf("Digite seu CPF: ");
    fgets(novousuario.cpf,sizeof(novousuario.cpf),stdin);
    if (usuarioExiste("usuarios", novousuario.cpf)) {
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
    

    FILE* arquivo = fopen("usuarios", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    writeUser(&novousuario);
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
        int users_excedido = user_limit_over("usuarios");
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

// salvar cotas apos atualizar cotacao :::: 
void salvar_cota(BolsaCripto* moedas, const unsigned int qtd_moedas){
    FILE* arquivo = fopen("moedas", "wb");
    // Salvando cotas em arquivo binario
    if(arquivo == NULL){
        printf("Não foi possivel abrir o arquivo para salvar cotas");
        exit(1);
    }
    
    

    fwrite(moedas,sizeof(BolsaCripto),qtd_moedas, arquivo);
    fclose(arquivo);
}

// ### ler_cota :::: 
BolsaCripto* ler_moedas(unsigned int* qtd_moedas){
    BolsaCripto* moedas;
    
    FILE* readFile = fopen("moedas", "rb");
    if(readFile == NULL){
        printf("Não foi possivel abrir o arquivo para ler\n");
        *qtd_moedas = 0;
        return 0;
    }

    fseek(readFile,0,SEEK_END);
    size_t pos = ftell(readFile);
    *qtd_moedas = pos/sizeof(BolsaCripto);
    moedas = malloc((*qtd_moedas)*sizeof(BolsaCripto));
    if(moedas == NULL){
        puts("Nao foi possivel alocar memoria para as moedas");
        exit(1);
    }
    fseek(readFile,0,SEEK_SET);
    if((*qtd_moedas) == 0){
        puts("Nenhuma moeda no banco de criptomoedas");
        fclose(readFile);
        return NULL;
    }
    if (fread((moedas), sizeof(BolsaCripto), *qtd_moedas, readFile) != (*qtd_moedas)) {
        printf("Erro ao ler as cotas.\n");
    }
    fclose(readFile);
    
    return moedas;
}

// alterar_valor_moeda ::::: 
void alterar_valor_moeda(BolsaCripto* moedas, const unsigned int qtd_moedas){
    // gerando o numero aleatorio entre -5% e 5%
    srand(time(NULL));
    float numero_gerado =((rand() % 5) + 1.0)/100.0;
    int escolhe_sinal = rand()% 2;
    if(escolhe_sinal == 1){
        numero_gerado*=-1;
    }
    printf("\nAumentou/Diminuiu %.2f%c \n",numero_gerado*100.0,37);
    
    for(unsigned int i = 0; i < qtd_moedas; ++i){
        moedas[i].cota += numero_gerado;
    }

   salvar_cota(moedas, qtd_moedas);
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

// consultar_saldo :::::: 
char consultar_saldos(User* usuario, BolsaCripto* moedas, unsigned int qtd_moedas, Saldo* saldos){
    char opcao;
    puts("\t\tSaldo na Carteira\n\n");
    printf("REAL: %.2f\n", usuario->reais);
    FILE* readSaldos = fopen("saldos", "rb");
    if(readSaldos == NULL){
        puts("Nenhum Saldo de Cripto Existente no sistema");
    }
    
    while(fread(saldos,sizeof(Saldo),1,readSaldos)){
        if(!strcmp(saldos->cpf,  usuario->cpf)){
            for(unsigned int i = 0; i < qtd_moedas; ++i){
                if(saldos->idCoin == moedas[i].idcoin ){
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

char consultar_extrato(User* usuario, BolsaCripto* moedas, Saldo* saldos, unsigned int qtd_moedas){
    char opcao;
    int limite = (usuario->qntd_extrato);
    FILE* readSaldos = fopen("saldos", "rb");
    printf("\t\tExtrato da conta de %s\n",usuario->nome);
    for(int i = 0; i < limite; i++ ){
        printf("%s",usuario->extrato[i]);
        while(fread(saldos,sizeof(Saldo),1,readSaldos)){
            if(!strcmp(saldos->cpf,  usuario->cpf)){
        
                for(unsigned int i = 0; i < qtd_moedas; ++i){
                    if(saldos->idCoin == moedas[i].idcoin ){
                    
                        printf(" %s: %.4f",moedas[i].sigla, saldos->saldo/moedas[i].cota);
                    }
                }
            }
        }
        fseek(readSaldos,0,SEEK_SET);
        printf("\n");
    }

   do{
        puts("1 - Voltar");
        printf("Opcao: ");
        opcao = getchar();
        limpaBuffer();
    }while(opcao != 49);

    return opcao;
}

int check_buy_sell(int tipo, float valor, float  cota, float possui_reais,float possui_moeda,float taxa){

    switch(tipo){
        case COMPRA:
            if(valor <= 0 || valor > possui_reais){
                return 0; // não pode comprar
            }
            
            else if((possui_reais-=valor + (valor*taxa)) < 0){
                return 0; // nao pode comprar
            }
            break;
        case VENDA:
            if(valor <= 0 || valor > (possui_moeda*cota)){
                return 0; // nao pode comprar
            }
            break;
        }
        return 1; // pode vender/comprar
}

// comprar_cripto :::::: 
void comprar_cripto(User* usuario, BolsaCripto* moedas, unsigned int qtd_moedas, Saldo* saldos){
    int opcao;
    int possuiSaldo = 0;
    int validId = 0;
    float valor, saldoAtual;
    int confirm;
    int pode;
    int coinId;
    
    if (qtd_moedas == 0 || moedas == NULL) {
        puts("Nenhuma criptomoeda disponível.");
        return;
    }

    puts("\t\tComprar criptomoedas\n");
    for(unsigned int i = 0; i < qtd_moedas; ++i){
        printf("[%d] %s - R$ %.2f\n",(moedas[i].idcoin) + 1, moedas[i].nome, moedas[i].cota);
    }

    printf("Qual das opcoes deseja  comprar: ");
    scanf("%d", &opcao);
    limpaBuffer();
    
    for(unsigned int i = 0; i < qtd_moedas; ++i){
        if((moedas[i].idcoin + 1) == opcao){
            coinId = opcao - 1;
            validId = 1;
            break;
        }
    }
    
    if(!validId){
        puts("Opcao Invalida.");
    }
    else{
        printf("Quantos R$ deseja aplicar em %s: ", moedas[coinId].nome);
        scanf("%f", &valor);
        limpaBuffer();
        // coletando saldo atual da moeda
        FILE* readSaldos = fopen("saldos", "rb");
        if(readSaldos == NULL){
            puts("Primeiro usuario a comprar uma cripto");
            FILE* createSaldos = fopen("saldos", "ab");
            if(createSaldos == NULL){
                puts("Erro na criacao do arquivo saldos");
                return;
            }
            fclose(createSaldos);
        }
        else{
        // coletando o saldo atual da moeda
        
        while(fread(saldos,sizeof(Saldo),1,readSaldos)){
            if(!strcmp(saldos->cpf, usuario->cpf)){
                if(saldos->idCoin == moedas[coinId].idcoin){
                        puts("\t\t Saldo Atual da moeda: ");
                        printf("%s: R$%.2f\n",moedas[coinId].nome, saldos->saldo);
                        possuiSaldo = 1;
                        break;
                    }
                }
            }
            fclose(readSaldos);
        }
        if(!possuiSaldo){
            memset(saldos,0,sizeof(Saldo));
            
            saldoAtual = 0.0;
        }
        else{
            saldoAtual = saldos->saldo;
        }
        pode = check_buy_sell(COMPRA,valor,moedas[coinId].cota,usuario->reais,saldoAtual,moedas[coinId].txbuy);
    
        if(pode){
            if(check_password(usuario->senha)){
                printf("Voce vai adquirir + %.4f %s e sera combrado uma taxa de %.0f%c\n", valor/moedas[coinId].cota,moedas[coinId].nome,moedas[coinId].txbuy*100,37);
                printf("[1] Confirmar / [0] Cancelar: ");
                scanf("%d", &confirm);
                limpaBuffer();
                if(confirm == 1){
                    usuario->reais -= valor  + (valor*moedas[coinId].txbuy);
                    saldos->saldo += valor;
                    strcpy(saldos->cpf, usuario->cpf);
                    saldos->idCoin =  moedas[coinId].idcoin;
                    save_acao(COMPRA,usuario,valor,moedas[coinId].sigla,moedas[coinId].cota,moedas[coinId].txbuy);
                    save_saldo(saldos);
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
    }
        
        do{
            puts("\n1 - Voltar");
            printf("Opcao: ");
            opcao = getchar();
            limpaBuffer();
        }while(opcao != 49);
    }

void vender_cripto(User* usuario, BolsaCripto* moedas,Saldo* saldos, unsigned int qtd_moedas){
    int opcao, coinId;
    int validId = 0;
    float valor;
    int confirm;   
    int pode;
    
    puts("\t\tVender criptomoedas\n");
    FILE* readSaldos = fopen("saldos", "rb");
    if(readSaldos == NULL){
        puts("Nenhum Saldo de Cripto Existente no sistema");
        return;
    }
    
    while(fread(saldos,sizeof(Saldo),1,readSaldos)){
        if(!strcmp(saldos->cpf,  usuario->cpf)){
            for(unsigned int i = 0; i < qtd_moedas; ++i){
                if(saldos->idCoin == moedas[i].idcoin ){
                    printf("[%d] %s: R$%.2f\n",saldos->idCoin,moedas[i].nome, saldos->saldo);
                }
            }
        }
    }
    fseek(readSaldos, 0, SEEK_SET);
    printf("Qual criptomoeda deseja  vender: ");
    scanf("%d", &opcao);
    limpaBuffer();
    while(fread(saldos,sizeof(Saldo),1,readSaldos)){
        if(!strcmp(saldos->cpf,  usuario->cpf)){
            if((opcao) == saldos->idCoin){
                for(unsigned int i = 0; i < qtd_moedas; ++i){
                    if(moedas[i].idcoin == saldos->idCoin){
                        coinId = i;
                    }
                }
                validId = 1;
                break;
            }
        }
    }
    if(!validId){
        puts("Opcao Invalida");
    }
    else{
        printf("Quantos R$ deseja vender em %s: ",moedas[coinId].nome);
        scanf("%f", &valor);
        limpaBuffer();
        pode = check_buy_sell(VENDA,valor,moedas[coinId].cota,usuario->reais,saldos->saldo,moedas[coinId].txsell);
        if(pode){
            if(check_password(usuario->senha)){
                        printf("Voce vai vender %.4f e sera combrado uma taxa de %.0f%c\n", valor/moedas[coinId].cota,moedas[coinId].txsell*100,37);
                        printf("[1] Confirmar / [0] Cancelar: ");
                        scanf("%d", &confirm);
                        limpaBuffer();
                        if(confirm){
                            usuario->reais += valor  - (valor*moedas[coinId].txsell);
                            saldos->saldo -= valor;
                            strcpy(saldos->cpf, usuario->cpf);
                            saldos->idCoin = moedas[coinId].idcoin;
                            save_acao(VENDA,usuario,valor,moedas[coinId].sigla,moedas[coinId].cota, moedas[coinId].txsell);
                            save_saldo(saldos);
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

        }

    do{
        puts("\n1 - Voltar");
        printf("Opcao: ");
        opcao = getchar();
        limpaBuffer();
    }while(opcao != 49);
}

int main(void){
    unsigned int qtd_moedas;
    BolsaCripto* moedas = ler_moedas(&qtd_moedas);
    User loginUsuario;
    Saldo saldos;
    
    
    salvar_cota(moedas, qtd_moedas);
    
    char resposta,opcao;

    if (usuario()) {  
        if (login(&loginUsuario)) {
    // pós login Menu
            do{
                resposta = menu();

                switch(resposta){
                    case '1':
                        consultar_saldos(&loginUsuario,moedas,qtd_moedas, &saldos);
                        break;

                    case '2':
                        consultar_extrato(&loginUsuario,moedas,&saldos,qtd_moedas);
                        break;

                    case '3':
                        AdicionarSaldo(&loginUsuario);
                        break;

                    case '4':
                        SacarSaldo(&loginUsuario);
                        break;
                    
                    case '5':
                        comprar_cripto(&loginUsuario,moedas, qtd_moedas, &saldos);
                        break;

                    case '6':
                        vender_cripto(&loginUsuario,moedas,&saldos, qtd_moedas);
                        break;

                    case '7':
                    
                        puts("Valor atual da moeda\n");

                        ler_moedas(&qtd_moedas);
                        for(unsigned int i = 0; i < qtd_moedas; ++i){
                            printf("%s : R$ %.2f\n", moedas[i].nome, moedas[i].cota);
                        }

                        alterar_valor_moeda(moedas, qtd_moedas);
                        ler_moedas(&qtd_moedas); // salvando valor alterado

                        puts("\nCotacao atualizada!");
                        for(unsigned int i = 0; i < qtd_moedas; ++i){
                            printf("%s : R$ %.2f\n", moedas[i].nome, moedas[i].cota);
                        }
                        
                        do{
                            puts("1 - Voltar");
                            printf("Opcao: ");
                            opcao = getchar();
                            limpaBuffer();
                        }while(opcao != 49);

                        break;
                    
                    case '8':
                        puts("\nFim da Sessao.");
                        free(moedas);
                        break;
                }
            }while(resposta != '8');
        }
    }
    return 0; 
}
