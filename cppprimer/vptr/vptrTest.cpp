#include <iostream>
#include <typeinfo>
#include <functional>

typedef long long address;
typedef void(*printF)(void);

class Base
{
public:
    Base(int i):baseI(i) {}
    int getI() { return baseI; }
    static void countI() {}
    virtual void print(void) { std::cout << "Base::print()" << std::endl; }
    virtual void print1(void) { std::cout << "Base::print1()" << std::endl; }
    virtual ~Base() {  std::cout << "Base::~Base()" << std::endl; }
    // 析构函数定义在其他成员函数的前面就会有问题，为什么？
    // 搜了下，析构函数占两个位，但是暂时不知道为什么
private:
    int baseI;
    static int baseS;
};

void testBase(Base& p)
{
    std::cout << "对象的内存初始地址：" << &p << std::endl;
    std::cout << "type_info信息：" << std::endl;
    std::cout << typeid(p).name() << std::endl;
    std::type_info* r = (std::type_info*)*((address*)*(address*)(&p) - 1);
    std::cout << r->name() << std::endl;

    std::cout << "虚函数指针的地址：" << (address*)(&p) << std::endl;

    std::cout << "虚函数表地址：" << (address*)*(address*)(&p) << std::endl;
    std::cout << "析构函数slot：" << (address*)*(address*)(&p) << std::endl;
    std::cout << "析构函数的地址：" << (address*)*(address*)*(address*)(&p) << std::endl;
    std::cout << "printslot：" << ((address*)*(address*)(&p) + 1) << std::endl;
    std::cout << "print的地址：" << (address*)*((address*)*(address*)(&p) + 1) << std::endl;

    printF f = (printF)* ((address*)*(address*)(&p));
    printF g = (printF)* ((address*)*(address*)(&p) + 1);
    printF h = (printF)* ((address*)*(address*)(&p) + 2);
    f();
    std::cout << "f end" << std::endl;
    g();
    std::cout << "g end" << std::endl;
    h();
    std::cout << "h end" << std::endl;
}

int main()
{
    Base b(1);
    testBase(b);
    std::cout << "all end" << std::endl;

    return 0;
}
