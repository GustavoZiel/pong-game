# [Pong Game](hhttps://github.com/GustavoZiel)
Desenvolvido por:
- Gustavo Gabriel Ribeiro - 13672683
- Artur De Vlieger Lima - 13671574
- João Pedro Soares de Azevedo Calixto - 13732011
- Lucas de Souza Brandão - 13695021

## Como instalar

O jogo foi totalmente desenvolvido em C, sem o advento de qualquer recurso referente a outra linguagem, ele utiliza das sequintes bibliotecas:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
```

Cujas as quais, exceto pela biblioteca **ncurses**, estão todas já incluídas em C nativo.

A biblioteca **ncurses** foi utilizada para o desenvolvimento da interface gráfica do jogo, veja seu [guia de uso](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/) para mais informações.

### Como instalar ncurses:

> É recomendado tentar executar o programa (exemplificado na seção **Como executar**) para verificar se **ncurses** já está instalada. Um aviso do terminal em conjunto com instruções para instalá-la, específicas ao seus sistema, deve aparecer caso não esteja. Você pode também prosseguir com os comandos a seguir

Em sistemas baseados em Debian (como o Ubuntu):

```sh
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Em sistemas baseados em Red Hat (como o Fedora):

```sh
sudo dnf install ncurses-devel
```

## Como executar

Para rodar o jogo, deve-se executar as instruções :
```sh
make all # Compilar arquivos e gerar executável
make run # Executar jogo
ou
make all run
```

Caso alguma mudança seja feita nos arquivos do jogo, como mudar a dificuldade ou tamanho da arena (mencionado na seção **Como personalizar**), é necessário recompilar o programa.

```sh
make clean # Excluir arquivos gerados
make all run # Recompilar e executar
ou 
make clean all run
```
## Como jogar

**Menu** é controlado pelas **arrow keys** : <kbd>↑</kbd> <kbd>↓</kbd> <br>
**Player 1** controla a raquete da esquerda com <kbd>w</kbd> <kbd>s</kbd> <br>
**Player 2** controla a raquete da direita com <kbd>↑</kbd> <kbd>↓</kbd> 

**No más, jogue PONG !!!**

## Como personalizar

É possível alterar a dificuldade do jogo descomentando e comentando as partes necessárias no arquivo **menu.h**.
<br><br>
É possível criar configurações personalizadas alterando como desejar as definições: **SPEED TAM_X TAM_Y TAM_RAQUETE** , porém se atente aos valores fornecidos, bugs podem ser gerados caso haja inconsistências.

## Descrições dos semáforos e threads utilizados

### Threads

No total, foram implementadas **três threads** para melhorar a eficiência e a responsividade do jogo. 

Duas dessas threads são dedicadas ao controle das raquetes, uma para a raquete do player 1 e outra para a raquete do player 2. A terceira thread é responsável pelo movimento da bola. 

A razão para essa abordagem é que as operações envolvendo as raquetes e a bola são independentes entre si e podem ocorrer simultaneamente, otimizando assim o desempenho do jogo.

Código desenvolvido em **logic.c**:
```c
pthread_t p, q, r;
pthread_create(&p, NULL, move_raquete_wrapper, (void *)(setup->jogadores[0]));
pthread_create(&q, NULL, move_raquete_wrapper, (void *)(setup->jogadores[1]));
pthread_create(&r, NULL, move_ball_wrapper, (void *)(setup));
```
### Semáforos

Os semáforos foram utilizados para controlar a ação de atualizar a tela, ou seja, após qualquer um dos 3 objetos presentes no jogo - primeira raquete, segunda raquete e bola - se moverem, a tela precisa ser atualizada para que o movimento seja percebido pelos jogadores. 

Essa ação é realizada pela seguinte função presente na biblioteca ncurses: 

```c++
wrefresh(window);
```

Após a incorporação de mutexes, o código ficou da seguinte forma:

```c++
pthread_mutex_lock(&mutex);
wrefresh(window);
pthread_mutex_unlock(&mutex);
```

Dessa forma, condições de corrida, antes presentes e que poderiam ocasionar glitches de interface, são mitigadas pelo uso dos semáforos. 

Assista ao vídeo abaixo para visualizar como se apresentam os possíveis glitches e como são evitados.

## Vídeo de apresentação

<!-- Link aqui -->

## Imagens do jogo

### Menu
![Menu](Images/Menu.png)

### Start
![Start](Images/Start.png)

### Play
![Play](Images/Play.png)

### End
![End](Images/End.png)
