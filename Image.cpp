#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include "Image.h"
using namespace std;

#define medianFrom(list, size) (((size % 2) == 0) ? list[(size/2)-1] : list[((size+1)/2)-1])
#define amount(x)  ((x >= 0) ? x : -x)
#define round(x) (floor(x + 0.5))

//######################## IMAGE CLASS ##############################
void Image::init(int x , int y)
{
    this->x = x;
    this->y = y;
    this->field = new double*[x];
    for (int i = 0; i < x; i++)
    {
        this->field[i] = new double[y];
        if(this->field[i]==NULL){
            cout<<"Out of Memory"<<endl;
            exit(1);
        } 
        fill(this->field[i], this->field[i]+y, 0.0);
    } 

    this->histo = NULL;
}

Image::Image(int x)
{
    init(x, x);
}
Image::Image(int x, int y)
{
    init(x, y);
}
Image::Image(int x, int y, string fname)
{
    init(x, y);
    this->load(fname);
}

Image::~Image()
{
    for(int i = 0; i < this->x; i++)
    {
        delete[] field[i];
        field[i] = NULL;
    }

    if(this->histo != NULL)
        delete[] histo;
}

// print Image
void operator<<(ostream &os, Image &b)
{
    for (int i = 0; i < b.x; i++)
    {
        for (int j = 0; j < b.y; j++)
        {
            cout<<b[i][j]<<" ";
        }
        cout<<endl;
    }
}

// load Image
void Image::load(string fname)
{
    ifstream fin(fname.c_str());
    if(!fin.good())
    {
        cout<<"File Error >>> "<<fname<<endl;
        exit(1);
    }
    int line = 0;
    for (int i = 0; i < this->x; i++)
    {
        for (int j = 0; j < this->y; j++)
        {
            fin>>this->field[i][j];
        }
    }
    fin.close();   
}

// write Image
void Image::write(string fname)
{
    ofstream fout(fname.c_str());
    if(fout.fail())
    {
        cout<<"File Error >>> "<<fname<<endl;
        exit(1);
    }
    for (int i = 0; i < this->x; i++)
    {
        for (int j = 0; j < this->y; j++)
        {
            fout<<this->field[i][j]<<" ";        
        }
        fout<<"\n";
    }
    fout.close();
}

void Image::operator=(Image a)
{
    if(this->x != a.x || this->y != a.y)
    {
        cout<<"wrong resolution!"<<endl;
        exit(1);
    }

    for (int i = 0; i < a.x; i++)
    {
        for(int j = 0; j < a.y; j++)
        {
            this->field[i][j] = a[i][j];
        }
    }
}


Image operator+(Image a, Image b)
{
    if(a.x != b.x || a.y != b.y)
    {
        cout<<"wrong resolution!"<<endl;
        exit(1);
    }
    for (int i = 0; i < a.x; i++)
    {
        for(int j = 0; j < a.y; j++)
        {
            a.field[i][j] += b.field[i][j];
        }
    }
    return a;
}


double Image::averageGrey()
{
    double summe=0.0;
    int count=0;
	for(int i=0; i<this->x; i++)
    {
		for(int j=0; j<this->y; j++)
        {
			summe += this->field[i][j];
			if(this->field[i][j]!=0.0)
				count++;
		}
	}
	return summe/(double)count;
}

double Image::contrast(double m) // input Average Grey
{
	double summe=0.0;
    int count=0;
	for(int i=0; i<this->x; i++)
    {
		for(int j=0; j<this->y; j++)
        {
			if(this->field[i][j]!=0.0)
            {
				summe += (this->field[i][j]-m)*(this->field[i][j]-m);
				count++;
			}
		}
	}
	return summe/count;
}

void Image::getGreyValues()
{ 
    double maxelem, minelem;
    this->minGrey = *min_element(&(this->field[0][0]), &(this->field[0][this->y]));
    this->maxGrey = *max_element(&(this->field[0][0]), &(this->field[0][this->y]));
    for (int i = 1; i < this->x; i++)
    {
        minelem = *min_element(&(this->field[i][0]), &(this->field[i][this->y]));
        maxelem = *max_element(&(this->field[i][0]), &(this->field[i][this->y]));

        if(maxelem > this->maxGrey)
            this->maxGrey = maxelem;
    
        if(minelem < this->minGrey)
            this->minGrey = minelem;   
    }
}

void Image::histogram(string fname)
{
    this->getGreyValues();
    // histogram stuff
    // shift the start point to the right, and make negative values "positive"
    // a bit like IEEE 754
    this->bias = (int)amount(this->minGrey);
    this->buffer = (int)this->maxGrey + this->bias + 1; 
    this->histo = new int[this->buffer];
    fill(this->histo,  this->histo+this->buffer, 0);

    for (int i = 0; i < this->x; i++)
    {
        for (int j = 0; j < this->y; j++)
        {
            int nextInt = (int)round(this->field[i][j]);
            this->histo[nextInt + this->bias]++;
        }    
    }
    if(fname == "print")
    {
        for (int i = 0; i < this->buffer; i++)
        {
            cout<<i-this->bias<<": "<<this->histo[i]<<"\n"; // print the "real" values
        }
    }
    else
    {    
        if(fname != "None")
        {
            ofstream fout(fname);
            if(fout.fail())
            {
                cout<<"File Error >>> "<<fname<<endl;
                exit(1);
            }

            for (int i = 0; i < this->buffer; i++)
            {
                fout<<this->histo[i]<<"\n";
            }
            fout.close();
        }
    }
}

void Image::filter(int mode, string fname)
{
    Filter f(mode);
    int fsize = f.getSize(); 
    int u = ceil((fsize/2));
    double s = (double)f.getSum();
    Image bgauss(this->x, this->y);
	for(int i = u; i < this->x - u; i++)
	{
		for (int j = u; j < this->y -u; j++)
		{
			double summe = 0.0;
			for(int k = 0; k < fsize; k++)
			{
				for (int l = 0; l < fsize; l++)
				{
					summe += this->field[i-u+k][j-u+l] * (double)f[k][l];
				}			
			}
			summe /= s;
			bgauss.field[i][j] = summe;
		}
	}
	bgauss.write(fname);
}

void Image::medianFilter(int d, string fname)
{
    int size = d*d;
    double list[size];
    int listIndex = 0;
    int u = ceil((d/2));
    Image out(this->x, this->y);
	for(int i = u; i < this->x - u; i++)
	{
        for (int j = u; j < this->y -u; j++)
        {
            for (int k = i-u; k < d+(i-u); k++)
            {
                for (int l = j-u; l < d+(j-u); l++)
                {
                    list[listIndex++] = this->field[k][l];
                }
            }
            sort(&(list[0]), &(list[size]));
            out.field[i][j] =  medianFrom(list, size);
            fill(list, list+size, 0.0);
            listIndex =0;
		}
	}
	out.write(fname);
}


void Image::histogramEqualization(string fname)
{
    // init histogram
    this->histogram("None");

    double pixelNumber = (double)(this->x*this->y);
    Image out(this->x, this->y);
    for (int i = 0; i < this->x; i++)
    {
        for (int j = 0; j < this->y; j++)
        {
            double sum = 0.0;
            int oldGrey = (int)round(this->field[i][j]);
            for(int k = this->minGrey; k < oldGrey; k++)
            {
                sum += this->histo[k+this->bias] / pixelNumber;
            }
            out[i][j] = (this->maxGrey - this->minGrey) * sum;
        }
    }
    
    out.write(fname);
}



//######################## FILTER CLASS ##############################
void Filter::init(int nx)
{
    this->sum = 0;
    this->x = nx;
    this->mask = new int*[this->x];
    for (int i = 0; i < this->x; i++)
    {
        this->mask[i] = new int[x];
        if(this->mask[i]==NULL){
            cout<<"Mem. Error"<<endl;
            exit(1);
        }
        fill(this->mask[i], this->mask[i]+x, 0);
    } 
}


void operator<<(ostream &os, Filter &fi)
{
    for (int i = 0; i < fi.x; i++)
    {
        for (int j = 0; j < fi.x; j++)
        {
            cout<<fi.mask[i][j]<<" ";
        }
        cout<<endl;
    }
}


Filter::Filter(int mode)
{
    switch (mode)
    {
        case Gauss3x3:
        {
            this->init(3);
            int gfilter[3][3] = { 
                {1, 2 ,1}, 
                {2, 4 ,2}, 
                {1, 2 ,1}
            };

            for (int i = 0; i < this->x; i++)
            {
                for (int j = 0; j < this->x; j++)
                {
                    this->mask[i][j] = gfilter[i][j];
                    this->sum += gfilter[i][j];
                }   
            }
            break;
        }
        case Gauss5x5:
        {
            this->init(5);
            int gfilter[5][5] = { 
                {1, 4,  6,  4,  1}, 
                {4, 16, 24, 16, 4}, 
                {6, 24, 36, 24, 6},
                {4, 16, 24, 16, 4},
                {1, 4,  6,  4,  1}
            };
            for (int i = 0; i < this->x; i++)
            {
                for (int j = 0; j < this->x; j++)
                {
                    this->mask[i][j] = gfilter[i][j];
                    this->sum += gfilter[i][j];
                }   
            }
            break;
        }
        case SobelSx:
        {
            this->init(3);
            int gfilter[3][3] = { 
                {1, 0 ,-1}, 
                {2, 0 ,-2}, 
                {1, 0 ,-1}
            };
            for (int i = 0; i < this->x; i++)
            {
                for (int j = 0; j < this->x; j++)
                {
                    this->mask[i][j] = gfilter[i][j];
                    this->sum += gfilter[i][j];
                }   
            }
            break;
        }
        case SobelSy:
        {
            this->init(3);
            int gfilter[3][3] = { 
                {1,   2,  1}, 
                {0,   0,  0}, 
                {-1, -2, -1}
            };
            for (int i = 0; i < this->x; i++)
            {
                for (int j = 0; j < this->x; j++)
                {
                    this->mask[i][j] = gfilter[i][j];
                    this->sum += gfilter[i][j];
                }   
            }
            break;  
        }
        default:
            exit(1);
    }

    if(this->sum == 0)
        this->sum = 1;  
}

Filter::~Filter()
{
    for(int i = 0; i < this->x; i++)
    {
        delete[] mask[i];
        mask[i] = NULL;
    }   
}   
