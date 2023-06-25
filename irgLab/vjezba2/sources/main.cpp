// Local Headers

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

struct Tocka
{
	int x;
	int y;
};

struct Brid
{
	int a;
	int b;
	int c;
} ;

struct Poligon
{
	Tocka vrh;
	Brid brid;
	bool lijevo;
};

int drawed = 0;
int width = 97, height = 97;

Poligon poligon;

std::vector<std::pair<int, int>> klikovi;
std::vector<std::pair<int, int>> positions;
vector<Poligon> poligoni;
vector<Tocka> tocke;
vector<pair<int, int>> unutra;

bool drawingComplete = false;
bool allowDrawing = false;

void bresenham(Grafika &grafika, int x0, int y0, int x1, int y1);
void napraviBrid();
void checkPoly();
void checkDot();

void lineDrawSetup(Grafika &grafika, Poligon &poligon1, Poligon poligon2)
{
	bresenham(grafika, poligon1.vrh.x, height - poligon1.vrh.y - 1, poligon2.vrh.x, height - poligon2.vrh.y - 1);
	if(poligoni.size() > 2){
		napraviBrid();
		checkPoly();
	}
}

void checkDot(int x, int y){
	int sum = 0;
	int i, r, iznad, ispod, na;

	for (size_t i = 0; i < poligoni.size() - 1; i++)
	{
		sum += (poligoni[i+1].vrh.x - poligoni[i].vrh.x) * (poligoni[i+1].vrh.y + poligoni[i].vrh.y);
	}
	sum += (poligoni[0].vrh.x - poligoni[poligoni.size() - 1].vrh.x) * (poligoni[0].vrh.y + poligoni[poligoni.size() - 1].vrh.y);

	ispod = iznad = na = 0;
	for(i = 0; i < poligoni.size(); i++){
		r = poligoni[i].brid.a*x + poligoni[i].brid.b*y + poligoni[i].brid.c;
		if(r > 0){
			iznad++;
		} else if(r < 0){
			ispod++;
		} else {
			na++;
		}
	}

	if(sum > 0 && iznad == 0){
		cout << "Tocka je unutar poligona" << endl;
		unutra.push_back(make_pair(x, y));
	} else if(sum < 0 && ispod == 0){
		cout << "Tocka je unutar poligona" << endl;
		unutra.push_back(make_pair(x, y));
	} else {
		cout << "Tocka je izvan poligona" << endl;
		positions.push_back(make_pair(x, height - y - 1));
	}
	
}

void drawPoly(Grafika &grafika){
	int i, i0, y, xmin, xmax, ymin, ymax;
	double L, D, x;

	xmin = xmax = poligoni[0].vrh.x;
    ymin = ymax = poligoni[0].vrh.y;
    for (i = 1; i < poligoni.size(); i++) {
        if (poligoni[i].vrh.x < xmin) xmin = poligoni[i].vrh.x;
        else if (poligoni[i].vrh.x > xmax) xmax = poligoni[i].vrh.x;
    	if (poligoni[i].vrh.y < ymin) ymin = poligoni[i].vrh.y;
        else if (poligoni[i].vrh.y > ymax) ymax = poligoni[i].vrh.y;
    }
	
	
	for (y = ymin; y <= ymax; y++) {
        L = xmin;D = xmax;
        i0 = poligoni.size() - 1;
        for (i = 0; i < poligoni.size(); i0 = i++) {
            if (poligoni[i0].brid.a == 0) {
                if (poligoni[i0].vrh.y == y) {
                    if (poligoni[i0].vrh.x < poligoni[i].vrh.x) {
                        L = poligoni[i0].vrh.x;
                        D = poligoni[i].vrh.x;
                    }
                    else {
                        L = poligoni[i].vrh.x;
                        D = poligoni[i0].vrh.x;
                    }
                    break;
                }
            }
            else {
                x = (-poligoni[i0].brid.b * y - poligoni[i0].brid.c) / (double)poligoni[i0].brid.a;
                if (!poligoni[i0].lijevo) {
                    if (L < x)
                        L = x;
                }
                else {
                    if (D > x)
                        D = x;
                }
            }
        }
        bresenham(grafika, round(L), height - y - 1, round(D), height - y - 1);
    }

	allowDrawing = false;
	drawingComplete = true;
}

void checkPoly(){
	int i, i0, r, iznad, ispod, na;

	ispod = iznad = na = 0;
	i0 = poligoni.size() - 2;
	for(i = 0; i < poligoni.size(); i++, i0++){
		if(i0>= poligoni.size()) i0 = 0;
		r = poligoni[i0].brid.a*poligoni[i].vrh.x + poligoni[i0].brid.b*poligoni[i].vrh.y + poligoni[i0].brid.c;
		if(r == 0) na++;
		else if(r > 0) iznad++;
		else ispod++;
	}

	if(ispod != 0 && iznad != 0){
		cout << "Poligon nije konveksan!" << endl;
	}
}

void napraviBrid()
{
	int i0 = poligoni.size() - 2;
	for (int i = 0; i < poligoni.size() - 1; i++) {
		Brid brid;
		brid.a = poligoni[i0].vrh.y - poligoni[i].vrh.y;
		brid.b = -(poligoni[i0].vrh.x - poligoni[i].vrh.x);
		brid.c = poligoni[i0].vrh.x * poligoni[i].vrh.y - poligoni[i0].vrh.y * poligoni[i].vrh.x;
		poligoni[i0].brid = brid;
		poligoni[i0].lijevo = (poligoni[i0].vrh.y < poligoni[i].vrh.y);
		i0 = i;
	}
	
}

void drawLine(Grafika &grafika, int xs, int ys, int xe, int ye)
{
	int x, yc, korr, a, yf;
	
	if (ye - ys <= xe - xs) {
		a = 2*(ye - ys);
		yc = ys; yf = -(xe - xs); korr = - 2*(xe-xs);
		for (x = xs; x <= xe; x++) {
			grafika.osvijetliFragment(x, yc, glm::vec3(0.6, 0.2, 0));\
			positions.push_back(std::make_pair(x, yc));
			yf += a;
			if (yf > 0) {
				yf += korr;
				yc++;
			}
		}
	} else {
		x = xe; xe = ye; ye = x;
		x = xs; xs = ys; ys = x;
		a = 2*(ye - ys);
		yc = ys; yf = - (xe - xs); korr = -2 *(xe - xs);
		for (x = xs; x <= xe; x++) {
			grafika.osvijetliFragment(yc, x, glm::vec3(0.6, 0.2, 0));
			positions.push_back(std::make_pair(yc, x));
			yf += a;
			if(yf > 0) {
				yf += korr;
				yc++;
			}
		}
	}
}

void drawLineInverted(Grafika &grafika, int xs, int ys, int xe, int ye)
{
	int x, yc, korr, a, yf;
	if ( -(ye-ys) <= (xe-xs)) {
		a = 2*(ye - ys);
		yc = ys; yf = (xe - xs); korr = 2*(xe - xs);
		for(x = xs; x <= xe; x++) {
			grafika.osvijetliFragment(x, yc, glm::vec3(0.6, 0.2, 0));
			positions.push_back(std::make_pair(x, yc));
			yf += a;
			if (yf <= 0) {
				yf += korr;
				yc--;
			}
		}
	} else {
		x = xe; xe = ys; ys = x;
		x = xs; xs = ye; ye = x;
		yc = ys; yf=(xe - xs); korr = 2*(xe - xs);
		a = 2 * (ye - ys);
		for (x = xs; x <= xe; x++) {
			grafika.osvijetliFragment(yc, x, glm::vec3(0.6, 0.2, 0));
			positions.push_back(std::make_pair(yc, x));
			yf += a;
			if (yf <= 0) {
				yf += korr;
				yc--;
			}
		}
	}
}

void bresenham(Grafika &grafika, int x0, int y0, int x1, int y1)
{
	if (x0 <= x1)
	{
		if (y0 <= y1)
		{
			drawLine(grafika, x0, y0, x1, y1);
		}
		else
		{
			drawLineInverted(grafika, x0, y0, x1, y1);
		}
	}
	else
	{
		if (y0 >= y1)
		{
			drawLine(grafika, x1, y1, x0, y0);
		}
		else
		{
			drawLineInverted(grafika, x1, y1, x0, y0);
		}
	};
	drawed += 2;
}

void klikMisa(int x, int y, int vrsta)
{

	if (vrsta == 0){
		Poligon poligon;
		Tocka tocka;
		tocka.x = x;
		tocka.y = y;
		poligon.vrh = tocka;
		tocke.push_back(tocka);
		poligoni.push_back(poligon);
		klikovi.push_back(std::make_pair(x, y));
	}
	if (vrsta == 1){
		if(!drawingComplete){
			allowDrawing = true;
		} else {
			checkDot(x, y);
		}
	}
}

int main(int argc, char *argv[])
{
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	// prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti())
	{

		// osvjezavanje pozicije razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		grafika.pobrisiProzor();

		// iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
		{
			for (int j = 0; j < width; j += 1)
			{
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}
		}

		if (allowDrawing && !drawingComplete){
			drawPoly(grafika);
		}
		

		for (int i = 0; i < positions.size(); i++)
			grafika.osvijetliFragment(positions[i].first, positions[i].second, glm::vec3(0.6, 0.2, 0));

		for (size_t i = 0; i < unutra.size(); i++)
		{
			grafika.osvijetliFragment(unutra[i].first, height - unutra[i].second - 1, glm::vec3(0.2, 0.6, 0));
		}
		

		// iscrtavanje pritisnutih fragmenata
		// ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
		if (poligoni.size() > 1)
		{
			lineDrawSetup(grafika, poligoni[poligoni.size() - 2], poligoni[poligoni.size() - 1]);
		}

		for (int i = 0; i < klikovi.size(); i++)
			grafika.osvijetliFragment(klikovi[i].first, height - klikovi[i].second - 1, glm::vec3(0.6, 0.2, 0));

		grafika.iscrtajRaster();

		// kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		// kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));
	}

	return EXIT_SUCCESS;
}
