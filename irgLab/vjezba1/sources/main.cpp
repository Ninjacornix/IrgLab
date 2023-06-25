// Local Headers

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

std::vector<std::pair<int, int>> klikovi;
std::vector<std::pair<int, int>> positions;

bool odsjecanje = false;
bool draw = true;
int width = 100, height = 100;
int drawed = 0;

void bresenham(Grafika &grafika, int x0, int y0, int x1, int y1);

void drawLine(Grafika &grafika, int xs, int ys, int xe, int ye)
{
	int x, yc, korr, a, yf;
	
	if (ye - ys <= xe - xs) {
		a = 2*(ye - ys);
		yc = ys; yf = -(xe - xs); korr = - 2*(xe-xs);
		for (x = xs; x <= xe; x++) {
			if(!odsjecanje){
				grafika.osvijetliFragment(x, yc, glm::vec3(0.6, 0.2, 0));
				positions.push_back(std::make_pair(x, yc));
			} else {
				if((x >= 25 && x <= 75) && (yc >= 25 && yc <= 75)){
					grafika.osvijetliFragment(x, yc, glm::vec3(0.6, 0.2, 0));
					positions.push_back(std::make_pair(x, yc));
				}
			}
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
			if(!odsjecanje){
				grafika.osvijetliFragment(yc, x, glm::vec3(0.6, 0.2, 0));
				positions.push_back(std::make_pair(yc, x));
			} else {
				if ((x >= 25 && x <= 75) && (yc >= 25 && yc <= 75))
				{
					grafika.osvijetliFragment(yc, x, glm::vec3(0.6, 0.2, 0));
					positions.push_back(std::make_pair(yc, x));
				}
			}
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
			if(!odsjecanje){
				grafika.osvijetliFragment(x, yc, glm::vec3(0.6, 0.2, 0));\
				positions.push_back(std::make_pair(x, yc));
			} else {
				if((x >= 25 && x <= 75) && (yc >= 25 && yc <= 75)){
					grafika.osvijetliFragment(x, yc, glm::vec3(0.6, 0.2, 0));
					positions.push_back(std::make_pair(x, yc));
				}
			}
			yf += a;
			if (yf <= 0) {
				yf += korr;
				yc--;
			}
		}
	} else {
		cout << "Inverted" << endl;
		x = xe; xe = ys; ys = x;
		x = xs; xs = ye; ye = x;
		yc = ys; yf=(xe - xs); korr = 2*(xe - xs);
		a = 2 * (ye - ys);
		for (x = xs; x <= xe; x++) {
			if(!odsjecanje){
				grafika.osvijetliFragment(yc, x, glm::vec3(0.6, 0.2, 0));
				positions.push_back(std::make_pair(yc, x));
			} else {
				if ((x >= 25 && x <= 75) && (yc >= 25 && yc <= 75))
				{
					grafika.osvijetliFragment(yc, x, glm::vec3(0.6, 0.2, 0));
					positions.push_back(std::make_pair(yc, x));
				}
			}
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
	}
	drawed += 2;
}

void inverse(){
	odsjecanje = !odsjecanje;
}

void klikMisa(int x, int y, int vrsta)
{
	if (vrsta == 0){
		std::cout << "lijevi:";
		klikovi.push_back(std::make_pair(x, y));
	}
	if (vrsta == 1){
		std::cout << "desni:";
		inverse();
	}

}

int main(int argc, char *argv[])
{
	std::cout << argv[0] << std::endl;
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	// prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti())
	{

		// osvjezavanje pozicija razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		grafika.pobrisiProzor();

		// iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1)
			{
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}

		if (positions.size() > 0)
		{
			for (int i = 0; i < positions.size(); i++)
				grafika.osvijetliFragment(positions[i].first, positions[i].second, glm::vec3(0.6, 0.2, 0));
		}
		if(odsjecanje){
			for (int i = round(width/4); i <= 3 * round(width/4); i++){
				grafika.osvijetliFragment(i, round(height/4), glm::vec3(0.2, 0.6, 0));
			}

			for (int i = round(width/4); i <= 3 * round(width/4); i++){
				grafika.osvijetliFragment(i, 3 * round(height/4), glm::vec3(0.2, 0.6, 0));
			}

			for (int i = round(height/4); i <= 3 * round(height/4); i++){
				grafika.osvijetliFragment(round(width/4), i, glm::vec3(0.2, 0.6, 0));
			}

			for (int i = round(height/4); i <= 3 * round(height/4); i++){
				grafika.osvijetliFragment(3 * round(width/4), i, glm::vec3(0.2, 0.6, 0));
			}
				
		}

		// iscrtavanje pritisnutih fragmenata
		// ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
		for (int i = 0; i < klikovi.size(); i++)
			grafika.osvijetliFragment(klikovi[i].first, height - klikovi[i].second - 1, glm::vec3(0.6, 0.2, 0));

		if (drawed != klikovi.size() && klikovi.size() > 1 && klikovi.size() % 2 == 0)
		{
			bresenham(grafika, klikovi[klikovi.size() - 2].first, height - klikovi[klikovi.size() - 2].second - 1, klikovi[klikovi.size() - 1].first, height - klikovi[klikovi.size() - 1].second - 1);
		}

		grafika.iscrtajRaster();

		// kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		// kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));
	}

	return EXIT_SUCCESS;
}
