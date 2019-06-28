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
#include <cstdio>
#include <stdio.h>

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

    int j = 1;          //zienna do tworzenia przyciskow

    bool warunek_danych = false;

    int sprawdz = 0;
    int etap = 0;
    int liczba = 0;
    int numer = 0;
    int pin = 0;
    int kasa = 0;
    int funkcja_bankomatu = 0;
    int kwota_do_wyplaty = 0;
    int kwota_do_wplaty = 0;
    int proba_logowania = 1;
    int dane = 0;
    int numer_do_wyslania = 0;
    int rodzaj_operacji = 0;

    int *saldo;
    int *login;
    int *haslo;
    string *wlasciciel;

    string zamieniony;
    string imie;

/*  obsluga sieci  */

    char* sendbuf;
    char recvbuf[100];
    int bytesSent;
    int bytesRecv;

char str_1[100];
char str_2[100];
char* str_3;
char str_4[100];

double liczba1;

HWND hText[6];
HWND hStatic[6];
HWND hS_text[6];

SOCKET mainSocket;

double func(HWND handle)
{
    // Pobiera ilosc znaku w polu
    DWORD length = GetWindowTextLength(handle);
    // Alokuje Pamiec dla tych znakow
    LPSTR buf = (LPSTR) GlobalAlloc(GPTR, length);
    // Pobiera tekst z okna i zapisuje je w buforze
    GetWindowText(handle, (LPSTR) buf, length + 1);
    double wynik = atoi(buf);
    GlobalFree( buf );
    return wynik;
}

void value(int x1, int y1, int x2, int y2, HWND *hText, HWND hWnd, HINSTANCE hInstance)
{
    *hText = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT",
                               NULL, WS_CHILD | WS_VISIBLE ,
                               x1, y1, x2, y2, hWnd, NULL, hInstance, NULL);
}

void tekst(int x1, int y1, int x2, int y2, HWND *hStatic , HWND hWnd, HINSTANCE hInstance)
{
    *hStatic = CreateWindowEx(0, "STATIC",
                            NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
                            x1, y1, x2, y2, hWnd, NULL, hInstance, NULL);
}

void wstaw(HWND *hStatic, char *lancuch)
{
    SetWindowText(*hStatic, lancuch);
}

void funkcja_sendbuf()
{
    bytesSent = send( mainSocket, sendbuf, strlen(sendbuf), 0 );
}

void funkcja_recvbuf()
{
    memset(recvbuf, 0, 100 * sizeof(int));
    bytesRecv = recv( mainSocket, recvbuf, 100, 0 );
}

void funkcja_recvbuf_bez_memset()
{
    bytesRecv = recv( mainSocket, recvbuf, 100, 0 );
}

void kasowanie_memset()
{
    memset(recvbuf, 0, 100 * sizeof(int));
}

void wstaw_liczbe()
{
    ostringstream zz;
    ostringstream aa;
    aa << numer;
    zz << liczba;
    string bb = aa.str();
    string yy = zz.str();
    bb = bb + yy;
    istringstream cc(bb);
    cc >> numer;
}

void wstawianie_liczby_do_okna()  // wpisywanie i laczenie cyfr i ich wyswiatlanie po wpisaniu liczb z klawiatury
{
    ostringstream pp;
    pp << numer;
    string ww = pp.str();
    switch(etap)
    {
    case 1:
    {
    if(ww.size() >= 4)
    {
        numer = liczba;
    }
    else
    {
        wstaw_liczbe();
    }
    }
    break;
    case 2:
    {
    if(ww.size() >= 4)
    {
        numer = liczba;
    }
    else
    {
        wstaw_liczbe();
    }
    }
        break;
    case 4:
    {
        numer = liczba;
    }
        break;
    case 5:
    {
        wstaw_liczbe();
    }
        break;
    case 6:
    {
        wstaw_liczbe();
    }
        break;
    default:
        {

        }
    }
    liczba1 = numer;
    itoa(liczba1, str_1, 10);
    wstaw(&hText[0], str_1);
}

void zerowanie_okienka()
{
    numer = 0;
    wstaw(&hText[0], (char*) "");
}

void zamiana_recvbuf_na_sprawdz()
{
    sprawdz = atoi(recvbuf);
}

void zamiana_numer_do_wyslania_na_sendbuf()
{
    ostringstream ns;
    ns << numer_do_wyslania;
    zamieniony = ns.str();
    sendbuf = new char[ zamieniony.size() + 1 ];
    strcpy( sendbuf, zamieniony.c_str() );
}

void klawiatura()
{

}

void komunikaty_logowania()
{
    switch(proba_logowania)
    {
    case 1:
        {
            wstaw(&hStatic[0], (char*)" WITAMY!");
            wstaw(&hStatic[1], (char*)" PODAJ NUMER KONTA:  \n (4 CYFRY)");
        }
        break;
    case 2:
        {
            wstaw(&hStatic[0], (char*)" WITAMY!");
            wstaw(&hStatic[1], (char*)" PODAJ KOD PIN:  \n (4 CYFRY)");
        }
        break;
    case 3:
        {
            wstaw(&hStatic[0], (char*)" PODANY NUMER KONTA NIE ISTNIEJE!");
            wstaw(&hStatic[1], (char*)" PODAJ POPRAWNY NUMER KONTA:  \n (4 CYFRY)");
        }
        break;
    case 4:
        {
            wstaw(&hStatic[0], (char*)" PODANY NUMER KONTA I KOD PIN NIE ZGADZAJA SIE!");
            wstaw(&hStatic[1], (char*)" PODAJ POPRAWNY NUMER KONTA: \n (4 CYFRY)");
        }
        break;
    case 5:
        {
            wstaw(&hStatic[0], (char*)" PODANY NUMER KONTA I KOD PIN NIE ZGADZAJA SIE!");
            wstaw(&hStatic[1], (char*)" PODAJ POPRAWNY KOD PIN:  \n (4 CYFRY)");
        }
        break;
    case 6:
        {
            wstaw(&hStatic[0], (char*)" PODANY NUMER KONTA I KOD PIN ZGADZAJA SIE!");
            wstaw(&hStatic[1], (char*)" ");
            Sleep(1000);
            wstaw(&hStatic[1], (char*)" WITAMY! CO CHCESZ ZROBIC?");
            wstaw(&hStatic[0], (char*)" 1: WYPLACIC PIENIADZE   \n 2: WPLACIC PIENIADZE ");
            wstaw(&hStatic[2], (char*)" IMIE I NAZWISKO WLASCICIELA:");
            wstaw(&hStatic[3], str_3);
            wstaw(&hStatic[4], (char*)" SALDO:");
            wstaw(&hStatic[5], str_2);
        }
        break;
    case 7:
        {
            wstaw(&hStatic[0], (char*)" PODALES ZLY NUMER! WYBIERZ POPRAWNY NUMER! ");
            wstaw(&hStatic[1], (char*)" 1: WYPLACIC PIENIADZE   \n 2: WPLACIC PIENIADZE");
        }
        break;
    case 8:
        {
            wstaw(&hStatic[0], (char*)" PODAJ KWOTE WPLATY:");
            wstaw(&hStatic[1], (char*)" ");
        }
        break;
    case 9:
        {
            wstaw(&hStatic[0], (char*)" PODAJ KWOTE WYPLATY:");
            wstaw(&hStatic[1], (char*)" ");
        }
        break;
    case 10:
        {
            wstaw(&hStatic[0], (char*)" NIESTETY MASZ ZA MALO SRODKOW NA KONCIE!");
            wstaw(&hStatic[1], (char*)" PODAJ POPRAWNA KWOTE DO WYPLATY:");
        }
        break;
    case 11:
        {
            wstaw(&hStatic[0], (char*)" DZIEKUJEMY ZA SKORZYSTANIE Z \n NASZYCH USLUG! ZYCZYMY MILEGO DNIA!");
            wstaw(&hStatic[1], (char*)" NACISNIJ OK ABY ZALOGOWAC PONOWNIE!");
            wstaw(&hStatic[2], (char*)" ");
            wstaw(&hStatic[3], (char*)" ");
            wstaw(&hStatic[4], (char*)" ");
            wstaw(&hStatic[5], (char*)" ");
            etap = 7;
        }
        break;
    default:
        {

        }
    }
}

void sprawdzanie_numeru_konta()
{
    zamiana_numer_do_wyslania_na_sendbuf();
    funkcja_sendbuf();
    sprawdz = 0;
    funkcja_recvbuf();
    zamiana_recvbuf_na_sprawdz();
    if(sprawdz == 1)
    {
        etap = 2;
        if(proba_logowania == 4)
        {
            proba_logowania = 5;
        }
        else
        {
            proba_logowania = 2;
        }
    }
    else
    {
        etap = 1;
        proba_logowania = 3;
    }
}

void sprawdzanie_kodu_PIN()
{
    zamiana_numer_do_wyslania_na_sendbuf();
    funkcja_sendbuf();
    sprawdz = 0;
    funkcja_recvbuf();
    zamiana_recvbuf_na_sprawdz();
    if(sprawdz == 1)
    {
        etap = 3;
        proba_logowania = 6;
    }
    else
    {
        proba_logowania = 4;
        etap = 1;
    }
}

void wplata()
{
    zamiana_numer_do_wyslania_na_sendbuf();
    funkcja_sendbuf();
    proba_logowania = 11;
    funkcja_recvbuf();
}

void wyplata()
{
    zamiana_numer_do_wyslania_na_sendbuf();
    funkcja_sendbuf();
    sprawdz = 0;
    funkcja_recvbuf();
    sprawdz = strtol(recvbuf, NULL, 10);
    switch(sprawdz)
    {
    case 0:
        {
            proba_logowania = 10;
        }
        break;
    case 1:
        {
            proba_logowania = 11;
        }
        break;
    default:
        {

        }
    }
}

void odbieranie_danych_wlasciciela()
{
    kasowanie_memset();
    if(dane == 0)
    {
        kasowanie_memset();
        dane = 1;
        itoa(dane, sendbuf, 10);
        funkcja_sendbuf();
    }
    if(dane == 1)
    {
        funkcja_recvbuf_bez_memset();
        kasa = atoi(recvbuf);
        dane = 2;
        itoa(dane, sendbuf, 10);
        itoa(kasa, str_2, 10);
        funkcja_sendbuf();
    }
    if(dane == 2)
    {
        funkcja_recvbuf_bez_memset();
        imie = recvbuf;
        dane = 3;
        itoa(dane, sendbuf, 10);
        str_3 = new char[ imie.size() + 1 ];
        strcpy( str_3, imie.c_str() );
        funkcja_sendbuf();
    }
    if(dane == 3)
    {
        etap = 4;
        funkcja_recvbuf_bez_memset();
        warunek_danych = true;
        dane = 0;
    }
}

void obsluga_menu()
{
    rodzaj_operacji = numer_do_wyslania;
    switch(rodzaj_operacji)
    {
    case 1:
        {
            zamiana_numer_do_wyslania_na_sendbuf();
            funkcja_sendbuf();
            proba_logowania = 9;
            etap = 5;
        }
        break;
    case 2:
        {
            zamiana_numer_do_wyslania_na_sendbuf();
            funkcja_sendbuf();
            proba_logowania = 8;
            etap = 6;
        }
        break;
    default:
        {
            proba_logowania = 7;
            etap = 4;
        }
    }
}

void wielkie_menu()
{
    sprawdz = 0;
    switch (etap)
    {
    case 1:
        {
            kasowanie_memset();
            sprawdzanie_numeru_konta();
        }
        break;
    case 2:
        {
            kasowanie_memset();
            sprawdzanie_kodu_PIN();
        if(etap == 3)
        {
            kasowanie_memset();
            do
            {
            kasowanie_memset();
            odbieranie_danych_wlasciciela();
            }while(warunek_danych == false);
        }
        }
        break;
    case 4:
        {
            kasowanie_memset();
            obsluga_menu();
        }
        break;
    case 5:
        {
            kasowanie_memset();
            wyplata();
        }
        break;
    case 6:
        {
            kasowanie_memset();
            wplata();
        }
        break;
    case 7:
        {
            etap = 1;
            proba_logowania = 1;
        }
        break;
    default:
        {
            kasowanie_memset();
            etap = 1;
            proba_logowania = 1;
        }
    }
    komunikaty_logowania();
    zerowanie_okienka();
}

HWND Przycisk;
HWND Przycisk0;
HWND PrzyciskOK;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc (HWND hwnd,
                          UINT msg,
                          WPARAM wParam,
                          LPARAM lParam );
double func(HWND handle);
void value(int x1, int y1, int x2, int y2, HWND *hText, HWND hWnd, HINSTANCE hInstance);
void tekst(int x1, int y1, int x2, int y2, HWND *hText, HWND hWnd, HINSTANCE hInstance);
void wstaw(HWND *hStatic, char *lancuch);
/* Variable */

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Bankomat");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

    HWND hWnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
      hWnd = CreateWindowEx (
           WS_EX_WINDOWEDGE,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Bankomat"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1000,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
          );

// tworzenie klienta

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

if( connect( mainSocket,( SOCKADDR * ) & service, sizeof( service ) ) == SOCKET_ERROR )
{
    printf( " Failed to connect.\n" );
    WSACleanup();
    return 1;
}

 bytesRecv = SOCKET_ERROR;

 printf( " Waiting for a server to connect...\n" );

sendbuf = (char*)" Client connected!";
bytesSent = send( mainSocket, sendbuf, strlen( sendbuf ), 0 );

while( bytesRecv == SOCKET_ERROR )
{
    bytesRecv = recv( mainSocket, recvbuf, 32, 0 );

    if( bytesRecv == 0 || bytesRecv == WSAECONNRESET )
    {
        printf( " Connection closed.\n" );
        break;
    }

    if( bytesRecv < 0 )
         return 1;

    printf( " %s\n", recvbuf );
}

// koniec tworzenia klienta

    int x = 50;
    int y = 50;
    int w = 0;
    int k = 0;

    for(k = 1; k < 4; k++)
    {
        y = k * y;
        y += 150;
        for(w = 3; w < 6; w++)
        {
            x = w * x;
            x += 100;
            itoa(j, str_4, 10);
            Przycisk = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", str_4 , WS_CHILD | WS_VISIBLE,
            x, y, 50, 50, hWnd, ( HMENU ) j, hThisInstance, NULL );
            x = 50;
            j++;
        }
        y = 50;
    }

    Przycisk0 = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", " 0 ", WS_CHILD | WS_VISIBLE,
    300, 350, 50, 50, hWnd, ( HMENU ) ID_PRZYCISK0, hThisInstance, NULL );
    PrzyciskOK = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", " OK ", WS_CHILD | WS_VISIBLE,
    430, 140, 50, 50, hWnd, ( HMENU ) ID_PRZYCISKOK, hThisInstance, NULL);

    value(290, 150, 65, 30, &hText[0], hWnd, hThisInstance);

    tekst(200, 10, 300, 40, &hStatic[0], hWnd, hThisInstance);
    tekst(200, 70, 300, 40, &hStatic[1], hWnd, hThisInstance);
    tekst(550, 10, 200, 40, &hStatic[2], hWnd, hThisInstance);
    tekst(550, 70, 200, 40, &hStatic[3], hWnd, hThisInstance);
    tekst(550, 130, 200, 40, &hStatic[4], hWnd, hThisInstance);
    tekst(550, 190, 200, 40, &hStatic[5], hWnd, hThisInstance);

    ShowWindow (hWnd, nCmdShow);
    UpdateWindow(hWnd);

    sprawdz = 0;
    etap  = 1;
    proba_logowania = 1;
    komunikaty_logowania();

    /* Make the window visible on the screen */
    ShowWindow (hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;

}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_COMMAND:
    {
        switch( wParam )
        {
            case 1:
            {
                liczba = 1;
                wstawianie_liczby_do_okna();
            }
            break;
            case 2:
            {
                liczba = 2;
                wstawianie_liczby_do_okna();
            }
            break;
            case 3:
            {
                liczba = 3;
                wstawianie_liczby_do_okna();
            }
            break;
            case 4:
            {
                liczba = 4;
                wstawianie_liczby_do_okna();
            }
            break;
            case 5:
            {
                liczba = 5;
                wstawianie_liczby_do_okna();
            }
            break;
            case 6:
            {
                liczba = 6;
                wstawianie_liczby_do_okna();
            }
            break;
            case 7:
            {
                liczba = 7;
                wstawianie_liczby_do_okna();
            }
            break;
            case 8:
            {
                liczba = 8;
                wstawianie_liczby_do_okna();
            }
            break;
            case 9:
            {
                liczba = 9;
                wstawianie_liczby_do_okna();
            }
            break;
            case 0:
            {
                liczba = 0;
                wstawianie_liczby_do_okna();
            }
            break;

            case ID_PRZYCISKOK:
            {
                numer_do_wyslania = numer;
                wielkie_menu();
            }
            break;
            default:
            {
                return 0;
            }
        }
        }
        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
    return 0;
}



// wysylac program z gitchaba
