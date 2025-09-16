#include <iostream>
using namespace std;
int main()
 {
 int*p1=new int;
 int*p2=new int(10);
 int*p3=new int[10];
 p3[0]=1;
 p3[1]=2;
 p3[2]=3;
 cout<<*p1<<endl<<*p2<<endl<<*p3<<endl;
 delete p1;
 delete p2;
 delete []p3;
 p1= nullptr;
 p2=nullptr;
 p3=nullptr;
 }