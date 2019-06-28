#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <string>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <winsock2.h>

#define ID_PRZYCISK1 1
#define ID_PRZYCISK2 2
#define ID_PRZYCISK3 3
#define ID_PRZYCISK4 4
#define ID_PRZYCISK5 5
#define ID_PRZYCISK6 6
#define ID_PRZYCISK7 7
#define ID_PRZYCISK8 8
#define ID_PRZYCISK9 9
#define ID_PRZYCISK0 0
#define ID_PRZYCISKOK 1234

using namespace std;

    int ilosc = 0;      //ilosc kont
    int ile = 20;       //przesuniecie w tablicy ASCII do szyfrowania

    int sprawdz = 0;
    bool dziala = true;
    bool warunek_loginu = false;
    bool warunek_pinu = false;
    bool warunek_danych = false;
    bool warunek_wyplaty = false;
    bool koniec = false;
    int rodzaj_operacji = 0;
    int numer = 0;
    int pin = 0;
    int kasa = 0;
    int konto = 0;
    int numer_konta = 0;
    int kwota_do_wyplaty = 0;
    int kwota_do_wplaty = 0;
    int dane = 0;

    int *saldo;
    int *login;
    int *haslo;
    string *wlasciciel;

    string imie;
    string pobierz;
    string zamieniony;

    char* str_3;

    //zmienne do winsock

    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    char *sendbuf;
    char recvbuf[100] = " ";

    SOCKET mainSocket;

void funkcja_sendbuf()
{
    bytesSent = send( mainSocket, sendbuf, strlen(sendbuf), 0 );
}

void funkcja_recvbuf()
{
    memset(recvbuf, 0, 100 * sizeof(int));
    bytesRecv = recv( mainSocket, recvbuf, 100, 0 );
}

void kasowanie_memset()
{
    memset(recvbuf, 0, 100 * sizeof(int));
}

void pobierz_dane()
{
        int t = 0;
        int p = 0;
        ilosc = 0;
        saldo = new int[100];
        login = new int[100];
        haslo = new int[100];
        wlasciciel = new string[100];
        ifstream pobierz_dane("baza_danych.txt" , ios::in);
        if(pobierz_dane.good()==false)
        {
            cout << " Plik nie istnieje !! " << endl;
        }
        while( !pobierz_dane.eof() )
        {
            getline(pobierz_dane , imie);
            wlasciciel[ilosc] = imie;

            getline(pobierz_dane , pobierz);
            numer = atoi(pobierz.c_str());
            login[ilosc] = numer;

            getline(pobierz_dane , pobierz);
            kasa = atoi(pobierz.c_str());
            saldo[ilosc] = kasa;

            getline(pobierz_dane , pobierz);
            p = pobierz.length();
            for (t = 0; t < p; t++)
            {
                pobierz[t] -= ile;
            }
            pin = atoi(pobierz.c_str());
            haslo[ilosc] = pin;

            ilosc++;
        };
        pobierz_dane.close();
}

void zapisz_dane()
{
        int t = 0;
        int y = 0;
        int p = 0;
        ofstream zapisz_dane("baza_danych.txt" , ios::trunc);
              for(t = 0; t < ilosc; t++)
              {
                zapisz_dane << wlasciciel[t] << endl ;
                zapisz_dane << login[t] << endl ;
                zapisz_dane << saldo[t] << endl ;
                pin = haslo[t];
                ostringstream zins;
                zins << pin;
                pobierz = zins.str();
                p = pobierz.length();
                for (y = 0; y < p; y++)
                {
                    pobierz[y] += ile;
                }
                if(t == (ilosc - 1))
                {
                    zapisz_dane << pobierz;
                }
                else
                {
                  zapisz_dane << pobierz << endl ;
                }
              }
        zapisz_dane.close();
}

void zamiana_sprawdz_na_sendbuf()
{
    ostringstream ns;
    ns << sprawdz;
    zamieniony = ns.str();
    sendbuf = new char[ zamieniony.size() + 1 ];
    strcpy( sendbuf, zamieniony.c_str() );
}

void zamiana_kasa_na_sendbuf()
{
    ostringstream ns;
    ns << kasa;
    zamieniony = ns.str();
    sendbuf = new char[ zamieniony.size() + 1 ];
    strcpy( sendbuf, zamieniony.c_str() );
}

void sprawdzanie_numeru_konta()
{
    int w = 0;
    numer_konta = atoi(recvbuf);
    kasowanie_memset();
    for(w = 0; w < ilosc; w++)
    {
        if(numer_konta == login[w])
        {
            konto = w;
            sprawdz = 1;
            zamiana_sprawdz_na_sendbuf();
            warunek_loginu = true;
            cout << " Numer konta jest poprawny!" << endl;
            break;
        }
        else
        {
            sprawdz = 0;
            zamiana_sprawdz_na_sendbuf();
            warunek_loginu = false;
            cout << " Numer konta jest bledny!" << endl;
        }
    }
}

void sprawdzanie_pinu()
{
    pin = atoi(recvbuf);
    if(pin == haslo[konto])
    {
        sprawdz = 1;
        zamiana_sprawdz_na_sendbuf();
        warunek_pinu = true;
        cout << " Kod PIN jest poprawny!" << endl;
    }
    else
    {
        sprawdz = 0;
        zamiana_sprawdz_na_sendbuf();
        warunek_pinu = false;
        warunek_loginu = false;
        cout << " Numer konta oraz kod PIN nie zgadzaja sie!" << endl;
    }
}

void wysylanie_danych_wlasciciela()
{
    dane = atoi(recvbuf);
    memset(recvbuf, 0, 100 * sizeof(int));
    kasowanie_memset();
    if(dane == 1)
    {
        kasa = saldo[konto];
        zamiana_kasa_na_sendbuf();
        warunek_danych = false;
        cout << " Wyslano saldo!" << endl;
    }
    if(dane == 2)
    {
        imie = wlasciciel[konto];
        str_3 = new char[ imie.size() + 1 ];
        strcpy( str_3, imie.c_str() );
        sendbuf = str_3;
        warunek_danych = false;
        cout << " Wysylanio imie i nazwisko wlasciciela!" << endl;
    }
    if(dane == 3)
    {
        sprawdz = 0;
        sendbuf = (char*)"abc";
        warunek_danych = true;
        cout << " Wyslano dane wlasciciela!" << endl;
    }
}

void wyplata()
{
    kwota_do_wyplaty = atoi(recvbuf);
    if(kwota_do_wyplaty > kasa)
    {
        sprawdz = 0;
        zamiana_sprawdz_na_sendbuf();
        warunek_wyplaty = false;
        cout << " Brak mozliwosci wyplaty!" << endl;
    }
    else
    {
        kasa -= kwota_do_wyplaty;
        saldo[konto] = kasa;
        zapisz_dane();
        sprawdz = 1;
        zamiana_sprawdz_na_sendbuf();
        warunek_wyplaty = true;
        cout << " Wyplata!" << endl;
    }
}

void wplata()
{
    kwota_do_wplaty = atoi(recvbuf);
    kasa += kwota_do_wplaty;
    saldo[konto] = kasa;
    zapisz_dane();
    cout << " Wplata!" << endl;
}

void obsluga_bankomatu()
{
    pobierz_dane();
    kasowanie_memset();
    sprawdz = 0;
    do
    {

    //sprawdzanie loginu

    do
    {
        funkcja_recvbuf();
        sprawdzanie_numeru_konta();
        funkcja_sendbuf();
    }while(warunek_loginu == false);

    //sprawdzanie pinu

        sprawdz = 0;
        funkcja_recvbuf();
        sprawdzanie_pinu();
        funkcja_sendbuf();
    }while(warunek_pinu == false);

    //wysylanie danych

    sprawdz = 0;
    do
    {
        funkcja_recvbuf();
        wysylanie_danych_wlasciciela();
        funkcja_sendbuf();
    }while(warunek_danych == false);

    //obsluga menu

    funkcja_recvbuf();
    rodzaj_operacji = atoi(recvbuf);
    sprawdz = 0;
    if(rodzaj_operacji == 1)
    {
        cout << " Wybrano wyplate gotowki!" << endl;
        do
        {
        funkcja_recvbuf();
        wyplata();
        funkcja_sendbuf();
        }while(warunek_wyplaty == false);
    }
    if(rodzaj_operacji == 2)
    {
        cout << " Wybrano wplate gotowki!" << endl;\
        funkcja_recvbuf();
        wplata();
        sendbuf = (char*)"1";\
        funkcja_sendbuf();
    }
    dziala = true;
}

int main ()
{
 pobierz_dane();

WSADATA wsaData;

int result = WSAStartup( MAKEWORD( 2, 2 ), & wsaData );
if( result != NO_ERROR )
     printf( " Initialization error.\n" );

	mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
if( mainSocket == INVALID_SOCKET )
{
    printf( " Error creating socket: %d\n", WSAGetLastError() );
    WSACleanup();
    return 1;
}

	sockaddr_in service;
memset( & service, 0, sizeof( service ) );
service.sin_family = AF_INET;
service.sin_addr.s_addr = inet_addr( "127.0.0.1" );
service.sin_port = htons( 27015 );

	if( bind( mainSocket,( SOCKADDR * ) & service, sizeof( service ) ) == SOCKET_ERROR )
{
    printf( " bind() failed.\n" );
    closesocket( mainSocket );
    return 1;
}

if( listen( mainSocket, 1 ) == SOCKET_ERROR )
     printf( " Error listening on socket.\n" );

     SOCKET acceptSocket = SOCKET_ERROR;
printf( " Waiting for a client to connect...\n" );

while( acceptSocket == SOCKET_ERROR )
{
    acceptSocket = accept( mainSocket, NULL, NULL );
}

mainSocket = acceptSocket;

funkcja_recvbuf();
cout <<  " " << recvbuf << endl;

sendbuf = (char*)" Server connected!";
funkcja_sendbuf();

do
{
    obsluga_bankomatu();
    cout << " Logowanie ponowne!" << endl;
}while(dziala == true);

}
