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

//Função Não funcional!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // char usuario1[80];
    // char senha[80];    
    
    // printf("Digite o nome de usuario: \n");
    // scanf("%s", usuario1);
    // printf("Digite a Senha: \n");
    // scanf("%s", senha);

    // FILE* arquivo = fopen("Usuario", "rb");
    // if (arquivo == NULL) {
    //     printf("Erro ao abrir o arquivo.\n");
    //     return 0;
    // }

    // User usuariolido;

    // while (fread(&usuariolido, sizeof(User), 1, arquivo)) {
    //     if (strcmp(usuariolido.usuario, usuario1) == 0 && strcmp(usuariolido.senha, senha) == 0) {
    //         fclose(arquivo);
    //         printf("Logado Com sucesso!");
    //         return 1;

    //     }
    // }
    // printf("Tente Novamente!");
    // fclose(arquivo);
    // return 0;
}
int cadastro(void){
    char usuario[80];
    char senha[30];
       
    printf("Digite o nome de usuario: \n");
    scanf("%s", usuario);
    printf("Digite a Senha: \n");
    scanf("%s", senha);
    if (usuarioExiste("usuario",usuario))
        
    {    
        cadastro();  
    }
    
    else{
        FILE *arquivo = fopen("Usuario", "ab");
        User user;
        fwrite(user.usuario, sizeof(char), strlen(user.usuario), arquivo);
        fwrite(user.senha, sizeof(char), strlen(user.senha), arquivo);     
        fclose(arquivo);
        printf("Usuario Cadastrado com Sucesso\n");
        login();
    }
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

