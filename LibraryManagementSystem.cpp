#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Book{
    public:
    string title;
    string author;

    //  default constructor
    Book() {
        title = "UnknownTitle";
        author = "UnknownAuthor";
        isbn = "ISBN";
        copiesAvailable = 0;
        totalCopies = 0;
    }

    Book(const Book& book, string new_isbn) {
        title = book.title;
        author = book.author;
        isbn = new_isbn;
        copiesAvailable = book.copiesAvailable;
        totalCopies = book.totalCopies;
    }

    // when all parameters are passed
    Book(string t, string a, string i, int ca, int tc) {
        title = t;
        author = a;
        isbn = i;
        copiesAvailable = ca;
        totalCopies = tc;
    }

    // getters for isbn, cop_Ava, tot_cop
    string get_isbn() {
        return isbn;
    }

    int get_copies_available() {
        return copiesAvailable;
    }

    int get_total_copies() {
        return totalCopies;
    }

    // utility methods
    bool borrowBook() {
        if (copiesAvailable > 0) {
            return true;
        }
        cout << "Invalid request! Copy of book not available\n";
        return false;
    }

    bool returnBook() {
        if (copiesAvailable + 1 <= totalCopies) {
            return true;
        }
        cout << "Invalid request! Copy of book exceeds total copies\n";
        return false;
    }

    void printDetails() {
        cout << title << " " << author << endl;
    }

   // to update count of copies
   // ? do we have to update both totalCopies and copiesAvailable
   void updateCopies(int n) {
        if(copiesAvailable+n< 0) { cout << "Invalid request! Count becomes negative\n"; }
        else {
            copiesAvailable+= n;
            totalCopies+= n;
        }
   }

   void update_copies_available(int n) { //own
        copiesAvailable += n;
   }

    private:
    string isbn;
    int copiesAvailable;
    int totalCopies;
};

class Member{
    public:
    string name;

    Member(string id, string n, int limit) {
        memberID = id;
        name = n;
        borrowLimit = limit;
    }

    Member(string id, string n) {
        memberID = id;
        name = n;
        borrowLimit = 3;
    }

    string get_memberID() {
        return memberID;
    } //own

    bool borrowBook(string isbn) {
        borrow_count = 0;
        for (auto pair : borrowedBooks){
            borrow_count += pair.second;
        }
        if (borrow_count == borrowLimit) {
            cout << "Invalid request! Borrow limit exceeded\n";
            return false;
        }
        borrowedBooks[isbn]++;
        return true;
    }

    bool returnBook(string isbn) {
        for (auto& pair : borrowedBooks) {
            if (pair.first == isbn) {       
                return true;
            }
        }
        cout << "Invalid request! Book not borrowed\n";
        return false;
    }

    void update_borrowings(string isbn) {
        borrowedBooks[isbn]--;
                if(borrowedBooks[isbn] == 0) {
                    borrowedBooks.erase(isbn);
                }
    }

    int copies_bought(string isbn) {
        return borrowedBooks[isbn];
    }

    void printDetails() {
        if (borrowedBooks.empty()) return;
        for (auto pair : borrowedBooks) {
            cout << memberID << " " << name << " " << pair.first << " " << pair.second << endl;
        }
    }

    private:
    string memberID;
    map<string, int> borrowedBooks;
    int borrow_count;
    int borrowLimit;
};

class Library{
    private:
    vector<Book> books;
    vector<Member> members;

    public:

    vector<Book>& getBooks() { return books; }
    vector<Member>& getMembers() { return members; }

    bool addBook(Book& book) {
        for(Book &b : books) {
            if(b.get_isbn() == book.get_isbn()) {
                cout << "Invalid request! Book with same isbn already exists\n";
                return false;
            }
        }
        // if (book.get_total_copies() < book.get_copies_available()) return false;
        books.push_back(book);
        return true;
    }

    bool registerMember(Member& member) {
        for(Member& m : members) {
            if(m.get_memberID() == member.get_memberID()){
                cout << "Invalid request! Member with same id already exists\n";
                return false;
            }
        }
        members.push_back(member);
        return true;
    }

    bool borrowBook(string memberID, string isbn) {
        Book* book = nullptr;
        for(Book& b : books) {
            if (b.get_isbn() == isbn) {
                book = &b;break;
            } 
        }
        if (!book) {
        cout << "Invalid request! No such book exists\n";
        return false;
        }
        Member* member = nullptr;
        for (Member& m : members) {
            if (m.get_memberID() == memberID) {
                member = &m; break;
            }
        }
        if (!member) {
        cout << "Invalid request! No such member exists\n";
        return false;
        }
        if (book->borrowBook()) {
            if (member->borrowBook(isbn)) {
                book->update_copies_available(-1);
                return true;
            }
        }
    return false;
    }

    bool returnBook(string memberID, string isbn) {
        Book* book = nullptr;
        for(Book& b : books) {
            if (b.get_isbn() == isbn) { book = &b; break; }
        }
        if (!book) {
        cout << "Invalid request! No such book exists\n";
        return false;
        }
        Member* member = nullptr;
        for (Member& m : members) {
            if (m.get_memberID() == memberID) {
                member = &m; break;
            }
        } 
        if (!member) {
        cout << "Invalid request! No such member exists\n";
        return false;
        }
        // int copies = member->copies_bought(isbn);
        if (book->returnBook()) {
            if (member->returnBook(isbn)){
                book->update_copies_available(1);
                member->update_borrowings(isbn);
                return true;
            }
        }
        return false;
    }

    void printBook(string isbn) {
        for (auto b : books) {
            if (b.get_isbn() == isbn) {
                b.printDetails();
                return;
            }
        }
    }

    void printMember(string id) {
        for(auto m : members) {
            if (m.get_memberID() == id) {
                m.printDetails();
                return;
            }
        }
    }

    void printLibraryDetails() {
        for (auto b : books) {
            cout << b.title << " " << b.author << " " << b.get_copies_available() << endl;
        }
        for (auto m : members) {
            cout << m.get_memberID() << " " << m.name << endl;
        }
    }
};

vector<string> string_to_words(string s) {
    vector<string> v;
    string word="";
    for (char ch : s) {
        if (ch == ' '){
            v.push_back(word);
            word = "";
        }
        else word.push_back(ch);
    }
    return v;
}

int main() {
    string s;
    cin >> s;

    Library library;

    while(s != "Done") {
        if(s == "Book") {
            string book;
            cin >> book;
            if(book == "None"){
                Book b;
                library.addBook(b);
            }else if(book == "ExistingBook"){
                string old_isbn, new_isbn;
                cin >> old_isbn >> new_isbn;
                for (auto& b : library.getBooks()) {
                    if (b.get_isbn() == old_isbn) {
                        Book new_book(b, new_isbn);
                        library.addBook(new_book);
                        break;
                    }
                }
            }else {
                string title, author, s_ca, s_tc;
                cin >> title >> author >> s_ca >> s_tc;
                int ca = stoi(s_ca); // copies available
                int tc = stoi(s_tc); // total copies
                Book b(book, title, author, ca, tc);
                library.addBook(b);
            }
            
        }

        if (s == "UpdateCopiesCount") {
            string isbn;
            int new_count;
            cin >> isbn >> new_count;
            for (auto& b : library.getBooks()) {
                if (b.get_isbn() == isbn) {
                    b.updateCopies(new_count);
                    break;
                }
            }
        }

        if (s == "Member") {
            string w1, w2, w3;
            cin >> w1 >> w2 >> w3;
            if (w1 == "NoBorrowLimit") {
                Member m(w2, w3);
                library.registerMember(m);
            }
            else {
                int limit = stoi(w3);
                Member m(w1, w2, limit);
                library.registerMember(m);
            }
        }

        if (s == "Borrow") {
            string id, isbn;
            cin >> id >> isbn;
            library.borrowBook(id, isbn);
        }

        if (s == "Return") {
            string id, isbn;
            cin >> id >> isbn;
            library.returnBook(id, isbn);
        }

        if (s == "PrintBook") {
            string isbn;
            cin >> isbn;
            library.printBook(isbn);
        }

        if (s == "PrintMember") {
            string id;
            cin >> id;
            library.printMember(id);
        }

        if (s == "PrintLibrary") {
            library.printLibraryDetails();
        }

        cin >> s;
    }
}