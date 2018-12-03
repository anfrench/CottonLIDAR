#include <iostream>
#include <string>
using namespace std;

unsigned char hexval(unsigned char c);
void hex2ascii(const string& in, string& out);

int main()
{
	string hex = "200010070640800C4091801720102000000FF0002020202020202020202020102020102020303070E0A0A0A0A0A0A0E0D1E1F1F1E1E1E1F1E1E1E1E1E1E1E1E1E1E1B140A01000000000002070C13171A1B1C1D1E1E1D1E1D1E1E1E1E1E1E1E1E1E1D1C1B191817161615151415100802000000000000000402000000000000000000000004040808060301000000000000000000000000000000000000000000000000000000000000000000000001040100000507010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006EB9E8030000E20B0100901E";
	 string ascii = "";
	cout << hex << endl<<endl<<endl<<endl<<endl<<endl;

	hex2ascii(hex, ascii);

	cout << hex.length() << endl;

	return 0;	
}



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
