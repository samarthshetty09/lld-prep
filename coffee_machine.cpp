#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

using namespace std;

/* 
- The coffee vending machine should support different types of coffee,
  such as espresso, cappuccino, and latte.
- Each type has a specific price and recipe (ingredients and their quantities).
- The machine should have a menu to display the available coffee options and prices.
- Users should be able to select a coffee type and make a payment.
- The machine should dispense the selected coffee and provide change if necessary.
- The machine should track the inventory of ingredients and notify when they are running low.
- The machine should handle multiple user requests concurrently, ensuring thread safety.
*/

class Coffee {
    private:
        string name;
        double cost;
        unordered_map<string, int> recipe;
    
    public:
        Coffee() : name(""), cost(0.0), recipe({}) {} // Default constructor

        Coffee(string name, double cost, unordered_map<string, int> recipe) 
            : name(name), cost(cost), recipe(recipe) {}

        string getName() { return name; }
        double getCost() { return cost; }
        unordered_map<string, int> getRecipe() { return recipe; }
};

class CoffeeMachine {
    private:
        unordered_map<string, Coffee> menu;
        unordered_map<string, int> inventory;
        mutex mtx; // Mutex for thread safety

    public:
        CoffeeMachine() {
            // Initializing coffee menu
            menu.emplace("Latte", Coffee("Latte", 2.00, {{"Milk", 2}, {"Sugar", 1}, {"Foam", 1}}));
            menu.emplace("Espresso", Coffee("Espresso", 3.00, {{"Water", 1}, {"Coffee", 2}}));
            menu.emplace("Cappuccino", Coffee("Cappuccino", 2.50, {{"Milk", 1}, {"Foam", 1}, {"Coffee", 2}}));

            // Initializing inventory
            inventory["Milk"] = 4;
            inventory["Foam"] = 4;
            inventory["Water"] = 4;
            inventory["Sugar"] = 3;
            inventory["Coffee"] = 5; // Added missing ingredient
        }

        void displayMenu() {
            cout << "\n------- Menu -------\n";
            for (auto& coffee: menu) {
                cout << coffee.second.getName() << ": $" << coffee.second.getCost() << endl;
            }
            cout << "--------------------\n";
        }

        bool ingredientCheck(const unordered_map<string, int>& recipe) {
            for(auto& ingredient: recipe) {
                if(inventory[ingredient.first] < ingredient.second) {
                    cout << "Low on: " << ingredient.first << endl;
                    return false;
                }
            }
            return true;
        }

        void selectAndPay(string coffeeName, double amount) {
            lock_guard<mutex> lock(mtx); // Locking machine for thread safety

            // Check if coffee exists in menu
            if (menu.find(coffeeName) == menu.end()) {
                cout << "Error: Coffee type not found.\n";
                return;
            }

            Coffee selectedCoffee = menu[coffeeName];
            auto recipe = selectedCoffee.getRecipe();

            // Check ingredient availability
            if (!ingredientCheck(recipe)) {
                cout << "Cannot prepare coffee due to low ingredients.\n";
                return;
            }

            // Payment processing
            double cost = selectedCoffee.getCost();
            if (amount < cost) {
                cout << "Error: Insufficient amount. " << coffeeName << " costs $" << cost << ".\n";
                return;
            }

            double change = amount - cost;
            dispenseCoffee(recipe);
            cout << "Here is your " << coffeeName << ". Enjoy!\n";
            if (change > 0) {
                cout << "Your change: $" << change << endl;
            }
        }

        void dispenseCoffee(const unordered_map<string, int>& recipe) {
            for(auto& ingredient: recipe) {
                inventory[ingredient.first] -= ingredient.second;
            }
            cout << "Success: Coffee dispensed!\n";
        }
};

int main() {
    CoffeeMachine machine;
    string choice;
    double payment;

    while (true) {
        machine.displayMenu();
        
        cout << "\nEnter coffee name (or type 'exit' to quit): ";
        cin >> choice;
        if (choice == "exit") break;

        cout << "Enter payment amount: $";
        cin >> payment;

        machine.selectAndPay(choice, payment);
    }

    cout << "Thank you for using the Coffee Machine!\n";
    return 0;
}
