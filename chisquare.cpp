//
//  main.cpp
//  Chisquare
//
//  Created by Guillem Pérez on 9/20/18.
//  Copyright © 2018 Guillem Pérez. All rights reserved.
//

#include "pch.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "afxwin.h"
#include <afxdb.h>
#include <cmath>
#include <string>
using namespace std;



//Function Declarations
void  computerowtotals(int number[9][12], int arrayrowtotal[9], int Nrow);
void computecolumntotals(int number[9][12], int arraycoltotal[12], int &Ncol);
void computenij(int number[9][12], int arraycoltotal[12], int arrayrowtotal[9], int Ncol, double finalchisquare[12]);
void printtable(int number[9][12], CStringList& typeaccidentlist, string accidents[9]);
void computechisq(double finalchisquare[12], double &chisquared);
void computedegreesfreedom(int degfreedom);
void computecrammersv(double crammersv, double chisquared, int Ncol);
void computecontingency(double contingency, double chisquared, int Ncol);

int main() {
    
    //Database Connection
    CString Connection;
    CDatabase database;
    CString username;
    CString password;
    username = "Server\\SQLEXPRESS(Copy and Paste from SQLServer)\\username";
    Connection = _T("Driver={SQL Server Native Client 11.0};Server=Server\\SQLEXPRESS; AttachDbFilename=(Root to File with double backslashes EX: )C:\\ChidataSQL.mdf;Trusted_Connection=Yes;Uid=") + username + _T(";Pwd=") + password + _T(";");
    database.OpenEx(Connection, CDatabase::useCursorLib);
    
    //Query (Retrieve Data)
    CString query = "SELECT TypeofAccident, January, February, March,April,May, June, July, August, September, October, November, December  FROM AccidentTable";
    CRecordset recordset(&database);
    CString temp, record;
    recordset.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
    
    //Declaration of Variables
    CString typeofaccident, sjanuary, sfebruary, smarch, sapril, smay, sjune, sjuly, saugust, sseptember, soctober, snovember, sdecember;
    const int size = 9;
    int january, february, march, april, may, june, july, august, september, october, november, december;
    CStringList typeaccidentlist;
    
    int number[9][12];
    string accidents[9];
    CString typeaccident[9];
    int arraycoltotal[12];
    int arrayrowtotal[9];
    double chisqr = 0;
    
    int totalallcol = 0;
    int totalallrow = 0;
    
    int row = 0;
    int col = 0;
    
    int Nrow = 0;
    int Ncol = 0;
    
    double finalchisquare[12];
    int nij = 0;
    
    double chisquared = 0;
    int degfreedom = 0;
    double crammersv = 0;
    double contingency = 0;
    
    //Data Append to the Multi Dimensional Array number[][]
    while (!recordset.IsEOF())
    {
        record = _T("");
        register int len = recordset.GetODBCFieldCount();
        
        recordset.GetFieldValue("TypeofAccident", typeofaccident);
        recordset.GetFieldValue("January", sjanuary);
        recordset.GetFieldValue("February", sfebruary);
        recordset.GetFieldValue("March", smarch);
        recordset.GetFieldValue("April", sapril);
        recordset.GetFieldValue("May", smay);
        recordset.GetFieldValue("June", sjune);
        recordset.GetFieldValue("July", sjuly);
        recordset.GetFieldValue("August", saugust);
        recordset.GetFieldValue("September", sseptember);
        recordset.GetFieldValue("October", soctober);
        recordset.GetFieldValue("November", snovember);
        recordset.GetFieldValue("December", sdecember);
        
        col = 0;
        typeaccidentlist.AddHead(typeofaccident);
        
        //Type Parsing (string to int) + Appending into Multi Dimensional Array.
        january = atoi(sjanuary);
        number[row][col] = january;
        
        february = atoi(sfebruary);
        number[row][++col] = february;
        
        march = atoi(smarch);
        number[row][++col] = march;
        
        april = atoi(sapril);
        number[row][++col] = april;
        
        may = atoi(smay);
        number[row][++col] = may;
        
        june = atoi(sjune);
        number[row][++col] = june;
        
        july = atoi(sjuly);
        number[row][++col] = july;
        
        august = atoi(saugust);
        number[row][++col] = august;
        
        september = atoi(sseptember);
        number[row][++col] = september;
        
        october = atoi(soctober);
        number[row][++col] = october;
        
        november = atoi(snovember);
        number[row][++col] = november;
        
        december = atoi(sdecember);
        number[row][++col] = december;
        
        row = row + 1;
        
        
        recordset.MoveNext();
        
    }
    
    //Function calls
    computerowtotals(number, arrayrowtotal, Nrow);
    computecolumntotals(number, arraycoltotal, Ncol);
    printtable(number, typeaccidentlist, accidents);
    computenij(number, arraycoltotal, arrayrowtotal, Ncol, finalchisquare);
    computechisq(finalchisquare, chisquared);
    computedegreesfreedom(degfreedom);
    computecrammersv(crammersv, chisquared, Ncol);
    computecontingency(contingency, chisquared, Ncol);
    
    
    
    
    return 0;
    
}

void printtable(int number[9][12], CStringList& typeaccidentlist, string accidents[9]) {
    
    //Get Position pointer from TypeAccidentList
    POSITION pos = typeaccidentlist.GetHeadPosition();
    int x = 8;
    while (pos != NULL) {
        //Value Casting and append to accidents array
        CString value = typeaccidentlist.GetNext(pos);
        accidents[x] = value;
        x--;
        
    }
    
    //Print Rows
    for (int i = 0; i < 9; i++) {
        
        cout << accidents[i] << setw(12);
        
        //Print Columns (For each row will print Row i Col j: Row1Col1, Row1Col2...Row1Col12)
        for (int j = 0; j < 12; j++) {
            
            cout << number[i][j] << setw(12);
        }
        
        cout << endl;
        
    }
    
}

//Computerowtotals function, Computes data from every row, arrayrowtotal[] contains the sum for each row at index i.
void  computerowtotals(int number[9][12], int arrayrowtotal[9], int Nrow) {
    
    int totalcol = 0;
    
    
    for (int i = 0; i < 9; i++) {
        
        totalcol = 0;
        
        
        
        for (int j = 0; j < 12; j++) {
            
            totalcol = totalcol + number[i][j];
            
        }
        
        
        arrayrowtotal[i] = totalcol;
        
        //N = total of each rows value. (Iterates 9 times as there are 9 rows.)
        Nrow = Nrow + totalcol;
        
        
    }
    
}

void computecolumntotals(int number[9][12], int arraycoltotal[12], int& Ncol) {
    
    int totalrow = 0;
    
    //Loop through columns
    for (int j = 0; j < 12; j++) {
        
        totalrow = 0;
        
        //Loop through Rows
        for (int i = 0; i < 9; i++) {
            
            totalrow = totalrow + number[i][j];
            
        }
        
        arraycoltotal[j] = totalrow;
        Ncol = Ncol + totalrow;
        
        
        
        
    }
    
    
}

void computenij(int number[9][12], int arraycoltotal[12], int arrayrowtotal[9], int Ncol, double finalchisquare[12]) {
    
    double Nij = 0;
    double nij = 0;
    
    double chisq = 0;
    
    //Iteration between 12 Columns
    for (int i = 0; i < 12; i++) {
        
        chisq = 0;
        
        //Iteration 9 Rows
        for (int j = 0; j < 9; j++) {
            Nij = number[j][i];
            nij = (arrayrowtotal[j] * arraycoltotal[i]) / Ncol;
            
            //Calculating individual Chi Squares for each Column-row
            chisq = chisq + (pow((Nij - nij), 2) / nij);
            
        }
        
        //Appending value onto array for summatory later
        finalchisquare[i] = chisq;
        
        
    }
}

void computechisq(double finalchisquare[12], double &chisquared) {
    
    //Summatory for Chi Squared
    for (int i = 0; i < 12; i++) {
        chisquared = chisquared + finalchisquare[i];
    }
    
    cout << endl << "Chi Squared: " << chisquared << endl;
}

void computedegreesfreedom(int degfreedom) {
    
    //Degrees of freedom = Ni*Nj-Ni-Nj+1
    degfreedom = 9 * 12 - 9 - 12 + 1;
    
    cout << "Degrees of Freedom: " << degfreedom << endl;
}
void computecrammersv(double crammersv, double chisquared, int Ncol) {
    
    //Crammers V = Square root (Chi Squared / (Column//Row, as it is the same, total summatory * Ni - 1))
    crammersv = sqrt(chisquared / (Ncol * 8));
    
    cout << "Crammer's V: " << crammersv << endl;
}

void computecontingency(double contingency, double chisquared, int Ncol) {
    
    //Contingency coeficient = Square root (Chi Squared / (ChiSq + Summatory of either col or row (The Same)))
    contingency = sqrt(chisquared / (chisquared + Ncol));
    
    cout << "Contingency Coeficient: " << contingency << endl;
}
