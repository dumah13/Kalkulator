#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <string.h>

//Maksymalna dlugosc tablicy znakow (stringa) bufora Inputow
#define MAXSTRLEN 1000

//Struktura liczby zespolonej na ktorej wykonywane sa obliczenia
struct LiczbaZespolona {
	double czesc_rzeczywista;
	double czesc_urojona;
}; typedef struct LiczbaZespolona Zesp;


//Struktura Elementu stosu
struct ElementStosu {
	Zesp wartosc;
	struct ElementStosu* Poprzedni;
};  typedef struct ElementStosu Element;

//Zmienna globalna pelniaca funkcje Stack Pointera - wskaznika stosu
Element* szczyt_stosu = NULL;

#pragma region NaglowkiFunkcji

//Inicjalizacja stosu
void init();

//Sprawdza, czy stos jest pusty
//Zwraca: 1 - pusty stos, 0 - niepusty stos
int Puste();

//Zdejmuje Element ze stosu
//Zwraca: Zdjety Element badz 0 gdy stos jest pusty
Zesp Pop();

//Dodaje i zdejmuje dwie liczby zespolone z gory stosu i odklada wynik na stos
//Zwraca: Wynik dodawania
Zesp Dodaj();

//Odejmuje i zdejmuje dwie liczby zespolone z gory stosu i odklada wynik na stos
//Zwraca: Wynik odejmowania
Zesp Odejmij();

//Dzieli i zdejmuje dwie liczby zespolone z gory stosu i odklada wynik na stos
//Zwraca: Wynik dzielenia lub 0 w przypadku dzielenia przez 0
Zesp Podziel();

//Mnozy i zdejmuje dwie liczby zespolone z gory stosu i odklada wynik na stos
//Zwraca: Wynik mnozenia
Zesp Pomnoz();

//Konwertuje wpisane przez uzytkownika wyrazenie w serie polecen w celu wykonania dzialan
//Zwraca: Kod polecenia (0 - wykonano dzialania, -1 - zakoncz dzialanie)
int Zdekoduj_Wyrazenie(char* bufor);

//Wyswietla w konsoli Stos
void Wyswietl_Stos();

//Usuwa wszystkie elementy stosu (poza podstawa)
void Wyczysc_Stos();

//Odklada liczbe zespolona na stos
void Push(Zesp liczba);

//Wyswietla zapytanie o Input od uzytkownika z podanym komunikatem
//Zwraca: adres bufora z wpisanym przez uzytkownika Inputem
char* Wczytaj_Konsola(char komunikat[]);

//Usuwa i zwalnia z pamieci najwyzej polozony element stosu
void Zwolnij_Pamiec();

#pragma endregion Deklaracja naglowkow funkcji oraz ich opisy.


int main(int argc, char* argv[]) {
	char* bufor = malloc(1);
	int krok_programu = 0;
	Zesp liczba;

	init();

	for (;;) //Nieskonczona petla dzialania programu
	{
		switch (krok_programu) {
		case 0://Wyswietlanie naglowka programu
			printf("----\tKalkulator Liczb Zespolonych\t----\n");
			krok_programu = 1;
			break;
		case 1://Petla wpisywania wyrazen
			printf("Stos:\n");

			Wyswietl_Stos();
			Puste() ? printf("Pusty stos.\n") : printf("\n");
			free(bufor);

			if ((bufor = Wczytaj_Konsola("Podaj wyrazenie\ndozwolone dzialania: \n/\n*\n+\n-\n= aby zakonczyc i wyczyscic stos\n\n")) == -1) { krok_programu = 100; continue; };
			if (Zdekoduj_Wyrazenie(bufor) == -1) { krok_programu = 2; continue; };
			liczba = Pop();
			Push(liczba);
			system("cls");
			printf("Wynik wyrazenia:\n%.2f%+.2fi\n",liczba.czesc_rzeczywista, liczba.czesc_urojona);
			break;
		case 2://Zakoncz wpisywanie obecnego dzialania i wyczysc stos
			liczba = Pop();
			Wyczysc_Stos();
			system("cls");
			printf("Wynik koncowy to:\n%.2f%+.2fi\n", liczba.czesc_rzeczywista, liczba.czesc_urojona);
			krok_programu = 0;
			break;
		case 100://Zamkniecie programu
			Wyczysc_Stos();
			Zwolnij_Pamiec();
			return 0;
		}
	}
}

void Wyswietl_Stos() {
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

	if (Puste()) {
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
	while (!Puste()) {
		Zwolnij_Pamiec();
	}
}

char* Wczytaj_Konsola(char komunikat[]) {
	char c;
	char* bufor = calloc(MAXSTRLEN, sizeof(*bufor));

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

int Puste() {
	return szczyt_stosu->Poprzedni == NULL ? 1 : 0;
}

int Zdekoduj_Wyrazenie(char* bufor) {
	double rzecz;
	int uroj;
	int status = 1;
	int rzeczywista_zapelniona = 0;
	char oper;
	const char s[2] = " ";
	char* token;
	Zesp liczba_zesp;
	Zesp wynik;

	token = strtok(bufor, s);

	while (token)
	{
		if ((status = sscanf(token, "%lf", &rzecz)) == 1) {//wczytano liczbe zespolona
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
				Odejmij();
				break;
			case '*':
				Pomnoz();
				break;
			case '/':
				Podziel();
				break;
			case '=':
				//free(token);
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
	//free(token);
	return 0;
}