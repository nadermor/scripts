#include <iostream>

using namespace std;

class BaseClass {
    public:
        //BaseClass() {};
        void f(const char* s = "unknown") {
            cout << "f() in BaseClass called from " << s << endl;
        //h();
        }
    protected:
        void g(const char* s = "unknown") {
            cout << "g() in BaseClass called from " << s << endl;
        }
    private:
        void h(const char* s = "unknown") {
            cout << "h() in BaseClass called from " << s << endl;
        }
};
class Derived1Lv1 : public virtual BaseClass {
    public:
        void f(const char* s = "unknown") {
            cout << "f() in Derived1Lv1 called from " << s << endl;
            g("Derived1Lv1");
            h("Derived1Lv1");
        }
        void h(const char* s = "unknown") {
            cout << "h() in Derived1Lv1 called from " << s << endl;
        }
};
class Derived2Lv1 : public virtual BaseClass {
    public:
        void f(const char* s = "unknown") {
            cout << "f() in Derived2Lv1 called from " << s << endl;
            g("Derived2Lv1");
            // h("Derived2Lv1");
            // h() in private in BC, f() in D2L1 only has protected access.
        }
};
class DerivedLv2 : public Derived1Lv1, public Derived2Lv1 {
    public:
        void f(const char* s = "unknown") {
            cout << "f() in DerivedLv2 called from " << s << endl;
            g("DerivedLv2");
            // without virtual keyword in lv1 classes
            // error: reference to 'g' is ambiguous
            // candidates are : void BaseClass::g(char*)
            //                  void BaseClass::g(char*)
            Derived1Lv1::h("DerivedLv2");
            BaseClass::f("DerivedLv2");
        }
};

int main(int argc, char* argv[]) {
    BaseClass bc;
    Derived1Lv1 d1l1;
    Derived2Lv1 d2l1;
    DerivedLv2 dl2;

    // BaseClass::f("main"); 
    // error: cannot call member function without object
    
    bc.f("main");
    //bc.g("main");
    // error g() is protected
    // Notice: my misunderstanding:
    // the access specifier (private protected public) is for MEMBERS!!
    // NOT FOR INSTANCES!!!
    //bc.h("main");

    d1l1.f("main"); // f() is a member of D1L1, so it has access to g() in BC
    // d1l1.g("main");
    // error g() is protected
    // Notice: my misunderstanding:
    // the access specifier (private protected public) is for MEMBERS!!
    // NOT FOR INSTANCES!!!
    d1l1.h("main");

    d2l1.f("main");
    // d2l1.g("main");
    // error g() is protected
    // d2l1.h("main");

    dl2.f("main");
    // dl2.g("main");
    // error BaseClass's g() is protected, note here virtual plays a role
    dl2.h();
    // D1L1's h()

    return 0;
}

// This way of using string is stupid
// Warning: deprecated conversion from string constant to 'char*' \
// [-Wwrite-strings]
// Why?
// The problem is that you're trying to convert a string (with type const char*[])
// to char*. You can convert a const char*[] to a const char*.
// This conversion is for C compatibility.
