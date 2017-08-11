#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <bitset>
#include <vector>
using namespace std;
typedef bitset<8> _BYTE;

const int maxn = 10001;

vector<int>adj[maxn];
vector<int>path;
bool visit[maxn];
enum directions { NONE, NOR = 1, EAS = 2, SOU = 4, WES = 8 };

const int BMP_SIZE = 512, CELL_SIZE = 16;

int dfs(int i, int sz){
    visit[i] = 1;
    path.push_back(i);
    if(i == sz * sz -1){
        //for(auto i : path)cout<<i <<" ";
        return 1;
    }
    for(int k = 0; k < adj[i].size(); k++){
        if(visit[adj[i][k]])continue;
            if(dfs(adj[i][k], sz))return 1;
    }
    path.pop_back();
    return 0;
}

int i = 0;
void draw(int x1 , int y1, int x2, int y2){

	glBegin(GL_LINES);
	 glPointSize(15);
		glColor3f(1.0f,0.0f,0.f);
		glVertex2i(x1,y1);glVertex2i(x2,y2);
		i+=1;
	glEnd();

}

class mazeGenerator
{
public:

    _BYTE*    _world;
    int      _s, _ptX, _ptY;
    mazeGenerator()
    {
	_world = 0;
    }

    ~mazeGenerator() { killArray(); }

    void create( int side )
    {
	_s = side;
	generate();

    }

private:
    void generate()
    {
	killArray();
	_world = new _BYTE[_s * _s];
	ZeroMemory( _world, _s * _s );
	_ptX = rand() % _s; _ptY = rand() % _s;
	carve();
    }

    void carve()
    {
	while( true )
	{
	    int d = getDirection();
	    if( d < NOR ) return;

	    switch( d )
	    {
	        case NOR:
            _world[_ptX + _s * _ptY] |= NOR; _ptY--;
		    _world[_ptX + _s * _ptY] = SOU | SOU << 4;
		break;
	        case EAS:
		    _world[_ptX + _s * _ptY] |= EAS; _ptX++;
		    _world[_ptX + _s * _ptY] = WES | WES << 4;
		break;
		case SOU:
		    _world[_ptX + _s * _ptY] |= SOU; _ptY++;
		    _world[_ptX + _s * _ptY] = NOR | NOR << 4;
		break;
		case WES:
		    _world[_ptX + _s * _ptY] |= WES; _ptX--;
		    _world[_ptX + _s * _ptY] = EAS | EAS << 4;
	    }
	}
    }
    int getDirection()
    {
	int d = 1 << rand() % 4;
	while( true )
	{
	    for( int x = 0; x < 4; x++ )
	    {
		if( testDir( d ) ) return d;
		d <<= 1;
		if( d > 8 ) d = 1;
	    }
	    d = ( _world[_ptX + _s * _ptY].to_ulong() & 0xf0 ) >> 4;
	    if( !d ) return -1;
	    switch( d )
	    {
		case NOR: _ptY--; break;
		case EAS: _ptX++; break;
		case SOU: _ptY++; break;
		case WES: _ptX--; break;
	    }
            d = 1 << rand() % 4;
	}
    }

    bool testDir( int d )
    {
	switch( d )
	{
	    case NOR: return ( _ptY - 1 > -1 && !_world[_ptX + _s * ( _ptY - 1 )].to_ulong() );
	    case EAS: return ( _ptX + 1 < _s && !_world[_ptX + 1 + _s * _ptY].to_ulong() );
	    case SOU: return ( _ptY + 1 < _s && !_world[_ptX + _s * ( _ptY + 1 )].to_ulong() );
	    case WES: return ( _ptX - 1 > -1 && !_world[_ptX - 1 + _s * _ptY].to_ulong() );
	}
	return false;
    }

    void killArray() { if( _world ) delete [] _world; }
};

void drawSol(int sz){
   // glClear(GL_COLOR_BUFFER_BIT);
    for(int i=0;i<path.size()-1;i++ ){
            int x1 = path[i]%sz;
            x1 *= CELL_SIZE;
            int y1 = path[i]/sz;
            y1 *= CELL_SIZE;
            int x2 = path[i+1]%sz;
            x2 *= CELL_SIZE;
            int y2 =  path[i+1]/sz;
            y2 *= CELL_SIZE;

        	glBegin(GL_LINES);
             glPointSize(60);
                glColor3f(0.0f,0.9f,0.0f);
                glVertex2i(x1+CELL_SIZE/2,y1+CELL_SIZE/2);glVertex2i(x2+CELL_SIZE/2,y2+CELL_SIZE/2);
                //Sleep(25);
            glEnd();
            glFlush();

    }
}


void init(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    gluOrtho2D(0,1000,0,1000);
    glMatrixMode(GL_PROJECTION);
}

    void display(){
	glClear(GL_COLOR_BUFFER_BIT);

        int _s = 50;
        srand( GetTickCount() );
        mazeGenerator mg;
        mg.create(_s);
        _BYTE *ans = mg._world;
//	glBegin(GL_LINES);
//	 glPointSize(15);
//		glColor3f(0.9f,0.9f,0.f);
//		glVertex2i(0,0);glVertex2i(0,1000);
//		glVertex2i(0,0);glVertex2i(1000,0);
//	glEnd();
for( int y = 0; y < _s; y++ )
	{

	    int yy = y * _s;
	    for( int x = 0; x < _s; x++ )
	    {//getchar();
		_BYTE b = mg._world[x + yy];
		int nx = x * CELL_SIZE,
		    ny = y * CELL_SIZE;

		if( !( b.to_ulong() & NOR ) )
		{
            draw(nx,ny,nx+CELL_SIZE+1,ny);
		}
		else{
            ///cout<<"north ";
            //swap(x,y);
            adj[x+_s*y].push_back(x+_s*(y-1));
           // printf("%d %d\n", x+_s*y, x+_s*(y-1));
            //swap(x,y);
		}
		if( !( b.to_ulong() & EAS ) )
		{
             draw(nx + CELL_SIZE, ny, nx+CELL_SIZE+1, ny + CELL_SIZE+1);
		}
		else{
		   // cout<<"east ";

		    //swap(x,y);
		    adj[x+_s*y].push_back((x+1)+_s*y);
           // printf("%d %d\n", x+_s*y, (x+1)+_s*y);
            //swap(x,y);
		}
		if( !( b.to_ulong() & SOU ) )
		{
            draw(nx , ny+CELL_SIZE, nx+CELL_SIZE+1, ny + CELL_SIZE);
		}
		else{
		   // cout<<"south ";

		    //swap(x,y);
		    adj[x+_s*y].push_back(x+_s*(y+1));
          // printf("%d %d\n", x+_s*y, x+_s*(y+1));
           //swap(x,y);
		}
		if( !( b.to_ulong() & WES ) )
		{
            draw(nx , ny, nx, ny + CELL_SIZE +1);
		}
		else{
		   // cout<<"west ";
		    //swap(x,y);
		    adj[x+_s*y].push_back((x-1)+_s*y);
           // printf("%d %d\n", x+_s*y, (x-1)+_s*y );
            //swap(x,y);
		}
	//	cout<<"\n";
    }
   // printf(" \n");
}
glFlush();
dfs(0, _s);
//printf("\n");
for(int i = 0; i < maxn; i++)adj[i].clear(), visit[i] = 0;

drawSol(_s);

path.clear();
glFlush();
    }

    int main(int argc ,char** argv)
    {
    	glutInit(&argc,argv);
    	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    	glutInitWindowPosition(0,0);
    	glutInitWindowSize(1366,768);
    	glutCreateWindow("first prog");
    	init();
    	glutDisplayFunc(display);

    	glutMainLoop();
    	return 0;
    }
