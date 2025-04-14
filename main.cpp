#include <fstream>
  #include <iostream>
  #include <limits>
  #include <iomanip> //input output manipulation
  #include <sstream> //string int conversion

  using namespace std;
  //bale C++ makes use of standard namespace or std:: , for readability we made use of using namespace std

  //these are ANSI Escape Sequences defined using the preprocessor define directive

  #define GREEN "\033[32m"
  #define LIGHT_YELLOW "\033[93m"
  #define LIGHT_MAGENTA "\033[95m" 
  #define RED "\033[31m"
  #define LIGHT_BLUE "\033[94m"
  #define BLUE "\033[34m"
  #define CYAN "\033[36m"
  #define RESET "\033[0m"
  #define MAX_BOOKS 100

  //we define MAX BOOKS instead of writing 100 directly for
  //a) Readability
  //b) so we can alter the size easily here when needed

//"define" names a constant value that cannot be changed during program execution
//this also allows the programmer to change MAX BOOKS quickly without going one by one

//structures and members obtained from opac
  struct Details {
      string library;
      string callNumber;
  };

  struct Books {
    string title;
    string author;
    string category;
    string isbn;
    string edition;
    string year;
    string subject;
    Details details;
    int searches;
  };
  //TAKE NOTE WE HAVE 10 STRUCTURE MEMBERS, 10 is our magic number for this program
  //any instances of 10 refers to these, with the first or the "0"th being the title


  //Arrays serves as a pointer to the first element, and since our struct is an array, we pass it as a pointer
  //and since its a pointer we tamper with memory, ergo
  //we PASS BY REFERENCE
  void addBook(Books *book, int size);
  void displayByCategory(Books *book, int size);
  void searchBook(Books *book, int size);
  void mostPopularTitles(Books *book, int size);
  void loadFromFile(Books *book, int &size);
  void clear(); 
  void mainMenu();
  bool checkBook(string title, Books *book, int size); //returs a bool value after comparison
  //declares functions to be initialized later

  int main() {

    int size = 0;
    Books book[MAX_BOOKS];
    loadFromFile(book, size); //loads all the books from the file back into the array, and sets size to the next index to be initialized, this allows the program to remember the books after it is closed and reopened
      //load from file also GETS the size from size.txt file back and gives size its new value

    //infinite loop unlessed we returned 0
    while (true) {

      mainMenu(); //loads the menu interface when called

      int choice;

      cout << "Enter your choice: ";

      while (!(cin >> choice) || choice < 1 || choice > 5) { //if we fail to read a valid input cin returns FALSE, a false being returned means the loop executes due to the negation or not operator
              cin.clear(); 
              clear();
              cout << RED << "-----Invalid input-----" << RESET; 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            mainMenu();
          cout << "Enter your choice: ";
      }

      switch (choice) {
                  case 1:
                    if (size >= MAX_BOOKS){cout << RED " MAX BOOKS REACHED" RESET << endl; break;} //doesnt allow further adding after 100
                      while (true) {
                          clear();
                          cout << LIGHT_YELLOW "\n---------Add a Book!-------------" RESET << endl;
                          addBook(book, size);
                          size++;
                          char backChoice;
                          cout << "Press 'B' to " << LIGHT_YELLOW << "continue adding books" << RESET << ", or any other key to go back to the main menu: ";
                          cin >> backChoice;
                          if (toupper(backChoice) != 'B') {clear(); break;}// If the user wants to go back to the main menu
                      }
                      break;

                  case 2:
                      while (true) {
                          clear();
                          cout << BLUE "\n-----------Category Search------------" RESET << endl;
                          displayByCategory(book, size);
                          char backChoice;
                          cout << "Press 'B' to " << LIGHT_YELLOW << "continue searching by category" << RESET << ", or any other key to go back to the main menu: ";
                          cin >> backChoice;
                         if (toupper(backChoice) != 'B') {clear(); break;} // If the user wants to go back to the main menu
                      }
                      break;

                  case 3:
                      while (true) {
                          clear();
                          cout << CYAN "\n------------Book Search--------------" RESET << endl;
                          searchBook(book, size);
                          char backChoice;
                          cout << "Press 'B' to " << LIGHT_YELLOW << "continue searching for books" << RESET ", or any other key to go back to the main menu: ";
                          cin >> backChoice;
                          if (toupper(backChoice) != 'B') {clear(); break;} // If the user wants to go back to the main menu
                      }
                      break;

                  case 4:
                          clear();
                          cout << LIGHT_MAGENTA "\n-----Most Popular Titles-------" RESET << endl;
                          mostPopularTitles(book, size);
                      break;

                  case 5:
                      cout << "Exiting the program." << endl;
                      return 0;

                  default:
                      cout << "Invalid choice. Please try again." << endl;
                      break;
              }

          }
      }


/* gets the line and stores data to the array again when the program is ran,
sstream reads integer values from strings and is used to store integers back to the program */
//called when program starts, to store back, the title, category and searches for easy access
//the rest will be read from the file, but these three are stored so that our program wont go through line by line in a txt file to look for
//a specific title, instead we use the DOT ACCESSOR FOR READABILITY PURPOSES, EASY ACCESS and BETTER CODE FLOW

  void loadFromFile(Books *book, int &size){
    ifstream file("books.txt");
    //stores portion of data back to the array, this is so we dont refer to the file and search line by line 
    //though we wont be putting back all since the rest can be easily tracked when searching instead
    if (file.is_open()){
      string line;
      int i = 0;
       while (getline(file, line)){ //constantly reads from file and stores it line by line
         book[i].title = line;
         getline(file, line);
         getline(file, line);
         book[i].category = line;
         for(int i = 0; i < 7; i++){//each getline goes down to the file, it doesnt store some of them in order to spare memory
           getline(file, line);     //but at certain lines it stores data like when it hits title, category and searches
         }                          //getline is called 10 times to move through each MEMBER of the structure
         stringstream ss(line);
         ss >> book[i].searches;
         i++;
         size++;
       }
    }

  }

//MINOR FUNCTIONS

  void clear(){

    cout << "\033[2J\033[1;1H";
      //ANSI escape code to clear the screen

  }

  //displays the main menu
  void mainMenu() {
    cout <<  "\n  Library Management System" << endl;
    cout << GREEN "---------------------------------" << endl;
    cout << "| 1. Add Book                  |"  << endl;
    cout << "| 2. Display Books by Category |" << endl;
    cout << "| 3. Search Book               |" << endl;
    cout << "| 4. Most Popular Titles       |" << endl;
    cout << "| 5. Exit                      |" << endl;
    cout << "--------------------------------" RESET << endl;
}

/*checks if book exists*/
/*The function 'checkBooks' is a boolean type, meaning it only returns true or false. Based on the algorithm of the bool function, 
if the function returns false, then the text thas has been inputted is similar to the existing texts in the data base. 
If it returns true then the inputted is unique and is not a duplicate

Now, we take this bool 'checkBook' function as a parameter in the while loop of the 'addBook' function.
In the while loop, it basically negates the checkBook function. This implies that if the checkBook returns false, 
then the syntax inside the while loop will be implemented until the checkBooks returns true*/

bool checkBook(string title, Books *book, int size){

   for(int i = 0; i < title.length(); i++){
        title[i] = toupper(title[i]);
    }

string upperbooktitle;
    for (int i = 0; i < size; i++) {
        upperbooktitle = book[i].title;

        for(int j = 0; j < upperbooktitle.length(); j++){
        upperbooktitle[j] = toupper(upperbooktitle[j]);
        }

      if (upperbooktitle == title) {
        return false;
      }
    }
    return true;
} 

/*adding of books functionality,
bale getline is tantamount to C's gets() and gets a string input ignoring null terminator,
    cin.ignore allows ignoring the \n from the previous cin statement
    .empty() is a function that returns a bool value that checks if a string is empty, checking this prevents empty inputs from users
*/

/*The program remprompts if
EMPTY INPUTS
INVALID YEAR
EXISTING TITLE
MAX BOOKS REACHED
*/

  void addBook(Books *book, int size) {

    char userEditionChoice;
    cin.ignore();
    cout << "Enter Book Title: ";
    getline(cin, book[size].title);


    while (book[size].title.empty()) {
        cout << RED "Title cannot be empty." RESET << endl;
        cout << "Enter Book Title: ";
        getline(cin, book[size].title);
    }

    while(!checkBook(book[size].title, book, size)){
        cout << RED "Book Already Exists." RESET << endl;
        cout << "Enter Book Title: ";
        getline(cin, book[size].title);
    }

    cout << "Enter Book Author: ";
    getline(cin, book[size].author);
    while (book[size].author.empty()) {
        cout << RED "Author cannot be empty." RESET << endl;
        cout <<  "Enter Book Author: ";
        getline(cin, book[size].author);
    }

    cout << "Enter Book Category: ";
    getline(cin, book[size].category);
    while (book[size].category.empty()) {
        cout << RED "Category cannot be empty." RESET << endl;
        cout << "Enter Book Category: ";
        getline(cin, book[size].category);
    } 

    cout << "Enter Book ISBN: ";
    getline(cin, book[size].isbn);
    while (book[size].isbn.empty()) {
        cout << RED "ISBN cannot be empty." RESET << endl;
        cout << "Enter Book ISBN: ";
        getline(cin, book[size].isbn);
    }

    cout << "Do you want to add an edition for this book? (Press Y): ";
    cin >> userEditionChoice;
    if (userEditionChoice == 'y' || userEditionChoice == 'Y') {
        cout << "Enter Book Edition: ";
        cin.ignore();
        getline(cin, book[size].edition);
        while (book[size].edition.empty()) {
            cout << RED "Edition cannot be empty." RESET << endl;
            cout << "Enter Book Edition: ";
            getline(cin, book[size].edition);
        }
    } else {
      book[size].edition = "N/A";
    }

    cout << "Enter Book Year: ";
    while(true) {
        if (cin >> book[size].year && book[size].year.length() == 4) {
            break;
        } else {
            cout << RED "Invalid Input. \n" RESET;
            cout << "Enter book year (4 Digits): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cin.ignore(); //ignore clears \n after cin

    cout << "Enter Book Subject: ";
    getline(cin, book[size].subject);
    while (book[size].subject.empty()) {
        cout << RED "Subject cannot be empty." RESET << endl;
        cout << "Enter Book Subject: ";
        getline(cin, book[size].subject);
    }

    cout << "Enter Library: ";
    getline(cin, book[size].details.library);
    while (book[size].details.library.empty()) {
        cout << RED "Library cannot be empty." RESET << endl;
        cout << "Enter Library: ";
        getline(cin, book[size].details.library);
    }

    cout << "Enter Call Number: ";
    getline(cin, book[size].details.callNumber);
    while (book[size].details.callNumber.empty()) {
        cout << RED "Call number cannot be empty." RESET << endl;
       cout << "Enter Library: ";
        getline(cin, book[size].details.callNumber);
    }

    //fstream allows reading and writing to files in books.txt in APPEND mode, which adds at the end of the file
    std::fstream file("books.txt", ios::app);
    file << book[size].title << endl; //same as cout but to file instead of terminal
    file << book[size].author << endl;
    file << book[size].category << endl;
    file << book[size].isbn << endl;
    file << book[size].edition << endl;
    file << book[size].year << endl;
    file << book[size].subject << endl;
    file << book[size].details.library << endl;
    file << book[size].details.callNumber << endl;
    file << book[size].searches << endl;

    book[size].searches = 0;

    cout << LIGHT_YELLOW "\n----------------------------" RESET << endl;
    cout << "Press" LIGHT_YELLOW " Enter " RESET "to Continue....\n";
    cin.ignore();
    clear();
  }

/* we have a loop in display and search that UPPERCASES input and data to check matches, this allows us to ignore case sensitivity of the program, paki review the loop structure nalang since its important */

  void displayByCategory(Books *book, int size) {

    string category; 
    string UpperCat;
    int count = 0;
    cin.ignore();
    cout << "Enter book category: ";
    getline(cin, category);

    while (category.empty()) {
        cout << RED "Invalid Search." RESET << endl;
       cout << "Enter book category: ";
        getline(cin, category);
    }

    UpperCat = category;

    for(int i = 0; i < UpperCat.length(); i++){
        UpperCat[i] = toupper(UpperCat[i]);
    }

    cout << "\nBooks in " << LIGHT_YELLOW << category << RESET " category:" << endl;
string upperbookcat;
    for (int i = 0; i < size; i++) {
        upperbookcat = book[i].category;
        for(int j = 0; j < upperbookcat.length(); j++){
        upperbookcat[j] = toupper(upperbookcat[j]);
        }

      if (upperbookcat == UpperCat) {
        cout << book[i].title << endl;
        count++;
      }
    }
      //checks and prints all cases where the categories match

      //if no book found in that category
      if(count == 0){
        cout <<"\nNo Book Found in " << LIGHT_YELLOW << category << RESET << " Category.\n" << endl;
      }

    cout << "Press" LIGHT_YELLOW " Enter " RESET "to Continue....\n";
    cout << BLUE "\n----------------------------" RESET << endl;
    cin.ignore();
    clear();

  }

  void searchBook(Books *book, int size) {
    string title;

    cin.ignore();
    cout << "Enter book title: ";
    getline(cin, title);
    string toUpperTitle = title;
    for(int i = 0 ; i < toUpperTitle.length() ; i++){
      toUpperTitle[i] = toupper(toUpperTitle[i]);
    }
    cout << CYAN "-------------------------------------\n" RESET << endl;

    //ifstream READS from file
    std::ifstream file("books.txt");
    if (file.is_open()) {
      string line;

      int i = 0;


      while (getline(file, line)) {
           string toUpperLine = line;
          for(int j = 0 ; j < line.length() ; j++){

            toUpperLine[j] = toupper(toUpperLine[j]);
          }

        if (i % 10 == 0) { // title is always divisible by 10 (from 0), check file
          if (toUpperLine == toUpperTitle) {  
            cout << "Title: \t\t" << setw(20) << right << line << endl; //setw is from the iomanip library and allows us to space our strings
            getline(file, line);
            cout << "Author: \t" << setw(20) << right << line << endl; //we also shifted our string to the right
            getline(file, line);
            cout << "Category: \t" << setw(20) << right << line << endl;
            getline(file, line);
            cout << "ISBN: \t\t" << setw(20) << right << line << endl;
            getline(file, line);
            cout << "Edition:  \t" << setw(20) << right << line << endl;
            getline(file, line);
            cout << "Year: \t\t" << setw(20) << right << line << endl;
            getline(file, line);
            cout << "Subject: \t" << setw(20) << right << line << endl;
            getline(file, line);
            cout << "Library: \t" << setw(20) << right << line << endl;
            getline(file, line);
            cout << "Call No.: \t" << setw(20) << right << line << endl;
            int pos = file.tellg(); //saves position of where the searches is and closes it
            file.close();
            book[i/10].searches++;//updates the searches of the said book

            //ofstream writes to file
            std::ofstream update("books.txt", ios::in);
            update.seekp(pos); //loads position of where the searches was
            update << book[i/10].searches;
            update.close();

            cout << "\nAmount of times the book is Searched: " << book[i/10].searches<< endl;
            cout << CYAN "\n-------------------------------------" RESET << endl;
            cout << "Press" LIGHT_YELLOW " Enter " RESET "to Continue....\n";
            cin.ignore();
            clear();

            return;
          }
        }
        i++;

    }


      file.close();
    } else {
      cout << "Unable to open file";
    }
    cout << "Book not found." << endl;

    cout << "Press" LIGHT_YELLOW " Enter " RESET "to Continue....\n";
    cout << CYAN "\n----------------------------" RESET << endl;
    cin.ignore();
    clear();

  }

  void mostPopularTitles(Books *book, int size) {

   Books mostPopular[11];  //This struct serves as a container to put books with many searches
   string temptitle;
    int tempsearches;

    //initializing values in the struct with zero to avoid garbage values
    for(int i = 0; i < 11; i++){
      mostPopular[i].searches = 0;
    }

    //this for loop will go through each book that was stored in the 'Books' struct
    for(int i = 0; i < size; i++){

        //this inner for loop will now compare the searches of the current subjected book to the existing values of the mostPopular struct. note: the values of the 'mostPopular' struct is set to zero so the zero will be replaced with the values of the current book at first
        for(int j = 9; j >= 0; j--){

          //if the current subjected book's search is greater than the existing value in the struct, it will replace it
          if(book[i].searches >= mostPopular[j].searches){
            temptitle = mostPopular[j].title;
            mostPopular[j].title = book[i].title;
            mostPopular[j+1].title = temptitle;

            tempsearches = mostPopular[j].searches;
            mostPopular[j].searches = book[i].searches;
            mostPopular[j + 1].searches = tempsearches;
          } else { break; }

      }

    }

    for(int i = 0; i != 10 && i < size; i++){
        cout << i + 1 << ". " << mostPopular[i].title << "\nSearches: "<< mostPopular[i].searches << "\n" << endl;
    }

    cout << "Press" LIGHT_YELLOW " Enter " RESET "to Continue....\n";
    cout << LIGHT_MAGENTA "\n----------------------------" RESET << endl;
    cin.get();
    cin.get();
    clear();

  }
