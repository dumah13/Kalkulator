#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAXSTRLEN 100

struct LiczbaZespolona {
	double czesc_rzeczywista;
	double czesc_urojona;
}; typedef struct LiczbaZespolona Zesp;

struct ElementStosu {
	Zesp wartosc;
	struct ElementStosu* Poprzedni;
};  typedef struct ElementStosu Element;

Element* szczyt_stosu = NULL;

void init();
Zesp Pop();
Zesp Dodaj();
Zesp Odejmij();
Zesp Podziel();
Zesp Pomnoz();
int ZdekodujWyrazenie(char* bufor);
void WyswietlStos();
void Wyczysc_Stos();
void Push(Zesp liczba);
char* WczytajKonsola(char komunikat[]);
void Zwolnij_Pamiec();

int main(int argc, char* argv[]) {
	char* bufor;
	int krok_programu = 0;
	Zesp i = { 5, 1 };
	Zesp j = { 6, 3 };
	Zesp liczba;

	init();

	for (;;)
	{
		switch (krok_programu) {
		case 0:
			printf("----\tKalkulator Liczb Zespolonych\t----\n");
			krok_programu = 1;
			break;
		case 1:
			krok_programu = 2;
			break;
		case 2:
			printf("Stos:\n");

			WyswietlStos();

			if ((bufor = WczytajKonsola("\nPodaj wyrazenie\ndozwolone dzialania: \n/\n*\n+\n-\n= aby zakonczyc i wyczyscic stos\n\n")) == -1) { krok_programu = 100; continue; };
			if (ZdekodujWyrazenie(bufor) == -1) { krok_programu = 3; continue; };
			liczba = Pop();
			Push(liczba);
			printf("Wynik wyrazenia:\n%.2f%+.2fi\n",liczba.czesc_rzeczywista, liczba.czesc_urojona);
			break;
		case 3:
			liczba = Pop();
			Wyczysc_Stos();
			printf("Wynik koncowy to:\n%.2f%+.2fi\n", liczba.czesc_rzeczywista, liczba.czesc_urojona);
			krok_programu = 0;
			break;
		case 100:
			Wyczysc_Stos();
			Zwolnij_Pamiec();
			return 0;
		}
	}
}

void WyswietlStos() {
	Element* iterator_stosu;

	iterator_stosu = szczyt_stosu;
	while (iterator_stosu->Poprzedni != NULL) {
		printf("%.2f%+.2fi\n", iterator_stosu->wartosc.czesc_rzeczywista, iterator_stosu->wartosc.czesc_urojona);
		iterator_stosu = iterator_stosu->Poprzedni;
	}
}

void init() {
	Zesp zero = { 0, 0 };
	Push(zero);
}

void Push(Zesp liczba) {
	Element* nowy_element = malloc(sizeof(Element));
	nowy_element->wartosc = liczba;
	nowy_element->Poprzedni = szczyt_stosu;
	szczyt_stosu = nowy_element;
}

Zesp Pop() {
	Zesp liczba = szczyt_stosu->wartosc;

	if (szczyt_stosu->Poprzedni == NULL) {
		printf("Blad 1 - osiagnieto koniec stosu.\n");
		return liczba;
	}

	Zwolnij_Pamiec();

	return liczba;
}

void Zwolnij_Pamiec() {
	Element* stary_szczyt = szczyt_stosu;
	szczyt_stosu = szczyt_stosu->Poprzedni;
	free(stary_szczyt);
}

void Wyczysc_Stos() {
	while (szczyt_stosu->Poprzedni != NULL) {
		Zwolnij_Pamiec();
	}
}

char* WczytajKonsola(char komunikat[]) {
	char c;
	char* bufor = calloc(MAXSTRLEN, sizeof(&bufor));

	printf("\n %s \nWcisnij ',' aby wyjsc z programu\n\n", komunikat);
	gets(bufor);
	if (bufor == NULL) {
		printf("Blad 2 - Blad odczytu");
	}
	else if (bufor[0] == ',' && strlen(bufor) == 1) {
		printf("\nKoncze program\n");
		return -1;
	}

	return bufor;
}

Zesp Dodaj() {
	Zesp y = Pop();
	Zesp x = Pop();
	Zesp wynik = { x.czesc_rzeczywista + y.czesc_rzeczywista, x.czesc_urojona + y.czesc_urojona };

	Push(wynik);

	return wynik;
}

Zesp Odejmij() {
	Zesp y = Pop();
	Zesp x = Pop();
	Zesp wynik = { x.czesc_rzeczywista - y.czesc_rzeczywista, x.czesc_urojona - y.czesc_urojona };

	Push(wynik);

	return wynik;
}

Zesp Pomnoz() {
	Zesp y = Pop();
	Zesp x = Pop();
	Zesp wynik;

	wynik.czesc_rzeczywista = (x.czesc_rzeczywista * y.czesc_rzeczywista - x.czesc_urojona * y.czesc_urojona);
	wynik.czesc_urojona = (x.czesc_rzeczywista * y.czesc_urojona + y.czesc_rzeczywista * x.czesc_urojona);

	Push(wynik);

	return wynik;
}

Zesp Podziel() {
	Zesp y = Pop();
	Zesp x = Pop();
	Zesp temp;
	Zesp wynik;
	int temp2;

	y.czesc_urojona *= -1;
	Push(x);
	Push(y);
	Pomnoz();
	temp = Pop();
	temp2 = (pow(y.czesc_rzeczywista, 2) + pow(y.czesc_urojona, 2));

	if (!temp2) {
		printf("\nNiedozwolone dzielenie przez 0!\n");
		y.czesc_urojona *= -1;
		Push(x);
		Push(y);
		wynik.czesc_rzeczywista = 0;
		wynik.czesc_urojona = 0;
		return wynik;
	}

	wynik.czesc_rzeczywista = temp.czesc_rzeczywista / temp2;
	wynik.czesc_urojona = temp.czesc_urojona / temp2;

	Push(wynik);

	return wynik;
}

int ZdekodujWyrazenie(char* bufor) {
	int rzecz;
	int uroj;
	int status = 1;
	int rzeczywista_zapelniona = 0;
	char oper;
	const char s[2] = " ";
	char* token = calloc(strlen(bufor, sizeof(*token)));
	Zesp liczba_zesp;
	Zesp wynik;

	token = strtok(bufor, s);

	while (token)
	{
		if ((status = sscanf(token, "%d", &rzecz)) == 1) {//wczytano liczbe zespolona
			if (!rzeczywista_zapelniona)
			{
				rzeczywista_zapelniona = 1;
				liczba_zesp.czesc_rzeczywista = rzecz;
				token = strtok(NULL, s);
				continue;
			}
			else {
				rzeczywista_zapelniona = 0;
				liczba_zesp.czesc_urojona = rzecz;
				Push(liczba_zesp);
			}
		}
		else if (status = sscanf(token, "%c", &oper)) {
			if (rzeczywista_zapelniona)
			{
				rzeczywista_zapelniona = 0;
				liczba_zesp.czesc_urojona = 0;
				Push(liczba_zesp);
			}

			switch (oper) {
			case '+':
				Dodaj();
				break;
			case '-':
				Odejmij;
				break;
			case '*':
				Pomnoz();
				break;
			case '/':
				Podziel();
				break;
			case '=':
				return -1;
			case 'i':
				token[0] = ' ';
				continue;
			default:
				printf("\nNierozpoznany operator %c.\n", oper);
				break;
			}
		}
		token = strtok(NULL, s);
	}

	if (rzeczywista_zapelniona)
	{
		rzeczywista_zapelniona = 0;
		liczba_zesp.czesc_urojona = 0;
		Push(liczba_zesp);
	}

}