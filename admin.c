#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIMITE_USUARIOS 10
#define LIMITE_EXTRATOS 100
enum tipo_consulta { SALDO = 1, EXTRATO = 2 };

// aqui estão as criptomoedas
typedef struct BolsaCripto {
  int idcoin;
  char sigla[4];
  char nome[255];
  float cota, txbuy, txsell;

} BolsaCripto;

// saldo de cada moeda do usuario
typedef struct Saldo {
  char cpf[80];
  int idcoin;
  float saldo;
} Saldo;

typedef struct User {

  char cpf[80];
  char nome[50];
  char senha[80];
  char extrato[LIMITE_EXTRATOS][200];
  int qntd_extrato;
  int qtd_coins;
  float reais;

} User;

char *clear_newLine(char *string) {

  string[strcspn(string, "\n")] = 0;

  return string;
}

char dados_user(User *usuario, BolsaCripto *moedas, unsigned int qtd_moedas) {
  Saldo saldos;
  puts("\t\tDados do Usuario\n\n");
  printf("CPF: %s\n", usuario->cpf);
  printf("Nome: %s\n", usuario->nome);
  printf("Quantidade Extratos: %d\n", usuario->qntd_extrato);
  printf("REAL: %.2f\n", usuario->reais);

  FILE *readSaldos = fopen("saldos", "rb");
  if (readSaldos == NULL) {
    puts("Nenhum Saldo de Cripto Existente no sistema");
  }

  while (fread(&saldos, sizeof(Saldo), 1, readSaldos)) {
    if (!strcmp(saldos.cpf, usuario->cpf)) {
      for (unsigned int i = 0; i < qtd_moedas; ++i) {
        if (saldos.idcoin == moedas[i].idcoin) {
          printf("%s: R$%.2f\n", moedas[i].nome, saldos.saldo);
        }
      }
    }
  }
}

void limpaBuffer() {
  int c;
  // Lê e descarta todos os caracteres até o '\n' ou EOF
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

int login_admin() {
  char linha[255];
  char login[50];
  char senha[50];
  char login_arquivo[50];
  char senha_arquivo[50];

  FILE *readAdmins = fopen("logins_admin.csv", "r");
  if (readAdmins == NULL) {
    puts("Erro ao abrir o arquivo das credencias dos admins.");
    return 0;
  }

  puts("TELA DE LOGIN ADMINISTRADOR");

  printf("User: ");
  fgets(login, sizeof(login), stdin);
  strcpy(login, clear_newLine(login));

  printf("Senha: ");
  fgets(senha, sizeof(senha), stdin);
  strcpy(senha, clear_newLine(senha));

  // pular os atributos
  if (fgets(linha, sizeof(linha), readAdmins) == NULL) {
    fclose(readAdmins);
    puts("Erro: Arquivo vazio ou problema ao ler os atributos.");
    return 0;
  }

  // Ler cada credencial do arquivo
  while (fgets(linha, sizeof(linha), readAdmins)) {
    // Separar login e senha usando vírgula como separador
    if (sscanf(linha, "%[^,],%s", login_arquivo, senha_arquivo) == 2) {
      // Comparar login e senha
      if (strcmp(login, login_arquivo) == 0 &&
          strcmp(senha, senha_arquivo) == 0) {
        fclose(readAdmins);
        return 1;
      }
    }
  }

  fclose(readAdmins);
  return 0;
}

int gera_idcoin() {
  BolsaCripto recentCoin;

  FILE *moedas = fopen("moedas", "rb");
  if (moedas == NULL) {
    puts("Primeira moeda do programa.");
    return 0;
  }

  if (fseek(moedas, -(sizeof(BolsaCripto)), SEEK_END) != 0) {
    puts("Nao foi possivel posicionar o ponteiro no ultimo registro do "
         "arquivo.");
    fclose(moedas);

    return -1;
  }

  if (fread(&recentCoin, sizeof(BolsaCripto), 1, moedas) != 1) {
    puts("Erro na leitura do arquivo");
    fclose(moedas);

    return -1;
  }

  int id_gerado = recentCoin.idcoin + 1;

  fclose(moedas);

  return id_gerado;
}

void listar_moedas() {
  FILE *arquivo = fopen("moedas", "rb");
  if (arquivo == NULL) {
    puts("Nenhuma criptomoeda registrada.");
    return;
  }

  BolsaCripto moeda;
  puts("\t\tLista de Criptomoedas");
  while (fread(&moeda, sizeof(BolsaCripto), 1, arquivo) == 1) {
    printf("ID: %d\n", moeda.idcoin);
    printf("Nome: %s\n", moeda.nome);
    printf("Cotacao: %.2f\n", moeda.cota);
    puts("--------------------------");
  }

  fclose(arquivo);
}

void listar_users() {
  User usuarios;
  FILE *rUsers = fopen("usuarios", "rb");
  if (rUsers == NULL) {
    puts("Sem usuarios para ler");
    return;
  }
  while (fread(&usuarios, sizeof(User), 1, rUsers)) {
    printf("CPF: %s\n", usuarios.cpf);
    printf("Nome: %s\n", usuarios.nome);
    puts("-----------------");
  }
  fclose(rUsers);
}

void consultar_saldos(BolsaCripto *moedas, unsigned int qtd_moedas) {
  char opcao, cpf[80];
  int encontrou = 0;
  Saldo saldos;
  User usuario;
  puts("\t\tConsultar Saldo de Investidor");
  listar_users();
  printf("Insira o CPF: ");
  fgets(cpf, sizeof(cpf), stdin);
  strcpy(cpf, clear_newLine(cpf));

  printf("\t\tSaldo na Carteira do cpf: %s\n\n", cpf);

  FILE *readUsers = fopen("usuarios", "rb");
  if (readUsers == NULL) {
    puts("Nao foi possivel abrir o arquivo usuarios para ler");
    return;
  }
  while (fread(&usuario, sizeof(User), 1, readUsers)) {
    if (strcmp(usuario.cpf, cpf) == 0) {
      encontrou = 1;
      break;
    }
  }
  fclose(readUsers);
  if (!encontrou) {
    puts("CPF invalido");
    return;
  }
  printf("REAL: %.2f\n", usuario.reais);
  FILE *readSaldos = fopen("saldos", "rb");
  if (readSaldos == NULL) {
    puts("Nenhum Saldo de Cripto Existente no sistema");
  }

  while (fread(&saldos, sizeof(Saldo), 1, readSaldos)) {
    if (!strcmp(saldos.cpf, usuario.cpf)) {
      for (unsigned int i = 0; i < qtd_moedas; ++i) {
        if (saldos.idcoin == moedas[i].idcoin) {
          printf("%s: R$%.2f\n", moedas[i].nome, saldos.saldo);
        }
      }
    }
  }
  fclose(readSaldos);
  do {
    puts("1 - Voltar");
    printf("Opcao: ");
    opcao = getchar();
    limpaBuffer();
  } while (opcao != 49);

  return;
}

// verificando se é possivel cadastrar novos usuarios
int user_limit_over(char *nome_arquivo) {
  FILE *arquivo = fopen(nome_arquivo, "rb");
  if (arquivo == NULL) {
    return 0; // se o arquivo não existir é possivel cadastrar usuarios
  }
  fseek(arquivo, 0, SEEK_END);
  unsigned int tamanho_arquivo =
      ftell(arquivo); // pegando o tamanho total do arquivo
  fclose(arquivo);
  if (tamanho_arquivo < sizeof(User) * LIMITE_USUARIOS) {
    return 0;
  }
  return 1;
}

int usuarioExiste(char *nomeArquivo, char *nome) {
  FILE *arquivo = fopen(nomeArquivo, "rb");
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

void writeUser(User *user) {
  FILE *wfile = fopen("usuarios", "ab");
  if (wfile == NULL) {
    puts("First User!");
  }

  fwrite(user, sizeof(User), 1,
         wfile); // escrevendo credenciais + extrato do usuario + saldo de reais

  fclose(wfile);
}

int cadastro(void) {
  User novousuario;
  printf("\t\tCadastro\n");
  printf("Digite o CPF: ");
  fgets(novousuario.cpf, sizeof(novousuario.cpf), stdin);
  strcpy(novousuario.cpf, clear_newLine(novousuario.cpf));
  if (usuarioExiste("usuarios", novousuario.cpf)) {
    printf("Usuario existente ");
    return 0;
  }
  printf("Digite seu nome: ");
  fgets(novousuario.nome, sizeof(novousuario.nome), stdin);
  strcpy(novousuario.nome, clear_newLine(novousuario.nome));

  printf("Digite sua senha: ");
  fgets(novousuario.senha, sizeof(novousuario.senha), stdin);
  strcpy(novousuario.senha, clear_newLine(novousuario.senha));

  novousuario.reais = 0;
  novousuario.qntd_extrato = 0;
  novousuario.qtd_coins = 0;

  FILE *arquivo = fopen("usuarios", "ab");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de usuários.\n");
    return 0;
  }

  writeUser(&novousuario);
  fclose(arquivo);
  return 1;
}

BolsaCripto *ler_moedas(unsigned int *qtd_moedas) {
  BolsaCripto *moedas;

  FILE *readFile = fopen("moedas", "rb");
  if (readFile == NULL) {
    printf("Não foi possivel abrir o arquivo para ler\n");
    *qtd_moedas = 0;
    return 0;
  }

  fseek(readFile, 0, SEEK_END);
  size_t pos = ftell(readFile);
  *qtd_moedas = pos / sizeof(BolsaCripto);
  moedas = malloc((*qtd_moedas) * sizeof(BolsaCripto));
  if (moedas == NULL) {
    puts("Nao foi possivel alocar memoria para as moedas");
    exit(1);
  }
  fseek(readFile, 0, SEEK_SET);
  if ((*qtd_moedas) == 0) {
    puts("Nenhuma moeda no banco de criptomoedas");
    fclose(readFile);
    return NULL;
  }
  if (fread((moedas), sizeof(BolsaCripto), *qtd_moedas, readFile) !=
      (*qtd_moedas)) {
    printf("Erro ao ler as cotas.\n");
  }
  fclose(readFile);

  return moedas;
}

bool coinExists(char *coinName, char *coinTicker) {
  BolsaCripto coins;

  FILE *criptomoedas = fopen("moedas", "rb");
  if (criptomoedas == NULL) {

    return false;
  }

  while (fread(&coins, sizeof(BolsaCripto), 1, criptomoedas)) {

    if (strcmp(coinName, coins.nome) == 0) {
      puts("Este nome de moeda ja existe");
      return true;
    }
    if (strcmp(coinTicker, coins.sigla) == 0) {
      puts("Esta sigla de moeda ja existe");
      return true;
    }
  }
  return false;
}

void adicionar_moeda() {
  BolsaCripto moeda;
  puts("\t\tCadastrar Criptomoeda");

  int idcoin = gera_idcoin();

  FILE *arquivo = fopen("moedas", "ab");
  if (arquivo == NULL) {
    puts("Erro na abertura do arquivo moedas");
    exit(1);
  }

  if (idcoin != -1) {

    moeda.idcoin = idcoin;

    while (true) {
      printf("Digite o nome da moeda: ");
      fgets(moeda.nome, sizeof(moeda.nome), stdin);
      strcpy(moeda.nome, clear_newLine(moeda.nome));

      if (!coinExists(moeda.nome, "")) {
        break;
      }
    }

    while (true) {
      printf("Digite a sigla da moeda: ");
      fgets(moeda.sigla, sizeof(moeda.sigla), stdin);
      strcpy(moeda.sigla, clear_newLine(moeda.sigla));
      // limpaBuffer();

      if (!coinExists("", moeda.sigla)) {
        break;
      }
    }

    printf("Digite a cotacao");
    scanf("%f", &moeda.cota);
    limpaBuffer();

    printf("Digite a taxa de compra: ");
    scanf("%f", &moeda.txbuy);
    limpaBuffer();

    printf("Digite a taxa de venda: ");
    scanf("%f", &moeda.txsell);
    limpaBuffer();

    fwrite(&moeda, sizeof(BolsaCripto), 1, arquivo);

    puts("Criptomoeda Cadastrada com Sucesso");

  } else {
    puts("Erro na criacao de uma nova moeda");
  }

  fclose(arquivo);
}

void update_saldos(int idcoin, char *cpf) {

  FILE *rSaldos = fopen("saldos", "rb+");
  if (rSaldos == NULL) {
    return;
  }

  // Obtendo numero total de saldos
  fseek(rSaldos, 0, SEEK_END);
  size_t pos = ftell(rSaldos);
  size_t qtd_saldos = pos / sizeof(Saldo);

  Saldo *all_saldos = malloc(qtd_saldos * sizeof(Saldo));
  fseek(rSaldos, 0, SEEK_SET);
  fread(all_saldos, sizeof(Saldo), qtd_saldos, rSaldos);
  if (all_saldos == NULL) {
    puts("Erro na alocacao de memoria para all_saldos");
    return;
  }
  fclose(rSaldos);

  Saldo *filteredSaldos = malloc((qtd_saldos - 1) * sizeof(Saldo));
  if (filteredSaldos == NULL) {
    puts("Erro na alocacao de memoria para filteredSaldos");
    free(all_saldos);
    return;
  }
  int cont = 0;

  // Pegando os saldos que nao tiveram a moeda removida
  if(idcoin >= 0){
  for (unsigned int i = 0; i < qtd_saldos; ++i) {
    if (all_saldos[i].idcoin != idcoin) {
      filteredSaldos[cont] = all_saldos[i];
      cont++;
    }
  }
  }
  else{
    for (unsigned int i = 0; i < qtd_saldos; ++i) {
        if (strcmp(all_saldos[i].cpf, cpf) != 0) {
        filteredSaldos[cont] = all_saldos[i];
        cont++;
        }
    }
  }
  free(all_saldos);
  // Escrevendo os saldos que nao tiveram a moeda removida no arquivo
  FILE *rewriteSaldos = fopen("saldos", "wb");
  if (rewriteSaldos == NULL) {
    puts("Impossivel reescrever saldos");
    free(filteredSaldos);
    return;
  }

  fwrite(filteredSaldos, sizeof(Saldo), cont, rewriteSaldos);

  free(filteredSaldos);

  fclose(rewriteSaldos);

  printf("Todos os saldos com idCoin %d foram excluidos\n\n", idcoin);
}

void remover_moeda(BolsaCripto *moedas, unsigned int qtd_moedas) {
  int id, indexCoin;
  int valid = 0;

  puts("\t\tExcluir Criptomoeda");
  listar_moedas();
  printf("Insira o ID da Moeda que deseja remover: ");
  scanf("%d", &id);
  for (unsigned int i = 0; i < qtd_moedas; ++i) {
    if (moedas[i].idcoin == id) {
      valid = 1;
      indexCoin = i;
      break;
    }
  }
  if (!valid) {
    puts("ID invalido");
  } else {
    BolsaCripto *filteredCoins = malloc((qtd_moedas - 1) * sizeof(BolsaCripto));

    FILE *wCriptos = fopen("moedas", "wb");
    if (wCriptos == NULL) {
      puts("Nenhuma cripto cadastrada");
      return;
    }
    int filtIndex = 0;
    for (unsigned int i = 0; i < qtd_moedas; ++i) {
      if (moedas[i].idcoin != id) {
        filteredCoins[filtIndex] = moedas[i];
        filtIndex++;
      }
    }
    fwrite(filteredCoins, sizeof(BolsaCripto), qtd_moedas - 1, wCriptos);
    printf("A moeda %s foi removida.\n", moedas[indexCoin].nome);
    fclose(wCriptos);
    update_saldos(id, "");
    listar_moedas();
  }
}

void salvar_cota(BolsaCripto *moedas, const unsigned int qtd_moedas) {
  FILE *arquivo = fopen("moedas", "wb");
  // Salvando cotas em arquivo binario
  if (arquivo == NULL) {
    printf("Não foi possivel abrir o arquivo para salvar cotas");
    exit(1);
  }

  fwrite(moedas, sizeof(BolsaCripto), qtd_moedas, arquivo);
  fclose(arquivo);
}

void alterar_valor_moeda(BolsaCripto *moedas, const unsigned int qtd_moedas) {
  // gerando o numero aleatorio entre -5% e 5%
  srand(time(NULL));
  float numero_gerado = ((rand() % 5) + 1.0) / 100.0;
  int escolhe_sinal = rand() % 2;
  if (escolhe_sinal == 1) {
    numero_gerado *= -1;
  }
  printf("\nAumentou/Diminuiu %.2f%c \n", numero_gerado * 100.0, 37);

  for (unsigned int i = 0; i < qtd_moedas; ++i) {
    moedas[i].cota += numero_gerado;
  }

  salvar_cota(moedas, qtd_moedas);
}

void RemoverUsuarios(BolsaCripto *moedas, unsigned int qtd_moedas) {
  char cpf[80];
  int validCpf = 0;
  int confirm = 0;

  User usuarios;

  puts("\t\tRemover Investidor");
  listar_users();
  printf("Insira o CPF do Investidor que deseja remover: ");
  fgets(cpf, sizeof(cpf), stdin);
  strcpy(cpf, clear_newLine(cpf));

  FILE *coletarusers = fopen("usuarios", "rb+");
  if (coletarusers == NULL) {
    puts("Nenhum usuario cadastrado");
    return;
  }
  while (fread(&usuarios, sizeof(User), 1, coletarusers)) {
    if (strcmp(cpf, usuarios.cpf) == 0) {
      validCpf = 1;
      dados_user(&usuarios, moedas, qtd_moedas);
      puts("[1] Excluir\n[2] Cancelar");
      printf("Opcao: ");
      scanf("%d", &confirm);
    }
  }
  if (!validCpf) {
    puts("CPF invalido");
    fclose(coletarusers);
    return;
  }
  if (confirm == 1) {

    // Obtendo numero total de usuarios
    fseek(coletarusers, 0, SEEK_END);
    size_t pos = ftell(coletarusers);
    size_t qtd_usuarios = pos / sizeof(User);

    User *all_users = malloc(qtd_usuarios * sizeof(User));
    rewind(coletarusers);
    fread(all_users, sizeof(User), qtd_usuarios, coletarusers);
    if (all_users == NULL) {
      puts("Erro na alocacao de memoria para all_users");
      return;
    }
    fclose(coletarusers);

    User *filteredUsers = malloc((qtd_usuarios - 1) * sizeof(User));
    if (filteredUsers == NULL) {
      puts("Erro na alocacao de memoria para filteredUsers");
      free(all_users);
      return;
    }
    int cont = 0;

    // Pegando os usuarios que nao foram removidos
    for (unsigned int i = 0; i < qtd_usuarios; i++) {
      if (strcmp(all_users[i].cpf, cpf) != 0) {
        filteredUsers[cont] = all_users[i];
        cont++;
      }
    }
    free(all_users);

    // Escrevendo os usuarios que nao foram removidos no arquivo
    FILE *rewriteUsers = fopen("usuarios", "wb");
    if (rewriteUsers == NULL) {
      puts("Impossivel reescrever usuarios");
      free(filteredUsers);
      return;
    }

    fwrite(filteredUsers, sizeof(User), cont, rewriteUsers);

    free(filteredUsers);
    update_saldos(-1, cpf);
    fclose(rewriteUsers);

    printf("Usuario com CPF [%s] foi removido.\n", cpf);
  }
  return;
}

void consultar_extrato(BolsaCripto *moedas, unsigned int qtd_moedas) {
  char opcao, cpf[80];
  int encontrou = 0;
  Saldo saldos;
  User usuario;

  puts("\t\tConsultar Extrato de Investidor");
  listar_users();
  printf("Insira o CPF: ");
  fgets(cpf, sizeof(cpf), stdin);
  strcpy(cpf, clear_newLine(cpf));

  printf("\t\tExtrato do cpf: %s\n\n", cpf);

  FILE *readUsers = fopen("usuarios", "rb");
  if (readUsers == NULL) {
    puts("Nao foi possivel abrir o arquivo usuarios para ler");
    return;
  }
  while (fread(&usuario, sizeof(User), 1, readUsers)) {
    if (strcmp(usuario.cpf, cpf) == 0) {
      encontrou = 1;
      break;
    }
  }
  if (!encontrou) {
    puts("CPF invalido");
    fclose(readUsers);
    return;
  }
  fclose(readUsers);

  int limite = (usuario.qntd_extrato);

  FILE *readSaldos = fopen("saldos", "rb");

  printf("\t\tExtrato da conta de %s\n", usuario.nome);
  for (int i = 0; i < limite; i++) {
    printf("%s", usuario.extrato[i]);
    while (fread(&saldos, sizeof(Saldo), 1, readSaldos)) {
      if (!strcmp(saldos.cpf, usuario.cpf)) {

        for (unsigned int i = 0; i < qtd_moedas; ++i) {
          if (saldos.idcoin == moedas[i].idcoin) {

            printf(" %s: %.4f", moedas[i].sigla, saldos.saldo / moedas[i].cota);
          }
        }
      }
    }
    fseek(readSaldos, 0, SEEK_SET);
    printf("\n");
  }

  do {
    puts("1 - Voltar");
    printf("Opcao: ");
    opcao = getchar();
    limpaBuffer();
  } while (opcao != 49);

  return;
}

int menu() {
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
  scanf("%d", &opcao);
  limpaBuffer();

  return opcao;
}

int main(void) {
  unsigned int qtd_moedas;
  BolsaCripto *moeda = ler_moedas(&qtd_moedas);
  int opcao;
  if (login_admin()) {
    while (true) {
      switch (menu()) {
      case 1:
        puts("Cadastrar Investidor");
        if (!user_limit_over("usuarios")) {
          cadastro();
        }
        break;
      case 2:
        RemoverUsuarios(moeda, qtd_moedas);
        break;
      case 3:
        adicionar_moeda();
        ler_moedas(&qtd_moedas);
        break;
      case 4:
        remover_moeda(moeda, qtd_moedas);
        ler_moedas(&qtd_moedas);
        break;
      case 5:
        consultar_saldos(moeda, qtd_moedas);
        break;
      case 6:
        consultar_extrato(moeda, qtd_moedas);
        break;
      case 7:

        puts("Valor atual da moeda\n");

        ler_moedas(&qtd_moedas);
        for (unsigned int i = 0; i < qtd_moedas; ++i) {
          printf("%s : R$ %.2f\n", moeda[i].nome, moeda[i].cota);
        }

        alterar_valor_moeda(moeda, qtd_moedas);
        ler_moedas(&qtd_moedas); // salvando valor alterado

        puts("\nCotacao atualizada!");
        for (unsigned int i = 0; i < qtd_moedas; ++i) {
          printf("%s : R$ %.2f\n", moeda[i].nome, moeda[i].cota);
        }

        do {
          puts("1 - Voltar");
          printf("Opcao: ");
          scanf("%d", &opcao);
          limpaBuffer();
        } while (opcao != 1);

        break;
      case 8:
        puts("Finalizando sessao");
        free(moeda);
        exit(1);
        break;
      default:
        puts("Opcao invalida");
        break;
      }
    }
  } else {
    puts("Login ou senha invalido");
  }
  return 0;
}