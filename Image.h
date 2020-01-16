#include <iostream>

enum mode {Gauss3x3, Gauss5x5 , SobelSx, SobelSy};
class Filter
{
    public:
        Filter(int);
        ~Filter();
        int* operator[](int x){return this->mask[x];};
        int GetSum()const {return this->sum;};
        int GetSize()const {return this->x;};
        friend void operator<<(std::ostream &, Filter &);

    private:
        void init(int);
        int x;
        int **mask;
        int sum;
};

class Image
{
    public:
        Image();
        Image(int);
        Image(int,int);
        Image(int, int, std::string);
        ~Image();
        float* operator[](int x){return this->field[x];};
        friend void operator<<(std::ostream &, Image &);
        friend Image operator+(Image, Image);
        void operator=(Image);
        void load(std::string);
        void write(std::string);
        float AverageGrey();
        float Contrast(float);
        void Histo(int, std::string);
        void filter(int, std::string);

    private:
        void init(int,int);
        int x;
        int y;
        float **field;
};