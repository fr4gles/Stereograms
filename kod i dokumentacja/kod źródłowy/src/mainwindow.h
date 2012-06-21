#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStringListModel>
#include <QImageReader>

#include "stereogramgenerator.h"

/**
 * Ui - namespace - do u�ywania obiekt�w stworzonych za pomoc� Designer'a w QtCreatorze
 * @see mainwindow.ui
 * @see MainWindow
 */
namespace Ui
{
    class MainWindow;
}

/**
 * Main Window - G��wna Klasa .
 * Klasa, kontroluj�ca wszelkie dzia�ania aplikacji.
 * @note MainWindow musi dzia�a�.
 * @file mainwindow.h
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    /**
     * MainWindow - r�czne dodawanie/ustawianie element�w.
     * Konstruktor klasy MainWindow
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * Destruktor klasy MainWindow
     */
    ~MainWindow();

private slots:

    /** @brief funkcja odpowiadaj�ca za w�wietlenie na labelu informaji o wczytanym obrazku
      * @return void
     */
    void setLabel_info(int w, int h, float size, bool allgray);

    /** @brief funkcja odpowiadaj�ca za w�wietlenie na status barze aktulanych informacji/podpowiedzi
      * @return void
     */
    void onStatusBarChanged(QString tmp);

    /** @brief funkcja ob�usuj�ca przycisk przenosz�cy u�ytkownika do odpowiedniej generowania stereogramu
      * @return void
     */
    void on_pushButton_Generate_clicked();

    /** @brief funkcja ob�usuj�ca przycisk otwieraj�cy OpenFileDialog
      * @return void
     */
    void on_pushButton_Open_clicked();

    /** @brief funkcja ob�usuj�ca przycisk zamykaj�cy program
      * @return void
     */
    void on_pushButton_Koniec_clicked();

    /** @brief funkcja ob�usuj�ca przycisk powracaj�cy do g�ownego menu z zak�adki generowanie stereogramu
      * @return void
     */
    void on_pushButton_clicked();

    /** @brief Wybor zakladki 'Gra' z menu glownego programu.
      * @return void
     */
    void on_pushButton_Games_clicked();

    /** @brief funkcja ob�usuj�ca przycisk otwieraj�cy SaveFileDialog - zapisuje obrazek
      * @return void
     */
    void on_pushButton_Save_clicked();

    /** @brief W zakladce wyboru gier wybieramy gre w ktora chcemy zagrac.
      * @return void
     */
    void on_pushButton_wg_Graj_clicked();

    /** @brief Powrot do menu glownego z zakladki wybory gier.
     */
    void on_pushButton_wg_Menu_clicked();

    /** @brief Akcja na klikniecie odpowiedzi do gry.
      * @return void
     */
    void on_pushButton_game_Ok_clicked();

    /** @brief funkcja ob�usuj�ca przycisk powracaj�cy do g�ownego menu z zak�adki z wynikami gry
      * @return void
     */
    void on_pushButton_2_end_menu_clicked();

    /** @brief funkcja ob�usuj�ca check odpowiedzialny za wygenerowanie stereogramu z pomocniczymi kropkami
      * @return void
     */
    void on_checkBox_clicked();

    /** @brief Wybor zakladki 'Stworz scenariusz' w menu glownym.
      * @return void
     */
    void on_pushButton_Scenariusz_clicked();

    /** @brief Powrot do menu z zakladki scenariusz.
      * @return void
     */
    void on_pushButton_scen_menu_clicked();

    /** @brief Przycisk akcji dodajacy obrazek do scenariusza.
      * @return void
     */
    void on_pushButton_scen_add_clicked();

    /** @brief Dodanie wszystkich obrazkow i przejscie do kolejnego etapu.
      * @return void
     */
    void on_pushButton_scen_dalej_clicked();

    /** @brief Powrot do menu z zakladki stworzenia scenariusza.
      * @return void
     */
    void on_pushButton_scen2_menu_clicked();

    /** @brief Dodanie hasla do gry przy tworzeniu scenariusza.
      * @return void
     */
    void on_pushButton_scen2_add_clicked();

    /** @brief Zakonczenie tworzenia scenariusza i zapis do pliku.
      * @return void
     */
    void on_pushButton_scen2_ok_clicked();

    /** @brief funkcja ob�usuj�ca combobox odpowiedzialny za zmian� kolor�w na stereogramie
      * @return void
     */
    void on_comboBox_activated(int index);

    /** @brief Wybor zakladki 'o programie' z menu glownego.
      * @return void
     */
    void on_pushButton_Oprogramie_clicked();

    /** @brief Powrot do menu z zakladki o programie.
      * @return void
     */
    void on_pushButton_oprogramie_menu_clicked();

    /** @brief funkcja ob�usuj�ca combobox odpowiedzialny za zmian� DPI generowanego stereogramu
      * @return void
     */
    void on_comboBox_DPI_activated(int index);

    /** @brief funkcja ob�usuj�ca combobox odpowiedzialny za rozstawu oczu generowanego stereogramu
      * @return void
     */
    void on_comboBox_RozstawOczu_activated(int index);

    /** @brief funkcja ob�usuj�ca checkbox odpowiedzialny za zmian� "wkl�s�o�ci" stereogramu - wi�cej w dokumentacji.
      * @return void
     */
    void on_checkBox_wkleslosc_clicked();

    /** @brief funkcja ob�usuj�ca checkbox odpowiedzialny za to czy obrazek ma si� dopasowywa� do okna czy te� pozosta� w oryginalnym kszta�cie
      * @return void
     */
    void on_checkBox_dopasujDoOkna_clicked();

private:
    /**
     * update - od�wie�aj�ca okienko - dopasowuje rozmiar wczytanego, a nast�pnie wygenerowanego pliku ( stereogramu )
     * @param dopasuj - czy obrazek ma by� dopasowany do okna ( domy�lan warto�� - tak )
     * @see openFile();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void update(bool dopasuj=true);

    /**
     * openFile - okienko dialogowe do wyboru pliku.
     * @see wczytajPlik();
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @return void
     */
    void openFile();

    /**
     * saveFile - zapisuje plik ( stereogram ) - w okre�lonej rozdzielczo�ci, wybranym rozszerzeniem oraz innymi ustawieniami
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void saveFile();

    /**
     * wczytajPlik - metoda wczytuj�ca obrazek
     * @param fileName - string nazwy pliku.
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return bool
     */
    bool wczytajPlik(QString fileName);

    /**
     * resizeEvent - event odpowiadaj�cy za updatowanie rozmiaru obrazka
     * @param event - rozszerzenie obrazka
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void resizeEvent(QResizeEvent *);


    /**
     * setElementsDisabled - pozwala na �atwe "w��czanie" lub "wy��czanie" element�w
     * @param disabled - czy dane elementy maj� by� w��czone b�d� nie
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void setElementsDisabled(bool disabled);

    /**
      * Zmienna *ui - pozwala si� odwo�ywa� do obiekt�w stworzonych przez Designer'a
      * Zmienna prywatna.
      */
    Ui::MainWindow *ui;

    /**
      * Zmienna *fileName - wczytuje �cie�k� dost�pu do pliku oraz jego nazw�.
      * Zmienna prywatna.
      */
    QString *fileName;

    /**
      * Zmienna *imageOrg - wczytany oryginalny obrazek
      * Zmienna prywatna.
      */
    QImage *imageOrg;

    /**
      * Zmienna *imageCpy - na pocz�tku przechowuje kopi� imageOrg a potem sam stereogram.
      * Zmienna prywatna.
      */
    QImage *imageCpy;

    /**
      * Zmienna *imageToSave - przechowuje obrazek gotowy do zapisania.
      * Zmienna prywatna.
      */
    QImage *imageToSave;

    /**
      * Zmienna scaledImage - przechowuje kopi� imageCpy - odpowiednio dopasowan� do rozmiaru okna
      * Zmienna prywatna.
      */
    QImage scaledImage;

    /**
      * Zmienna wczytano - na wej�ciu false, po wygenerowaniu true | pomaga w optymalizacji zu�ycia pami�ci
      * Zmienna prywatna.
      */
    bool wczytano;


    /**
      * Zmienna _stereogramGenerator - obiekt klasy StereogramGenerator - odpowiedzialny za obs�ug� generowania stereogramu
      * Zmienna prywatna.
      */
    StereogramGenerator _stereogramGenerator;



    /* do gry */
    /**
      * Zmienna tytulGra - przechowuje nazwe gry
      * Zmienna prywatna.
      */
    QString tytulGra;
    /**
      * Zmienna ileObrazkowGra - przechowuje liczbe obrazkow w grze
      * Zmienna prywatna.
      */
    int ileObrazkowGra;
    /**
      * Zmienna adresObrazkaGra - przechowuje adres do obrazka w grze
      * Zmienna prywatna.
      */
    QStringList adresObrazkaGra;
    /**
      * Zmienna hasloObrazkaGra - przechowuje odpowiedz do obrazka w grze
      * Zmienna prywatna.
      */
    QStringList hasloObrazkaGra;
    /**
      * Zmienna punktyGra - przechowuje ilosc zgromadzonych punkt�w w grze
      * Zmienna prywatna.
      */
    int  punktyGra;
    /**
      * Zmienna ktoryObrazekGra - przechowuje aktualny obrazek do wyswietlenia w grze
      * Zmienna prywatna.
      */
    int ktoryObrazekGra;
    /* koniec do gry */
    /* generator scenariuszy */
    /**
      * Zmienna fileListScene - przechowuje adresy obrazkow w generatorze scenariuszy
      * Zmienna prywatna.
      */
    QStringList fileListScen;
    /**
      * Zmienna haslaListScene - przechowuje odpowiedzi do obrazkow w generatorze scenariuszy
      * Zmienna prywatna.
      */
    QStringList haslaListScen;
    /*koniec generator scenariuszy */


    /* funkcje do gry */
    /** @brief Wczytanie parametrow gry.
     *
     *  @param gra Nazwa gry w ktora chcemy zagrac.
     * @see statsGame();
     * @see showGame();
     * @see endGame();
     * @return void
     */
    void loadGame(QString);
    /** @brief Uaktulnienie statystyk  oraz punktow gry.
     *
     *  @param wygrana Sprawdza czy odpowiedz jest prawidlowa.
     * @return void
     */
    void statsGame(int);
    /** @brief Wyswietlenie obrazka w grze.
      * @see endGame();
      * @return void
     */
    void showGame();
    /** @brief Wyswietla zakladke podsumowania gry.
      * @return void
     */
    void endGame();

};

#endif // MAINWINDOW_H
