#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<iostream>
#include<cstdlib>

namespace CGP{

  //KLASA ABSTRAKCYJNA, PIERWOWZOR DO TWORZENIA OBIEKTÓW WARTOSCI NA KTÓRYCH OPERUJE CGP
  class Operational{
  public:
    Operational(){};
  };

  template <typename T>
  class Block{
  private:
//POLA POMOCNICZE I OPISOWE
    int uniqueIdentifier;
    char blockType;  //czy to blok wejsciowy ('i'), wyjściowy ('o') czy zwykły ('n') czy nieokreślony (0)
    bool crucial;     //czy ma on znaczenie w propagacji sygnalu

//POLA DEFINIUJACE
    T * value;        //wartosc zmiennej bloku
    Block* inputA;  //wejscie A bloku
    Block* inputB;  //wejscie B bloku

    //POLA FUNKCYJNE

  public:
//KONSTRUKTORY
    Block(int ID, Block * inA, Block * inB){
      inputA = inA;
      inputB = inB;
      uniqueIdentifier = ID;
      blockType = 0;  //domyślnie jako blok nieokreślony
      crucial = false;  //domyślnie blok niekodujący fenotypu
    }

//METODY GET
    Block * getInputA(){return inputA;}                                         //zwraca wskaznik na inputA
    Block * getInputB(){return inputB;}                                         //zwraca wskaznik na inputB
    int getID(){return uniqueIdentifier;}                                       //zwraca ID bloku
    bool isCrucial(){return crucial;}                                           //zwraca czy blok definiuje fenotyp
    bool isInputBlock(){return blockType;}                                      //zwraca typ bloku
//METODY SET
    //void setInputA(){}
    void setBlockType(char newType){blockType = newType;}
    void setCruciality(bool newCruciality){crucial = newCruciality;}
//METODY PODRZĘDNE
    void print();

  };

  //klasa CGP jest główną klasą przechowującą metody i dane związane z implementacją CGP
  template <typename T>
  class CGP_Algorithm{
    typedef Operational (* formula)(Operational *valA, Operational *valB);  //definicja wskaznika na funkcje
    static formula ** formulas;
    static std::list<formula> formulasList; //lista do ktorej uzytkownik wrzuca zdefiniowane przez siebie funkcje

    int nRow;       //liczba wierszy siatki operacji
    int nCol;       //liczba kolumn siatki operacji
    int nFormulas;  //liczba możliwych zaimplementowanych operacji

    //std::list<T> operations;
    Block<T> ** blocks;  //tablica na wszystkie bloki uzyte w CGP

    public:
      CGP_Algorithm(int rows, int cols): nRow(rows), nCol(cols){
        blocks = new Block<T>*[nRow];
        for(int r=0; r<nRow; r++){
          blocks[r] = new Block<T>[nCol];
        }
      }

      //funkcja dodajaca formula to listy FormulaList
      static void addFormula(formula f){
        formulasList.push_back(f);
      }

      //funkcja  poswiadczajaca, ze wszystkie dodano wszystie przewidziane funkcje
      static void enoughFormulas(){
        formulas = new formula*[formulasList.size()];
        for(int i=0 ; i<formulasList.size() ; i++)
          formulas[i] = formulasList.pop_front();
      }

  };


}


#endif
