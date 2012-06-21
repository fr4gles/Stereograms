#ifndef STEREOGRAMGENERATOR_H
#define STEREOGRAMGENERATOR_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QPainter>

/**
 * StereogramGenerator - klasa odpowiadaj¹ca za generowanie stereogramów
 * Zawiera algorytm oraz funckje pomocnicze w tworzeniu stereogramu z dostarczonego obrazka
 * Opis algorytmu znajduje siê dokumentacji.
 * @file stereogramgenerator.h
 * @note Klasa generuj¹ca stereogramy
 * @see MainWindow
 */
class StereogramGenerator : public QObject
{
    Q_OBJECT
public:
        /**
         * StereogramGenerator - wstêpna inicjalizacja argumentów, konstruktor
         * Konstruktor klasy MainWindow
         * @see setDefault();
         * @see changeDefault();
         */
        explicit    StereogramGenerator(QObject *parent = 0);

        /**
         * Destruktor klasy StereogramGenerator
         */
                    ~StereogramGenerator();

        /**
         * setImage - ustawia plik, z którego bêdzie tworzony stereogram
         * @param *img - przekazany obrazek do wygenerowania ( *img = *imageCpy )
         * @see getImage();
         * @see generate();
         * @return void
         */
        inline      void setImage(QImage *img){_imageToGenerate = img;}

        /**
         * getImage - zwraca wygenerowany plik ( stereogram )
         * @see setImage();
         * @see generate();
         * @return QImage - wygenerowany stereogram
         */
        inline      QImage* getImage(){return _imageGeneratedStereogram;}

        /**
         * generate - g³ówna funkcja klasy StereogramGenerator, zawiera w sobie algorytm generuj¹cy stereogramy oraz wywo³uje funckje pozwalaj¹ce na zmianê rozmiaru, koloru, wklês³oœci, rysowania pomocniczych kropek
         * @param convex - zmienna odpowiadaj¹ca za "wklês³oœæ" - to czy wygenerowany obrazek bêdzie widoczny jako zwyk³y obiekt lub jako wyciêty.
         * @param color - odpowiada za kolory w jakich ma zostaæ wygenerowany stereogram
         * @param circles -  odpowiada za to czy na obrazku maj¹ siê pojawiæ pomocnicze w zobaczeniu stereogramu kó³ka
         * @param size - odpowiada za rozmiar w jakim ma byæ wygenerowancy stereogram - g³ownie na potrzeby zapisu do pliku
         * @param keepAspectRatio - odpowiada za to czy obrazek po zmianie rozmiaru ma siê dopasowaæ do przyjêtych poprocji czy porzuciæ swoje poporcje i dopasowaæ siê rozmiaru ( rozci¹gn¹æ, sp³aszczyæ )
         * @see setImage();
         * @see drawCirclesOnImage();
         * @see calculateImageDepth();
         * @see roundSomething();
         * @see separateSomething();
         * @see setStatusBarLabel();
         * @return void
         */
        void        generate(int convex, int color=false, bool circles=false, int size=4, int keepAspectRatio=true);

        /**
         * setDefault - ustawia wartoœci domyœlne klasy StereogramGenerator
         * @see generate();
         * @see changeDefault();
         * @return void
         */
        void        setDefault(int DPI, double distanceBetweenEyes);

        /**
         * changeDefault - zmienia wartoœci domyœlne
         * @param currentDPI - ustawia DPI wybrane przez u¿ytkownika
         * @param currentDistanceBetweenEyes - ustawia odleg³oœæ miêdzy oczay ( w calach ) wybran¹ przez u¿ytkonika.
         * @see generate();
         * @see changeDefault();
         * @return void
         */
        void        changeDefault(int currentDPI, int currentDistanceBetweenEyes);

    signals:
        /**
         * setStatusBarLabel - sygna³ wysy³any do klasy MainWindow, w która obs³uguje zmienn¹ ui - odpowiedzialn¹ za GUI
         * @param tmp - string, ci¹g znaków jaki ma siê pojawiæ na status barze
         * @return void
         */
        void        setStatusBarLabel(QString tmp);
    public slots:
        // empty

    private:

        /**
         * drawCirclesOnImage - funckja rysuj¹ca na obrazku pomocnicze kropki
         * ( zmienne pomagaj¹ okreœliæ w jakim miejscu maj¹ siê znaleŸæ kropki )
         * @param maxX - szerokoœæ obrazka
         * @param maxY - wysokoœæ obrazka
         * @see generate();
         * @see calculateImageDepth();
         * @return void
         */
        void        drawCirclesOnImage(int maxX, int maxY);

        /**
         * calculateImageDepth - funkcja oblczaj¹ca "g³êbokoœæ" ( w skali szaroœci ) pixel po pixelu
         * ( dla koloru bia³ego - 0, dla koloru czarnego - 1 )
         * @param **imageDepth - tablica, która zostanie uzupe³niona wartoœciami danych pixeli na skali szaroœci
         * @param convex - wklês³oœæ
         * @see generate();
         * @see drawCirclesOnImage();
         * @return void
         */
        void        calculateImageDepth(double **imageDepth, int convex);

        /**
         * roundSomething - funkcja zaokr¹glaj¹ca dan¹ liczbê ( np typu double ) do int'a
         * @param something - zmienna do zaokr¹glenia
         * @see generate();
         * @see drawCirclesOnImage();
         * @return int - zwraca zaokr¹glon¹ liczbê
         */
        int         roundSomething(double something);

        /**
         * separateSomething - funkcja ( rozdzielaj¹ca ) pozycjê, pobiera wartoœæ pixela w skali szaroœci
         * a nastêpnie zwraca odleg³oœæ w jakiej powinien znaleŸæ od "œciany" obrazka
         * @param something - pozycja
         * @see generate();
         * @see roundSomething();
         * @return int - zwraca rozdzielon¹ liczbê
         */
        int         separateSomething(double something);

        /**
          * Zmienna _imageToGenerate - przechowuje obrazek, z którego bêdzie generowany stereogram
          * Zmienna prywatna.
          */
        QImage      *_imageToGenerate;

        /**
          * Zmienna _imageCopy - przechowuje kopiê oryginanego obrazka ( na potrzeby zmieny rozmiaru itd. )
          * Zmienna prywatna.
          */
        QImage      *_imageCopy;

        /**
          * Zmienna _imageGeneratedStereogram - przechowuje wygerowany stereogram
          * Zmienna prywatna.
          */
        QImage      *_imageGeneratedStereogram;

        /**
          * Zmienna _widthOfImage_X - przechowuje szerokoœæ ( w pixelach ) przeskalowanego obrazka ( generowanego stereogramu )
          * Zmienna prywatna.
          */
        int         _widthOfImage_X;

        /**
          * Zmienna _widthOfImage_X - przechowuje wykosoœæ ( w pixelach )  przeskalowanego obrazka ( generowanego stereogramu )
          * Zmienna prywatna.
          */
        int         _heightOfImage_Y;


        /**
          * Zmienna _wygenerowano - true jeœli wczytano obrazek - pomaga w zarz¹dzaniu pamiêci¹
          * Zmienna prywatna.
          */
        bool        _wygenerowano;


        /**
          * Zmienna _distanceBetweenEyes - odleg³oœæ miêdzy oczami ( w calach )
          * Zmienna prywatna.
          */
        double      _distanceBetweenEyes;

        /**
          * Zmienna _DPI - DPI ekranu, zmienna mo¿e byæ zmieniona przez u¿ytkownika, domyœlnie 96
          * Zmienna prywatna.
          */
        double      _DPI;

        /**
          * Zmienna _depthOfField - g³êbia widzialnego pola, u³amek ( 1/3 ) z widzialnego dystansu
          * Zmienna prywatna.
          */
        double      _depthOfField;

        /**
          * Zmienna _eyeSeparation - zmienna odpowiadaj¹ca rozdzieleniu "oczu", pola widzialnego
          * zale¿na od _distanceBetweenEyes oraz _DPI
          * _eyeSeparation = roundSomething(_distanceBetweenEyes*_DPI)
          * Zmienna prywatna.
          */
        int         _eyeSeparation;

        /**
          * Zmienna  _farOfDots - wykorzystywana przy rysowaniu pomocniczych kropek - wyznacza ich odleg³oœæ
          * Zmienna prywatna.
          */
        int         _farOfDots;
};

#endif // STEREOGRAMGENERATOR_H
