//yash halgaonkar
//cs a 
//65
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef struct tooldata
{
   int toolNum;
   char toolName[20];
   int quantity;
   double cost;
} toolData;

static char FILENAME[] = "tools.dat";

void textFile(fstream &readFromFile);
void updateRecord(fstream &updateFile);
void newRecord(fstream &insertInFile);
void deleteRecord(fstream &deleteFromFile);
void outputLine(ostream &output, toolData tool);


void CreateDataFile()
{

   fstream testFileExists;
   testFileExists.open(FILENAME,ios::in);

   // if file is not existing, create it by opening it in OUT mode
   if(!testFileExists)
   {
	   testFileExists.close();
	   testFileExists.open(FILENAME,ios::out | ios::binary);

	   //Make 100 default entries in the tools.dat file

	      toolData data;

	      strcpy(data.toolName, "");
	      data.quantity = 0;
	      data.cost = 0.0;
	      data.toolNum = 0;

	      for(int i = 0; i < 100; i++)
	       testFileExists.write((char*)&data, sizeof(data));

	      testFileExists.close();

	      cout<<"Created....";
   }

   // If the file exist....no need to create it
   else
   {
	   testFileExists.close();
	   cout<<"existing...";
   }


}


int main()
{
   CreateDataFile();

	fstream inOuttools(FILENAME, ios::in | ios::out);

   int choice;

  while (1)
  {
    cout<< "\n1. Display tools.dat on console as well as copy its contents to a file print.txt" << endl
      << "2. Update inventory" << endl
      << "3. add a new item" << endl
      << "4. delete an item" << endl
      << "5. end program" << endl ;

   cout << "Enter your choice" << endl;
   cin >> choice;

      switch (choice)
	  {
         case 1: textFile(inOuttools); break;
         case 2: updateRecord(inOuttools); break;
         case 3: newRecord(inOuttools); break;
         case 4: deleteRecord(inOuttools); break;
         case 5: exit(0);
         default: cerr << "Incorrect choice" << endl; break;
      }

      inOuttools.clear(); // resets end-of-file flag
   }

   return 0;
}



void textFile(fstream &readFromFile)
{
   ofstream outPrintFile("print.txt");

   if (!outPrintFile)
   {
      cerr << "File could not be opened." << endl;
      exit(1);
   }


      cout << setiosflags(ios::left) << setw(6) << "Tool No."
      << setw(20) << "Tool Name" << setw(11) << "Quantity"
      << setiosflags(ios::right) << setw(10) << "Cost" << endl;

   outPrintFile << setiosflags(ios::left) << setw(6) << "Tool"
      << setw(20) << "Tool Name" << setw(11) << "Quantity"
      << setiosflags(ios::right) << setw(10) << "Cost" << endl;

 readFromFile.seekg(0,ios::beg);

   toolData tool;

   while (!readFromFile.eof())
  {
	   readFromFile.read((char *)&tool, sizeof(tool));

      if (tool.toolNum != 0) 
      {
         outputLine(outPrintFile, tool);
         outputLine(cout, tool);
      }


   }

   outPrintFile.close();
}



void updateRecord(fstream &updateFile)
{
   int account;
   toolData tool;

      cout << "Enter tool number to update (1 - 100): ";
      cin >> account;

   updateFile.seekg((account - 1) * sizeof(tool));

   updateFile.read((char *)&tool, sizeof(tool));

   //If record is not having default values, only then it can be updated
   if (tool.toolNum != 0)
   {
     cout<<"\nOld record is: "<<endl;
      outputLine(cout, tool);

      cout << endl << "Enter new cost and new quantity";
      cin >> tool.cost>>tool.quantity;

      cout<<"\nNow, New record is: "<<endl;
      outputLine(cout, tool);

      //Write the updated record in the file, at the same place
      updateFile.seekp((account - 1) * sizeof(toolData));
      updateFile.write((char *)&tool, sizeof(toolData));
   }
   else
      cerr << "Tool No." << account << " has no information." << endl;
}

void newRecord(fstream &insertInFile)
{
   char text[80];
   toolData tool;
   int account;

   cout << "Enter new tool number (1 - 100): ";
   cin >> account;

   insertInFile.seekg((account - 1) * sizeof(tool));

   // read the current status of the account-1 record
   insertInFile.read((char *)&tool, sizeof(toolData));

   //check whether the current toolNum for account-1 record is default(i.e. 0)or not
   if (tool.toolNum == 0)
   {
	   tool.toolNum = account;

      cout << "Enter the tool name: ";
      cin>>text;

      // if tool name is greater than 19 characters,
      // then copy only first 19 characters to tool.toolName
      if ( strlen(text) > 19)
      {
         strncpy (tool.toolName, text, 19);
         tool.toolName[20] = '\0';
         cout << "Name is too long. It has been shortened to "
            << tool.toolName << endl;
      }

      else
      {
         strcpy(tool.toolName, text);
         tool.toolName[20] = '\0';
      }

      cout << "Enter the quantity: ";
      cin >> tool.quantity;

      cout << "Enter the cost: ";
      cin >> tool.cost;

      //add the record details directly at the required place
      insertInFile.seekp((account - 1) * sizeof(tool));
      insertInFile.write((char *)&tool, sizeof(tool));
   }

   else
      cerr << "Tool No. " << account
      << " already contains information." << endl;

}

void deleteRecord(fstream &deleteFromFile)
{

   int account;
   toolData tool;

   cout << "Enter tool number to delete (1 - 100): ";
   cin >> account;

   deleteFromFile.seekg((account - 1) * sizeof(toolData));

   deleteFromFile.read((char *)&tool, sizeof(toolData));

   //Header settings
    cout << setiosflags(ios::left) << setw(6) << "Tool"
         << setw(20) << "Last Name" << setw(11) << "First Name"
         << setiosflags(ios::right) << setw(10) << "cost" << endl;

//display the record before deleting
 outputLine(cout, tool);

 //If record is not having default values (i.e. if it exists),
 //only then it can be deleted
   if (tool.toolNum != 0)
{
	   // create a default structure variable
	   // and place it at the same position
      toolData blanktool = {0, "", 0, 0.0};
      deleteFromFile.seekp((account - 1) * sizeof(toolData));
      deleteFromFile.write((char *)&blanktool, sizeof(toolData));

      cout << "Tool No. " << account << " is deleted." << endl;
   }

 else
      cout << "Tool No. " << account << " is empty. Cannot be deleted..." << endl;
}



void outputLine(ostream &output, toolData tool)
{
      output << setiosflags(ios::left) << setw(6) << tool.toolNum
      << setw(20) << tool.toolName << setw(11) << tool.quantity
      << setiosflags(ios::fixed | ios::showpoint | ios::right)
      << setw(10) << setprecision(2) << tool.cost << endl;
   }
