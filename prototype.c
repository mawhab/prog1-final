#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct // struct for DOB
{
    int day;
    int month;
    int year;
} Date;
typedef struct // dtruct for contact
{
    char Firstname[20];
    char Lastname[20];
    Date DateofBirth;
    char adress[100];
    char phonenumber[20];
    char email[30];
} Contact;

Contact c[1000]; // main array to be worked on
int count=0; // number of contacts currently in array
// functions that modify/read from array
void Loadfromfile();
void save();
void printentry(Contact s);
int search();
int whosolder(Date dob1, Date dob2);
void swap(Contact *ent1, Contact *ent2);
void sortprint();
void sort(int low, int high, int choice);
int part(int low, int high, int choice);
int leapYear( int year);
int validDate(Date DOB);
int validPhone(char x[]);
int validEmail(char x[]);
void ADD(int index);
void delete(int index);
void deleteContact();
void MODIFY();

int main(){
    int running = 1; // variable to terminate program
    int option;
    int t;
    while(running){ // main loop
        printf("\t\t\t\t(Welcome to phonebook)\n");
        printf("\t\t\t\t----------------------\n");
        printf("1)Load \t\t\"Loads contacts from a file\"\n2)Search \t\"Searches for a contact with given last name in the directory\"\n");
        printf("3)Add\t\t\"Adds a contact to the directory\"\n4)Delete\t\"Deletes a contact with given last name from directory\"\n");
        printf("5)Modify\t\"Modifies a contact currently in the directory\"\n6)Print \t\"Prints all contacts in directory sorted\"\n");
        printf("7)Save \t\t\"Saves current directory into a file\"\n8)Quit \t\t\"Exits the program\"");
        printf("\nPlease select a Command By entering it's number in menu  ");
        scanf("%d", &option);
        switch(option){ // switch case to parse command
            case 1:
                Loadfromfile();
                break;
            case 2:
                search();
                break;
            case 3:
                ADD(count);
                break;
            case 4:
                deleteContact();
                break;
            case 5:
                MODIFY();
                break;
            case 6:
                sortprint();
                break;
            case 7:
                save();
                break;
            case 8:
                printf("\x1b[31mWarning! all unsaved data wil be discarded\x1b[0m \n"); // warning in case of exit
                printf("Are you sure you want to get out of the program ?\n");
                printf("yes,quit press 1\nNo, return to menu press any other character\nEnter choice:\n");
                scanf("%d",&t);
                if(t==1)
                    running = 0;
                break;
            default:
                printf("\x1b[31mInvalid choice please try again.\x1b[0m\n");
                break;
            }
    }
    return 0;
}





void Loadfromfile(){ // function to load contacts from file
    system("cls");
    FILE*f;
    char file[50]; // variable to hold file name
    printf("\x1b[31m(WARNING!!.txt FILES ONLY)\x1b[0m\n");
    printf("Enter the Name of the file: ");
    scanf("%s", file); // reading file name
    strcat(file,".txt"); // adding .txt extension
    f=fopen(file,"r"); // opening file
    if(f!=NULL) // checking in case of error
    {   
        while(!feof(f)) // reading contacts untill end of file
        {
            fscanf(f,"\n%[^,],",c[count].Lastname);
            fscanf(f,"%[^,],",c[count].Firstname);
            fscanf(f,"%d-%d-%d,",&c[count].DateofBirth.day,&c[count].DateofBirth.month,&c[count].DateofBirth.year);
            fscanf(f,"%[^,],",c[count].adress);
            fscanf(f,"%[^,],",c[count].phonenumber);
            fscanf(f,"%s",c[count].email);
            count++; // increasing count by every element
        }
        fclose(f); // closing file
        printf("\x1b[32mFile is loaded in the array\x1b[0m\n");
    }
    else
    {
        printf("\x1b[31mError in loading (File not found) \x1b[0m"); // error in case of not openning file
    }
}


void save(){ // function to save current array to file
    int i;
    FILE*f;
    char fileS[50]; // variable to hold file name
    printf("Enter the Name of the file To Save in \n: "); 
    scanf("%s", fileS); // reading file name
    strcat(fileS,".txt"); // adding .txt extension
    f=fopen(fileS,"w"); // openning file
    if(f!=NULL) // checking if there was error in openning file
    {
        for(i=0; i<count; i++) // printing contacts to file
        {
            fprintf(f,"%s,",c[i].Lastname);
            fprintf(f,"%s,",c[i].Firstname);
            fprintf(f,"%d-%d-%d,",c[i].DateofBirth.day,c[i].DateofBirth.month,c[i].DateofBirth.year);
            fprintf(f,"%s,",c[i].adress);
            fprintf(f,"%s,",c[i].phonenumber);
            fprintf(f,"%s\n",c[i].email);

        }

        printf("\x1b[32mDirectory is saved in the external file\x1b[0m\n"); // prompt to declare success
        fclose(f); // closing file
    }
    else
        printf("\x1b[31mError in opening file\x1b[0m"); // prompt in case of error

}



void printentry(Contact s){ // function to print contcat details
    printf("%s,%s,%s,%s,%s,%d-%d-%d\n",s.Lastname,s.Firstname,s.adress,s.phonenumber,s.email,s.DateofBirth.day,s.DateofBirth.month,s.DateofBirth.year);
}

int search(){ // function that searches for and prints a contact
    char last[20]; // variable to hold last name to search for
    int found = 0; // variable to check if any have been found
    printf("please enter last name to search for\n");
    scanf("%s", last); // scanning last name to search for
    for(int i=0;i<count;i++){
        if(strcasecmp(c[i].Lastname, last)==0){ // checking if last name matches last name of current element
            printf("%d\t", i+1);
            printentry(c[i]); // print it if it matches
            found = 1; // element(s) have been found
        }
    }
    if(!found) // prompt if no elements have been found
        printf("\x1b[31mNo contacts with last name (%s) have been found!\x1b[0m\n", last);

    return found;
}



/*------------------------------------------START OF PRINT FUNCTION---------------------------------------*/
int whosolder(Date dob1, Date dob2){ // function to check whos older between 2 dates of birth for sorting
    if(dob1.year>dob2.year) // checking from year
        return 1;
    else if(dob1.year<dob2.year)
        return 2;
    else{
        if(dob1.month>dob2.month) // if years are same check months
            return 1;
        else if(dob1.month<dob2.month)
            return 2;
        else{
            if(dob1.day>dob2.day) // if months are same check day
                return 1;
            else if(dob1.day<dob2.day)
                return 2;
        }
    }
    return 0; // if all are same return 0 for both are equal
}


void swap(Contact *ent1, Contact *ent2){// function to swap two contacts
    Contact temp;
    // copying every entry from contact 1 to temp
    strcpy(temp.Firstname, ent1->Firstname);
    strcpy(temp.Lastname, ent1->Lastname);
    strcpy(temp.email, ent1->email);
    strcpy(temp.adress, ent1->adress);
    strcpy(temp.phonenumber, ent1->phonenumber);
    temp.DateofBirth.day = ent1->DateofBirth.day;
    temp.DateofBirth.month = ent1->DateofBirth.month;
    temp.DateofBirth.year = ent1->DateofBirth.year;

    // copying every entry from contact 2 to conatct 1 
    strcpy(ent1->Firstname, ent2->Firstname);
    strcpy(ent1->Lastname, ent2->Lastname);
    strcpy(ent1->email, ent2->email);
    strcpy(ent1->adress, ent2->adress);
    strcpy(ent1->phonenumber, ent2->phonenumber);
    ent1->DateofBirth.day = ent2->DateofBirth.day;
    ent1->DateofBirth.month = ent2->DateofBirth.month;
    ent1->DateofBirth.year = ent2->DateofBirth.year;


    // copying every entry from temp to contact 2
    strcpy(ent2->Firstname, temp.Firstname);
    strcpy(ent2->Lastname, temp.Lastname);
    strcpy(ent2->email, temp.email);
    strcpy(ent2->adress, temp.adress);
    strcpy(ent2->phonenumber, temp.phonenumber);
    ent2->DateofBirth.day = temp.DateofBirth.day;
    ent2->DateofBirth.month = temp.DateofBirth.month;
    ent2->DateofBirth.year = temp.DateofBirth.year;
}

int part(int low, int high, int choice){ // partitioning function that takes a pivot and checks against it
    Contact pivot = c[high]; // pivot used here is high index
    int j=low-1; // index to be moved if condition is true
    for(int i=low;i<high;i++){
        if(choice==1){ // choice being 1 means to sort by DOB
            if(whosolder(pivot.DateofBirth, c[i].DateofBirth)==2){ // if pivot is larger then swap element i with j+1
                swap(&c[i], &c[++j]);
            }
        }else{ // choice being 2 means to sort by name but no need to check if its 2 as it was checked previously
            if(strcasecmp(pivot.Lastname, c[i].Lastname)>0){// same as before
                swap(&c[i], &c[++j]);
            }
        }
    }
    swap(&c[++j], &c[high]); // swapping to get pivot in correct position
    return j; // returninng index
}
void sort(int low, int high, int choice){ // sort function that uses quicksort algorithm by partitioning the array
    if(low<high){ // base case check
        int p = part(low, high, choice); // calling partition function
        sort(low, p-1, choice); // recursivly sorting
        sort(p+1, high, choice);
    }
}

void sortprint(){ // function that sorts array then prints sorted array
    printf("Please enter 1 to sort by DOB and 2 for name:\n");
    int choice;
    scanf("%d", &choice);
    if(choice==1 || choice==2) // checking if input is valid
        sort(0, count-1, choice); // sorting the array
    else{
        printf("\x1b[31m Invalid input, please try again.\x1b[0m\n");
        return;
    }
    for(int i=0;i<count;i++){ // printing sorted array
        printentry(c[i]);
    }
}
/*------------------------------------------END OF PRINT FUNCTION---------------------------------------*/


/*------------------------------------------START OF ADD FUNCTION---------------------------------------*/

int leapYear( int year) // checking if its a leap year
{
    if (year%4==0)
    {
        if(year%100==0)
        {
            if(year%400==0)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;

}
int validDate(Date DOB) // function to check if date entered is valid
{
    if(DOB.year>2021 || DOB.year<1900){ // year has to be within limits
        return 0;
    }
    if(DOB.month>12 || DOB.month<1){ // month has to be within limits
        return 0;
    }
    if(DOB.day>31 || DOB.day<1) // day has to be within limits
    {
        return 0;
    }
    if(DOB.month==4 || DOB.month==6 || DOB.month==9 || DOB.month==11)
    {// checking special cases
        if(DOB.day>30 || DOB.day<1)
            return 0;
    }
    if(DOB.month==2) // checking special case
    {
        if (leapYear(DOB.year)==1)
        {
            if(DOB.day>29 || DOB.day<1)
                return 0;
        }
        else
        {
            if(DOB.day>28 || DOB.day<1)
                return 0;
        }
    }

    return 1;
}


int validPhone(char x[])
{
    if(strlen(x)>15 || strlen(x)<7) // if phone number is longer than 15 chars or less than7 chars its invalid
        return 0;
    int i;
    for(i=0;i<strlen(x);i++){
        if(!isdigit(x[i]) && !isspace(x[i])) // if char is not digit or space then its invalid
          return 0;
    }
    return 1; //  if it has passed for loop it is valid

}
int validEmail(char x[]) // checking if email is valid
{
    int i,atFound=0,dotFound=0, at=0, dot=0; // variables to be used in function
    for(i=0;i<strlen(x);i++){ // going over email
        if(x[i]=='@'){ // recording index of @
            atFound=1;
            at=i;
        }
        if(x[i]=='.'){ // recording index of .
           dotFound=1;
           dot=i;
        }
        if(isspace(x[i])) // invalid if there is space
            return 0;

    }
    if(dot<at) // if . came before @ then email is invalid
        return 0;
    if(atFound==0) // if @ wasnt found then email is invalid
        return 0;
    if(dotFound==0) // if . wasnt found then email is invalid
        return 0;

    return 1; // if all cases passed email is valid

}
void ADD(int index)
{   
    //if(count>0)
    //    strcat(c[count-1].email, "\n"); 
        // adding new line character to previous entry so that format is kept consistent
    int i;
    do
    {
        printf("Enter first name: \n");
        scanf("%s",c[index].Firstname);
        for(i=0; i<strlen(c[index].Firstname); i++) // loop to check if name is valid
        {
            if (!isalpha(c[index].Firstname[i])) // if char isnt alpha then name is invalid
            {
                printf("\x1b[31mInvalid entry!\x1b[0m\n");
                break;
            }
        }
        i--;
    }
    while(!isalpha(c[index].Firstname[i])); // if name is invalid reask user for name

    do
    {
        printf("Enter last name: \n");
        scanf("%s",c[index].Lastname);
        for(i=0; i<strlen(c[index].Lastname); i++)
        {
            if (!isalpha(c[index].Lastname[i]))
            {
                printf("\x1b[31mInvalid entry!\x1b[0m\n");
                break;
            }
        }
        i--;
    }
    while(!isalpha(c[index].Lastname[i])); // same as first name

    do
    {
        printf("Enter date of birth(dd-mm-yyyy): \n"); // scanning date
        scanf("%d-%d-%d",&c[index].DateofBirth.day,&c[index].DateofBirth.month,&c[index].DateofBirth.year);
        if (!validDate(c[index].DateofBirth)){ // checking if date entered is valid
            printf("\x1b[31mInvalid date!\x1b[0m\n");
        }
    } while(!validDate(c[index].DateofBirth)); // retake input until input is valid


    printf("Enter address: \n");
    scanf(" "); // prompting input
    gets(c[index].adress); // getting address
    do
    {
        printf("Enter phone number: \n");
        scanf(" "); // prompting input
        gets(c[index].phonenumber); // scanning phone number
        if(!validPhone(c[index].phonenumber)){ // checking if phonenum is valid
            printf("\x1b[31mInvalid phone number!\x1b[0m\n");
        }
    }while(!validPhone(c[index].phonenumber)); // if phone number isnt valid input again

    
    do
    {
        printf("Enter email: \n");
        scanf("%s",c[index].email);
        if(validEmail(c[index].email)==0){ // checking if email is valid
            printf("\x1b[31mInvalid email address!\x1b[0m\n");
        }
        
    }while(validEmail(c[index].email)==0); // if email isnt valid input again

    printf("\x1b[32mContact has been added successfully!\x1b[0m\n");
    count++; // contact has been added

}

/*------------------------------------------END OF ADD FUNCTION---------------------------------------*/


/*------------------------------------------START OF Delete FUNCTION---------------------------------------*/

void delete(int index){ // function to delete contact at given index in the array
    for(int i=index;i<count-1;i++){
        swap(&c[i], &c[i+1]); // move 1 element to the left starting from deleted element
        // uses swap function to swap elements to put the deleted element at the end of array
    }
    count--; // reduce number of elements in array by 1
}

void deleteContact(){
    char first[20]; // variable to  hold first element
    char last[20]; // variable to hold last name
    int found = 0; // to keep track of how many elements have been deleted
    printf("Please enter first name of the contact: \n");
    scanf("%s", first); // scanning first name
    printf("Please enter last name of the contact: \n");
    scanf("%s", last); // scanning last name
    for(int i=0;i<count;i++){
        if(strcasecmp(first, c[i].Firstname)==0 && strcasecmp(last, c[i].Lastname)==0){ 
            // if first and last name of current element match the input, delete this element
            delete(i); // calling the delete function
            i--; // rechecking current element after deletion
            found++; // increasing number of deleted elements
        }
    }
    if(!found) // prompt if no elements were found
        printf("\x1b[31mNo contacts with first name (%s) and last name (%s) have been found!\x1b[0m\n", first, last);
    else // printing number of fdeleted elements
        printf("\x1b[32m%d contacts have been deleted!\x1b[0m\n", found);
}


/*------------------------------------------END OF Delete FUNCTION---------------------------------------*/

void MODIFY()
{
    int j,found;

    found = search(); // searching for and printing the contacts with matching last name
    // function returns number of elements found
    if(found>=1) // prompt user
        printf("Please choose the number of the contact you want to modify:\n");
    else
        return; // if no matches were found return
    scanf("%d",&j); // reading index of contact to modify
    ADD(--j); // calling add function to modify given index
    count--; // add function increases count but no contacts have been added so remove that count
}