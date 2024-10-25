#include <GL/glut.h>
#include <cmath>
#include <ctime>

// Posição da nave
float naveX = 0.0f, naveY = 0.0f;

// Parâmetros dos teapots
struct Teapot {
    float x, y, scale;
    bool visible;
    float rotation;
};
Teapot teapots[4];

// Variáveis de controle de tempo e estado
clock_t startTime;
float tempoPassado;
const float tempoDesaparecimento = 10.0f; // Teapots desaparecem após 10 segundos

// Função para verificar colisão simples (baseada em distâncias)
bool Colidiu(float x1, float y1, float x2, float y2, float raio) {
    float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return dist < raio;
}

// Função para desenhar os teapots
void DesenhaTeapots() {
    for (int i = 0; i < 4; i++) {
        if (teapots[i].visible) {
            glPushMatrix();
            glTranslatef(teapots[i].x, teapots[i].y, 0.0f);
            glRotatef(teapots[i].rotation, 0.0f, 1.0f, 0.0f);
            glScalef(teapots[i].scale, teapots[i].scale, teapots[i].scale);
            glutWireTeapot(0.1f);
            glPopMatrix();
        }
    }
}

// Função para desenhar a nave
void DesenhaNave() {
    glPushMatrix();
    glTranslatef(naveX, naveY, 0.0f);
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex2f(0.0f, 0.1f);
    glVertex2f(-0.05f, -0.05f);
    glVertex2f(0.05f, -0.05f);
    glEnd();
    glPopMatrix();
}

// Função de atualização e animação
void AtualizaCena(int valor) {
    // Atualiza o tempo
    tempoPassado = (float)(clock() - startTime) / CLOCKS_PER_SEC;

    for (int i = 0; i < 4; i++) {
        // Rotaciona os teapots
        teapots[i].rotation += 2.0f;
        if (teapots[i].rotation > 360.0f) teapots[i].rotation -= 360.0f;

        // Diminui a escala dos teapots até sumirem em 10 segundos
        if (teapots[i].visible) {
            teapots[i].scale = 1.0f - (tempoPassado / tempoDesaparecimento);
            if (teapots[i].scale <= 0.0f) {
                teapots[i].visible = false;
            }
        }

        // Verifica colisão com a nave
        if (Colidiu(naveX, naveY, teapots[i].x, teapots[i].y, 0.15f) && teapots[i].visible) {
            teapots[i].visible = false; // Teapot desaparece ao colidir com a nave
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, AtualizaCena, 0); // Aproximadamente 60 FPS
}

// Função de controle de teclado
void Teclado(unsigned char tecla, int x, int y) {
    float velocidade = 0.05f;
    switch (tecla) {
    case 'w':
        naveY += velocidade;
        break;
    case 's':
        naveY -= velocidade;
        break;
    case 'a':
        naveX -= velocidade;
        break;
    case 'd':
        naveX += velocidade;
        break;
    }
}

// Função para desenhar a cena
void DesenhaCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Desenha a nave
    DesenhaNave();

    // Desenha os teapots
    DesenhaTeapots();

    glutSwapBuffers();
}

// Função de inicialização
void Inicializa() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo preto
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Projeção ortogonal 2D

    // Inicializa teapots em diferentes posições
    teapots[0] = { -0.5f, 0.5f, 1.0f, true, 0.0f };
    teapots[1] = { 0.5f, 0.5f, 1.0f, true, 0.0f };
    teapots[2] = { -0.5f, -0.5f, 1.0f, true, 0.0f };
    teapots[3] = { 0.5f, -0.5f, 1.0f, true, 0.0f };

    startTime = clock(); // Registra o tempo de início
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Nave VS Teapots");

    Inicializa();

    glutDisplayFunc(DesenhaCena);
    glutKeyboardFunc(Teclado);
    glutTimerFunc(16, AtualizaCena, 0); // Inicia a animação com ~60 FPS

    glutMainLoop();
    return 0;
}