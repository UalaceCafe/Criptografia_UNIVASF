# Atividade 3 - Cifra de Hill

## Descrição

Este programa implementa na linguagem _Ruby_ o algorithmo de criptografia da Cifra de Hill.

## Como usar

Com um interpretador _Ruby_ instalado, execute o comando:

```bash
hill.rb --[enc|dec]=input --out=output --key=key
```

Onde:
* `input` é o arquivo de entrada. As flags `enc` ou `dec` determinam se o arquivo deve ser criptografado ou decriptografado, respectivamente. Não é possível usar ambas as flags ao mesmo tempo.
* `output` é o arquivo de saída. Se esta flag for omitida, o arquivo de saída terá o mesmo nome do arquivo de entrada, com a adição do sufixo `_out`.
* `key` é o arquivo contendo a chave de criptografia. Deve ser uma matriz quadrada de números inteiros, onde cada elemento é separado por um espaço e cada linha é separada por uma quebra de linha.

Todos os caminhos para os arquivos são relativos ao diretório onde o programa está sendo executado.

O programa já contém um arquivo de exemplo (`plaintext.txt`) para entrada, bem como uma chave de exemplo (`key.txt`).

## Características

* Os arquivos de entrada e saída suportam todos os caracteres ASCII, incluindo espaços e quebras de linha, além dos caracteres de controle e não-imprimíveis.
* A chave pode ser uma matriz quadrada de qualquer ordem, desde que a mesma seja invertível e que o determinante seja coprimo com 128 (número de caracteres ASCII).
