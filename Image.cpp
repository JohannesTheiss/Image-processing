#include <iostream>
#include <fstream>
#include <math.h>
#include "Image.h"
using namespace std;

//######################## IMAGE CLASS ##############################
void Image::init(int x , int y)
{
    this->x = x;
    this->y = y;
    this->field = new float*[x];
    for (int i = 0; i < x; i++)
    {
        this->field[i] = new float[y];
        if(this->field[i]==NULL){
            cout<<"Kein Speicher"<<endl;
            exit( 1 );
        }
        for (int j = 0; j < y; j++)
        {
            this->field[i][j] = 0.0;
        }  
    } 
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


float Image::AverageGrey()
{
    float summe=0, count=0;
	for(int i=0; i<this->x; i++){
		for(int j=0; j<this->y; j++){
			summe += (float)this->field[i][j];
			if(this->field[i][j]!=0)
				count++;
		}
	}
	return summe/count;
}

float Image::Contrast(float m) // input Average Grey
{
	float summe=0, count=0;
	for(int i=0; i<this->x; i++){
		for(int j=0; j<this->y; j++){
			if(this->field[i][j]!=0){
				summe += (this->field[i][j]-m)*(this->field[i][j]-m);
				count++;
			}
		}
	}
	return summe/count;
}

void Image::Histo(int buffer, string fname)
{
    int *hist = new int[buffer];
    for (int i = 0; i < buffer; i++)
    {
        hist[i] = 0;
    }
    
    for (int i = 0; i < this->x; i++)
    {
        for (int j = 0; j < this->y; j++)
        {
            hist[(int)floor(this->field[i][j] + 0.5)]++;
        }    
    }

    if(fname != "None")
    {
        ofstream fout(fname);
        if(fout.fail())
        {
            cout<<"File Error >>> "<<fname<<endl;
            exit(1);
        }

        for (int i = 0; i < buffer; i++)
        {
            fout<<hist[i]<<"\n";
        }
        fout.close();
    }
    delete[] hist;
}

void Image::filter(int mode, string fname)
{
    Filter f(mode);
    int u = ceil((f.GetSize()/2));
    float s = (float)f.GetSum();
    int fsize = f.GetSize(); 
    Image bgauss(this->x, this->y);
	for(int i = u; i < this->x - u; i++)
	{
		for (int j = u; j < this->y -u; j++)
		{
			float summe = 0.0;
			for(int k = 0; k < fsize; k++)
			{
				for (int l = 0; l < fsize; l++)
				{
					summe += (float)this->field[i-u+k][j-u+l] * (float)f[k][l];
				}			
			}
			summe /= s;
			bgauss.field[i][j] = summe;
		}
	}
	bgauss.write(fname);
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
        for (int j = 0; j < this->x; j++)
        {
            this->mask[i][j] = 0;
        }       
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
                {1, 2 , 1}, 
                {0, 0 ,0 }, 
                {-1, -2 ,-1}
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
