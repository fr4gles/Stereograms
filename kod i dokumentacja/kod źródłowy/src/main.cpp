/**
 *  @mainpage
 *  @par Stereogramiksy
 *  @author Micha� Franczyk, Micha� Golonka, Szymon Jagie�a
 *  @date 2012.02.04
 *  @version 1.2
 *  @par program wykorzystuje biblioteke Qt
 *  @brief Program do generowania stereogram�w.
 *
 *      Generator Stereorgam�w.
 *         - Program do generowania stereogram�w, umo�liwia edycj� ustawie� generowania:
 *              -# rozstaw oczu
 *              -# DPI ekranu
 *              -# zmiana koloru bia�ego na inny ( do wyboru kilka )
 *              -# dodanie pomocniczych kropek na obrazek
 *              -# zapis obrazka do wybranego formatu oraz wybranej rozdzielczo�ci
 *              -# zmian� ustawie� dopasowania do okna
 *
 *         - Program wy�wietla r�wnie� podstawowe informacje o:
 *              -# rozdzielczo�ci pliku
 *              -# podaje informacje czy obrazek jest w skali szaro�ci
 *              -# rozmiar wczytanego pliku w MB
 *
 *      Gra - zgadnij ( zobacz ) co jest na stereogramie
 *         - Program umo�liwia tryb gry;
 *              -# mo�na gra� w dost�pne w li�cie gier gry
 *              -# trzeba odpowiedzie� na zadane pytanie, je�li odpowied� b�dzie poprawna dostaniemy za to punkty
 *              -# je�li poszczeg�lna gra sk�ada si� z wi�cej ni� jeden, poziom�w - wtedy mo�na usyzka� wi�cej ni� 10 punkt�w.
 *                  I zostaniemy automatycznie przeniesieni do nowego poziomu.
 *         - Pogram umo�liwia tryb - stw�rz scenariusz
 *              -# mo�emy zrobi� w�asny poziom gry
 *              -# mo�e on by� dowolnie skomplikowany
 *              -# wszystko jest zapisywane do pliku - dlatego �atwo podzieli� si� p�niej dan� gr� ze znajomymi
 *
 */

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":new/icon/stereo.png"));
    w.show();

    return a.exec();
}
