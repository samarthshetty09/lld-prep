#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Pizza {

    public:
        virtual string getDescription() /* const */ = 0;
        virtual double getCost() /* const */ = 0;
        
        virtual ~Pizza() {};

};

class Margarita: public Pizza {
    string getDescription() /* const override */ {
        return "Margarita";
    }

    double getCost() /* const override */ {
        return 6.00;
    }

};

class ToppingDecorator: public Pizza {
    protected:
        //unique_ptr<Pizza> pizza;
        Pizza *pizza;
    public:
        // ToppingDecorator(unique_ptr<Pizza> p) : pizza(move(p)) {};
        ToppingDecorator(Pizza* p) : pizza(p) {};
};

class Cheese: public ToppingDecorator {
    public:
        //Cheese(unique_ptr<Pizza> p): ToppingDecorator(move(p)) {};
        Cheese(Pizza* p): ToppingDecorator(p){};
        string getDescription() /* const override */ {
            return pizza->getDescription() + " + Cheese";
        }
        double getCost() /* const override */ {
            return pizza->getCost() + 2.00;
        }
};

class Olive: public ToppingDecorator {
    public:
        Olive(Pizza* p): ToppingDecorator(p) {};

        string getDescription() /* const override */ {
            return pizza->getDescription() + " + Olive";
        }

        double getCost() /* const override */ {
            return pizza->getCost() + 3.00;
        }
};

int main() {
    Pizza* p = new Margarita();
    p = new Cheese(p);
    p = new Olive(p);
    cout << p->getDescription() << endl;
    cout << "Cost: " << p->getCost() << endl;

}
