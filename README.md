BlackJack

MEEC Prog 2015/2016 1º Semestre

Authors: André Agostinho 84001, Miguel Freire 84145

Programa elaborado para a entrega final do projeto da cadeira de Programação

========================
INSTRUÇÔES DE UTILIZAÇÂO
========================
*O projeto deverá compilar com o comando "make" executado no diretorio base do projeto. O comando executado é o seguinte: "gcc -g -Wall -pedantic -std=c99 -I/usr/local/include -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image ./src/*.c -o blackjack"

*O executável será gerado neste diretório com o nome blackjack. 

*O ficheiro de configuração dos EA (matrizes) tem o nome EA.txt e encontra-se na pasta config.

*Os ficheiros de configuração do jogo fornecidos pelo professor encontram-se na pasta config, mas poderá obviamente usar quaisquer outros ficheiros.

*O programa poderá ser executado com o comando: "./blackjack ./config/ficheiro1.txt ./config/EA.txt".

========================
OBSERVAÇÔES
========================
Todos os ficheiros de código fonte encontram-se na pasta src.
Todos os ficheiros de imagem (e fonte) encontram-se na pasta img.
Na pasta config encontram-se o ficheiro de configuração dos EA e os 3 ficheiros de configuração do jogo fornecidos pelo professor.

Ao invés de apresentar ao pé dos jogadores BU ou BJ quando os jogadores se encontram BUSTED ou têm um BLACKJACK, foram utilizados uns overlays que documentam nao só estes dois acontecimentos de um jogador como também outros estados do mesmo.

Ao invés de as teclas seta para cima e seta para baixo controlarem o delay dos EA estas controlam a velocidade dos mesmos, ou seja, ao aumentar a velocidade o delay deverá diminuir. O delay inicial deverá no entanto ser se 1s como pedido.

