#include <iostream>

enum mode {Gauss3x3, Gauss5x5 , SobelSx, SobelSy};
class Filter
{
    public:
        Filter(int);
        ~Filter();
        int* operator[](int x){return this->mask[x];};
        int getSum()const {return this->sum;};
        int getSize()const {return this->x;};
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
        double* operator[](int x){return this->field[x];};
        friend void operator<<(std::ostream &, Image &);
        friend Image operator+(Image, Image);
        void operator=(Image);
        void load(std::string);
        void write(std::string);
        double averageGrey();
        double contrast(double);
        void histo(std::string);
        void filter(int, std::string);
        void medianFilter(int dimensions, std::string fileName);
        void histogramEqualization(std::string);

    private:
        void init(int,int);
        void getGreyValues();
        double maxGrey;
        double minGrey;
        int x;
        int y;
        double **field;
};