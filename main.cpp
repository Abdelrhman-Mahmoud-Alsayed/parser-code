#include <iostream>
#include <iostream>
#include<fstream>
#include<ctype.h>
#include<string>

using namespace std;
enum token{
    digit_sy , alpha_sy , dot_sy , dash_sy ,error_sy , end_source_sy ,qmark_sy
};
class parser{
    private:
        ifstream f;
        token curr_token;

        token get_token(){

            char ch;
            string s;
            int state=0;
            while(state >=0 && state<=6 && !f.eof()){

                switch(state){
                case 0:
                    f.get(ch);
                    if(f.eof()) return end_source_sy;
                    else if (isspace(ch)) {s=ch; state=0;}
                    else if (isdigit(ch)) {s=ch; state=1;}
                    else if (ch=='-') state=2;
                    else if (isalpha(ch)) {s=ch; state=3;}
                    else if (ch=='.') state=4;
                    else if (ch=='?') state=5;
                    else state=6;
                    break;

                case 1:
                    f.get(ch);
                    if(isdigit(ch)) { s+=ch; state=1;}
                    else if (ch=='-') state=2;
                    else {f.putback(ch); return digit_sy;}
                    break;

                case 2:
                    f.get(ch);
                    if (isspace(ch)) {s+=ch; state=2;}
                    else if (isdigit(ch)||isalpha(ch)) {s+=ch; state=3;}
                    else {f.putback(ch);  return dash_sy; }
                    break;

                case 3:
                    f.get(ch);
                    if(isalpha(ch)) {s+=ch; state=3;}
                    else if (ch=='.') state=4;
                    else if (ch=='?') state=5;
                    else { f.putback(ch);  return alpha_sy;}
                    break;

                case 4:
                    f.get(ch);
                    if (isspace(ch)) {s+=ch; state=3;}
                    else {f.putback(ch); return dot_sy;}
                    break;

                case 5:
                    f.get(ch);
                    if(isspace(ch)) {s+=ch; state=5;}
                    else if(isalpha(ch)) {s+=ch; state=3;}
                    else {f.putback(ch); return qmark_sy;}
                    break;

                case 6:
						return error_sy; break;
                }
            }
            return end_source_sy;
        }
        string name(token t){
            switch(t){
            case digit_sy: return "int token";break;
			case qmark_sy: return "? token";break;
			case alpha_sy: return "alpha token";break;
			case dot_sy: return ". token";break;
			case dash_sy: return "- token";break;
            case error_sy: return "error token";break;
			case end_source_sy: return "End of file";break;
        }

        }

    public:

        parser(string filename){
            f.open(filename);
        }
        ~parser(){
			f.close();
		}
        void match(token terminal){

            if(curr_token==terminal)
                cout<<name(curr_token)<<" is matched"<<endl;
            else
                syntax_error(curr_token);

            curr_token=get_token();
        }
        void syntax_error(token terminal){

            cout<<name(terminal)<<" is not expected\n";
        }
        void sample_parser(){
            curr_token=get_token();
            program();
            match(end_source_sy);
        }
        //<program> ::= <question> | <answer>
        void program(){
            if(curr_token==digit_sy)
            {
                match(digit_sy);
                match(dash_sy);
                question();
            }
            else if(curr_token==alpha_sy)
            {
                match(alpha_sy);
                match(dash_sy);
                answer();
            }
            else if(curr_token==end_source_sy)
                match(end_source_sy);
            else
                syntax_error(curr_token);
        }
        //<question> ::= <digit> - <statement> ?
        void question(){
            if(curr_token==digit_sy)
            {
                match(digit_sy);
                match(dash_sy);
                statement();
                match(qmark_sy);
            }
            else
                syntax_error(curr_token);

        }
        //<answer> ::= <alpha> - <statement> . {<answer>}
        void answer(){
            while(curr_token==alpha_sy)
            {
                match(alpha_sy);
                match(dash_sy);
                statement();
                match(dot_sy);
            }
        }
        //<statement> ::= <alpha> | <digit>
        void statement(){
            if(curr_token==alpha_sy)
                match(alpha_sy);
            else if(curr_token==digit_sy)
                match(digit_sy);
        }
};

int main()
  {
      string name;
      cout<<"Enter file Name:";
       cin>>name;
      parser p(name);
      p.sample_parser();
      p.~parser();

    return 0;
  }
