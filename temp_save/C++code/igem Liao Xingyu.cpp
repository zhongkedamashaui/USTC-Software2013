#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define NN 100 
#define PETS 128
#define STEP (1.0/PETS)
#define MAXTIME 100
#define INITIALVALUE 2.5
#define DIMENSX 220
#define DIMENSY 1800
using namespace std;

class mainFunc
{
public:
    mainFunc()
    {
        int ik;
        p=new double[DIMENSY];
        q=new double[DIMENSY];
        r=new double[DIMENSY];
        nn=new double[DIMENSY];
        for(ik=0;ik<DIMENSY;++ik)
        {
            p[ik]=18;q[ik]=9;r[ik]=2;nn[ik]=6;
        }
        nong=new double[DIMENSY];
        MaxMa=new double*[DIMENSY];
        for(ik=0;ik<DIMENSY;++ik)MaxMa[ik]=new double[DIMENSX];
    }
    double **MaxMa;
    double *nong;
    void Network_1(double **ReguMatrix,int nx,int ny);
    void Network_2(double **Matr,int nx,int ny);
private:
    double *p,*q,*r,*nn;
    void RandMatrix(double **a,double **b,const int nx,const int ny)
    double FaNexVal(double **Matr,double a[],const int nx,const int i,double p[],double q[],double nn[],double r[])
};




void mainFunc::RandMatrix(double **a,double **b,const int nx,const int ny)
{
    int i1,j1,k1,m1;
    //srand((unsigned)time(0));
    for(i1=0;i1!=ny+1;++i1)
        for(j1=0;j1!=nx+1;++j1)
    {
        if(i1<ny&&j1<nx)
        {
            m1=(int)a[i1][j1];
            switch(m1)
            {
                case 1:b[i1][j1]=(double)(rand()%101)/150.0+0.5;break;
                case -1:b[i1][j1]=-(double)(rand()%101)/150.0-0.5;break;
                case 0:b[i1][j1]=0;break;
                default :m1=rand()%2;
                if(m1)b[i1][j1]=(double)(rand()%101)/150.0+0.5;
                else b[i1][j1]=-(double)(rand()%101)/150.0-0.5;
            }
        }
        else
        {
            m1=(int)(a[i1][j1]*100);
            if(rand()%100<50+abs(m1)/2)
                k1=m1>0?(double)(rand()%101)/150.0+0.5:-(double)(rand()%101)/150.0-0.5;
            else if(rand()%10)k1=0;
            else k1=m1<0?(double)(rand()%101)/150.0+0.5:-(double)(rand()%101)/150.0-0.5;
            b[i1][j1]=k1;
        }
    }
}

double mainFunc::FaNexVal(double **Matr,double a[],const int nx,const int i,    double p[],double q[],double nn[],double r[])
{
    int j;
    double m[3];m[0]=0;
    m[0]=0-r[i]*a[i];
    m[2]=0;m[1]=0;
    for(j=0;j<nx;++j)
    {
        if(Matr[i][j]>0)m[1]+=pow(a[j],Matr[i][j]);
        else if(Matr[i][j]<0)m[2]+=pow(a[j],-Matr[i][j]);
    }
    if(m[1]==0)
    m[0]+=(p[i]/(nn[i])+q[i]/(nn[i]+m[2]));
    else m[0]+=(p[i]*m[1]/(nn[i]+m[1])+q[i]/(nn[i]+m[2]));
    return m[0];
}

/*double NextValue(double Matr[][DIMENS],double a[],const int n,const int i,double step ,   double p[],double q[],double nn[],double r[])
{
    int j;double b[DIMENS];
    for(j=0;j<n;++j)b[j]=a[j]+step*FaNexVal(Matr,a,n,j,p,q,nn,r)/2.0;
    return FaNexVal(Matr,b,n,i,p,q,nn,r);
}*/

void mainFunc::Network_1(double **ReguMatrix,int nx,int ny)
{
    double *a,*b,*c,*d,*e,*f,sum(0),AbsValue(1),MaxScore(-1),Score(0);
    double FenShu[101];
    int i(0),j=0,cou(0),k(0);
    double **TempMatrix;
    a=new double[DIMENSY];
    b=new double[DIMENSY];
    c=new double[DIMENSY];
    d=new double[DIMENSY];
    e=new double[DIMENSY];
    f=new double[DIMENSY];
    TempMatrix=new double*[DIMENSY];
    for(i=0;i<DIMENSY;++i)
        TempMatrix[i]=new double[DIMENSX];
    double step=STEP;
    int pets=PETS;
    for(i=0;i<101;++i)
        FenShu[i]=0;
    for(i=0;i<ny;++i)
        a[i]=b[i]=e[i]=INITIALVALUE;  
    while(k<NN)                               
    {
        ++k;
        RandMatrix(ReguMatrix,TempMatrix,nx,ny); 
        AbsValue=10;
        j=0;
        step=STEP;
        pets=PETS;
        while(AbsValue>0.000001&&cou<MAXTIME)
        {
            ++j;
            for(i=0;i<ny;++i)
            {
                a[i]+=FaNexVal(TempMatrix,b,nx,i,p,q,nn,r)*step;
                if(a[i]<0.000001)
                    {
                        cou=MAXTIME+2;
                        break;
                    }
            }
            if(cou>=MAXTIME)
                break;   
            for(i=0;i<ny;++i)
                b[i]=a[i];
            if(j%(pets/8)==0)
            {
                AbsValue=0;
                for(i=0;i<ny;++i)
                    {
                        AbsValue+=fabs(a[i]-e[i]);
                    }
                for(i=0;i<ny;++i)
                    e[i]=a[i];
                if(AbsValue<5&&pets==PETS&&j==PETS)
                    {
                        step=STEP*4.0;
                        pets=PETS/4;
                        ++cou;
                        j=0;
                        continue;
                    }
            }
            if(j%pets==0)
                {
                    ++cou;
                    j=0;
                }
        }
        step=STEP;
        pets=PETS;
        if(cou>=MAXTIME)break;
        for(i=0;i<ny;++i)
            {
                c[i]=d[i]=a[i];
            }
        c[ny]=0;
        d[ny]=c[ny];
        cou=0;
        AbsValue=10;
        j=0;
        while(AbsValue>0.000001&&cou<MAXTIME)
        {
            ++j;
            for(i=0;i<ny+1;++i)
            {
                c[i]+=FaNexVal(TempMatrix,d,nx+1,i,p,q,nn,r)*step;
                if(c[i]<0.000001)
                    {
                        cou=MAXTIME+2;
                        break;
                    }
            }
            for(i=0;i<ny+1;++i)
                d[i]=c[i];
            if(j%(pets/8)==0)
            {
                AbsValue=0;
                for(i=0;i<ny+1;++i)
                {
                    AbsValue+=fabs(c[i]-f[i]);
                }
                for(i=0;i<ny+1;++i)
                    f[i]=c[i];
                if(AbsValue<5&&pets==PETS&&j==PETS)
                    {
                        step=STEP*4.0;
                        pets=PETS/4;
                        ++cou;
                        j=0;
                        continue;
                    }
            }
            if(j%pets==0)
                {
                    j=0;
                    ++cou;
                }
        }
        if(cou>=MAXTIME)
            {
                sum+=1;
                FenShu[0]+=1;
                break;
            } 
        AbsValue=0;
        sum+=1;
        for(i=0;i<ny;++i)
            AbsValue+=(fabs(c[i]-a[i]))/(c[i]>a[i]?a[i]:c[i]);
        Score=1-AbsValue*AbsValue/(ny*ny/9.0+AbsValue*AbsValue);
        Score*=100;
        Score*=(1-(pow(((double)cou)/MAXTIME,3)));
        if(Score>MaxScore)    
        {
            for(i=0;i<ny+1;++i)
            for(j=0;j<nx+1;++j)
                MaxMa[i][j]=TempMatrix[i][j];
            MaxScore=Score;
        }
        FenShu[(int)Score]+=1;   
    }
    for(i=0;i<101;++i)
        FenShu[i]/=sum;  
    ofstream fi1;
    fi1.open("Score");
    for(i=0;i<101;++i)
        fi1<<i+0.5<<' '<<FenShu[i]<<endl;
    fi1.close();
    for(i=0;i<ny;++i)
        a[i]=b[i]=e[i]=INITIALVALUE;
    AbsValue=10;
    j=0;
    cou=0;
    step=STEP;
    pets=PETS;
    while(AbsValue>0.000001&&cou<MAXTIME)    
    {
        ++j;
        for(i=0;i<ny;++i)
            a[i]+=FaNexVal(MaxMa,b,nx,i,p,q,nn,r)*step;
        for(i=0;i<ny;++i)
            b[i]=a[i];
        if(j%(pets/8)==0)
        {
            AbsValue=0;
            for(i=0;i<ny;++i)
                AbsValue+=fabs(a[i]-e[i]);
            for(i=0;i<ny;++i)
                e[i]=a[i];
            if(AbsValue<5&&pets==PETS&&j==PETS)
                {
                    step=STEP*4.0;
                    pets=PETS/4;
                    ++cou;
                    j=0;
                    continue;
                }
        }
        if(j%pets==0)
            {
                ++cou;
                j=0;
            }
    }
    for(i=0;i<ny;++i)
        {
            c[i]=d[i]=f[i]=a[i];
        }
    c[ny]=0;
    d[ny]=f[ny]=c[ny];
    ofstream igemSfw;
    igemSfw.open("EachGeneChange");
    if(!igemSfw)
        exit(0);
    j=0;
    cou=0;
    AbsValue=10;
    step=STEP;
    pets=PETS;
    while(AbsValue>0.00000001&&cou<MAXTIME)  
    {
        ++j;
        for(i=0;i<ny+1;++i)
                c[i]+=FaNexVal(MaxMa,d,nx+1,i,p,q,nn,r)*step;
        for(i=0;i<ny+1;++i)
            d[i]=c[i];
        if(j%(pets/8)==0)
        {
            igemSfw<<cou+j*step<<' '<<flush;
            for(i=0;i<ny;++i)igemSfw<<c[i]<<' '<<flush;
            igemSfw<<c[ny]<<endl;
            AbsValue=0;
            for(i=0;i<ny+1;++i)
                AbsValue+=fabs(c[i]-f[i]);
            for(i=0;i<ny+1;++i)
                f[i]=c[i];
            if(AbsValue<5&&pets==PETS&&j==PETS)
            {
                step=STEP*4.0;
                pets=PETS/4;
                ++cou;
                j=0;
                continue;
            }
        }
        if(j%pets==0)
        {
            ++cou;
            j=0;
        }
    }
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
    delete[] e;
    delete[] f;
    for(i=0;i<DIMENSY;++i)
        delete[] TempMatrix[i];
}



void mainFunc::Network_2(double **Matr,int nx,int ny)
{
    int i,j=0,cou=0;
    double b[DIMENSY],AbsValue=10,c[DIMENSY];
    for(i=0;i<ny;++i)
        {
            nong[i]=INITIALVALUE;
            b[i]=nong[i];
            c[i]=b[i];
        }
    int pets=PETS;
    double step=STEP;
    while(AbsValue>0.0000001&&cou<MAXTIME) 
    {
        ++j;
        for(i=0;i<ny;++i)
            {
                nong[i]+=FaNexVal(Matr,b,nx,i,p,q,nn,r)*step;
                if(nong[i]<0.0000001)
                    {
                        cou=MAXTIME+2;
                        break;
                    }
            }
        if(cou>=MAXTIME)break;                
        for(i=0;i<ny;++i)
            b[i]=nong[i];
        if(j%(pets/8)==0)
        {
            AbsValue=0;
            for(i=0;i<ny;++i)
                {
                    AbsValue+=fabs(nong[i]-c[i]);
                }
            for(i=0;i<ny;++i)
                c[i]=nong[i];
            if((AbsValue<5)&&(pets==PETS)&&(j==PETS))
                {
                    step=STEP*4.0;
                    pets=PETS/4;
                    ++cou;
                    j=0;
                    continue;
                }
        }
        if(j%pets==0)
            {
                ++cou;
                j=0;
            }
    }
    if(cou>=MAXTIME) 
    for(i=0;i<ny;++i)
        nong[i]=-1;
}