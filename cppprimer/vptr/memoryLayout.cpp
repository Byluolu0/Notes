#include <iostream>
#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>

using std::cout;
using std::endl;

typedef long long address;
typedef void(*Fun)(void);

// 无继承 无virtual
class A 
{
public:
    A(int i):data(i) { cout << "A::A" << endl; }
    void f() { cout << "A::f" << endl; }
    static void sf() { cout << "A::sf" << endl; }
    ~A() { cout << "A::~A" << endl; }
    int data;
    static int sdata;
};

// 无继承 有virtual
class B 
{
public:
    B(int i):data(i) { cout << "B::B" << endl; }
    void f() { cout << "B::f" << endl; }
    static void sf() { cout << "B::sf" << endl; }
    virtual void vf() { cout << "B::vf" << endl; }
    virtual ~B() { cout << "B::~B" << endl; }
    int data;
    static int sdata;
};


// 单继承
class C 
{
public:
    C(int i):data(i) { cout << "C::C" << endl; }
    void f() { cout << "C::f" << endl; }
    static void sf() { cout << "C::sf" << endl; }
    virtual void vf() { cout << "C::vf" << endl; }
    virtual void vf1() { cout << "C::vf1" << endl; }
    virtual ~C() { cout << "C::~C" << endl; }
    int data;
    static int sdata;
};

class DC : public C
{
public:
    DC(int i):C(i) { cout << "DC::DC" << endl; }
    virtual void vf() { cout << "DC::vf" << endl; }
    virtual void vf1() { cout << "DC::vf1" << endl; }
    virtual void vdcf() { cout << "DC::vdcf" << endl; }
    virtual ~DC() { cout << "DC::~DC" << endl; }
};
// 多继承
class D1 {
public:
    D1(int d, int d1): data(d), data1(d1) { cout << "D1::D1" << endl; }
    virtual void vf() { cout << "D1::vf" << endl; }
    virtual void vf1() { cout << "D1::vf1" << endl; }
    void g() { cout << "D1::g" << endl; }
    void h() { cout << "D1::h" << endl; }
    virtual ~D1() { cout << "D1::~D1" << endl; }
    int data;
    int data1;
};

class D2 {
public:
    D2(int d, int d2): data(d), data2(d2) { cout << "D2::D2" << endl; }
    virtual void vf() { cout << "D2::vf" << endl; }
    virtual void vf2() { cout << "D2::vf2" << endl; }
    virtual void g() { cout << "D2::g" << endl; }
    virtual void h() { cout << "D2::h" << endl; }
    virtual ~D2() { cout << "D2::~D2" << endl; }
    int data;
    int data2;
};

class DD: public D1, public D2
{
public:
    DD(int d11, int d12, int d21, int d22): D1(d11, d12), D2(d21, d22) { cout << "DD::DD" << endl; }
    virtual void vf() { cout << "DD::vf" << endl; }
    virtual void vf1() { cout << "DD::vf1" << endl; }
    virtual void vf2() { cout << "DD::vf2" << endl; }
    virtual void g() { cout << "DD::g" << endl; }
    void h() { cout << "DD::h" << endl; }
    virtual ~DD() { cout << "DD::~DD" << endl; }
};

// 虚拟继承

// 菱形继承

int main()
{
    cout << "=== 无继承 无virtual ===" << endl;
    cout << "Class A" << endl;
    A a(10);
    cout << "size: " << sizeof(a) << endl;
    cout << "对象地址：" << &a << endl;
    cout << "成员data地址：" << &a.data << endl;
    cout << endl;

    cout << "=== 无继承 有virtual ===" << endl;
    cout << "对象的首地址有一个虚函数指针，指向虚函数表" << endl;
    cout << "Class B" << endl;
    B b(11);
    cout << "size: " << sizeof(b) << endl;
    cout << "B对象比A对象多一个虚函数指针(8byte)，加上字节对齐，所以是16byte" << endl;
    cout << "对象地址：" << &b << endl;
    cout << "成员data地址：" << &b.data << endl;
    cout << endl;
    
    address* vtbPtr = (address*)*(address*)(&b); 
    cout << "B对象虚函数表地址：" << vtbPtr << endl;
    Fun vf = (Fun)((address*)*vtbPtr);
    vf();

    cout << "=== 单继承 ===" << endl;
    cout << "对象的首地址有一个虚函数指针，指向虚函数表" << endl;
    C c(12);
    cout << "size C: " << sizeof(c) << endl;
    cout << "c对象地址：" << &c << endl;
    cout << "c成员data地址：" << &c.data << endl;

    address* cVtbPtr = (address*)*(address*)(&c);
    Fun cvf = (Fun) ((address*)*cVtbPtr);
    cvf();
    Fun cvf2 = (Fun) ((address*)*(cVtbPtr + 1));
    cvf2();
    cout << endl;

    DC dc(13);
    cout << "size DC: " << sizeof(dc) << endl;
    cout << "dc对象地址：" << &c << endl;
    cout << "dc成员data地址：" << &c.data << endl;

    address* dcVtbPtr = (address*)*(address*)(&dc);
    Fun dcvf = (Fun) ((address*)*dcVtbPtr);
    dcvf();
    Fun dcvf2 = (Fun) ((address*)*(dcVtbPtr + 1));
    dcvf2();
    // 析构函数占了两个指针的位
    Fun dcvdcf = (Fun) ((address*)*(dcVtbPtr + 4));
    dcvdcf();

    cout << endl;

    cout << "=== 普通多继承 ===" << endl;

    D1 d1(1, 2);
    D2 d2(3, 4);
    DD dd(1, 2, 3, 4);
    cout << "D1 D2 DD 这三个类一共有四个虚函数表" << endl;
    cout << "D1虚函数表地址：" << (address*)*(address*)(&d1) << endl;
    cout << "D2虚函数表地址：" << (address*)*(address*)(&d2) << endl;
    cout << "DD第一个虚函数表地址：" << (address*)*(address*)(&dd) << endl;
    cout << "DD第二个虚函数表地址：" << (address*)*((address*)(&dd) + 2) << endl;

    cout << "对象有多个虚函数指针，第一个在首地址，指向的虚函数表，对应第一个基类的虚函数表，子类实现的所有虚函数，有对应的函数则覆盖，没有则补上" << endl;
    cout << "接下来是第一个基类的数据成员" << endl;
    cout << "然后是第二个虚函数指针，指向第二个虚函数表，对应第二个基类的虚函数表，子类实现的所有虚函数，有对应的则覆盖，没有对应的则跳过" << endl;
    cout << "接下来是第二个基类的数据成员" << endl;
    cout << "以此类推" << endl;
    cout << "size: " << sizeof(DD) << endl;
    cout << "dd1:" << endl;

    // request for member ‘data’ is ambiguous
    // 编译报错，两个基类都有data成员，二义性
    // cout << dd.data << endl;

    cout << "dd地址：" << &dd << endl;
    D1* dd1 = &dd;
    // D1* dd1 = new DD(1, 2, 3, 4);
    cout << "对象地址：" << dd1 << endl;
    cout << "D1的data地址：" << &dd1->data << endl;
    cout << "D1的data1地址：" << &dd1->data1 << endl;

    // dd对应的d1 虚函数表地址(第一个虚函数表)
    address* d1VtbPtr = (address*)*(address*)dd1;
    cout << d1VtbPtr << endl;

    // dd的vf，覆写了
    Fun dd1vf = (Fun) (address*)*d1VtbPtr; 
    dd1vf();
    // dd的vf1，覆写了
    Fun dd1vf1 = (Fun) (address*)*(d1VtbPtr + 1); 
    dd1vf1();
    // dd的vf2，d1中没有这个虚函数（但d2有），补上去
    // +4是要跳过虚函数
    Fun dd1vf2 = (Fun) (address*)*(d1VtbPtr + 4); 
    dd1vf2();
    // dd的g，d1中没有这个虚函数，补上去
    Fun dd1g = (Fun) (address*)*(d1VtbPtr + 5); 
    dd1g();

    cout << "dd2:" << endl;
    D2* dd2 = &dd;
    cout << "对象地址：" << dd2 << endl;
    cout << "D2的data地址：" << &dd2->data << endl;
    cout << "D2的data1地址：" << &dd2->data2 << endl;

    // dd对应的d2 虚函数表地址(第二个虚函数表)
    address* d2VtbPtr = (address*)*(address*)dd2;
    cout << d2VtbPtr << endl;
    // dd的vf，覆写了
    Fun dd2vf = (Fun) (address*)*d2VtbPtr;
    dd2vf();
    // dd的vf2，覆写了
    Fun dd2vf2 = (Fun) (address*)*(d2VtbPtr + 1); 
    dd2vf2();
    // dd的g，覆写了
    Fun dd2g = (Fun) (address*)*(d2VtbPtr + 2); 
    dd2g();
    // dd的h，覆写了
    Fun dd2h = (Fun) (address*)*(d2VtbPtr + 3); 
    dd2h();
    cout << "dd的vf1，d2中没有这个虚函数（但d1有），这里不补，只在第一个虚函数表补" << endl;
}

/************
 * 注意事项 *
 ************/

// 析构函数占了两个指针的位