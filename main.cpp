#include <iostream>
#include <string>
using namespace std;

class Account {
protected:
    string accountNumber;
    string accountHolder;
    double balance;

public:
    Account(string number, string holder, double bal) : accountNumber(number), accountHolder(holder), balance(bal) {}

    virtual void displayDetails() {
        cout << "Account Details for " << getType() << " (ID: " << accountNumber << "):\n";
        cout << "   Holder: " << accountHolder << "\n";
        cout << "   Balance: $" << balance << "\n";
    }

    virtual void deposit(double amount) {
        balance += amount;
    }

    virtual bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return false;
        }
        balance -= amount;
        return true;
    }

    virtual string getType() const = 0;
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(string number, string holder, double bal, double rate) : Account(number, holder, bal), interestRate(rate) {}

    bool withdraw(double amount) override {
        if (balance - amount < 500) {  // Minimum balance requirement
            cout << "Insufficient balance after withdrawal. Minimum balance requirement not met.\n";
            return false;
        }
        return Account::withdraw(amount);
    }

    void displayDetails() override {
        Account::displayDetails();
        cout << "   Interest Rate: " << (interestRate * 100) << "%\n";
    }

    string getType() const override {
        return "Savings Account";
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(string number, string holder, double bal, double limit) : Account(number, holder, bal), overdraftLimit(limit) {}

    bool withdraw(double amount) override {
        if (amount > balance + overdraftLimit) {
            cout << "Overdraft limit exceeded.\n";
            return false;
        }
        balance -= amount;
        return true;
    }

    void displayDetails() override {
        Account::displayDetails();
        cout << "   Overdraft Limit: $" << overdraftLimit << "\n";
    }

    string getType() const override {
        return "Current Account";
    }
};

// Overloading the + operator for transferring money
CurrentAccount operator+(CurrentAccount &current, SavingsAccount &savings) {
    double transferAmount = 300;  // Fixed transfer amount
    if (savings.withdraw(transferAmount)) {
        current.deposit(transferAmount);
    }
    return current;
}

// Overloading the << operator for displaying account details
ostream &operator<<(ostream &os, Account &account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    cout << savings;
    cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    cout << "\nAccount Details after deposit and withdrawal:\n";
    cout << savings;
    cout << current;

    // Transfer 300 from savings to current
    current = current + savings;

    cout << "\nAccount Details after transfer:\n";
    cout << savings;
    cout << current;

    return 0;
}