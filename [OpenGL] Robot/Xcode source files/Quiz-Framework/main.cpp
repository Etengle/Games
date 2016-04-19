#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "texture_loader.h"

#define MENU_RUN_START 1
#define MENU_WALK_START 2
#define MENU_TIMER_STOP 5
#define MENU_CRY_START 4

#define MENU_TIMER_START 6
#define MENU_EXIT 3

const GLfloat tri_v1[3] = {-0.5f, -0.4f, 0.0f};
const GLfloat tri_v2[3] = { 0.5f, -0.4f, 0.0f};
const GLfloat tri_v3[3] = { 0.0f,  0.6f, 0.0f};

GLfloat light_pos[] = {-3.00, 5.00, 5.00, 1.00};
GLfloat light_diffuse[] = {1, 1, 1, 1.00};
GLfloat light_ambient[] = {0, 0, 0, 1.00};
GLfloat light_specular[] = {1.00, 1.00, 1.00, 1.00};
GLfloat a = 0.8;

GLfloat light_pos2[] = {-3.00, 5.00, 1.00, 1.00};
GLfloat spot_direction[] = {3.00, -5.00, -1.00};
GLint spot_exponent = 60, spot_cutoff = 50;


GLfloat diffuse[][4] = {{.7, .7, .7, 1.00}, {1,.8,.2,1.00}, {.8,.3,.1,1.00}, {.2,.2,1.0,1}};
GLfloat ambient[][4] = {{.8, .8, .8, 1.00}, {1,.5,0,1},{1,.3,.2,1},{.3,.3,.9,1}};
GLfloat specular[][4] = {{.6, .6, .6, 1.00}, {1,.5,0,1},{1,.3,.2,1},{.0,.0,.9,1}};
GLfloat shine[] = {50.0, 20, 10, 50};

float timer_cnt = 0;
bool timer_enabled = false;
unsigned int timer_speed = 8;

GLuint texture_jpg;
GLuint texture_png;
int head_angle = 0;
float body_rotate = 0.5f, up = 0.0f;
int run2 = 1;
int angle = 10, flag = 0, swing_angle = 0, swing_back = 0, swing_angle2 = 0;
bool stretch = false, run = false, pa = false, cry = false, light = false, spot = false;
int counter = 0, countc = 0;
int cline = 0, speed = 5;
int spin = 10;
float view_r = 2.5, view_angle = 1.3, view_angle2 = .5;

// Print OpenGL context related information.
void dumpInfo(void)
{
    //printf("Vendor: %s\n", glGetString (GL_VENDOR));
    //printf("Renderer: %s\n", glGetString (GL_RENDERER));
    //printf("Version: %s\n", glGetString (GL_VERSION));
    //printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void walkRun(int b){
    run = b;
    if (run) {
        cline = 10;
        speed = 8;
        run2 = 2;
    }
    else {
        cline = 0;
        speed = 5;
        up = 0;
        pa = false;
        run2 = 1;
    }
    cry = false;
    if (b >= 2){
        cline = 10;
        speed = 12;
        run2 = 2;
        cry = true;
    }
}

void setMaterial(int id){
    //if (!id||!spot){
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[id]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[id]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular[id]);
    glMaterialf(GL_FRONT, GL_SHININESS, shine[id]);
    //}
}

void runTransformation(int br){
    glRotatef(cline,1.0f,0.0f,0.0f);
    glRotatef(br*body_rotate*swing_angle,0.0f,-1.0f,0.0f);
    glTranslatef(0, up, 0.0f);
    if (run) glTranslatef(0, 0.0f, 0.05f);
}

void render_triangle(){
    glBegin(GL_TRIANGLES);
    {
        glColor3ub(timer_cnt, 0, 255 - timer_cnt);
        glVertex3fv(tri_v1);
        glColor3ub(255, timer_cnt, 255 - timer_cnt);
        glVertex3fv(tri_v2);
        glColor3ub(255 - timer_cnt, 0, timer_cnt);
        glVertex3fv(tri_v3);
    }
    glEnd();
}

void render_nthu(){
    setMaterial(0);
    glPushMatrix();
    {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_png);
    runTransformation(1);
    glBegin(GL_TRIANGLES);
    {
        
        glColor3f(0.8f, 0.8f, 0.8f);
        float l = 0.15f, r = 0.15f, z = 0.16;
        glTexCoord2f(0, 0); glVertex3f(-l, -r, z);
        glTexCoord2f(1, 0); glVertex3f(l, -r, z);
        glTexCoord2f(1, 1); glVertex3f(l, r, z);
        glTexCoord2f(1, 1); glVertex3f(l, r, z);
        glTexCoord2f(0, 1); glVertex3f(-l, r, z);
        glTexCoord2f(0, 0); glVertex3f(-l, -r, z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

void render_torso(){
    
    setMaterial(0);
    glPushMatrix();
    {
        
        glColor3f(0.8f,0.8f,0.8f);
        runTransformation(1);
        glTranslatef(0, 0.32f, 0.00f);
        //glScalef(1.5f,2.0f,1.0f);
        /*
        glRotatef(90,1.0f,0.0f,0.0f);
        GLUquadricObj *p;
        p = gluNewQuadric();
        //if (!run) gluQuadricDrawStyle(p, GLU_FILL);
        gluQuadricDrawStyle(p, GLU_LINE);
        gluCylinder(p, 0.20f, 0.20f, 0.60f, 50, 50);
         */
        //glColor3f(0.5f,0.5f,0.5f);
        glTranslatef(0, -0.32f, 0);
        glScalef(2.0f,3.0f,1.5f);
        glutSolidCube(0.2f);
    }
    glPopMatrix();
}

void render_head(){
    
    setMaterial(0);
    glPushMatrix();
    {
        
        runTransformation(1);
        // 脖子
        glTranslatef(0, 0.32f, 0);
        glColor3f(1.0f,0.8f,0.0f);
        //glutSolidSphere(0.05,50,50);
        
        glPushMatrix();
        {
        glTranslatef(0, -0.02f, 0);
        glRotatef(-90,1.0f,0.0f,0.0f);
        GLUquadricObj *p;
        p = gluNewQuadric();
        //if (!run) gluQuadricDrawStyle(p, GLU_FILL);
        gluQuadricDrawStyle(p, GLU_LINE);
        gluCylinder(p, 0.05f, 0.05f, 0.07f, 50, 50);
        }
        glPopMatrix();
            
        glRotatef(-swing_angle,0,1.0f,0.0f);
        // 頭
        glTranslatef(0, 0.15f, 0);
        glPushMatrix();
        {
        glColor3f(0.8f,0.8f,0.8f);
        glScalef(1.6f,1.0f,1.0f);
        glutSolidCube(0.2f);
        }
        glPopMatrix();
        // eyes
        glColor3f(0.5f,0.5f,0.5f);
        glPushMatrix();
        {
        
            if (cry){
                glColor3f(0.5f,0.5f,0.8f);
                setMaterial(3);
            }
            glPushMatrix();
            {
                glTranslatef(0.08f, 0, 0.1f);
                glScalef(0.8f,0.8f,0.1f);
                glutSolidSphere(0.05, 50, 50);
            }
            glPopMatrix();
            glPushMatrix();
            {
                glTranslatef(-0.08f, 0, 0.1f);
                glScalef(0.8f,0.8f,0.1f);
                glutSolidSphere(0.05, 50, 50);
                
            }
            glPopMatrix();
            // tears
            if (cry){
                setMaterial(3);
                glColor3f(0.5f,0.5f,0.8f);
                glPushMatrix();
                {
                    glTranslatef(0.09f, -0.03f, 0.1f);
                    glScalef(0.3f,0.3f,0.1f);
                    glutSolidSphere(0.05, 50, 50);
                }
                glPopMatrix();
                glPushMatrix();
                {
                    glTranslatef(-0.09f, -0.03f, 0.1f);
                    glScalef(0.3f,0.3f,0.1f);
                    glutSolidSphere(0.05, 50, 50);
                    
                }
                glPopMatrix();
                glPushMatrix();
                {
                    glTranslatef(0.1f, -0.05f, 0.1f);
                    glScalef(0.3f,0.3f,0.1f);
                    glutSolidSphere(0.05, 50, 50);
                    
                }
                glPopMatrix();
                glPushMatrix();
                {
                    glTranslatef(-0.1f, -0.05f, 0.1f);
                    glScalef(0.3f,0.3f,0.1f);
                    glutSolidSphere(0.05, 50, 50);
                    
                }
                glPopMatrix();
                glPushMatrix();
                {
                    glTranslatef(0.11f, -0.07f, 0.1f);
                    glScalef(0.3f,0.3f,0.1f);
                    glutSolidSphere(0.05, 50, 50);
                    
                }
                glPopMatrix();
                glPushMatrix();
                {
                    glTranslatef(-0.11f, -0.07f, 0.1f);
                    glScalef(0.3f,0.3f,0.1f);
                    glutSolidSphere(0.05, 50, 50);
                    
                }
                glPopMatrix();
            }
            
        }
        glPopMatrix();
        setMaterial(0);
        // mouth
        glPushMatrix();
        {
            glTranslatef(0, -0.05f, 0.11f);
            glColor3f(0.5,0.5,0.5);
            if (!run) {
                //glTranslatef(0, -0.01f, 0.0f);
                glRotatef(-180,0.0f,0.0f,1.0f);
            }
            
            glScalef(0.08f,0.08f,0.1f);
            
            glBegin(GL_TRIANGLES);
            {
                glVertex3fv(tri_v1);
                glVertex3fv(tri_v2);
                glVertex3fv(tri_v3);
            }
            glEnd();
        }
        glPopMatrix();
        setMaterial(2);
        // cones
        glPushMatrix();
        {
            glTranslatef(0.08f, 0.1f, 0);
            glColor3f(1.0f,0.3f,0.3f);
            glRotatef(90,-1.0f,0.0f,0.0f);
            glutSolidCone(0.05f, 0.2f, 50, 50);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(-0.08f, 0.1f, 0);
            glColor3f(1.0f,0.3f,0.3f);
            glRotatef(90,-1.0f,0.0f,0.0f);
            glutSolidCone(0.05f, 0.2f, 50, 50);
        }
        glPopMatrix();
        
    }
    glPopMatrix();
}

void render_legs(int dir){
    setMaterial(1);
    glPushMatrix();
    {
    // 大腿關節
    runTransformation(0);
    glColor3f(1.0f,0.8f,0.0f);
    glTranslatef(-0.10f*dir, -0.32f, 0);
    glutSolidSphere(0.05,50,50);
      
        glRotatef(swing_angle,1.0f*dir,0.0f,0.0f);
        // 大腿
        glPushMatrix();
        {
            glColor3f(1.0f,0.9f,0.0f);
            glTranslatef(0, -0.23f, 0);
            glScalef(1.1f,3.2f,1.1f);
            glutSolidSphere(0.08,50,50);
        }
        glPopMatrix();
        // 膝關節
        glColor3f(1.0f,0.8f,0.0f);
        glTranslatef(0, -0.48f, 0);
        glutSolidSphere(0.05,50,50);
        
        glRotatef(30*run2,1.0f,0.0f,0.0f);
        glRotatef(swing_angle,1.0f*dir,0.0f,0.0f);
        glPushMatrix();
        {
            glColor3f(1.0f,0.9f,0.0f);
            glTranslatef(0, -0.20f, 0);
            glScalef(1.0f,3.0f,1.0f);
            glutSolidSphere(0.08,50,50);
        }
        glPopMatrix();
        
        // 腳掌
        setMaterial(2);
        glRotatef(10*run2,1.0f,0.0f,0.0f);
        //glRotatef(swing_angle,1.0f*dir,0.0f,0.0f);
        glPushMatrix();
        {
            glColor3f(1.0f,0.3f,0.3f);
            glTranslatef(0, -0.42f, 0.1f);
            if (run) glTranslatef(0, 0, 0.1f);
            glScalef(2.0f,1.0f,5.0f);
            glutSolidCube(0.05);
        }
        glPopMatrix();

    }
    glPopMatrix();

}

void render_arms(int dir){
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setMaterial(1);
    glPushMatrix();
    {
    
    runTransformation(1);
    // render 臂關節
    glColor3f(1.0f,0.8f,0.0f);
    glTranslatef(-0.22f*dir, 0.25f, 0);
    glutSolidSphere(0.08,50,50);
        
    glRotatef(90*dir,0.0f,0.0f,1.0f);
    glRotatef(swing_angle,0.0f,1.0f,0.0f);
    
    // render 肘關節＋手臂
    glTranslatef(-0.45f*dir, 0.04, 0);
    
    glPushMatrix();
    {
        glColor3f(1.0f,0.8f,0.0f);
        glutSolidSphere(0.05,50,50);
        
    
        glColor3f(1.0f,0.9f,0.0f);
        glTranslatef(0.2f*dir, 0, 0);
        glScalef(5.0f,1.2f,1.2f);
        glutSolidSphere(0.05,50,50);
    }
    glPopMatrix();
    glPushMatrix();
    {
        //glRotatef(swing_angle*0.9*dir,0.0f,1.0f,0.0f);
        //glRotatef(-50, 0.0f,0.0f,1.0f);
        /*
        if (!stretch) glRotatef(-10*dir,0.0f,0.0f,1.0f);
        else
        */
        //glRotatef(angle*dir,0.0f,0.0f,1.0f);
        glRotatef(30*run2, 0.0f*dir,0.1f*dir,0.0f);
        glRotatef(swing_angle, 0.0f,1.0f,0.0f);
        glColor3f(1.0f,0.9f,0.0f);
        glTranslatef(-0.2f*dir, 0, 0);
        glPushMatrix();
        {
            // render 手腕
            glScalef(5.0f,1.0f,1.0f);
            glutSolidSphere(0.05,50,40);
        }
        glPopMatrix();
        
        setMaterial(2);
        // render 手掌
        glColor3f(1.0f,0.3f,0.3f);
        glTranslatef(-0.2f*dir, 0, 0);
        glutSolidSphere(0.06,50,50);
        
        glRotatef(90, 1.0f,0.0f,0.0f);
        
        // render 食指無名指
        setMaterial(0);
        glColor3f(0.8f,0.8f,0.8f);
        glRotatef(swing_angle*-body_rotate*0.8, 0.0f,0.0f,1.0f);
        glPushMatrix();
        {
            glTranslatef(-0.08f*dir, 0, 0);
            glScalef(8.0f,1.0f,1.0f);
            //glRotatef(20,0.0f,0.0f,1.0f);
            glPushMatrix();
            {
                glTranslatef(0, -0.03f*dir, 0);
                if (stretch && counter > 2)
                {
                    //glRotatef(20, 0, 1, 0);
                    glTranslatef(0.01f*dir, 0.005f*dir, 0.08f);
                    //glRotatef(90,0.0f,1.0f,0.0f);
                }
                glutSolidSphere(0.01,50,50);
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(0, 0.03f*dir, 0);
                if (stretch && counter > 4)
                {
                    //glRotatef(20, 0, 1, 0);
                    glTranslatef(0.01f*dir, 0.005f*dir, 0.08f);
                }
                glutSolidSphere(0.01,50,50);
            }
            glPopMatrix();
        }
        glPopMatrix();

        // render 大拇指小指
        glPushMatrix();
        {
            glTranslatef(-0.04f*dir, 0, 0);
            //glRotatef(0,0.0f,0.0f,1.0f);
            glPushMatrix();
            {
                glScalef(5.0f,1.0f,1.0f);
                glTranslatef(0, -0.06f*dir, 0);
                if (stretch && counter > 1)
                {
                    glTranslatef(0.01f*dir, 0.005f*dir, 0.08f);
                }
                glutSolidSphere(0.01,50,50);
            }
            glPopMatrix();
            glPushMatrix();
            {
                glScalef(4.0f,1.2f,1.2f);
                glTranslatef(0, 0.045f*dir, 0);
                if (stretch && counter)
                {
                    glRotatef(40, 0.0f, 0.0f, 1.0f*dir);
                    glTranslatef(0.01f*dir, -0.005f*dir, 0.08f);
                }
                glutSolidSphere(0.01,50,50);
            }
            glPopMatrix();
        }
        glPopMatrix();
        // render 中指
        glPushMatrix();
        {
            glTranslatef(-0.09f*dir, 0, 0);
            glScalef(9.0f,1.0f,1.0f);
            if (stretch && counter > 3)
            {
                //glRotatef(20, 0, 1, 0);
                glTranslatef(0.01f*dir, 0.005f*dir, 0.08f);
            }
            glutSolidSphere(0.01,50,50);
        }
        glPopMatrix();
    }
    glPopMatrix();
    
    }
    glPopMatrix();

}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // render_triangle();
    glTranslatef(0, 0.25f, 0.0);
    
    
    if (light){
        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
         glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, a);
       
        setMaterial(0);
    }
    else if (spot){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos2);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 5);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 3.5);
        setMaterial(0);
    }
    else{
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    render_torso();
    render_nthu();
    render_head();
	render_arms(1);
    render_arms(-1);
    render_legs(1);
    render_legs(-1);
    glTranslatef(0, -0.25f, 0.0);
    
    //

	glutSwapBuffers();
}


void lookChange(){
    glLoadIdentity();
    float x0 = view_r*sin(view_angle)*cos(view_angle2);
    float y0 = view_r*sin(view_angle)*sin(view_angle2);
    float z0 = view_r*cos(view_angle);
    gluLookAt(y0, z0, x0, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0);
    glutPostRedisplay();
}

void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	float viewportAspect = (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// gluOrtho2D(-1 * viewportAspect, 1 * viewportAspect, -1, 1);
    gluPerspective(65.0, (GLfloat) width/height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    lookChange();
    
}

void My_Timer(int val)
{
    if (timer_enabled) {
        

    timer_cnt += speed*(3.14/180);
    //if (timer_cnt >= 10000) timer_cnt = 0;
    if (run){
        if (up <= -0.03f) pa = false;
        else if (up >= 0.03f) pa = true;
        if (pa) up -= pow(1.1,2)*0.006f;
        else up += 0.003f;
    }
    // theta = Amplitude * sin(t)
    swing_angle = run2*30*sin(timer_cnt);
    head_angle = (head_angle+speed)%360;
    
	glutPostRedisplay();
    
    glutTimerFunc(timer_speed, My_Timer, val);
        
    }
}

void start(){
    if(!timer_enabled)
    {
        timer_enabled = true;
        glutTimerFunc(timer_speed, My_Timer, 0);
    }
}


void My_Mouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		//printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
	}
	else if(state == GLUT_UP)
	{
		//printf("Mouse %d is released at (%d, %d)\n", button, x, y);
	}
}

void My_Keyboard(unsigned char key, int x, int y)
{
	//printf("Key %c is pressed at (%d, %d)\n", key, x, y);
   
    switch(key){
        case 'a': case 'A':
            view_angle2 -= spin*(3.14/180);
            break;
        case 'd': case 'D':
            view_angle2 += spin*(3.14/180);
            break;
        case 'w': case 'W':
            view_angle += spin*(3.14/180);
            break;
        case 's': case 'S':
            view_angle -= spin*(3.14/180);
            break;
        case 'r': case 'R':
            start();
            countc = (countc+1)%3;
            walkRun(countc);
            break;
        case 'e': case 'E':
            if (!timer_enabled)
                start();
            else timer_enabled = !timer_enabled;
            break;
        case 'l': case 'L':
            //spot = !spot;
            if (spot) spot = 0;
            light = !light;
            
            break;
        case 'o': case 'O':
            if (light) light = 0;
            spot = !spot;
            
           break;
            
    }
    lookChange();
}



void My_SpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		//printf("F1 is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_PAGE_UP:
		//printf("Page up is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_LEFT:
		//printf("Left arrow is pressed at (%d, %d)\n", x, y);
        /*
        if (stretch) counter++;
        stretch = !stretch;
        if (counter > 5) counter = 0;
		*/
        view_angle2 -= spin*(3.14/180);
            break;
    case GLUT_KEY_RIGHT:
        view_angle2 += spin*(3.14/180);
            break;
    case GLUT_KEY_UP:
        view_angle += spin*(3.14/180);
        break;
    case GLUT_KEY_DOWN:
        view_angle -= spin*(3.14/180);
        break;
    case GLUT_KEY_END:
        walkRun(!run);
        break;
	default:
		//printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
    lookChange();
}

void My_Menu(int id)
{
    
	switch(id)
	{
	case MENU_RUN_START:
        countc = 1;
        start();
		break;
	case MENU_WALK_START:
        countc = 0;
        start();
		break;
    case MENU_CRY_START:
        countc = 2;
        start();
        break;
    case MENU_TIMER_STOP:
        timer_enabled = false;
        break;
    case MENU_TIMER_START:
            if (!timer_enabled){
                timer_enabled = true;
                 glutTimerFunc(timer_speed, My_Timer, 0);
            }
        break;
	case MENU_EXIT:
		exit(0);
		break;
	default:
		break;
	}
    walkRun(countc);
}

void initTextures()
{
	// load jpg
	texture_data tdata = load_jpg("nthu.jpg"); // return width * height * 3 uchars
	if(tdata.data == 0)
	{
		// load failed
        //printf("jpg fail\n");
		return;
	}
	glGenTextures(1, &texture_jpg);
    glBindTexture(GL_TEXTURE_2D, texture_jpg);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata.data); // Use GL_RGB
	glGenerateMipmap(GL_TEXTURE_2D);
	free_texture_data(tdata);

	// load png
	tdata = load_png("nthu.png"); // return width * height * 4 uchars
	if(tdata.data == 0)
	{
		// load failed
        //printf("png fail\n");
		return;
	}
	glGenTextures(1, &texture_png);
    glBindTexture(GL_TEXTURE_2D, texture_png);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data); // Use GL_RGBA
	glGenerateMipmap(GL_TEXTURE_2D);
	free_texture_data(tdata);
}

int main(int argc, char *argv[])
{
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(700, 1000);
	glutCreateWindow("Nerd's AlphaGo"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
    dumpInfo();
	////////////////////
	
	// Initialize OpenGL states.
	////////////////////////
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	initTextures();
	////////////////////////

	// Create a menu and bind it to mouse right button.
	////////////////////////////
	int menu_main = glutCreateMenu(My_Menu);
	int menu_timer = glutCreateMenu(My_Menu);
    
	glutSetMenu(menu_main);
	glutAddSubMenu("Action", menu_timer);
    glutAddMenuEntry("Start", MENU_TIMER_START);
    glutAddMenuEntry("Pause", MENU_TIMER_STOP);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutSetMenu(menu_timer);
    glutAddMenuEntry("Walk", MENU_WALK_START);
	glutAddMenuEntry("Run", MENU_RUN_START);
	glutAddMenuEntry("Cry & Run", MENU_CRY_START);
    

	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	////////////////////////////

	// Register GLUT callback functions.
	///////////////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0); 
	///////////////////////////////

	// Enter main event loop.
	//////////////
	glutMainLoop();
	//////////////
	return 0;
}
