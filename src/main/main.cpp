#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class abstract_shit {
public:
    virtual void foo() {
        cout << "from parent" << endl;
    }
};

template<typename T>
class base_shit : public abstract_shit {

public:
    void foo() override final {
        inner_class a;
        _foo(a);
    }

protected:

    class inner_class {
    public:
        T value;
    };

    virtual void _foo(inner_class shit) {
        cout << "shit" << shit.value << endl;
    }

};

class derive_shit1 : public base_shit<string> {

private:
    void _foo(inner_class shit) override {
        cout << "from derive_shit1 " << shit.value << endl;
    }

};

class derive_shit2 : public base_shit<int> {

private:
    void _foo(inner_class shit) override {
        cout << "from derive_shit2 " << shit.value << endl;
    }

};



int main() {
    vector<unique_ptr<abstract_shit>> shits;
    derive_shit1* shit1 = new derive_shit1();
    derive_shit2* shit2 = new derive_shit2();

    shits.push_back(unique_ptr<abstract_shit>(shit1));
    shits.push_back(unique_ptr<abstract_shit>(shit2));

    for (auto& c : shits) {
        c->foo();
    }
}








