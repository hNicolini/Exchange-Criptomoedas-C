#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
#include <stdbool.h>
typedef struct{
    char usuario[80];
    char senha[80];
}User;
int usuario(void);
int cadastro(void);
=======

typedef struct bolsa{
    // declarando as moedas
    float bitcoin, ethereum, ripple;
} bolsa;

>>>>>>> 258a9e8753903f5813a0d2f27734ff2507018ad6
int main(void){
    bolsa moedas;
    moedas.bitcoin = 312444.10;
    moedas.ethereum = 16233.20;
    moedas.ripple = 231.03;
    // Salvando cotas em arquivo binario
    FILE* cotas = fopen("cotas", "wb");
    fwrite(&moedas,sizeof(bolsa),1, cotas);
    fclose(cotas);
    // limpando a variavel bitcoin em moedas para teste
    moedas.bitcoin = 0;
    moedas.ethereum = 0;
    printf("Moeda zerada: %.2f\n", moedas.bitcoin);
    // Lendo as cotas do arquivo binario
    puts("Conteudo do arquivo");
    cotas = fopen("cotas", "rb");
    fread(&moedas, sizeof(bolsa), 1, cotas);
    printf("bitcoin: %.2f\nethereum: %.2f\nripple: %.2f", moedas.bitcoin, moedas.ethereum, moedas.ripple);
    fclose(cotas);

}
<<<<<<< HEAD


int UsuarioExiste(char *usuario_existente){
    FILE *arquivo_ler = fopen("Usuario", "rb");
    if(arquivo_ler == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }
    User user;
    while(fread(&user, sizeof(User), 1, arquivo_ler)){
        if(strcmp(user.usuario, usuario_existente) == 0){
            return 1;
        } 
    fclose(arquivo_ler);
    return 0;         
    }

}
int cadastro(void){
    char usuario[80];
    char senha[30];
    
    
    printf("Digite o nome de usuario: \n");
    scanf("%s", &usuario);
    printf("Digite a Senha: \n");
    scanf("%s", &senha);
    if (UsuarioExiste(usuario))
    {
        printf("Usuario Existente!\n");
        cadastro();
            
    }else{
       FILE *arquivo = fopen("Usuario", "ab");
       User user;
        strncpy(user.&usuario, usuario, sizeof(user.usuario));
        strncpy(user.&senha, senha, sizeof(user.senha));
        fwrite(&user, sizeof(User), 1, arquivo);
        fclose(arquivo);
        printf("Usuario Cadastrado com Sucesso\n");
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
=======
>>>>>>> 258a9e8753903f5813a0d2f27734ff2507018ad6
