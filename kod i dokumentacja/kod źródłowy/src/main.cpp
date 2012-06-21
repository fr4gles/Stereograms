/**
 *  @mainpage
 *  @par Stereogramiksy
 *  @author Micha³ Franczyk, Micha³ Golonka, Szymon Jagie³a
 *  @date 2012.02.04
 *  @version 1.2
 *  @par program wykorzystuje biblioteke Qt
 *  @brief Program do generowania stereogramów.
 *
 *      Generator Stereorgamów.
 *         - Program do generowania stereogramów, umo¿liwia edycjê ustawieñ generowania:
 *              -# rozstaw oczu
 *              -# DPI ekranu
 *              -# zmiana koloru bia³ego na inny ( do wyboru kilka )
 *              -# dodanie pomocniczych kropek na obrazek
 *              -# zapis obrazka do wybranego formatu oraz wybranej rozdzielczoœci
 *              -# zmianê ustawieñ dopasowania do okna
 *
 *         - Program wyœwietla równie¿ podstawowe informacje o:
 *              -# rozdzielczoœci pliku
 *              -# podaje informacje czy obrazek jest w skali szaroœci
 *              -# rozmiar wczytanego pliku w MB
 *
 *      Gra - zgadnij ( zobacz ) co jest na stereogramie
 *         - Program umo¿liwia tryb gry;
 *              -# mo¿na graæ w dostêpne w liœcie gier gry
 *              -# trzeba odpowiedzieæ na zadane pytanie, jeœli odpowiedŸ bêdzie poprawna dostaniemy za to punkty
 *              -# jeœli poszczególna gra sk³ada siê z wiêcej ni¿ jeden, poziomów - wtedy mo¿na usyzkaœ wiêcej ni¿ 10 punktów.
 *                  I zostaniemy automatycznie przeniesieni do nowego poziomu.
 *         - Pogram umo¿liwia tryb - stwórz scenariusz
 *              -# mo¿emy zrobiæ w³asny poziom gry
 *              -# mo¿e on byæ dowolnie skomplikowany
 *              -# wszystko jest zapisywane do pliku - dlatego ³atwo podzieliæ siê póŸniej dan¹ gr¹ ze znajomymi
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
