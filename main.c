#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct{
    char usuario[80];
    char senha[80];
}User;
#include <time.h>


int usuario(void);
int cadastro(void);


int usuarioExiste(char* nomeArquivo, char* nome) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    User usuariolido;

    // Ler cada nome do arquivo binário e comparar
    while (fread(&usuariolido, sizeof(User),1, arquivo)) {
        if (strcmp(usuariolido.usuario, nome) == 0) {
            fclose(arquivo);
            printf("Usuario Existente!\n");
            return 1; 
        }
    }

    fclose(arquivo);
    return 0;
}





int login(void){
    char usuario[80];
    char senha[80];

    printf("Digite o nome de usuario:\n");
    scanf("%s", usuario);
    printf("Digite sua senha:\n");
    scanf("%s", senha);

    FILE* arquivo = fopen("Usuario", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    User loginUsuario;
    int encontrado = 0;
    while (fread(&loginUsuario,sizeof(User),1,arquivo))
    {
        if (strcmp(loginUsuario.usuario,usuario) ==0  && strcmp(loginUsuario.senha,senha) == 0){
            encontrado = 1;
            break;
        }
    }
    fclose(arquivo);

    if (encontrado == 1)
    {
        printf("Logado com sucesso, Bem-Vindo - %s \n ", usuario);
    }
    else{
        printf("Nome de usuario ou senha incorreto(s) \n");
    }
    
    



}
int cadastro(void){
    User novousuario;
    printf("Digite seu nome de usuario:\n");
    scanf("%s", novousuario.usuario);

    if(usuarioExiste("Usuario",novousuario.usuario)){
        printf("Usuario Existente!\n");
        usuario();
    } 
    printf("Digite a sua senha:\n");
    scanf("%s", novousuario.senha);
    
    FILE* arquivo = fopen("Usuario", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    fwrite(&novousuario, sizeof(User),1,arquivo);
    fclose(arquivo);
    printf("\nUsuario registrado com sucesso!\n");
    login();
}




int usuario(void){
    int resposta;
    printf("Ja possui um Login?\n1- Sim\n2- Nao\n");
    scanf("%d", &resposta);
    if (resposta == 1)
    {
        login();
    }
    else if(resposta == 2)
    {
        cadastro();
    }
    else{
        printf("Numero invalido!\n");
        usuario();
    }
        
}

int main(void){
    usuario();

    return 0;
}

