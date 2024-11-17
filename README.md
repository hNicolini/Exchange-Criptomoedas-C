# Projeto - Exchange de Criptomoedas
## Sobre
  Esse programa foi feito para simular um aplicativo de compras e vendas de criptomoedas, nele é possível realizar diversas ações como usuario, sendo elas,
  efetuar o login, realizar o depósito da moeda REAL, sacar uma quantia dessa moeda igual ou inferior à que possui na 
  conta, também é possível utilizar dessa moeda REAL para realizar a compra das criptomoedas disponíveis no programa, após a compra de 
  qualquer criptomoeda, é possível vende-las, cada uma dessas ações citadas acima ficam gravadas na aba "Consultar Extrato", o programa também
  possui o mecanismo de atualizar a cotação de moedas, onde ele varia o valor de cada moeda em -5% a +5% do seu valor anterior. Lembrando que,
  as moedas **são fictícias**, portanto não possuem significado algum fora do programa. Existe o programa de administrador, onde ele pode realizar operações como, cadastrar novos investidores,
  cadastrar criptomoeda, remover criptomoeda, remover investidores, consultar saldo de investidores, consultar extrato de investidores e também é possível atualizar cotacao das moedas.

## Compilacao e Execução
### Compilando
  O programa é feito em dois arquivos C denominado "usuario.c" e "admin.c", utilizando o compilador gcc onde é realizado a traducao do codigo fonte para a linguagem da máquina, deve-se usar o comando:
                                      ```
                                        gcc -o nomedoexecutavel nomedoprograma.c
                                      ```
 - Esse comando vai gerar um arquivo .exe (executavel) onde ao abri-lo vai começar a rodar o programa.
 ### Etapas de compilacao
 - Primeiro ocorre um pré-processamento onde é feito a leitura das diretivas (```#include``` e ```#define```) do código:
   -  ```#include <stdio.h>``` essa diretiva permite o uso da biblioteca stdio.h que foi usada para possibilitar o uso das funções:
       -   ```printf()```,```scanf()```,```fgets()``` ,```sprintf()```,```getchar()``` para *I/O*
       -   ```fopen()```,```fclose```,```fread()```,```fwrite()```,```fseek()```,```ftell()``` para manipulacao dos arquivos.
   - ```#include <stdlib.h>``` é responsavel pelas funções:
       - ```srand()```, ```rand()``` para gerar numeros aleatorios
       - ```exit()``` para gerenciamento de erros.
   - ```#include <string.h``` dessa biblioteca foram utilizadas as funções:
       - ```strcmp()``` para verificar igualdade entre strings
       - ```strcpy()``` para copiar uma string
       - ```strcspn()``` para retir valores indesejados na string.
   - ```#include <time.h>``` biblioteca utilizada para obtenção de datas:
       - ```time()``` coleta a data atual do sistema utilizada em uma variavel tipo time_t
       - ```ctime()``` transforma o resultado obtido em ```time()``` para uma string.
   - ```#define LIMITE_USUARIOS 10``` é criado um macro onde LIMITE_USUARIOS é lido como o valor constante 10
   - ```#define LIMITE_EXTRATOS 100``` é criado um macro de LIMITE_EXTRATOS que é uma constante 100.

### Execução
  - Após a obtenção das bibliotecas e macros obtida pelas diretivas, o código é traduzido para linguagem máquina, onde é gerado o arquivo executável para utilização do programa. Utilizando Windows, digite o comando ```nomeexecutavel.exe``` para rodar o código traduzido.
  - Ao ser executado o código é jogado na memória e processado pela CPU, exibindo assim as saídas e também criando arquivos binários para armazenamento conforme definido na estrutura do código.

## Como usar
### Usuario
#### 1. Página inicial do programa

  <p align="center">Ao abrir o pograma você irá se deparar com a seguinte interface:</p>
  
   #### 1. Página de Login
   <p align="center">
     Após o administrador efetuar o cadastro do investidor, é possível realizar o login utilizando o CPF e senha cadastrados anteriormente
   </p> 
   
   #### 2. Menu de opções
   <p align="center">
     Login realizao com sucesso! você deve se deparar com essa tela:
     <br><br>
     <img src=https://github.com/user-attachments/assets/7b22362e-442d-4c21-889d-57f77ca14def>
     <br>
      Nela é possível escolher uma dentre as opções disponíveis, basta digitar o respectivo número da opção que deseja
   </p>

   ##### 2.1 Consultar o saldo
   <p align="center">
     Ao selecionar essa opção, será exibido para o usuário seus saldos:
     <br><br>
      <img src=https://github.com/user-attachments/assets/842e101f-b283-4f8a-9cb6-7d6553862858>
   </p>

   #### 2.2 Consultar extrato
   <p align="center">
     Ao selecionar a opção 2, o usuário será direcionado para a tela de consultar extrato onde o usuário pode checar as suas 100 últimas ações: <br>
     <img src=https://github.com/user-attachments/assets/03480b84-92d2-495b-94b1-426ceffcf1c1>

   </p>

   #### 2.3 Depositar
   <p align="center">
     A opção 3 é onde você realiza o deposito de dinheiro em R$, que cai diretamente no seu saldo como mostrado na imagem abaixo:
     <br><br>
     <img src=https://github.com/user-attachments/assets/fc886abc-1d1a-4bed-8d5e-fa7c144eb583>
     <br> 
   </p>

   #### 2.4 Sacar
   <p align="center">
     Na opção 4 é usada para realizar o saque de dinheiro R$ obtido no saldo da conta, o saque só poderá ser efetuado depois do usuario inserir sua senha:
     <br><br>
     <img src=https://github.com/user-attachments/assets/9a9652cb-4919-4cb4-b25b-2af3fbdd01ed>
     <br>

   </p>
  
  #### 2.5 Comprar criptomoedas
  <p align="center">
    Opção de número 5 é usada para efetuar a compra de criptomoedas:
    <br><br>
    <img src=https://github.com/user-attachments/assets/388473d5-4578-47f2-99d3-8efa5c0055f2>
    <br>
    Nessa tela o usuário vê as criptomoedas disponíveis para compra, junto de suas cotações em R$, ele pode selecionar a criptomoeda pelo número especificado.
    <br>
    Caso o usuário tenha escolhido alguma criptomoeda para comprar, será pedido o valor em R$ que deseja investir nela, ao inserir um valor válido, necessario inserir a senha do usuario, logo após será informado o valor unitário da moeda junto da taxa cobrada, o usuário precisa confirmar a compra para ela ser realizada.
    <br><br>
  </p>

  #### 2.6 Vender criptomoedas
   <p align="center">
     A opção número 6 permite que o usuário venda suas criptomoedas::
     <br><br>
     <img src=https://github.com/user-attachments/assets/62e92403-cf2d-402e-9208-c975c729b1a1>
     <br>
     É exibido o valor em R$ que o usuário possui de cada moeda, após selecionar a opção é pedido o valor em R$ que ele deseja vender e sua senha, após isso é exibido o valor unitário da moeda que vai restar para o usuario mais a taxa cobrada na venda, caso o usuario confirme a venda é realizada.
     <br><br>
   </p>
  
#### 2.7 Atualizar cotação
 <p align="center">
   Ao selecionar a opção número 7, será atualizada a cotação de todas as criptomoedas, variando o preço em -5% até +5% do seu valor atual, logo em seguida exibindo a cotação atualizada de cada criptomoeda
   <br><br>
   <img src=https://github.com/user-attachments/assets/8579f23f-faec-49a0-9707-97459f7d2297>
   <br>
   Isso é feito de forma automática, então o usuário só precisa digitar a opção de número 7 que já será feito o processo de atualização das cotas.
 </p>

 #### 2.8 Sair
   <p align="center">
     Essa opção é a última opção, ela é usada para encerrar o serviço, finalizando a interação com o usuário e fechando o programa.
### Administrador
     #### Pagina inicial
     <p align="center">Ao abrir o pograma será necessário realizar o login:</p>
     <br><br>
   <img src=https://github.com/user-attachments/assets/ddf9d22d-6692-45bb-94a6-05ed4720e2ae>
   <br>
     As credenciais de login dos administradores está localizada no arquivo "logins_admin.csv"
  </p>
  
   #### 2. Menu de opções
   <p align="center">
     Após efetuar o login de administrador será exibido esse menu:
     <br><br>
     <img src=https://github.com/user-attachments/assets/09a74985-f8e5-46c7-af21-521c43faeaf0>
      <br>
      Nela é possível escolher uma dentre as opções disponíveis, basta digitar o respectivo número da opção que deseja
   </p>

   ##### 2.1 Cadastrar Investidor
   <p align="center">
     Aqui você cadastra um novo investidor, digitando seu CPF, Nome e Senha de usuário:
     <br><br>
      <img src=https://github.com/user-attachments/assets/5194be17-888c-4849-ba6b-8e5807d0a1c2>

   </p>

   #### 2.2 Remover Investidor
   <p align="center">
     Ao selecionar a opção 2, será exibido os investidores cadastrados, digitando seu respectivo CPF será exibido os dados do usuario e pedira confirmacao para exclusão: <br>
     <img src=https://github.com/user-attachments/assets/2cb5851d-e790-48da-9436-7995fe466966>

   </p>

   #### 2.3 Cadastrar Criptomoeda
   <p align="center">
     A opção 3 é onde você cadastra uma criptomoeda no programa, digitando nome, sigla, cotacao e taxas de compra e venda:
     <br><br>
     <img src=https://github.com/user-attachments/assets/5086a3d3-18f9-46d2-846c-e2d95c28f55b>
     <br> 
     ATENÇÃO! A taxa de compra e venda é dada em %, então, por exemplo, digite 0.1 para 10%
   </p>

   #### 2.4 Remover Criptomoeda
   <p align="center">
     Na opção 4 é usada para realizar remover criptomoeda, digitando o ID da moeda desejada para excluir:
     <br><br>
     <img src=https://github.com/user-attachments/assets/9a9652cb-4919-4cb4-b25b-2af3fbdd01ed>
     <br>

   </p>
  
  #### 2.5 Comprar criptomoedas
  <p align="center">
    Opção de número 5 é usada para efetuar a compra de criptomoedas:
    <br><br>
    <img src=https://github.com/user-attachments/assets/388473d5-4578-47f2-99d3-8efa5c0055f2>
    <br>
    Nessa tela o usuário vê as criptomoedas disponíveis para compra, junto de suas cotações em R$, ele pode selecionar a criptomoeda pelo número especificado.
    <br>
    Caso o usuário tenha escolhido alguma criptomoeda para comprar, será pedido o valor em R$ que deseja investir nela, ao inserir um valor válido, necessario inserir a senha do usuario, logo após será informado o valor unitário da moeda junto da taxa cobrada, o usuário precisa confirmar a compra para ela ser realizada.
    <br><br>
  </p>

  #### 2.6 Vender criptomoedas
   <p align="center">
     A opção número 6 permite que o usuário venda suas criptomoedas::
     <br><br>
     <img src=https://github.com/user-attachments/assets/62e92403-cf2d-402e-9208-c975c729b1a1>
     <br>
     É exibido o valor em R$ que o usuário possui de cada moeda, após selecionar a opção é pedido o valor em R$ que ele deseja vender e sua senha, após isso é exibido o valor unitário da moeda que vai restar para o usuario mais a taxa cobrada na venda, caso o usuario confirme a venda é realizada.
     <br><br>
   </p>
  
#### 2.7 Atualizar cotação
 <p align="center">
   Ao selecionar a opção número 7, será atualizada a cotação de todas as criptomoedas, variando o preço em -5% até +5% do seu valor atual, logo em seguida exibindo a cotação atualizada de cada criptomoeda
   <br><br>
   <img src=https://github.com/user-attachments/assets/8579f23f-faec-49a0-9707-97459f7d2297>
   <br>
   Isso é feito de forma automática, então o usuário só precisa digitar a opção de número 7 que já será feito o processo de atualização das cotas.
 </p>

 #### 2.8 Sair
   <p align="center">
     Essa opção é a última opção, ela é usada para encerrar o serviço, finalizando a interação com o usuário e fechando o programa.

## Créditos
   - Kaio Custodio (22.224.028-5)
   - Henrique Nicolini (22.124.049-2)
  
    

     
  
   
