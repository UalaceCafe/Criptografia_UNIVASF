# Atividade 5 - AES

## Descrição

Este programa implementa na linguagem _C_ uma interface em linha de comando para utilização do AES no modo CBC. A biblioteca `tiny-AES-c`, que implementa o AES, pode ser encontrada no diretório `lib/`.

## Como compilar

### Com o _make_:
  No diretório principal do projeto, execute o comando:
  ```bash
  make
  ```

### Sem o _make_:
  No diretório principal do projeto, execute o comando:
  ```bash
  gcc -std=c99 -O2 src/main.c lib/aes.c -DCBC=1 -o bin/aes
  ```

Um executável será criado em `bin/aes`.

## Como usar

Com o programa compilado, execute o comando:

```bash
.\bin\aes --[enc|dec]=<input> --out=<output> --key=<key>
```

Onde:
* `<input>` é o arquivo de entrada. As flags `enc` ou `dec` determinam se o arquivo deve ser criptografado ou decriptografado, respectivamente. Não é possível usar ambas as flags ao mesmo tempo.
* `<output>` é o arquivo de saída.
* `<key>` deve ser uma string de 16 caracteres (128 bits) ASCII, que será usada como chave de criptografia.

Todos os caminhos para os arquivos são relativos ao diretório onde o programa está sendo executado.

O projeto já contém um arquivo criptografado de exemplo (`in.txt`) para entrada, que foi criptografado com a chave `5HBDcSO2lqClWJbs`. Para decriptografar este arquivo, por exemplo, execute o comando:

```bash
.\bin\aes --dec=in.txt --out=out.txt --key=5HBDcSO2lqClWJbs
```

Um arquivo `out.txt` será gerado no diretório atual.

## Créditos

* kokke: [tiny-AES-c](https://github.com/kokke/tiny-AES-c).
