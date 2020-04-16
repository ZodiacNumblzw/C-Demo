//author:Numb
#include<iostream>
#include<algorithm>
#include<mutex>
#include<thread>
using namespace std;
const int maxn=1e7+10;
//maxLen不能太小，否则线程太多会terminate called recursively
int maxLen=2e4;

//锁
mutex mu;
//线程总数
static int nums=0;
void Qsort(int a[],int l,int r)
{
    if(r<=l) return;
    int head=l,tail=r,base=a[l];
    while(head<tail)
    {
        while(head<tail&&a[tail]>=base) tail--;
        if(head<tail) a[head++]=a[tail];
        while(head<tail&&base>=a[head]) head++;
        if(head<tail) a[tail--]=a[head];
    }
    a[head]=base;
    if(head-l>maxLen||r-head>maxLen)
    {
        thread t1(Qsort,std::ref(a),l,head-1);
        thread t2(Qsort,std::ref(a),head+1,r);
        t1.join();
        t2.join();
        lock_guard<mutex> temp(mu);
        nums+=2;
    }
    else
    {
        Qsort(a,l,head-1);
        Qsort(a,head+1,r);
    }
    //lock_guard<mutex>muguard(mu);
    /*Qsort(a,l,head-1);
    Qsort(a,head+1,r);*/
}

int a[maxn];
int b[maxn];
int main()
{
    srand(time(NULL));
    int n;
    while(cin>>n)
    {
        nums=0;
        for(int i=1;i<=n;i++)
        {
            b[i]=rand()%100000;
        }
        for(int i=1;i<=n;i++) a[i]=b[i];
        clock_t start=clock();
        Qsort(a,1,n);
        clock_t stop=clock();
        cout<<(stop-start)/(double(CLOCKS_PER_SEC))<<' '<<nums<<endl;

        for(int i=1;i<=n;i++) a[i]=b[i];
        start=clock();
        sort(a+1,a+n+1);
        stop=clock();
        cout<<(stop-start)/(double(CLOCKS_PER_SEC))<<endl;
    }
    return 0;
}

/*
多线程快排比单线程快排快将近2倍
*/
