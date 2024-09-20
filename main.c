#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char usuario[80];
    char senha[80];
    float Saldo;
} User;

int usuario(void);
int cadastro(void);

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

//Função principal

int main(void) {
    User loginUsuario;

    if (usuario()) {  
        if (login(&loginUsuario)) {
            AdicionarSaldo(&loginUsuario); 
            SacarSaldo(&loginUsuario); 
        }
    }

    return 0;
}
