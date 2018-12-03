#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> getTimlessVector(string fileName);
string removeTime(string tag);
vector<string> condinseLines(vector<string> Lines);
bool isCommand(string line);
void writeToFile(vector<string>, string fileName);
unsigned char hexval(unsigned char c);
void hex2ascii(const string& in, string& out);
vector<string> dataHex2Ascii(vector<string> hexData);

int main(int argc, char *argv[])
{
	if (argc > 2) 
	{
            cout << "In  File = " << argv[1] << endl; 
            cout << "Out File = " << argv[2] << endl;
        } 
	else 
	{
            cout << "Wrong Number Of arguments.\n";
            cout << "Correct format:\n";
            cout << "\t./ExcessRemoval <Path to inFile> <Path to outFile>\n";
            cout << "Exiting...\n";
            return -1;
        }

	vector<string> data;
	
	data = getTimlessVector(argv[1]);	   //removing times
	data = condinseLines(data);		  // Puting scanns into one line each
	//data = dataHex2Ascii(data);		 //converting from hex to ascii
	writeToFile(data, argv[2]);		// writing to file
	
	
	
	
	return 0;
}


/*
	@param 		a vector <string>
	@returns 	a vector <string>
	
	Cycles though the input vector removing command lines.
	it also Condences all lines between comands into one string.
	All resulting strings are stored in a vector and returned. 
*/
vector<string> condinseLines(vector<string> lines)
{
	vector<string> condensed;
	string data;
	int start =0;
	
	while(isCommand(lines[start]))
	{
		start++;
	}
	
	for(int i=start; i<lines.size(); i++)
	{
		
		if(isCommand(lines[i]))
		{
			
			if(data.find("FF")!= string::npos)
			{
				condensed.push_back(data);
			}
			while(isCommand(lines[i]))
			{
				i++;
				if(!(i < lines.size())){break;}
			}
			if(i<lines.size())
			{
				data = lines[i];
			}
		}
		
		else
		{
			if(i<lines.size())
			{
				data += lines[i];
			}
		}
		
	}
	if(!isCommand(data)&& data!= condensed.back())
	{
		condensed.push_back(data);
	}
	
	return condensed;
}


/*
	@param 		A string
	@returns	A string
	
	Returns the sub string from 34 to the end of the string.
*/
string removeTime(string line)
{
	return line.substr(34, line.size()-34);
}

/*
	@param 		a file name (string)
	@returns 	a vector of strings 
	
	This method opens a file from the given "filename" argument.
	It goes though each line and removes the time stamp and 
	puts the line in a vector.
*/
vector<string> getTimlessVector(string fileName)
{
	vector<string> noTimeLines;
	
	ifstream infile(fileName.c_str()); //open the file
        
        if (infile.is_open() && infile.good()) 
	{
		string line = "";

		while (getline(infile, line))
		{
			noTimeLines.push_back(removeTime(line));  
       	 	}
            infile.close();
        }
	else 
	{
            cout << "Failed to open file..";
        }
	return noTimeLines;
}

/*
	Checks to see if the string contains a substring
	that would indicate that it was a command.
	
	@Returns	true or false
	@param 		a string
*/
bool isCommand(string line)
{
	bool flag = false;
	
	if(line.find("sMN") 	!= string::npos){flag=true;}
	if(line.find("sMA") 	!= string::npos){flag=true;}
	if(line.find("mLR") 	!= string::npos){flag=true;}
	if(line.find("mLR") 	!= string::npos){flag=true;}
	if(line.find("sWN") 	!= string::npos){flag=true;}
	if(line.find("sWA") 	!= string::npos){flag=true;}
	if(line.find("LRscnt") 	!= string::npos){flag=true;}
	if(line.find("data") 	!= string::npos){flag=true;}
	if(line.find("req") 	!= string::npos){flag=true;}
	if(line.find("Device") 	!= string::npos){flag=true;}
	if(line.find("connect") != string::npos){flag=true;}
	//if(line.find("") != string::npos){flag=true;}
	
	return flag;
}


/*
	@param 		vector of string
	@param 		a fileName (string)
	
	Opens the file with the inputed fileName.
	writes each line From the vector to the file.
	closes file.
*/
void writeToFile(vector<string> data, string fileName)
{
	ofstream outFile;
	outFile.open (fileName.c_str());

	for(int i=0; i<data.size(); i++)
	{
		outFile <<data[i]<<endl;
	}
			
			
	//closing files
	outFile.close();
	
}



/*
	
*/
void hex2ascii(const string& in, string& out)
{
    out.clear();
    out.reserve(in.length() / 2);
    for (string::const_iterator p = in.begin(); p != in.end(); p++)
    {
       unsigned char c = hexval(*p);
       p++;
       if (p == in.end()) break; // incomplete last digit - should report error
       c = (c << 4) + hexval(*p); // + takes precedence over <<
       out.push_back(c);
    }
}

/*
	@param		unsigned char 
	@return		unsigned char
	
	calculates the first four bits of
	a hex digit
*/
unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else return c - 'a' + 10;
}


vector<string> dataHex2Ascii(vector<string> hexData)
{
	string hex;
	string ascii;
	vector <string> asciiData;

	for(int i =0; i<hexData.size(); i++)
	{
		hex = hexData[i];
		hex2ascii(hex, ascii);
		asciiData.push_back(ascii);
	}
	
	return asciiData;
}





















