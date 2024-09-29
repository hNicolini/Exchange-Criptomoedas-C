# Projeto - Exchange de Criptomoedas
## Sobre
  Esse programa foi feito para simular um aplicativo de compras e vendas de criptomoedas, nele é possível realizar diversas ações como, cadastrar
  novos usuários, efetuar o login, realizar o depósito da moeda REAL, sacar uma quantia dessa moeda igual ou inferior à que possui na 
  conta, também é possível utilizar dessa moeda REAL para realizar a compra das criptomoedas disponíveis no programa, após a compra de 
  qualquer criptomoeda, é possível vende-las, cada uma dessas ações citadas acima ficam gravadas na aba "Consultar Extrato", o programa também
  possui o mecanismo de atualizar a cotação de moedas, onde ele varia o valor de cada moeda em -5% a +5% do seu valor anterior. Lembrando que,
  as moedas **são fictícias**, portanto não possuem significado algum fora do programa.

## Compilacao e Execução
### Compilando
  O programa é feito em um arquivo C denominado "main.c", utilizando o compilador gcc onde é realizado a traducao do codigo fonte para a linguagem da máquina, deve-se usar o comando:
                                      ```
                                        gcc -o nomedoexecutavel main.c
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
  - Ao ser executado o código é jogado na memória e processado pela CPU, exibindo assim as saídas e toda lógica definida no código.

## Como usar
  Ao executar o processamento do programa vai ser apresentado uma tela inicial de login
     
  
   
