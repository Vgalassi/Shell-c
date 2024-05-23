# Shell - Interpretador de Comandos <img src="https://runcode-app-public.s3.amazonaws.com/images/bash-shell-script-online-editor-compiler.original.png" alt="Shell Logo" width="30">

Este repositório contém o código fonte para um interpretador de comandos em linguagem C, também conhecido como shell. O shell é capaz de receber comandos interativamente via terminal ou de um arquivo batch, interpretar cada comando e decidir entre comandos internos (built-in) ou programas externos, executando-os em novos processos filhos.

## Objetivo 
Desenvolver uma aplicação Shell - interpretador de comandos em C.

## Pré-requisitos
1. Ter o **GIT** instalado <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/3f/Git_icon.svg/2048px-Git_icon.svg.png" alt="Git Logo" width="22"> 

   - Link para download -> https://git-scm.com/downloads

2. Ter o **VS Code** instalado (ou um compilador de sua preferência) <img src="https://cdn.iconscout.com/icon/free/png-256/free-visual-studio-code-3251603-2724650.png?f=webp" alt="VsCODE Logo" width="26"> 

   - Link para download -> https://code.visualstudio.com/download
  
### Pode ser executado na VM VirtualBox ou no Docker

3. Ter a **VM VirtualBox** instalada <img src="https://upload.wikimedia.org/wikipedia/commons/d/d5/Virtualbox_logo.png" alt="VM Logo" width="25"> 

   - Link para download -> https://www.virtualbox.org/wiki/Downloads

4. Ter o **Docker** instalado <img src="https://wiki.hornbill.com/images/7/70/Docker_logo.png" alt="Docker Logo" width="25"> 

   - Link para download -> https://docs.docker.com/get-docker/

## Passos para configuração na VM VirtualBox
1. Clonando o repositório:
  ```
  git clone https://github.com/Vgalassi/Shell-c.git
  ```
2. Abrir o VS Code e executar o comando para inicializar a VM VirtualBox:
```
ssh <usuário>@127.0.0.1 -p2222
```
3. Utilize os comandos para criar uma nova pasta chamada “labs” no diretório de usuário
```
mkdir labs
sudo mount -t vboxsf labs labs/
```
- O primeiro “labs” no comando `sudo` é o nome do repositório e o segundo “labs/” é o caminho para a pasta recém criada. 
- O comando `sudo` é utilizado para executar uma instrução como “superusuário” root
(vagamente equivalente a Administrador) do sistema e exigirá a senha configurada ao abrir a VM pela primeira vez
4. Utilizar os comandos para compilar o código:
  ```
  gcc -c main.c
  gcc -o main main.o
  ```
5. Para rodar o programa dentro da VM, utilize o comando:
  ```
  ./main
  ```

## Passos para configuração no Docker
1. Siga o passo 1 da configuração na VM VirtualBox
2. No terminal do VS Code (ou em outra plataforma que desejar), acesse o diretório em que o projeto e execute os comandos `COM O DOCKER ABERTO`:
```
docker build -t ubuntu22-gcc . 
docker run -it -v .:/Shell-c ubuntu22-gcc 
```
- `docker build`: Inicia a construção de uma imagem Docker.
- `-t ubuntu22-gcc`: Nomeia a imagem como ubuntu22-gcc.
- `.`: Usa o Dockerfile do diretório atual como contexto para construir a imagem.
3. Siga os passos 4 e 5 da configuração na VM VirtualBox

## Conceitos envolvidos
- Chamadas de sistema
- Criação e gerenciamento de processos
- Gerenciamento de arquivos

## Funcionalidade básica
- Receber comandos e interpretá-los em um laço infinito.
- Comandos internos (built-in):
  - `exit`: Finaliza o Shell.
  - `cd <caminho>`: Muda o diretório de trabalho.
  - `path <caminho> [<caminho> <caminho> ...]`: Define caminho(s) para busca de executáveis.
- Programas externos:
  - O shell procura por executáveis nos caminhos definidos com o built-in `path`.
  - Executa o programa em um novo processo, passando os argumentos solicitados pelo usuário.
  - Imprime na tela o valor retornado pelo programa executado.
  - O shell trata condições de erro e imprime mensagens para elas.

## Outras funcionalidades
1. **Clone do `cat`**: Lê o conteúdo do arquivo no argumento e o escreve na saída padrão.
2. **Clone do `ls`**: Lista o conteúdo do diretório atual, suportando os parâmetros `-l` e `-a`.
  - `-l` -> Informa as permissões de diferentes arquivos de um determinado repositório.
  - `-a` -> Informa os arquivos ocultos dentro de um repositório.
3. **Redirecionamento de saída**: A saída dos comandos pode ser redirecionada para um arquivo utilizando o comando `>`.
4. **Comandos em paralelo**: Vários comandos separados por `&` são executados de forma concorrente, em processos separados.

## Funcionamento do arquivo batch
- Um arquivo batch é um script de texto simples que contém uma série de comandos a serem executados pelo interpretador - Shell.
#### Passos para funcionamento de um arquivo batch
1. Compile o código com o comando
```
gcc -c main.c
gcc -o main main.o
```
2. Execute o código com a adição do arquivo batch configurado - exemplo:
```
./main meu_arquivo.batch
```

## Observações 
- Os argumentos de comandos são separados por espaços (um ou mais).
- Não é preciso se preocupar com caracteres de escape ou argumentos com espaços entre aspas.

## Contribuição
Sinta-se à vontade para contribuir com este projeto. Para isso, siga os passos abaixo:

1. Faça um fork do repositório.
2. Crie uma nova branch (git checkout -b feature/nova-feature).
3. Faça commit das suas alterações (git commit -am 'Adiciona nova feature').
4. Faça push para a branch (git push origin feature/nova-feature).
5. Abra um Pull Request.

## Licença
- Este projeto está licenciado sob a MIT License.