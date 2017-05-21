#define  _CRT_SECURE_NO_WARNINGS
// £adowanie bibliotek:
#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif
// Ustalanie trybu tekstowego:
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14159265358979
// Color Palette handle
HPALETTE hPalette = NULL;
// Application name and instance storeage
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;
// Rotation amounts
static GLfloat xRot = -60.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = -20.0f;


static GLfloat T1 = 0.0f;
static GLfloat R = 0.0f;
static GLfloat T2 = 0.0f;
static GLint direction = 0;
static GLint width = 0;
static GLint depth = 0;
GLint dir1 = 0;


static GLsizei lastHeight;
static GLsizei lastWidth;
// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury
// Declaration for Window procedure
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam);
// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);





// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0] * vector[0]) +
		     (vector[1] * vector[1]) +
		     (vector[2] * vector[2]));
	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;
	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}
// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;
	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];
	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];
	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}
// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;

	fAspect = (GLfloat)w / (GLfloat)h;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);
	// Establish perspective: 
	/*
	gluPerspective(60.0f,fAspect,1.0,400);
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);
	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Black brush
	glColor3f(0.0, 0.0, 0.0);
}
// LoadBitmapFile
// opis: ³aduje mapê bitow¹ z pliku i zwraca jej adres.
//       Wype³nia strukturê nag³ówka.
//	 Nie obs³uguje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;								// wskaŸnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag³ówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;			// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk³adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;
	// wczytuje nag³ówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fclose(filePtr);

		return NULL;
	}
	// wczytuje nag³ówek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ustawia wskaŸnik pozycji pliku na pocz¹tku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// przydziela pamiêæ buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	// sprawdza, czy uda³o siê przydzieliæ pamiêæ
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);

		return NULL;
	}
	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// sprawdza, czy dane zosta³y wczytane
	if (bitmapImage == NULL) {
		fclose(filePtr);

		return NULL;
	}
	// zamienia miejscami sk³adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// zamyka plik i zwraca wskaŸnik bufora zawieraj¹cego wczytany obraz
	fclose(filePtr);

	return bitmapImage;
}
void uklad()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(200, 0, 0);
	glVertex3d(-200, 0, 0);
	glVertex3d(0, 200, 0);
	glVertex3d(0, -200, 0);
	glVertex3d(0, 0, 200);
	glVertex3d(0, 0, -200);
	glEnd();
}
// Called to draw scene
void prostopadloscian(double a, double b, double c, double x=0, double y=0, double z=0)
{
	glBegin(GL_QUADS);

	glNormal3d(1, 0, 0);
	glColor3d(1, 0, 0);
	glVertex3d(a+x, -b+y, c+z);
	glVertex3d(a+x, -b+y, -c+z);
	glVertex3d(a+x, b+y, -c+z);
	glVertex3d(a+x, b+y, c+z);

	glNormal3d(0, 0, -1);
	glColor3d(0, 1, 0);
	glVertex3d(a+x, -b+y, -c+z);
	glVertex3d(-a+x, -b+y, -c+z);
	glVertex3d(-a+x, b + y, -c + z);
	glVertex3d(a + x, b + y, -c + z);

	glNormal3d(0, -1, 0);
	glColor3d(0, 0, 1);
	glVertex3d(a + x, -b + y, c + z);
	glVertex3d(-a + x, -b + y, c + z);
	glVertex3d(-a + x, -b + y, -c + z);
	glVertex3d(a + x, -b + y, -c + z);

	glNormal3d(0, 0, 1);
	glColor3d(1, 1, 0);
	glVertex3d(a + x, -b + y, c + z);
	glVertex3d(a + x, b + y, c + z);
	glVertex3d(-a + x, b + y, c + z);
	glVertex3d(-a + x, -b + y, c + z);

	glNormal3d(0, 1, 0);
	glColor3d(1, 0, 1);
	glVertex3d(a + x, b + y, -c + z);
	glVertex3d(-a + x, b + y, -c + z);
	glVertex3d(-a + x, b + y, c + z);
	glVertex3d(a + x, b + y, c + z);

	glNormal3d(-1, 0, 0);
	glColor3d(0, 1, 1);
	glVertex3d(-a + x, -b + y, -c + z);
	glVertex3d(-a + x, -b + y, c + z);
	glVertex3d(-a + x, b + y, c + z);
	glVertex3d(-a + x, b + y, -c + z);

	glEnd();
}

void prostopadloscian2(double a, double b, double c, double x = 0, double y = 0, double z = 0)
{
	glBegin(GL_QUADS);

	glNormal3d(1, 0, 0);
	glVertex3d(a + x, -b + y, c + z);
	glVertex3d(a + x, -b + y, -c + z);
	glVertex3d(a + x, b + y, -c + z);
	glVertex3d(a + x, b + y, c + z);

	glNormal3d(0, 0, -1);
	glVertex3d(a + x, -b + y, -c + z);
	glVertex3d(-a + x, -b + y, -c + z);
	glVertex3d(-a + x, b + y, -c + z);
	glVertex3d(a + x, b + y, -c + z);

	glNormal3d(0, -1, 0);
	glVertex3d(a + x, -b + y, c + z);
	glVertex3d(-a + x, -b + y, c + z);
	glVertex3d(-a + x, -b + y, -c + z);
	glVertex3d(a + x, -b + y, -c + z);

	glNormal3d(0, 0, 1);
	glVertex3d(a + x, -b + y, c + z);
	glVertex3d(a + x, b + y, c + z);
	glVertex3d(-a + x, b + y, c + z);
	glVertex3d(-a + x, -b + y, c + z);

	glNormal3d(0, 1, 0);
	glVertex3d(a + x, b + y, -c + z);
	glVertex3d(-a + x, b + y, -c + z);
	glVertex3d(-a + x, b + y, c + z);
	glVertex3d(a + x, b + y, c + z);

	glNormal3d(-1, 0, 0);
	glVertex3d(-a + x, -b + y, -c + z);
	glVertex3d(-a + x, -b + y, c + z);
	glVertex3d(-a + x, b + y, c + z);
	glVertex3d(-a + x, b + y, -c + z);

	glEnd();
}


void walec(double r, double h, double x=0, double y=0, double z=0)
{
	double PI = 3.14, a, b, alpha;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1, 0, 0);
	glNormal3d(0, 0, -1);
	glVertex3d(0+x, 0+y, 0+z);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		a = r*sin(alpha);
		b = r*cos(alpha);
		glVertex3d(a + x, b + y, 0 + z);

	}



	glEnd();

	glBegin(GL_QUAD_STRIP);

	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		a = r*sin(alpha);
		b = r*cos(alpha);
		glNormal3d(sin(alpha), cos(alpha), 0);
		glColor3d(1, 0, 1);
		glVertex3d(a + x, b + y, 0 + z);
		glVertex3d(a + x, b + y, h + z);


	}

	glEnd();



	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, 0, 1);
	glColor3d(0, 0, 1);
	glVertex3d(0+x, 0 + y, h + z);
	for (alpha = 2 * PI; alpha >= -8.0; alpha -= PI / 8.0)
	{
		a = r*sin(alpha);
		b = r*cos(alpha);
		glVertex3d(a + x, b + y, h + z);

	}



	glEnd();

}

void walec2(double r, double h, double x = 0, double y = 0, double z = 0)
{
	double PI = 3.14, a, b, alpha;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1, 0, 1);
	glNormal3d(0, 0, -1);
	glVertex3d(0 + x, 0 + y, 0 + z);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		a = r*sin(alpha);
		b = r*cos(alpha);
		glVertex3d(a + x, b + y, 0 + z);

	}



	glEnd();

	glBegin(GL_QUAD_STRIP);

	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		a = r*sin(alpha);
		b = r*cos(alpha);
		glNormal3d(sin(alpha), cos(alpha), 0);
		glColor3d(1, 0, 0);
		glVertex3d(a + x, b + y, 0 + z);
		glVertex3d(a + x, b + y, h + z);


	}

	glEnd();



	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, 1, 1);
	glColor3d(0, 0, 1);
	glVertex3d(0 + x, 0 + y, h + z);
	for (alpha = 2 * PI; alpha >= -8.0; alpha -= PI / 8.0)
	{
		a = r*sin(alpha);
		b = r*cos(alpha);
		glVertex3d(a + x, b + y, h + z);

	}



	glEnd();

}


void bryla(double a, double x = 0, double y = 0, double z = 0) {
	double tempx = x;
	double tempy = y;
	double tempz = z;
	glColor3f(0, 0, 0);

	for (int i = 0; i<a; i++) {
		for (int j = 0; j < a; j++) {
			for (int k = 0; k < a; k++) {
				prostopadloscian2(1, 1, 1, tempx, tempy, tempz);
				tempx += 1;
			}
			tempx = x;
			tempy += 1;
		}
		tempy = y;
		tempz += 1;
	}
}

void lampka()
{
	glColor3f(1, 0, 0);
	prostopadloscian2(2, 2, 2, -60, 40, 80);
}

void wykrywanieKolizji()
{

	if (29 + T1 > 30 && 4+T2>7 && -4+T2<23 && (13+T2)*sin(R*3.14/180) < 11.5 && (13+T2) * sin(R*3.14 / 180) > -11.5)
		lampka();
}

void maszyna()
{
	glPushMatrix();
	glTranslatef(-18, 0, 20);
	glRotatef(R, 0, 0, 1);
	glTranslatef(18, 0, -20);
	glPushMatrix();
	glTranslatef(T2, 0, 0); 
	walec(4, 12,0,0,30); //frez
	walec2(2, 12, 0, 0, 42); //uchwyt do frezu
	glPopMatrix();
	prostopadloscian(40, 6, 6, 30, 0, 60); //ramie frezarki
	walec(10, 20, -18, 0, 50); //czesc obrotowa
	glPopMatrix();
	walec2(10, 100, -18, 0, -50); //pion frezarki
	glPushMatrix();
	glTranslatef(0, 0, T1);
	prostopadloscian(52, 32, 4, 24, 0, 10); //stol roboczy
	bryla(15, 8, -7.5, 14); //bryla
	glPopMatrix();
	prostopadloscian(56, 50, 6, 20, 0, -56); //podstawa
}



void RenderScene(void)
{
	//float normal[3];	// Storeage for calculated surface normal
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////

	//Sposób na odróŸnienie "przedniej" i "tylniej" œciany wielok¹ta:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	uklad();

	maszyna();
	
	wykrywanieKolizji();
	//bryla();

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	// Flush drawing commands
	glFlush();
}
// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),			// Size of this structure
		1,                                      // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange, GreenRange, BlueRange;
	// Range for each color entry (7,7,and 3)

	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if (!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;
	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;
	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));
	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size
    // Build mask of all 1's.  This creates a number represented by having
    // the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
    // pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;
	// Loop through all the palette entries
	for (i = 0; i < nColors; i++) {
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}
	// Create the palette
	hRetPal = CreatePalette(pPal);
	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC, hRetPal, FALSE);
	RealizePalette(hDC);
	// Free the memory used for the logical palette structure
	free(pPal);
	// Return the handle to the new palette
	return hRetPal;
}
// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE hInst,
	                 HINSTANCE hPrevInstance,
	                 LPSTR     lpCmdLine,
	                 int       nCmdShow)
{
	MSG         msg;            // Windows message structure
	WNDCLASS	wc;             // Windows class structure
	HWND        hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// No need for background brush for OpenGL window
	wc.hbrBackground = NULL;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;
	// Register the window class
	if (RegisterClass(&wc) == 0)
		return FALSE;
	// Create the main application window
	hWnd = CreateWindow(lpszAppName,
						lpszAppName,
						// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
		                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					    // Window position and size
						0, 0,
		                (int)GetSystemMetrics(SM_CXSCREEN), (int)GetSystemMetrics(SM_CYSCREEN),
						NULL,
						NULL,
						hInstance,
						NULL);
	// If window was not created, quit
	if (hWnd == NULL)
		return FALSE;
	// Display the window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	// Process application messages until the application closes
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND   hWnd,
						UINT    message,
						WPARAM  wParam,
						LPARAM  lParam)
{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message) {
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);
		// Select the pixel format
		SetDCPixelFormat(hDC);
		// Create palette if needed
		hPalette = GetOpenGLPalette(hDC);
		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetupRC();
		glGenTextures(2, &texture[0]);  // tworzy obiekt tekstury			
										// ³aduje pierwszy obraz tekstury:
		/***************************************************************************/
		bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);

		glBindTexture(GL_TEXTURE_2D, texture[0]);  // aktywuje obiekt tekstury
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
		if (bitmapData)
			free(bitmapData);
		// ³aduje drugi obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);
		// ustalenie sposobu mieszania tekstury z t³em
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;
		/***************************************************************************/
		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		// Delete the palette if it was created
		if (hPalette != NULL)
			DeleteObject(hPalette);
		// Tell the application to terminate after the window
		// is gone.
		PostQuitMessage(0);
		break;
		// Window is resized.
	case WM_SIZE:
		// Call our function which modifies the clipping
		// volume and viewport
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;
		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
	case WM_PAINT: {
		// Call OpenGL drawing code
		RenderScene();
		SwapBuffers(hDC);
		// Validate the newly painted client area
		ValidateRect(hWnd, NULL);
	} break;
	// Windows is telling the application that it may modify
	// the system palette.  This message in essance asks the 
	// application for a new palette.
	case WM_QUERYNEWPALETTE:
		// If the palette was created.
		if (hPalette) {
			int nRet;
			// Selects the palette into the current device context
			SelectPalette(hDC, hPalette, FALSE);
			// Map entries from the currently selected palette to
			// the system palette.  The return value is the number 
			// of palette entries modified.
			nRet = RealizePalette(hDC);
			// Repaint, forces remap of palette in current window
			InvalidateRect(hWnd, NULL, FALSE);

			return nRet;
		} break;
		// This window may set the palette, even though it is not the 
		// currently active window.
	case WM_PALETTECHANGED:
		// Don't do anything if the palette does not exist, or if
		// this is the window that changed the palette.
		if ((hPalette != NULL) && ((HWND)wParam != hWnd)) {
			// Select the palette into the device context
			SelectPalette(hDC, hPalette, FALSE);
			// Map entries to system palette
			RealizePalette(hDC);
			// Remap the current colors to the newly realized palette
			UpdateColors(hDC);
			return 0;
		} break;

		// Key press, check for arrow keys to do cube rotation.
 	case WM_KEYDOWN: {
		if (wParam == VK_UP)
			xRot -= 2.0f;
		if (wParam == VK_DOWN)
			xRot += 2.0f;
		if (wParam == VK_LEFT)
			yRot -= 2.0f;
		if (wParam == VK_RIGHT)
			yRot += 2.0f;
		if (wParam == VK_ADD)
			zRot -= 2.0f;
		if (wParam == VK_SUBTRACT)
			zRot += 2.0f;


		if (wParam == 'Q' && T1<=14)
			T1 += 0.5f;
		if (wParam == 'A' && T1>=-54)
			T1 -= 0.5f;
		if (wParam == 'W' && R>=-135)
			R -= 0.5f;
		if (wParam == 'S' && R<=135)
			R += 0.5f;
		if (wParam == 'E' && T2>0)
			T2 -= 0.5f;
		if (wParam == 'D' && T2<60)
			T2 += 0.5f;

		xRot = (const int)xRot % 360;
		yRot = (const int)yRot % 360;
		zRot = (const int)zRot % 360;

		InvalidateRect(hWnd, NULL, FALSE);
	} break;

	// A menu command
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			// Exit the program
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
			// Display the about box
		case ID_HELP_ABOUT:
			DialogBox(hInstance,
				MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
				hWnd,
				(DLGPROC)AboutDlgProc);
			break;
		}
	} break;
	default:   // Passes it on if unproccessed

		return (DefWindowProc(hWnd, message, wParam, lParam));
	}


	
	/*if (T2 < 60 && direction==0) {
		T2 += 0.50f;
		InvalidateRect(hWnd, NULL, FALSE);
	}
	if (T2 == 60 && direction == 0) {
		direction = 1;
	}
	if (T2>0 && direction == 1) {
		T2 -= 0.50f;
		InvalidateRect(hWnd, NULL, FALSE);
	}
	if (T2 == 0 && direction == 1) {
		direction = 0;
	}*/


	return (0L);
}
// Dialog procedure.
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	switch (message) {
		// Initialize the dialog box
	case WM_INITDIALOG: {
		int i;
		GLenum glError;
		// glGetString demo
		SetDlgItemText(hDlg, IDC_OPENGL_VENDOR, (LPSTR)glGetString(GL_VENDOR));
		SetDlgItemText(hDlg, IDC_OPENGL_RENDERER, (LPSTR)glGetString(GL_RENDERER));
		SetDlgItemText(hDlg, IDC_OPENGL_VERSION, (LPSTR)glGetString(GL_VERSION));
		SetDlgItemText(hDlg, IDC_OPENGL_EXTENSIONS, (LPSTR)glGetString(GL_EXTENSIONS));
		// gluGetString demo
		SetDlgItemText(hDlg, IDC_GLU_VERSION, (LPSTR)gluGetString(GLU_VERSION));
		SetDlgItemText(hDlg, IDC_GLU_EXTENSIONS, (LPSTR)gluGetString(GLU_EXTENSIONS));
		// Display any recent error messages
		i = 0;
		do {
			glError = glGetError();
			SetDlgItemText(hDlg, IDC_ERROR1 + i, (LPSTR)gluErrorString(glError));
			i++;
		} while (i < 6 && glError != GL_NO_ERROR);

		return (TRUE);
	} break;
	// Process command messages
	case WM_COMMAND: {
		// Validate and Make the changes
		if (LOWORD(wParam) == IDOK)
			EndDialog(hDlg, TRUE);
	} break;
	// Closed from sysbox7
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	}

	return FALSE;
}
