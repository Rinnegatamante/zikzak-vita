#include "Graphics.h"

int Graphics::loadTexture(string filename) {
	ostringstream temp;
#ifndef WIN32
	temp << "data/" << filename;
#else
	temp << "data\\" << filename;
#endif
	filename = temp.str();
	
	Uint8 *rowhi, *rowlo;
	Uint8 *tmpbuf, tmpch;
	
	SDL_Surface *image;
	static int texcount = -1;

	image = SDL_LoadBMP(filename.c_str());
	if (!image) {
		cerr << "Unable to load texture " << filename
			<< ": " << SDL_GetError() << "\n";
		return 1;
	}
	
	texcount++;
	
	// GL surfaces are upsidedown and RGB, not BGR
	tmpbuf = (Uint8 *)malloc(image->pitch);
	if (!tmpbuf) {
		cerr << "Out of memory\n";
		return 1;
	}
	rowhi = (Uint8 *)image->pixels;
	rowlo = rowhi + (image->h * image->pitch) - image->pitch;
	for (int i=0; i<image->h/2; ++i ) {
		for (int j=0; j<image->w; ++j ) {
			tmpch = rowhi[j*3];
			rowhi[j*3] = rowhi[j*3+2];
			rowhi[j*3+2] = tmpch;
			tmpch = rowlo[j*3];
			rowlo[j*3] = rowlo[j*3+2];
			rowlo[j*3+2] = tmpch;
		}
		memcpy(tmpbuf, rowhi, image->pitch);
		memcpy(rowhi, rowlo, image->pitch);
		memcpy(rowlo, tmpbuf, image->pitch);
		rowhi += image->pitch;
		rowlo -= image->pitch;
	}
	free(tmpbuf);
		
	glGenTextures(1, &textures[texcount]);
	glBindTexture(GL_TEXTURE_2D, textures[texcount]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->w, image->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image->pixels);

	SDL_FreeSurface(image);
	return 0;
}

void Graphics::buildFont() {
	double cx;
	double cy;
	double cinc = 0.0625;

	fontbase = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	for(GLuint loop = 0; loop < 256; loop++) {
		cx = (float)(loop % 16) / 16.0f;
		cy = (float)(loop / 16) / 16.0f;

		glNewList(fontbase + loop,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx,1-cy-cinc);
				glVertex2i(0,0);
				glTexCoord2f(cx+cinc,1-cy-cinc);
				glVertex2i(16,0);
				glTexCoord2f(cx+cinc,1-cy);
				glVertex2i(16,16);
				glTexCoord2f(cx,1-cy);
				glVertex2i(0,16);
			glEnd();
			glTranslated(10, 0, 0);
		glEndList();
	}
}

void Graphics::glPrint(GLint x, GLint y, const char *string, float scale) {
	//if (set>1)
	int set = 0;
	scale = 1/scale;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,(GLint)(640*scale),0,(GLint)(480*scale),-1,1);
	//glOrtho(0,320,0,240,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated((GLint)(x*scale),(GLint)(y*scale),0);
	glListBase(fontbase-32+(128*set));
	glCallLists(strlen(string),GL_BYTE,string);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_BLEND);
}

int Graphics::init(int xres, int yres, bool fullscreen) {
	// initialize SDL for video output
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cerr << "Unable to initialize SDL: " << SDL_GetError() << "\n";
 		return 1;
	}

	Uint32 flags = SDL_OPENGL;
	if(fullscreen)
		flags |= SDL_FULLSCREEN;
	
	// create an OpenGL screen
	if ( SDL_SetVideoMode(xres, yres, 0, flags) == NULL ) {
		cerr << "Unable to create OpenGL screen: " << SDL_GetError() << "\n";
		SDL_Quit();
		return 1;
	}

	// set the title bar in environments that support it
	SDL_WM_SetCaption("Zik Zak", NULL);
	
	SDL_ShowCursor(SDL_DISABLE);   // hide mouse cursor
	SDL_WM_GrabInput(SDL_GRAB_ON); // grab mouse cursor focus
	
	glViewport(0, 0, xres, yres);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// black background
	glClearDepth(1.0);						// enables clearing of the depth buffer
	glDepthFunc(GL_LESS);					// the type of depth test to do
	glEnable(GL_DEPTH_TEST);				// enables depth testing
	glShadeModel(GL_SMOOTH);				// enables smooth color shading

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();						// reset the projection matrix

	// calculate the aspect ratio of the window
	gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	
	// load textures and prepare font
	if(loadTexture("font.bmp") == 0)
		buildFont();
	else
		return 1;
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	
	return 0;
}

void Graphics::cleanUp() {
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_Quit();
}

void Graphics::renderscene(Level &l, Player &p, Sound &s, Gamestate &gs) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotated(p.getPitch(), 1.0, 0.0, 0.0);
	glRotated(p.getYaw(), 0.0, 1.0, 0.0);
	glTranslatef((float)-p.getX()+(l.getXShift()/7.0),
		(float)p.getY()-15+(l.getYShift()/7.0),
		(float)p.getZ()-30+(l.getZShift()/7.0));
	
	p.render(l, s);

	for(int yloop = 0; yloop < Level::getYSize(); yloop++) {
		for(int zloop = 0; zloop < Level::getZSize(); zloop++) {
			for(int xloop = 0; xloop < Level::getXSize(); xloop++) {
				l.renderBlock(Coord3d(xloop, yloop-1, zloop));
			}
		}
	}

	// draw text according to the gamestate
	char a[100];
	switch(gs.getOperationState()) {
		case PLAY:
			if(l.getLevelNumber() != 0)
				if(p.isMoving()) {
					sprintf(a, "Time Left: %d", gs.getTimeLeft(l));
					glColor3f(1.0f,1.0f,1.0f);
					glPrint(490, 460, a, 1);
					sprintf(a, "Level %d", l.getLevelNumber());
					glPrint(10, 460, a, 1);
				} else {
					glColor3f(1.0f, 1.0f, 1.0f);
					for(int i = 0; i < l.getNumMessages(); i++) {
						glPrint(50, 400-(i*30), l.getMessage(i).c_str(), 1.2);
					}
				}
			break;
		case TITLE:
			sprintf(a, "Zik Zak");
			glColor3f(0.1f, 0.1f, 1.0f);
			glPrint(221, 389, a, 2);
			glColor3f(1.0f, 1.0f, 0.3f);
			glPrint(220, 390, a, 2);

			sprintf(a, "Andreas Textor");
			glColor3f(0.1, 0.1f, 1.0f);
			glPrint(225, 359, a, 1);
			glColor3f(1.0f, 1.0f, 0.3f);
			glPrint(224, 360, a, 1);

			glColor3f(1.0f, 1.0f, 1.0f);
			sprintf(a, "Press any key to start");
			glColor3f(1.0f, 1.0f, 1.0f);
			glPrint(180, 320, a, 1);
			break;
		case GAMEOVER:
			sprintf(a, "Time's Up");
			glColor3f(0.1f, 0.1f, 1.0f);
			glPrint(221, 339, a, 2);
			glColor3f(1.0f, 1.0f, 0.3f);
			glPrint(220, 340, a, 2);
			
			sprintf(a, "Press any key to restart level");
			glColor3f(1.0f, 1.0f, 1.0f);
			glPrint(150, 300, a, 1);
			break;
		case WON:
			sprintf(a, "Congratulations, you've won!");
			glColor3f(0.1f, 0.1f, 1.0f);
			glPrint(21, 339, a, 2);
			glColor3f(1.0f, 1.0f, 0.3f);
			glPrint(20, 340, a, 2);
			
			sprintf(a, "You have solved all levels.");
			glColor3f(1.0f, 1.0f, 1.0f);
			glPrint(180, 300, a, 1);
		default:
			break;
	}
	
	// swap buffers to display
	SDL_GL_SwapBuffers();
}


Graphics::Graphics() { }

Graphics::~Graphics() { }
