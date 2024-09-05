#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int usuario(void);

int main(void){
    usuario();
    return 0;
}
int login(void){

}
int cadastro(void){
    char usuario[80];
    char senha[30];
    FILE *arquivo = fopen("Usuario", "ab");
    printf("Digite seu nome de usuario:");
    fgets(usuario, sizeof(usuario), stdin);
    fprintf(arquivo,"Usuario: %s",&usuario);
    
    
    printf("Digite sua senha: \n");
    fgets(senha, sizeof(senha), stdin), "\n";
    fprintf(arquivo,"Senha: %s",&senha);

     


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
