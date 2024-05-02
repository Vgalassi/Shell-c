**Shell - Interpretador de Comandos em C**

Este repositório contém o código fonte para um interpretador de comandos em linguagem C, também conhecido como shell. O shell é capaz de receber comandos interativamente via terminal ou de um arquivo batch, interpretar cada comando e decidir entre comandos internos (built-in) ou programas externos, executando-os em novos processos filhos.

**Objetivo:**
Desenvolver uma aplicação Shell - interpretador de comandos em C.

**Conceitos envolvidos:**
- Chamadas de sistema
- Criação e gerenciamento de processos
- Gerenciamento de arquivos

**Funcionalidade básica:**
- Receber comandos e interpretá-los em um laço infinito.
- Comandos internos (built-in):
  - `exit`: Finaliza o Shell.
  - `cd <caminho>`: Muda o diretório de trabalho.
  - `path <caminho> [<caminho> <caminho> ...]`: Define caminho(s) para busca de executáveis.
- Programas externos:
  - O shell procura por executáveis nos caminhos definidos com o built-in `path`.
  - Executa o programa em um novo processo, passando os argumentos solicitados pelo usuário.
  - Imprime na tela o valor retornado pelo programa executado.
  - Emite uma mensagem de erro se o programa não for encontrado.

**Condições de erro:**
O shell trata condições de erro e imprime mensagens amigáveis para elas.

**Outras funcionalidades:**
1. Clone do `cat`: Lê o conteúdo do arquivo no argumento e o escreve na saída padrão.
2. Clone do `ls`: Lista o conteúdo do diretório atual, suportando os parâmetros `-l` e `-a`.
3. Redirecionamento de saída: A saída dos comandos pode ser redirecionada para um arquivo.
4. Comandos em paralelo: Vários comandos separados por `&` são executados de forma concorrente, em processos separados.

**Observações:**
- Os argumentos de comandos são separados por espaços (um ou mais).
- Não é preciso se preocupar com caracteres de escape ou argumentos com espaços entre aspas.
