#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/*
    La funzione fatta dal prof per creare il file, e' fatto con i piedi e vsCode non crea il file .ps
    Ho dovuto cacciare tutto in un unico file, perche' vsCode mi odia e ha deciso di non voler integrare i file .h
*/

#ifndef LIBPSGRAPH_H
#define LIBPSGRAPH_H

static FILE *ps_file;
static double g_x, g_y;
static double dir;

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

/** Starts a new graph with given filename */
void start( const char *filename );

/** Ends the graph */
void end( void );

/** Draws a line of given length (in millimeters) */
void draw( double length );

/** Makes a move of given length (in millimeters), without drawing */
void move( double length );

/** Turns right of a given angle (in degrees) */
void turn( double ang );

#endif

double rad2deg(double);
void koch(double, int);
void kochProf(double, int);
void mainProf();

int main(){
    int order = 5;
    double length = 100;

    start("koch_snowflake.ps");
    for(int i=0; i<3; i++){
        koch(length, order);
        turn(rad2deg(M_PI*2/3));
    }
    end();

	return 0;
}

void koch(double x, int n){

    if(n == 0)  draw(x);
    else{
        koch(x/3, n-1);
        turn(rad2deg(-M_PI/3));
        koch(x/3, n-1);
        turn(rad2deg(M_PI*2/3));
        koch(x/3, n-1);
        turn(rad2deg(-M_PI/3));
        koch(x/3, n-1);
    }

}

double rad2deg(double x){
    return (x*180/M_PI);
}

void mainProf(){
    const int n = 4;
    int i;
    start("koch-curve.ps");    
    for (i=0; i<5; i++) {
        kochProf(50,i);
        move(-50); turn(-90); move(15); turn(90);
    }
    end();
    
    start("koch-showflake.ps");
    kochProf(50, n); turn(120);
    kochProf(50, n); turn(120);
    kochProf(50, n);                 
    end();
}

/* Disegna la curva di Koch di ordine n e lunghezza x */
void kochProf(double x, int n){
    if (n == 0) {
        draw(x);
    } else {
        koch(x/3, n-1);
        turn(-60);
        koch(x/3, n-1);
        turn(120);
        koch(x/3, n-1);
        turn(-60);
        koch(x/3, n-1);
    }

}

void start( const char *filename )
{
    if ((ps_file = fopen( filename, "wt" )) == NULL) {
        fprintf(stderr, "Can not open %s. Stop\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf( ps_file, "%%!PS-Adobe-2.0\n" );
    g_x = 2*72; 
    g_y = 7*72;
    fprintf( ps_file, "%d %d moveto\n", (int)(g_x), (int)(g_y) );
    dir = 0.0;
}

void end( void )
{
    fprintf( ps_file, "stroke\nshowpage\n" );
    fclose( ps_file );
}

void draw( double length )
{
    length /= 0.352777778;
    g_x += length * cos( dir );
    g_y += length * sin( dir );
    fprintf( ps_file, "%d %d lineto\n", (int)(g_x), (int)(g_y) );
}

void move( double length )
{
    length /= 0.352777778;
    g_x += length * cos( dir );
    g_y += length * sin( dir );
    fprintf( ps_file, "%d %d moveto\n", (int)(g_x), (int)(g_y) );
}

void turn( double ang )
{
    dir -= M_PI * ang / 180.0;
}