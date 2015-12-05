#include "gui.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>

void render_line(int posx, int posy, const char * line)
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    glLoadIdentity();
    glRasterPos2i(posx, posy);
    int line_size = strlen(line);
    int i;
    for (i = 0; i < line_size; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, line[i]);
    }
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
}
void render_text(int posx, int posy, char * text, int win_width, int win_height)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluOrtho2D(0.0, win_width, 0.0, win_height);

    glColor4f(1, 1, 1, 1);

    int i = 0;
    char * line = strtok(text, "\n");
    while (line != NULL) {
        render_line(posx, posy - i*18, line);
        line = strtok(NULL, "\n");
        i++;
    }


    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glEnable(GL_LIGHTING);
}
