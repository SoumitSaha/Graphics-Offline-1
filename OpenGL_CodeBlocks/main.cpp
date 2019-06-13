#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define moveamount 2
#define slices_no 50
#define stacks_no 50

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double max_radius;
double side;
double radius;

struct point
{
	double x,y,z;
};

struct point pos;
struct point u;
struct point r;
struct point l;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(1,1,1);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		glColor3f(1, 0, 0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,int height,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height*sin(((double)i/(double)stacks)*(pi/2));
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		glColor3f(0, 1, 0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void draw_half_Sphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		glColor3f(1, 0, 0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void position_all_squares(){
    glPushMatrix();
	glTranslated(0, 0, max_radius);
	drawSquare(side);
    glPopMatrix();

    glPushMatrix();
	glTranslated(0, 0, -max_radius);
	drawSquare(side);
    glPopMatrix();

    glPushMatrix();
	glTranslated(0, max_radius, 0);
	glRotated(90, 1, 0, 0);
	drawSquare(side);
    glPopMatrix();

    glPushMatrix();
	glTranslated(0, -max_radius, 0);
	glRotated(90, 1, 0, 0);
	drawSquare(side);
    glPopMatrix();

    glPushMatrix();
	glTranslated(max_radius, 0, 0);
	glRotated(90, 0, 1, 0);
	drawSquare(side);
    glPopMatrix();

    glPushMatrix();
	glTranslated(-max_radius, 0, 0);
	glRotated(90, 0, 1, 0);
	drawSquare(side);
    glPopMatrix();
}

void position_all_cylinders(){
    glPushMatrix();
	{
		glTranslatef(side, side, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(side, -side, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, side, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, -side, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();


	glPushMatrix();
	{
	    //glRotated(-90, 1, 0, 0);
		glTranslatef(side, 0, side);
		glRotated(90, 1, 0, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, 0, side);
		glRotated(90, 1, 0, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();


	glPushMatrix();
	{
		glTranslatef(side, 0, -side);
		glRotated(90, 1, 0, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();


	glPushMatrix();
	{
		glTranslatef(-side, 0, -side);
		glRotated(90, 1, 0, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();



	glPushMatrix();
	{
		glTranslatef(0, side, side);
		glRotated(90, 0, 1, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, -side, side);
		glRotated(90, 0, 1, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();


	glPushMatrix();
	{
		glTranslatef(0, side, -side);
		glRotated(90, 0, 1, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, -side, -side);
		glRotated(90, 0, 1, 0);
		drawCylinder(radius, side, 50, 50);
	}
	glPopMatrix();
}

void position_all_spheres(){
	glPushMatrix();
	{
		glTranslatef(side, side, side); // moving to x+, y+, z+ co-ordinate
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, side, side);     // moving targeted co-ordinate
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, -side, side);    // moving targeted co-ordinate
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(side, -side, side);    // moving targeted co-ordinate
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(side, side, -side);    // moving targeted co-ordinate
		glRotatef(180, 1, 0, 0);            // rotating 180 degree
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, side, -side);   // moving targeted co-ordinate
		glRotatef(180, 1, 0, 0);            // rotating 180 degree
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-side, -side, -side);  // moving targeted co-ordinate
		glRotatef(180, 1, 0, 0);            // rotating 180 degree
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(side, -side, -side);   // moving targeted co-ordinate
		glRotatef(180, 1, 0, 0);            // rotating 180 degree
		draw_half_Sphere(radius, slices_no, stacks_no);
	}
	glPopMatrix();
}

void drawSS(){
    position_all_squares();
    position_all_cylinders();
    position_all_spheres();
}

void update_l_r_u(char vec, struct point p){
    switch(vec){
        case 'l':
            l.x = p.x;
            l.y = p.y;
            l.z = p.z;
            break;
        case 'r':
            r.x = p.x;
            r.y = p.y;
            r.z = p.z;
            break;
        case 'u':
            u.x = p.x;
            u.y = p.y;
            u.z = p.z;
            break;
    }
}

void keyboardListener(unsigned char key, int x,int y){
	double rotation_angle = 0.02;
	switch(key){

		case '1': // look left => l, r change, u unchanged
		    {
		        struct point new_l;
                struct point new_r;
                double to_make_unit;

                new_l.x = (l.x * cos(rotation_angle)) - (r.x * sin(rotation_angle));
                new_l.y = (l.y * cos(rotation_angle)) - (r.y * sin(rotation_angle));
                new_l.z = l.z;

                to_make_unit = sqrt((new_l.x * new_l.x) + (new_l.y * new_l.y) + (new_l.z * new_l.z));
                new_l.x = new_l.x/to_make_unit;
                new_l.y = new_l.y/to_make_unit;
                new_l.z = new_l.z/to_make_unit;

                // r = l x u
                new_r.x = (new_l.y * u.z) - (new_l.z * u.y);
                new_r.y = (new_l.z * u.x) - (new_l.x * u.z);
                new_r.z = (new_l.x * u.y) - (new_l.y * u.x);
                to_make_unit = sqrt((new_r.x * new_r.x) + (new_r.y * new_r.y) + (new_r.z * new_r.z));
                new_r.x = new_r.x/to_make_unit;
                new_r.y = new_r.y/to_make_unit;
                new_r.z = new_r.z/to_make_unit;

                update_l_r_u('l', new_l);
                update_l_r_u('r', new_r);
                break;
		    }
        case '2': // look left => l, r change, u unchanged
            {
		        struct point new_l;
                struct point new_r;
                double to_make_unit;

                new_l.x = (l.x * cos(rotation_angle)) + (r.x * sin(rotation_angle));
                new_l.y = (l.y * cos(rotation_angle)) + (r.y * sin(rotation_angle));
                new_l.z = l.z;

                to_make_unit = sqrt((new_l.x * new_l.x) + (new_l.y * new_l.y) + (new_l.z * new_l.z));
                new_l.x = new_l.x/to_make_unit;
                new_l.y = new_l.y/to_make_unit;
                new_l.z = new_l.z/to_make_unit;

                // r = l x u
                new_r.x = (new_l.y * u.z) - (new_l.z * u.y);
                new_r.y = (new_l.z * u.x) - (new_l.x * u.z);
                new_r.z = (new_l.x * u.y) - (new_l.y * u.x);
                to_make_unit = sqrt((new_r.x * new_r.x) + (new_r.y * new_r.y) + (new_r.z * new_r.z));
                new_r.x = new_r.x/to_make_unit;
                new_r.y = new_r.y/to_make_unit;
                new_r.z = new_r.z/to_make_unit;

                update_l_r_u('l', new_l);
                update_l_r_u('r', new_r);
                break;
		    }
        case '3': // look up => u , l changes, r unchanged
            {
		        struct point new_l;
                struct point new_u;
                double to_make_unit;

                new_l.x = (l.x * cos(rotation_angle)) + (u.x * sin(rotation_angle));
                new_l.z = (l.z * cos(rotation_angle)) + (u.z * sin(rotation_angle));
                new_l.y = l.y;

                to_make_unit = sqrt((new_l.x * new_l.x) + (new_l.y * new_l.y) + (new_l.z * new_l.z));
                new_l.x = new_l.x/to_make_unit;
                new_l.y = new_l.y/to_make_unit;
                new_l.z = new_l.z/to_make_unit;

                // u = l x r
                new_u.x = (new_l.z * r.y) - (new_l.y * r.z);
                new_u.y = (new_l.x * r.z) - (new_l.z * r.x);
                new_u.z = (new_l.y * r.x) - (new_l.x * r.y);
                to_make_unit = sqrt((new_u.x * new_u.x) + (new_u.y * new_u.y) + (new_u.z * new_u.z));
                new_u.x = new_u.x/to_make_unit;
                new_u.y = new_u.y/to_make_unit;
                new_u.z = new_u.z/to_make_unit;

                update_l_r_u('l', new_l);
                update_l_r_u('u', new_u);
                break;
		    }
        case '4': // look down => u , l changes, r unchanged
            {
		        struct point new_l;
                struct point new_u;
                double to_make_unit;

                new_l.x = (l.x * cos(rotation_angle)) - (u.x * sin(rotation_angle));
                new_l.z = (l.z * cos(rotation_angle)) - (u.z * sin(rotation_angle));
                new_l.y = l.y;

                to_make_unit = sqrt((new_l.x * new_l.x) + (new_l.y * new_l.y) + (new_l.z * new_l.z));
                new_l.x = new_l.x/to_make_unit;
                new_l.y = new_l.y/to_make_unit;
                new_l.z = new_l.z/to_make_unit;

                // u = l x r
                new_u.x = (new_l.z * r.y) - (new_l.y * r.z);
                new_u.y = (new_l.x * r.z) - (new_l.z * r.x);
                new_u.z = (new_l.y * r.x) - (new_l.x * r.y);
                to_make_unit = sqrt((new_u.x * new_u.x) + (new_u.y * new_u.y) + (new_u.z * new_u.z));
                new_u.x = new_u.x/to_make_unit;
                new_u.y = new_u.y/to_make_unit;
                new_u.z = new_u.z/to_make_unit;

                update_l_r_u('l', new_l);
                update_l_r_u('u', new_u);
                break;
		    }
        case '5': // tilt clockwise => r, u changes, l unchanged
            {
		        struct point new_r;
                struct point new_u;
                double to_make_unit;

                new_u.y = (u.y * cos(rotation_angle)) + (r.y * sin(rotation_angle));
                new_u.z = (u.z * cos(rotation_angle)) + (r.z * sin(rotation_angle));
                new_u.x = u.x;

                to_make_unit = sqrt((new_u.x * new_u.x) + (new_u.y * new_u.y) + (new_u.z * new_u.z));
                new_u.x = new_u.x/to_make_unit;
                new_u.y = new_u.y/to_make_unit;
                new_u.z = new_u.z/to_make_unit;

                // r = l x u
                new_r.x = (new_u.z * l.y) - (new_u.y * l.z);
                new_r.y = (new_u.x * l.z) - (new_u.z * l.x);
                new_r.z = (new_u.y * l.x) - (new_u.x * l.y);
                to_make_unit = sqrt((new_r.x * new_r.x) + (new_r.y * new_r.y) + (new_r.z * new_r.z));
                new_r.x = new_r.x/to_make_unit;
                new_r.y = new_r.y/to_make_unit;
                new_r.z = new_r.z/to_make_unit;

                update_l_r_u('r', new_r);
                update_l_r_u('u', new_u);
                break;
		    }
        case '6':
            {
		        struct point new_r;
                struct point new_u;
                double to_make_unit;

                new_u.y = (u.y * cos(rotation_angle)) - (r.y * sin(rotation_angle));
                new_u.z = (u.z * cos(rotation_angle)) - (r.z * sin(rotation_angle));
                new_u.x = u.x;

                to_make_unit = sqrt((new_u.x * new_u.x) + (new_u.y * new_u.y) + (new_u.z * new_u.z));
                new_u.x = new_u.x/to_make_unit;
                new_u.y = new_u.y/to_make_unit;
                new_u.z = new_u.z/to_make_unit;

                // r = l x u
                new_r.x = (new_u.z * l.y) - (new_u.y * l.z);
                new_r.y = (new_u.x * l.z) - (new_u.z * l.x);
                new_r.z = (new_u.y * l.x) - (new_u.x * l.y);
                to_make_unit = sqrt((new_r.x * new_r.x) + (new_r.y * new_r.y) + (new_r.z * new_r.z));
                new_r.x = new_r.x/to_make_unit;
                new_r.y = new_r.y/to_make_unit;
                new_r.z = new_r.z/to_make_unit;

                update_l_r_u('r', new_r);
                update_l_r_u('u', new_u);
                break;
		    }
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		    // move backward
            pos.x = pos.x - (l.x * moveamount);
            pos.y = pos.y - (l.y * moveamount);
            pos.z = pos.z - (l.z * moveamount);
			break;
		case GLUT_KEY_UP:		    // move forward
			pos.x = pos.x + (l.x * moveamount);
            pos.y = pos.y + (l.y * moveamount);
            pos.z = pos.z + (l.z * moveamount);
			break;

		case GLUT_KEY_RIGHT:        // move left
			pos.x = pos.x + (r.x * moveamount);
            pos.y = pos.y + (r.y * moveamount);
            pos.z = pos.z + (r.z * moveamount);
			break;
		case GLUT_KEY_LEFT:         // move right
			pos.x = pos.x - (r.x * moveamount);
            pos.y = pos.y - (r.y * moveamount);
            pos.z = pos.z - (r.z * moveamount);
			break;

		case GLUT_KEY_PAGE_UP:      // move up
		    pos.x = pos.x + (u.x * moveamount);
            pos.y = pos.y + (u.y * moveamount);
            pos.z = pos.z + (u.z * moveamount);
			break;
		case GLUT_KEY_PAGE_DOWN:    // move down
		    pos.x = pos.x - (u.x * moveamount);
            pos.y = pos.y - (u.y * moveamount);
            pos.z = pos.z - (u.z * moveamount);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    side -= 0.5;
		    radius += 0.5;
            if(side < 0.0){
                side = 0.0;
                radius = max_radius;
            }
			break;
		case GLUT_KEY_END:
		    side += 0.5;
		    radius -= 0.5;
		    if(side > max_radius){
                side = max_radius;
                radius = 0.0;
		    }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid=1-drawgrid;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	/**************************
	/ For controllable camera
	/ gluLookAt(pos.x, pos.y, pos.z,  pos.x+l.x, pos.y+l.y, pos.z+l.z,  u.x, u.y, u.z);
	/ where pos is position of camera
	/ u and l are 2 perpendicular unit vector(both perpendicular with another unit vector r)
    **************************/

    gluLookAt(pos.x, pos.y, pos.z,  pos.x+l.x, pos.y+l.y, pos.z+l.z,  u.x, u.y, u.z);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	pos.x = 100;
	pos.y = 100;
	pos.z = 0;

    u.x = 0;
    u.y = 0;
    u.z = 1;

    r.x = -(1/sqrt(2));
    r.y = -(r.x);
    r.z = 0;

    l.x = r.x;
    l.y = l.x;
    l.z = 0;

    max_radius = 30;
    side = 20;
    radius = max_radius - side;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Offline 1");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
