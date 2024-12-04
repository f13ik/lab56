#include <iostream>
using namespace std;
#include <string>

int id_t = 0;
class Customer{
private:
    string name, surname;
    unsigned int age, passport;

public:
    //конструктор
    Customer(
        string name_ = "Undefined",\
        string surname_ = "Undefined", \
        unsigned int age_ = 0,\
        unsigned int passport_ = 0\
    )
    {
        name = name_;
        surname = surname_;
        age = age_;
        passport = passport_;
    }
    //деструкт
    ~Customer(){

    }
    string getName() const{ 
        return name; 
    }
    string getSurname() const{
        return surname; 
    }
    unsigned int getAge() const{
        return age;
    }
    unsigned int getPassport() const {
        return passport;
    }

};

class Transaction{
private:
    string sender_card, reciver_card;
    int summ, id;
public:
    Transaction(
        string sender_card_ = "0000 0000 0000 0000",\
        string reciver_card_ = "0000 0000 0000 0000",\
        int id_ = 0,\
        int summ_ = 0
    )
    {
        summ = summ_;
        id = id_;
        sender_card = sender_card_;
        reciver_card = reciver_card_;
    }
    ~Transaction(){
    }
    void SayT(){
        cout << id << " - from: " << sender_card << " to " << reciver_card << " : " << summ << endl; 
    }
    void SayT(int id){
        cout << "from: " << sender_card << " to " << reciver_card << " : " << summ << endl; 
    }
};

class BankAccount : public Customer{
private:
    string card, cid, cvv;
    double summ;
    Transaction** blockchein;
    int blockcheinSize;
protected:
    double getSumm(){
        return summ;
    }
public:
    //конструктор
    BankAccount(
        string card_ = "0000 0000 0000 0000",
        string cid_ = "000000",
        string cvv_ = "000",
        double summ_ = 0.,
        const Customer& customer = Customer()
        
    ) : Customer(customer), card(card_), cid(cid_), cvv(cvv_), summ(summ_), blockchein(nullptr), blockcheinSize(0) {}
    //деструкт
    ~BankAccount(){
        for (int i{}; i < blockcheinSize; ++i){
            delete[] blockchein[i];
        }
        delete[] blockchein;
    }
    //это опера
    BankAccount& operator += (double deposit) {
        summ += deposit;
        //запись операции в блокчейн
        //создаем новый массив указателей, размера старого + 1 и заменяем его на старый
        Transaction* newT = new Transaction("bankomat", card, id_t++, deposit);
        Transaction** newBlockchein = new Transaction*[blockcheinSize + 1];  
        for (int i = 0; i < blockcheinSize; ++i) {
            newBlockchein[i] = blockchein[i];
        }
        newBlockchein[blockcheinSize] = newT;
        delete[] blockchein;
        blockchein = newBlockchein;
        ++blockcheinSize;
        return *this;
    }
    BankAccount& operator -= (double adeposit) {
        summ -= adeposit;
        Transaction* newT = new Transaction("bankomat", card, id_t++, adeposit);
        Transaction** newBlockchein = new Transaction*[blockcheinSize + 1];  
        for (int i = 0; i < blockcheinSize; ++i) {
            newBlockchein[i] = blockchein[i];
        }
        newBlockchein[blockcheinSize] = newT;
        delete[] blockchein;
        blockchein = newBlockchein;
        ++blockcheinSize;
        return *this;
    }
    void MainSayHistory(){
        if (blockcheinSize == 0){
            cout << "blockchein is empty\n";
            return; 
        }
        for (int i{}; i < blockcheinSize; ++i){
            blockchein[i]->SayT();
        }
    }

};

class SavingsAccount: public BankAccount{
private:
    string id;
    double stavka;
public:
    SavingsAccount(
        const Customer& customer,
        string id_ = "000000",
        double summ_ = 0,
        double stavka_ = 0.01,
        string card_ = "0000 0000 0000 0000",
        string cid_ = "000000",
        string cvv_ = "000"

    ) : BankAccount(card_, cid_, cvv_, summ_, customer), id(id_), stavka(stavka_) {}
    ~SavingsAccount(){
    }
    void addCash(double cash){
        *this += cash;
        cout << "nakop +cash:" << cash << endl;
    }
    void addCash(){
        double cur_summ = getSumm();
        double procent = cur_summ * stavka;
        *this += procent;
        cout << "nakop +procent: " << procent << endl;
    }
};

class LoanAccount: public BankAccount{
private:
    string id;
    int loan_summ;
public:
    LoanAccount(
        const Customer& customer,
        string id_ = "000000",
        double summ_ = 0.0,
        double loan_summ_ = 0.0,
        string card_ = "0000 0000 0000 0000",
        string cid_ = "000000",
        string cvv_ = "000"
    ){
        BankAccount(card_, cid_, cvv_, summ_, customer);
        id = id_;
        loan_summ = loan_summ_;
    }
    ~LoanAccount(){
    }
    void takeLoan(double loan){
        loan_summ += loan;
        cout << "loan +: " << loan << endl;
    }
    void payLoan(double amount) {
        if (loan_summ < amount) {
            cout << "loan < your summ\n";
            return;
        }
        loan_summ -= amount;
        cout << "loan -: " << amount << "\ncurrent loan: " << loan_summ << endl;
    }
};




int main(){
    cout << "Bank Sys started\n\n";

    //customer create
    
    string name, surname;
    cout << "name: ";
    cin >> name;
    cout << "surname: ";
    cin >> surname;

    unsigned int age, passport;
    cout << "age: ";
    cin >> age;
    cout << "passport: ";
    cin >> passport;

    Customer person(name, surname, age, passport);

    //acc create
    BankAccount* account = nullptr;
    while (true) {
        cout << "\nCreate bank account:\n1. Debet\n2. Nakop\n3. Loan\n4. Exit\n Enter option: ";
        int choice;
        cin >> choice;

        if (choice == 4) break;
        if (choice == 1){
            account = new BankAccount("1234 5678 9012 3456", "654321", "123", 0.0, person);
        } else if (choice == 2){
            account = new SavingsAccount(person, "123456", 0.0, 0.15, "5678 9012 3456 7890", "654322", "124");
        } else if (choice == 3){
            account = new LoanAccount(person, "1234456", 0.0, 100000.0, "9012 3456 7890 1234", "654323", "125");
        } else{
            cout << "Invalid\n";
            continue;
        }
        cout << "Acc created :)\n";
        break;
    }

    if (account == nullptr) {
        cout << "Acc error :(\n";
        return 0;
    }

    //menu
    while (true) {
        cout << "\nOpera:\n1. +depo\n2. -depo\n3. say hist\n4. exit\n enter: ";
        int operation;
        cin >> operation;

        if (operation == 4) break;
        if (operation == 1){
            cout << "enter +depo: ";
            double amount;
            cin >> amount;
            if (amount > 0) {
                *account += amount;
                cout << "+depo success :)\n";
            } else {
                cout << "invalid +depo :(\n";
            }
        } else if (operation == 2){
            cout << "enter -depo: ";
            double amount;
            cin >> amount;
            if (amount > 0) {
                *account -= amount;
                cout << "-depo success :)\n";
            } else {
                cout << "invalid -depo :(\n";
            }
        } else if (operation == 3) {
            cout << "hist:\n";
            account->MainSayHistory();
        } else {
            cout << "invalid num :(\n";
        }
    }

    delete account;
    cout << "end\n";
    return 0;
}