#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

#define MAXLIN 33
#define MAXCOL 30

HANDLE hThread1, hThread2, hThread3;
HANDLE mutex;
DWORD ThreadID1;

int  
    pontos = 0,
    pontosTotais = 244,
    direcaoFantasma = 0;

int pacman[2] = {24, 15};

int fantasma[2][2] = {
    {12, 13},
    {12, 16}
};

int 
    velo = 125, 
    veloFant = 175,
    gameover = 0,
    gameWin = 0;

int fruta = 0;

int mapa[MAXLIN][MAXCOL] = {
    {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
    {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9},
    {9, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 0, 4, 4, 0, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 4, 9, 9, 4, 0, 4, 9, 9, 9, 4, 0, 4, 4, 0, 4, 9, 9, 9, 4, 0, 4, 9, 9, 4, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 0, 4, 4, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 4, 4, 0, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 2, 9},
    {9, 1, 1, 1, 1, 1, 1, 0, 4, 3, 3, 3, 3, 9, 4, 4, 9, 3, 3, 3, 3, 4, 0, 1, 1, 1, 1, 1, 1, 9},
    {9, 9, 9, 9, 9, 9, 2, 0, 4, 3, 3, 3, 3, 9, 3, 3, 9, 3, 3, 3, 3, 4, 0, 2, 9, 9, 9, 9, 9, 9},
    {9, 9, 9, 9, 9, 9, 2, 0, 4, 4, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 4, 4, 0, 2, 9, 9, 9, 9, 9, 9},
    {9, 9, 9, 9, 9, 9, 2, 0, 4, 4, 9, 1, 1, 1, 8, 8, 1, 1, 1, 9, 4, 4, 0, 2, 9, 9, 9, 9, 9, 9},
    {9, 1, 1, 1, 1, 1, 1, 0, 3, 3, 9, 2, 9, 9, 9, 9, 9, 9, 2, 9, 3, 3, 0, 1, 1, 1, 1, 1, 1, 9},
    {9, 9, 9, 9, 9, 9, 9, 0, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 0, 9, 9, 9, 9, 9, 9, 9},
    {9, 1, 1, 1, 1, 1, 1, 0, 3, 3, 9, 2, 9, 9, 9, 9, 9, 9, 2, 9, 3, 3, 0, 1, 1, 1, 1, 1, 1, 9},
    {9, 9, 9, 9, 9, 9, 2, 0, 4, 4, 9, 1, 1, 1, 1, 1, 1, 1, 1, 9, 4, 4, 0, 2, 9, 9, 9, 9, 9, 9},
    {9, 9, 9, 9, 9, 9, 2, 0, 4, 4, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 4, 4, 0, 2, 9, 9, 9, 9, 9, 9},
    {9, 9, 9, 9, 9, 9, 2, 0, 4, 4, 9, 3, 3, 3, 3, 3, 3, 3, 3, 9, 4, 4, 0, 2, 9, 9, 9, 9, 9, 9},
    {9, 1, 1, 1, 1, 1, 1, 0, 3, 3, 9, 3, 3, 3, 4, 4, 3, 3, 3, 9, 3, 3, 0, 1, 1, 1, 1, 1, 1, 9},
    {9, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 0, 4, 4, 0, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 3, 3, 4, 4, 0, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 3, 0, 4, 4, 3, 3, 0, 2, 9},
    {9, 2, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 2, 9},
    {9, 1, 1, 1, 0, 4, 4, 0, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 0, 4, 4, 0, 1, 1, 1, 9},
    {9, 1, 1, 1, 0, 3, 3, 0, 4, 4, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 4, 4, 0, 3, 3, 0, 1, 1, 1, 9},
    {9, 2, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 4, 4, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 2, 9},
    {9, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 9},
    {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9},
    {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9}
};

void printMap() {
    system("cls");
    
    int i, j;
    for(i=0;i<MAXLIN;i++) {
        for(j=0;j<MAXCOL;j++) {
            switch(mapa[i][j]) {
                case 0: printf(" %c ", 250); break;                 // quadradinho do ponto
                case 1: printf("%c%c%c", 205, 205, 205); break;     // barra dupla deitada
                case 2: printf(" %c ", 186); break;                 // barra dupla em pé
                case 3: printf("%c%c%c", 196, 196, 196); break;     // barra simples deitada
                case 4: printf(" %c ", 179); break;                 // barra simples em pé
                case 8: printf("%c%c%c", 176, 176, 176); break;     // light shade
                case 9: printf("   "); break;                       // espaço
            }
        }
        printf("\n");
    }
}

void gotoxy(int x, int y){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

void desenharPacman(){
    gotoxy(pacman[1] * 3, pacman[0]);
    printf(" %c ", 232);
}

void apagaPacman(){
    gotoxy(pacman[1] * 3, pacman[0]);
    printf("   ");
}

void desenharFruta(){
    gotoxy(14*3, 24);
    printf(" %c ", 234);
}

void apagaFruta(){
    gotoxy(24, 14);
    printf("   ");
}

void andarPacman(int direcao){
    WaitForSingleObject(mutex,INFINITE);
    apagaPacman();

    switch (direcao) {
        case 0: pacman[0]--; break;
        case 1: pacman[0]++; break;
        case 2: pacman[1]--; break;
        case 3: pacman[1]++; break;
    }

    desenharPacman();
    Sleep(velo);
    ReleaseMutex(mutex);
}

void apagaFantasma(int fant[2]){
    gotoxy(fant[1] * 3,fant[0]);
    switch(mapa[fant[0]][fant[1]]) {
        case 0: printf(" %c ", 250); break;                 // quadradinho do ponto
        case 8: printf("%c%c%c", 176, 176, 176); break;     // light shade
        case 9: printf("   "); break;                       // espaço
    }
}

void desenharFantasma(int fant[2]){
    gotoxy(fant[1] * 3, fant[0]);
    printf(" %c ", 219);
}

void andarFantasma(int direcao, int i){
    WaitForSingleObject(mutex,INFINITE);
    apagaFantasma(fantasma[i]);

    switch (direcao) {
        case 0: fantasma[i][0]--; break;
        case 1: fantasma[i][0]++; break;
        case 2: fantasma[i][1]--; break;
        case 3: fantasma[i][1]++; break;
    }

    desenharFantasma(fantasma[i]);
    ReleaseMutex(mutex);
    Sleep(veloFant);
}

void configurartela(void) {
    CONSOLE_CURSOR_INFO info;
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);

    system("color 07");

    SetConsoleTitle("Pacman em C");

    SMALL_RECT windowSize = {0, 0, (MAXCOL*3) + 1, MAXLIN + 1};
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    COORD bufferSize = {(MAXCOL*3) + 1, MAXLIN + 1};
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(wHnd, &info);

    printMap();
    desenharFantasma(fantasma[0]);
    desenharFantasma(fantasma[1]);
    desenharPacman();
}

int verificarPonto(){
    int i, j, temPonto = 0;

    for(i = 0; i < MAXLIN; i++) {
        for(j = 0; j < MAXCOL; j++) {
            if(mapa[i][j] == 0){
                temPonto = 1;
                break;
            }
        }
    }
    
    return temPonto;
}

int verificarPosicao(int x, int y){
    WaitForSingleObject(mutex,INFINITE);
    int verify = 0;

    if(x == 24 && y == 14 && fruta){
        pontos += 30;
        fruta = 0;
        apagaFruta();
        if(!pontosTotais)
            gameWin = 1;
    }

    switch (mapa[x][y]) {
        case 0: 
            pontos++; 
            pontosTotais--;
            mapa[x][y] = 9; 
            if((pontos % 80) == 0){
                desenharFruta();
                fruta = 1;
            }
            if(!pontosTotais && !fruta)
                gameWin = 1;
            verify = 1; 
            break;
        case 9: verify = 1; break;
        default: verify = 0; break;
    }

    if(x == fantasma[0][0] && y == fantasma[0][1])
        { gameover = 1; verify = 1; }
    
    if(x == fantasma[1][0] && y == fantasma[1][1])
        { gameover = 1; verify = 1; }

    ReleaseMutex(mutex);
    return verify;
}

DWORD WINAPI moverPacman(LPVOID lpParam) {
    int tecla = 0;

    while(gameover == 0 && gameWin == 0){
        WaitForSingleObject(mutex,INFINITE);
        gotoxy(0, MAXLIN+1);
        printf("Pontos: %d\t",pontos);
        ReleaseMutex(mutex);
        
        if(kbhit())
            tecla=getch();

        // pra cima
        if(tecla=='w' || tecla=='W' || tecla==72){
            if(verificarPosicao(pacman[0] - 1, pacman[1])){
                andarPacman(0);
            }
        }

        // pra baixo                  
        if(tecla=='s' || tecla=='S' || tecla==80){
            if(verificarPosicao(pacman[0] + 1, pacman[1])){
                andarPacman(1);
            }
        }

        // pra esquerda
        if(tecla=='a' || tecla=='A' || tecla==75){
            if(verificarPosicao(pacman[0], pacman[1] - 1)){
                andarPacman(2);
            }
        }

        // pra direita         
        if(tecla=='d' || tecla=='D' || tecla==77){
            if(verificarPosicao(pacman[0], pacman[1] + 1)){
                andarPacman(3);
            }
        }
    }
        
    return 0;
}

// Verifica se a posição (x, y) está livre para o fantasma 'index'
int podeMoverFantasma(int x, int y, int fantasma_index){
    // 1. Verifica limites do mapa (O BUG PRINCIPAL)
    if (x < 0 || x >= MAXLIN || y < 0 || y >= MAXCOL) 
        return 0;

    WaitForSingleObject(mutex, INFINITE);

    // 2. Verifica se o destino é um caminho válido (0=ponto, 9=vazio)
    switch (mapa[x][y]) {
        case 0:
        case 9:
            // É um caminho válido, continua a verificação
            break;
        default:
            // É uma parede (1, 2, 3, 4, 8, etc)
            ReleaseMutex(mutex);
            return 0; 
    }

    // 3. Verifica colisão com Pac-Man
    if (x == pacman[0] && y == pacman[1]) {
        gameover = 1;
        ReleaseMutex(mutex);
        return 1; // É válido (e fatal)
    }

    // 4. Verifica colisão com o OUTRO fantasma
    int outro_fantasma = (fantasma_index == 0) ? 1 : 0;
    if (x == fantasma[outro_fantasma][0] && y == fantasma[outro_fantasma][1]) {
        ReleaseMutex(mutex);
        return 0; // Bloqueado pelo outro fantasma
    }

    // Se passou por tudo, o movimento é válido
    ReleaseMutex(mutex);
    return 1;
}

DWORD WINAPI cacarPacman(LPVOID lpParam) {
    int i = (int)lpParam; // i == 0 (fantasma inteligente)
    int direcaoAtual = 3; // 0=Cima, 1=Baixo, 2=Esquerda, 3=Direita

    // Pequeno delay inicial para dessincronizar os fantasmas
    Sleep(100); 

    while (gameover == 0 && gameWin == 0) {
        
        // Pega a posição ATUAL antes de qualquer cálculo
        int xAtual = fantasma[i][0];
        int yAtual = fantasma[i][1];

        int pode[4];
        // --- 1. Verificar quais movimentos são possíveis ---
        pode[0] = podeMoverFantasma(xAtual - 1, yAtual, i); // Cima
        pode[1] = podeMoverFantasma(xAtual + 1, yAtual, i); // Baixo
        pode[2] = podeMoverFantasma(xAtual, yAtual - 1, i); // Esquerda
        pode[3] = podeMoverFantasma(xAtual, yAtual + 1, i); // Direita

        // --- 2. Decidir se precisa de uma nova direção ---
        bool precisaDecidir = false;

        // Se a direção atual é inválida (bateu na parede)
        if (direcaoAtual == 0 && !pode[0]) precisaDecidir = true;
        else if (direcaoAtual == 1 && !pode[1]) precisaDecidir = true;
        else if (direcaoAtual == 2 && !pode[2]) precisaDecidir = true;
        else if (direcaoAtual == 3 && !pode[3]) precisaDecidir = true;

        // Calcula a direção reversa (de onde viemos)
        int dirReversa = -1;
        if (direcaoAtual == 0) dirReversa = 1; // Veio de Baixo
        if (direcaoAtual == 1) dirReversa = 0; // Veio de Cima
        if (direcaoAtual == 2) dirReversa = 3; // Veio da Direita
        if (direcaoAtual == 3) dirReversa = 2; // Veio da Esquerda

        // É uma junção? (ignora o caminho de onde viemos)
        int numOpcoes = 0;
        if (pode[0] && 0 != dirReversa) numOpcoes++;
        if (pode[1] && 1 != dirReversa) numOpcoes++;
        if (pode[2] && 2 != dirReversa) numOpcoes++;
        if (pode[3] && 3 != dirReversa) numOpcoes++;

        if (numOpcoes > 1) {
             precisaDecidir = true; // Chegou numa junção com >1 escolha
        }
        
        // --- 3. Lógica de Decisão (A parte "Inteligente") ---
        if (precisaDecidir) {
            int melhorDir = -1;
            int menorDist = 99999;
            
            WaitForSingleObject(mutex, INFINITE); 
            int pacmanX = pacman[0];
            int pacmanY = pacman[1];
            ReleaseMutex(mutex);

            // *** INÍCIO DA CORREÇÃO: LÓGICA DE PRIORIDADE ***
            // Testamos na ordem de prioridade: Cima > Esquerda > Baixo > Direita
            // Como usamos "<" (menor que), a primeira direção que encontrar a 
            // menor distância será a escolhida, resolvendo o empate.

            // Prioridade 1: Testar Cima (0)
            if (pode[0] && 0 != dirReversa) {
                int dist = abs((xAtual - 1) - pacmanX) + abs(yAtual - pacmanY); 
                if (dist < menorDist) {
                    menorDist = dist;
                    melhorDir = 0;
                }
            }

            // Prioridade 2: Testar Esquerda (2)
            if (pode[2] && 2 != dirReversa) {
                int dist = abs(xAtual - pacmanX) + abs((yAtual - 1) - pacmanY);
                if (dist < menorDist) {
                    menorDist = dist;
                    melhorDir = 2;
                }
            }

            // Prioridade 3: Testar Baixo (1)
            if (pode[1] && 1 != dirReversa) {
                int dist = abs((xAtual + 1) - pacmanX) + abs(yAtual - pacmanY);
                if (dist < menorDist) {
                    menorDist = dist;
                    melhorDir = 1;
                }
            }

            // Prioridade 4: Testar Direita (3)
            if (pode[3] && 3 != dirReversa) {
                int dist = abs(xAtual - pacmanX) + abs((yAtual + 1) - pacmanY);
                if (dist < menorDist) {
                    menorDist = dist;
                    melhorDir = 3;
                }
            }
            // *** FIM DA CORREÇÃO ***


            // Se não achou melhor direção (beco sem saída), a única opção é voltar
            if (melhorDir == -1) {
                // Só pode voltar se a direção reversa for válida
                if (dirReversa == 0 && pode[0]) melhorDir = 0;
                else if (dirReversa == 1 && pode[1]) melhorDir = 1;
                else if (dirReversa == 2 && pode[2]) melhorDir = 2;
                else if (dirReversa == 3 && pode[3]) melhorDir = 3;
                else {
                    // Preso! (Não deve acontecer, mas por segurança)
                    // Fica parado ou escolhe a primeira opção válida
                    if(pode[0]) melhorDir = 0;
                    else if(pode[1]) melhorDir = 1;
                    else if(pode[2]) melhorDir = 2;
                    else if(pode[3]) melhorDir = 3;
                    else melhorDir = -1; // Totalmente preso
                }
            }
            
            direcaoAtual = melhorDir;
        }

        // --- 4. Mover o fantasma ---
        // A função andarFantasma já tem o Sleep(veloFant)
        // A 'direcaoAtual' agora é garantida como válida (ou o fantasma está preso)
        if(direcaoAtual != -1) {
             andarFantasma(direcaoAtual, i);
        } else {
             // Está preso e não pode nem voltar. Apenas dorme.
             Sleep(veloFant);
        }
    }

    return 0;
}

DWORD WINAPI moverFantasma(LPVOID lpParam) {
    srand(time(NULL));
    
    int i = 1;

    int numero = rand() % 4;

    while(gameover == 0 && gameWin == 0){
        switch (numero) {
            case 0: // pra cima
                if(podeMoverFantasma(fantasma[i][0] - 1, fantasma[i][1], i))
                    andarFantasma(0, i);
                else
                    while (numero == 1 || numero == 0)
                        numero = rand() % 4;
                break;
            case 1: // pra baixo
                if(podeMoverFantasma(fantasma[i][0] + 1, fantasma[i][1], i))
                    andarFantasma(1, i);
                else
                    while (numero == 1 || numero == 0)
                        numero = rand() % 4;
                break;
            case 2: // pra esquerda
                if(podeMoverFantasma(fantasma[i][0], fantasma[i][1] - 1, i))
                    andarFantasma(2, i);
                else
                    while (numero == 2 || numero == 3)
                        numero = rand() % 4;
                break;
            case 3: // pra direita
                if(podeMoverFantasma(fantasma[i][0], fantasma[i][1] + 1, i))
                    andarFantasma(3, i);
                else
                    while (numero == 2 || numero == 3)
                        numero = rand() % 4;
                break;
        }
    }

    return 0;
}

void telaBranca() {
    for (int y = 0; y < MAXLIN; y++) {
        for (int x = 0; x < (MAXCOL * 3); x++) {
            gotoxy(x, y);
            printf("%c", 219);
        }
    }
}

void telaGameOver(){
    system("cls");
    telaBranca();
    Sleep(10);
    system("cls");
    telaBranca();
    Sleep(10);
    system("cls");
    telaBranca();
    
    Sleep(3000);
    system("cls");

    gotoxy((MAXCOL * 3) / 2 - 5, MAXLIN / 2);
    printf("GAME OVER");

    Sleep(5000);
}

void telaGameWin(){
    system("cls");
    telaBranca();
    Sleep(10);
    system("cls");
    telaBranca();
    Sleep(10);
    system("cls");
    telaBranca();
    
    Sleep(3000);
    system("cls");

    gotoxy((MAXCOL * 3) / 2 - 5, MAXLIN / 2 - 1);
    printf("GAME WIN");
    gotoxy((MAXCOL * 3) / 2 - 5, MAXLIN / 2);
    printf("PONTOS: %d", pontos);

    Sleep(5000);
}

void iniciarJogo(){
    configurartela();

    mutex = CreateMutex(NULL, FALSE, NULL);

    hThread1 = CreateThread(NULL, 0, moverPacman, NULL, 0, &ThreadID1);
    hThread2 = CreateThread(NULL, 0, cacarPacman, (LPVOID)0, 0, NULL);
    hThread3 = CreateThread(NULL, 0, moverFantasma, (LPVOID)1, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);
    CloseHandle(mutex);
}

void finalizarJogo(){
    system("cls");

    if (gameover)
        telaGameOver();
    else if (gameWin)
        telaGameWin();
}

int main() {
    iniciarJogo();
    finalizarJogo();

    return 0;
}