				     //Term Project:Hack Assembler(WithoutSymbols)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<map> //(part of c++ STL)to store the elements in combination of keys
#include<string.h>
#include<fstream>
#define LENGTH 255
#define L 16
using namespace std;
class Translator {
  map < string, string > dmap; //to store dest values
  map < string, string > jmap; //to store jump values
  map < string, string > cmap; //to store comp values
  static string c;
  static string a;
  static char A[];
  public:
    Translator() {
      dmap[""] = "000";
      dmap["M"] = "001";
      dmap["D"] = "010";
      dmap["MD"] = "011";
      dmap["A"] = "100";
      dmap["AM"] = "101";
      dmap["AD"] = "110";
      dmap["AMD"] = "111";

      cmap["0"] = "0101010";
      cmap["1"] = "0111111";
      cmap["-1"] = "0111010";
      cmap["D"] = "0001100";
      cmap["A"] = "0110000";
      cmap["!D"] = "0001101";
      cmap["!A"] = "0110001";
      cmap["-D"] = "0001111";
      cmap["-A"] = "0110011";
      cmap["D+1"] = "0011111";
      cmap["A+1"] = "0110111";
      cmap["D-1"] = "0001110";
      cmap["A-1"] = "0110010";
      cmap["D+A"] = "0000010";
      cmap["D-A"] = "0010011";
      cmap["A-D"] = "0000111";
      cmap["D&A"] = "0000000";
      cmap["D|A"] = "0010101";
      cmap["M"] = "1110000";
      cmap["!M"] = "1110001";
      cmap["-M"] = "1110011";
      cmap["M+1"] = "1110111";
      cmap["M-1"] = "1110010";
      cmap["D+M"] = "1000010";
      cmap["D-M"] = "1010011";
      cmap["M-D"] = "1000111";
      cmap["D&M"] = "1000000";
      cmap["D|M"] = "1010101";

      jmap[""] = "000";
      jmap["JGT"] = "001";
      jmap["JEQ"] = "010";
      jmap["JGE"] = "011";
      jmap["JLT"] = "100";
      jmap["JNE"] = "101";
      jmap["JLE"] = "110";
      jmap["JMP"] = "111";

    }

  bool checkinstr(string instr) { //To check the A instruction if it is integer
    for (int i = 0; instr[i] != '\0'; i++) {
      if (instr[i] > '0' || instr[i] < '9') {
        return true;
      }
      return false;
    }
  }
  string AInstruction(string value) {
    int in = -1;
    string answer;
    if (checkinstr(value)) //if the value is between 0 and 9
      in = atoi(value.c_str()); //converting string to number
    A[0] = 'a';
    int I = L - 1;//setting size 15 excluding 1 for opcode a
    A[I--] = '\0';

    if ( in != -1) {
      while ( in > 0) { //convert decimal to binary
        A[I--] = ( in % 2) + '0'; in /=2;
      }
      while (I >= 0) {
        A[I--] = '0'; //put remaining 0`s 
      }
      answer = a + A; //Opcode +15 binary bits
    }
    return answer;
  }

  string CInstruction(string de, string co, string ju) {
    string dest, comp, jump, answer;
    if (dmap.find(de) != dmap.end()) { //.find() to find the de in dmap till end of dmap
      dest = dmap[de];
    }
    if (cmap.find(co) != cmap.end()) { //.find() to find the de in dmap till end of dmap
      comp = cmap[co];
    }
    if (jmap.find(ju) != jmap.end()) { //.find() to find the de in dmap till end of dmap
      jump = jmap[ju];
    }
    if (!dest.empty() && !comp.empty() && !jump.empty()) { //if these aren`t empty then  store them in string named answer
      answer = c + comp + dest + jump;
    }
    return answer;

  }

};
string Translator::c = "111"; //static c opcode
string Translator::a = "0"; //static a opcode
char Translator::A[L];
//Another Class


class Code {
  ifstream file; //to handle file
  string value;
  string dest;
  string comp;
  string jump;
  bool isA;
  bool isC;
  bool comp1;
  bool comp2;
  int lineno; //to handle lines in the file
  static char data[]; 
  public:
    Code(string filename) { //Contructor
      this -> file.open(filename.c_str()); //open a file using string filename	
      lineno = -1;
    }

    ~Code() { //Destructor	
      this -> file.close(); //closing a file
    }

  //Functions
  void GetI() { //To get next instruction
    string line;
    getline(this -> file, line); //reading  line from file
    this -> isC = this -> CInstr(line); //if there is Cinstrunction on that line store it in isC
    this -> isA = this -> AInstr(line); //if there is Ainstrunction on that line store it in isA
  }

  void Getnext() { //To get next valid A or C instruction 
    bool valid = false; //valid=0
    while (!valid && this -> hasnext()) { //if file isn`t ended and valid is 1 then get next instruction from file 
      this -> GetI();//get next valid instruction
      valid = this -> isA || this -> isC; //store the instruction in valid whether is A or C instruction  
    }
    if (valid) {
      this -> lineno++; //goto next line (move pointer to next line to read that line)
    }
  }
    bool AInstr(string line) { //For A instruction
        int index = 0;
        int d_index = 0;
        skip(line, & index); //skip space at start of instrucion
        if (line[index] == '@') {
            index++;
            skip(line, & index); //skip space between @ and value
            while (line[index] != '\0' && !backslash(line[index]) && !space(line[index]) && !at(line[index])) { //goto next index if there is no space,no at symbol and no comment in between @ and value
                data[d_index++] = line[index++];
            }
        }
        skip(line, & index); //skip space at end of instrucion
        if (d_index > 0 && (line[index] == '\0' || backslash(line[index]))) {
            data[d_index] = 0;
            this -> value = data; //store value of array data in private data member value
            return true;
        }
        return false;
    }
  bool CInstr(string line) { //For C Instrction
    int index = 0;
    bool Comp = false;
    bool Dest = false;
    bool Jump = false;
    skip(line, & index); //skip space at start of instrucion
    if (line[index] != '\0' && !backslash(line[index]) && !space(line[index]) && !at(line[index])) { //check comment,space etc foe C Instruction
      Comp = this -> Rcomp(line); //store comp in C
      Dest = this -> Rdest(line); //store dest in D
      Jump = this -> Rjump(line); //store jump in J
    }
    return Comp + Dest + Jump; //return all dest,comp and jump
  }

  bool Rdest(string line) { //read destination
        int index = 0;
        int d_index = 0;
        skip(line, & index); //skip space at start of instrucion

        while (line[index] != '=' && line[index] != '\0' && !backslash(line[index]) && !space(line[index]) && !at(line[index]) && checkchar(line[index])) { //to check if there is no comment no space no @ symbol or equal sign before destination and also check if it is a valid character.
            data[d_index++] = line[index++];
        }
        skip(line, & index); //skip space at end of instrucion

        data[d_index] = '\0'; //put null character at the end of array

        if (line[index] == '=' && d_index > 0) { //if there is = sign after dest then then it is valid as dest=comp;jump
            this -> dest = data; //store value of dest in dest data member
            index++; //ignore = sign
            return true;
        } else if (line[index] != '=' && !at(line[index])) { //when there isn`t equal sign and at sign then dest will be null
            d_index = 0;
            this -> dest = ""; //store value of dest in dest data member

            index = 0;
            return true;
        }
        return false;
    }

  bool Rcomp(string line) { //To check comp for both dest=comp , comp;jump
    this -> comp1 = this -> Rcomp1(line);
    this -> comp2 = this -> Rcomp2(line);
  }

     bool Rcomp1(string line) { //read  computaion as dest=comp
        int index = 0;
        int d_index = 0;
        bool hasequal = false;
        skip(line, & index); //skip space at start of instrucion
	if(line[index]=='A' || line[index]=='M' || line[index]=='D'){
	if(line[++index]=='D' || line[index]=='M'){  index;}
	else if(line[index]=='D'){  index++; } 
	else                                           index=0;   
	}
	else    index=0;      
	if (line[++index] == '=' && line[index] != '@' && !backslash(line[index])) {
		    hasequal = true;
		    index++;
		}
        while (line[index] != ';' && line[index] != '\0' && !backslash(line[index]) && !space(line[index]) && !at(line[index])) { //goto next index if there is no space,no at symbol and no comment in between @ and value
            data[d_index++] = line[index++];
        }
         skip(line, & index); 
        if (hasequal == true && d_index > 0) {
            data[d_index] = '\0';
            this -> comp = data;
            index = 0;
            return true;
        }
        skip(line, & index); //skip space at end of instrucion
        data[d_index] = '\0';
        return false;
    }

  bool Rcomp2(string line) { //read  computaion as comp;jump
    int index = 0;
    int d_index = 0;
    skip(line, & index); //skip space at start of instrucion
    while (line[index] != ';' && line[index] != '\0' && !backslash(line[index]) && !space(line[index]) && !at(line[index])) { //goto next index if there is no space,no at symbol and no comment in between @ and value
      data[d_index++] = line[index++];
    }
    skip(line, & index); //skip space at end of instrucion
    data[d_index] = '\0';

    if (line[index] == ';' && d_index > 0) {
      this -> comp = data; //store value of comp in comp data member
	skip(line, & index);
	index++;
      return true;
    }
	
    data[d_index] = '\0'; //put null character at the end
 skip(line, & index);
    return false;
  }

  bool Rjump(string line) { //Read jump
    int index = 0;
    int d_index = 0;
    bool Semicolon = false; //to check if there is semi colon befor jump
    skip(line, & index); //skip space or tab at the start of instruction
    if (index == 0) { //To check the semicolon a particular index if inst starts from ! or M+1 etc
      if (line[index] == 'D' || line[index] == 'A' || line[index] == 'M' || line[index] == '0' || line[index] == '1') { 
	index++;
        if ((line[index] == '-') || (line[index] == '+') || (line[index] == '&') || (line[index] == '|'))
          index = index + 1;
        else
          index = 0;
      } else if (line[index] == '!' || line[index] == '-') {
        index++;
      } else
        index = index + 1;
      skip(line, & index); //skip space
      if (line[++index] == ';') { //if there is ; then set Semicolon=1
        Semicolon = true;
        index++; //ignore ;;
      }
    }

    skip(line, & index); //skip space 

    while ((line[index]) != '\0' && !space(line[index]) && !backslash(line[index]) &&
      !at(line[index])) { //To check if there is comment or space or line ended 
      data[d_index++] = line[index++];
    }
    skip(line, & index); 
    data[d_index] = '\0';
    if (Semicolon == true && d_index > 0) { //if there is semicolon 
      data[d_index] = '\0';
      this -> jump = data; //store value of jump in jump data member
      index++;
      return true;
    } else if (Semicolon != true) { //when there isn`t semicolon at start of array then jump will be null
      d_index = 0;
	   
      this -> jump = ""; //store value of jump in jump data member
      index = 0;
      return true;
    }
    return false;
  }

  //Checkers
  bool space(char ch) { //to check space or tab
    if (ch == ' ' || ch == '\t')
      return true;
    return false;
  }
  bool at(char ch) {
    if (ch == '@')
      return true;
    return false;
  }
  bool backslash(char ch) { //to check if there is comment
    if (ch == '/')
      return true;
    return false;
  }
  bool checkchar(char ch) { //check if the character is valid
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= 0 && ch <= 9))
      return true;
    return false;
  }
  bool skip(string line, int * index) {
    while (line[ * index] != '\0' && (line[ * index] == ' ' || line[ * index] == '\t')) { //if there is tab or space skip it 
      ( * index) ++;
    }
  }
  //Getters
  int getlineno() const {
    return lineno;
  }
  string getdest() const {
    return this -> dest;
  }
  string getcomp() const {
    return comp;
  }
  string getjump() const {
    return jump;
  }
  bool getisA() const {
    return isA;
  }
  bool getisC() const {
    return isC;
  }
  string getvalue() const {
    return value;
  }
  bool hasnext() {
    return !this -> file.eof(); //it returns false when file ended
  }
};
//Initialization of static member
char Code::data[LENGTH]; // as range of char is from 0 to 255

//Main
//Other functions for Main
char ofilename[30];
void outfilename(const char * filename) {
  int index = 0;
  for (int i = 0; filename[i] != '\0' && filename[i] != '.'; i++) //enter filename till the end of array or . character
    ofilename[index++] = filename[i];
  ofilename[index++] = '.'; //set the extension to .hack
  ofilename[index++] = 'h';
  ofilename[index++] = 'a';
  ofilename[index++] = 'c';
  ofilename[index++] = 'k';
  ofilename[index++] = '\0';
}

void Assemble(char * filename) {
  Code code(filename);
  Translator translator;
  string answer;
  string result;
  string Avalue;
  string dest;
  string comp;
  string jump;
  outfilename(filename);
  ofstream out(ofilename); //to write in file open it
  while (code.hasnext()) { //It stops when there arn`t more lines in file
    code.Getnext(); //To get next  instruction
    if (code.getisA()) { //if next instruction is A instruction 
      Avalue = code.getvalue();
      answer = translator.AInstruction(Avalue); //Translate A instruction
      if (!(answer.empty())) {
        out << answer << '\n'; //write output in file 
      } else {
        cout << "Invalid A instruction: " << Avalue << " on line " << code.getlineno() << '\n'; //To show error on console
        break;
      }
    } else if (code.getisC()) { //if next instruction is C instruction 
      dest = code.getdest();
      comp = code.getcomp();
      jump = code.getjump();
      answer = translator.CInstruction(dest, comp, jump); //Translate C instruction 
      if (!(answer.empty())) {
        out << answer << '\n'; //Write machine code of C instruction in file
      } else {
        cout << "Invalid C instruction: " << dest << "=" << comp << ";" << jump << " on line " << code.getlineno() << '\n';//To show error on console
        break;
      }
    }
  }
  out.close(); //close file
}
//Main
using namespace std;
int main() {
  char filename[30];
  //To print message on screen
  cout<<'\t'<<"-->This Assembler Ignores all the whitespaces, Empty lines /indentation, Line and In-line comments while generating machine code."<<'\n';
  cout<<'\t'<<"-->Assuming that no builtin symbols,labels and variables are being used."<<'\n';
  cout << "Enter Input File name:"; //enter the name of file present in current directory
  cin >> filename;
  Assemble(filename);
  return 0;
}
